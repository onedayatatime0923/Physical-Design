
#include "bstar.hpp"

void Bstar::solve() {

    State currentState(_db.blockSize());
    initState(currentState);

    norm(currentState);
    // printf("_RNorm: %f\n", _RNorm);
    // getchar();
    _aspectRationCost = bstarParam.aspectRationCostStart;
    anealing(currentState);
}

void Bstar::anealing(State& currentState) {
    vector<PerturbSeed> history;
    int bestState = -1;
    int feasibleState = -1;

    State nextState(_db.blockSize());
    _finalState = currentState;

    PerturbSeed seed;

    float temperature = bstarParam.startTemperature;
    int N = bstarParam.numStepCoefficient * _db.blockSize();
    // printf("block size: %d\n", _db.blockSize());
    int numStep = 0;
    int numUphill = 0;
    int numReject = 0;
    float feasibleCost, bestCost, currentCost, nextCost;
    float deltaCost;
    currentState.pack(_db);
    feasibleCost = INT_MAX;
    currentCost = bestCost = costWithRatio(currentState);

    int times = 0;
    while (true) {
        numStep = numUphill = numReject = 0;
        while (true) {
            #ifdef BSTAR_DEBUG
            printf("start one iteration\n");
            printf("T: %f\n", temperature);
            printf("numStep: %d\n", numStep);
            printf("numUphill: %d\n", numUphill);
            printf("numReject: %d\n", numReject);
            #endif

            nextState = currentState;
            randomPerturbSeed(nextState, seed);
            perturbState(nextState, seed);
            // nextState.print();
            // printf("here\n");
            nextState.pack(_db);
            // printf("end\n");

            // check feasibility
            if (check(nextState)) {
                _aspectRationCost *= bstarParam.aspectRationCostDecrement;
            }
            else {
                _aspectRationCost*= bstarParam.aspectRationCostIncrememt;
            }


            ++numStep;
            nextCost = costWithRatio(nextState);
            deltaCost = nextCost - currentCost;

            #ifdef BSTAR_DEBUG
            printf("  currentCost: %f\n", currentCost);
            printf("  nextCost: %f\n", nextCost);
            printf("  deltaCost: %f\n", deltaCost);
            printf("  bestCost: %f\n", bestCost);
            printf("  feasibleCost: %f\n", feasibleCost);
            printf("  accept rate: %f\n", exp(-1 * deltaCost / temperature));
            printf("  aspectRationCost: %f\n", _aspectRationCost * pow(_db.blockSize(), bstarParam.aspectRationPower) * _RNorm);
            #endif
            if (deltaCost <= 0 || drand() < exp(-1 * deltaCost / temperature)) {
                #ifdef BSTAR_DEBUG
                printf("accept\n");
                #endif
                if (deltaCost > 0) {
                    #ifdef BSTAR_DEBUG
                    printf("  uphill\n");
                    #endif
                    ++numUphill;
                }
                history.emplace_back(seed);
                currentState = nextState;
                currentCost = nextCost;
                if (currentCost < bestCost) {
                    bestState = history.size() - 1;
                    bestCost = currentCost;
                }
                if (check(currentState) && costWithoutRatio(currentState) < feasibleCost) {
                    feasibleState = history.size() - 1;
                    feasibleCost = currentCost;
                    // printf("find!\n");
                    // getchar();
                }
            }
            else {
                #ifdef BSTAR_DEBUG
                printf("reject\n");
                #endif
                ++numReject;
            }
            // printf("after %d iteration\n", numStep);
            // printf("N: %d\n", N);
            // getchar();
            if (numUphill > N || numStep > 2 * N) break;
        }
        temperature *= bstarParam.gamma;
        if (float(numReject) / (float)numStep > bstarParam.quitCondition || temperature < bstarParam.epsilon) {
            if (feasibleState == -1) {
                printf("start over\n");
                ++times;
                if (times > 10) break;
                temperature = bstarParam.startTemperature;
                _aspectRationCost *= bstarParam.aspectRationCostIterationIncrememt;

                // int s = (feasibleState != -1)? feasibleState: bestState;
                //
                // State stateTmp = _finalState;
                // assert(bestState < (int)history.size());
                // assert(feasibleState < (int)history.size());
                // for (int i = 0; i <= s; ++i) {
                //     perturbState(stateTmp, history[i]);
                // }
                //
                // // _finalState.print();
                // stateTmp.pack(_db);
                // stateTmp.dumpFile("output/input.layout", _db);
                // getchar();
            }
            else {
                break;
            }
        }
    }


    int s = (feasibleState != -1)? feasibleState: bestState;
    // int s = bestState;
    // printf("feasibleState: %d\n", feasibleState);
    assert(bestState < (int)history.size());
    assert(feasibleState < (int)history.size());
    for (int i = 0; i <= s; ++i) {
        perturbState(_finalState, history[i]);
    }

    // _finalState.print();
    _finalState.pack(_db);
    _finalState.dumpFile("output/input.layout", _db);
}
void Bstar::norm(State& state) {
    PerturbSeed seed;
    float totalR = 0;
    for (int i = 0; i < bstarParam.normSample; ++i) {
        randomPerturbSeed(state, seed);
        perturbState(state, seed);
        state.pack(_db);
        totalR += costWithoutRatio(state) / pow(state.size().ratio() - _db.size().ratio(), 2);
    }
    _RNorm = totalR / bstarParam.normSample;
}
void Bstar::initState(State& state) {
    state.setLeftChild(state.dummy(), 0);
    state.setParent(state.root(), state.dummy());
    for (int i = 0; i < state.blockSize(); ++i) {
        if (2 * i + 1 < state.blockSize()) {
            state.setLeftChild(i, 2 * i + 1);
            state.setParent(2 * i + 1, i);
        }
        if (2 * i + 2 < state.blockSize()) {
            state.setRightChild(i, 2 * i + 2);
            state.setParent(2 * i + 2, i);
        }
    }
}
void Bstar::perturbState(State& state, PerturbSeed& seed) {
    if (seed._operation == PerturbSeed::OpRotation) {
        // printf("rotation: %d\n", seed.rotationId());
        assert(block(seed.rotationId()).rotatable());
        state.rotate(seed.rotationId());
    }
    else if (seed._operation == PerturbSeed::OpSwap) {
        // printf("swap\n");
        // printf("  swap id: %d\n", seed.swapId()[0]);
        // printf("  swap id: %d\n", seed.swapId()[1]);
        state.swap(seed.swapId()[0], seed.swapId()[1]);
    }
    else if (seed._operation == PerturbSeed::OpDeleteInsert) {
        int deleteId, insertId;
        deleteId = seed.deleteId();
        insertId = seed.insertId();

        assert(deleteId != insertId);
        // printf("delete insert\n");
        // printf("  delete id: %d\n", deleteId);
        // printf("  insert id: %d\n", insertId);
        state.deleteInsert(deleteId, insertId, seed.insertDir());
    }
}
void Bstar::randomPerturbSeed(State& state, PerturbSeed& seed) {
    double p;
    p = drand();
    if (p <= ((double)1) / 3) {
        seed._operation = PerturbSeed::OpRotation;
        seed._operand.clear();
        int i = rand() % state.blockSize();
        while (!block(i).rotatable()) {
            i = rand() % state.blockSize();
        }
        seed._operand.emplace_back(i);
    }
    else if (p <= ((double)2) / 3) {
        seed._operation = PerturbSeed::OpSwap;
        seed._operand.clear();
        int a, b;
        a = rand() % state.blockSize();
        b = rand() % state.blockSize();
        while(a == b) {
            b = rand() % state.blockSize();
        }
        seed._operand.emplace_back(a);
        seed._operand.emplace_back(b);
    }
    else {
        seed._operation = PerturbSeed::OpDeleteInsert;
        seed._operand.clear();
        int a, b;
        a = rand() % state.blockSize();
        b = rand() % state.blockSize();
        while(a == b) {
            b = rand() % state.blockSize();
        }
        seed._operand.emplace_back(a);
        seed._operand.emplace_back(b);
        // if (state.leftChild(1) != State::Node::nullNode && state.rightChild(1) == State::Node::nullNode) {
        //     seed._insertDir = PerturbSeed::InsertRight;
        // }
        // else if (state.leftChild(1) == State::Node::nullNode && state.rightChild(1) != State::Node::nullNode) {
        //     seed._insertDir = PerturbSeed::InsertLeft;
        // }
        // else {
            if (drand() <= 0.5) {
                seed._insertDir = PerturbSeed::InsertLeft;
            }
            else {
                seed._insertDir = PerturbSeed::InsertRight;
            }
        // }
    }
}
float Bstar::costWithRatio(State& state) {
    // printf("apha: %f\n", _db.apha());
    // printf("state size: %s\n", state.size().str().c_str());
    // printf("state ratio: %f\n", state.size().ratio());
    // printf("db size: %s\n", _db.size().str().c_str());
    // printf("db ratio: %f\n", _db.size().ratio());
    // getchar();
    return (float)((_db.apha() * (state.size()[0] * state.size()[1])) + ((1 - _db.apha()) *state.wireLength()) + 
           (_aspectRationCost * pow(_db.blockSize(), bstarParam.aspectRationPower) * _RNorm * 
                pow(state.size().ratio() - _db.size().ratio(), 2)));
}
float Bstar::costWithoutRatio(State& state) {
    // printf("apha: %f\n", _db.apha());
    // printf("state size: %s\n", state.size().str().c_str());
    // printf("state ratio: %f\n", state.size().ratio());
    // printf("db size: %s\n", _db.size().str().c_str());
    // printf("db ratio: %f\n", _db.size().ratio());
    // getchar();
    return (float)((_db.apha() * (state.size()[0] * state.size()[1])) + ((1 - _db.apha()) *state.wireLength()));
}
bool Bstar::check(State& state) {
    return _db.size()[0] >= state.size()[0] && _db.size()[1] >= state.size()[1];
}
