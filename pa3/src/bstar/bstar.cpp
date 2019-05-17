
#include "bstar.hpp"

void Bstar::solve() {
    vector<PerturbSeed> history;
    int bestState = -1;

    State currentState(_db.blockSize());
    initState(currentState);
    State currentStateCopy = currentState;

    PerturbSeed seed;

    int temperature = bstarParam.startTemperature;
    int N = bstarParam.numStepCoefficient * _db.blockSize();
    int numStep = 0;
    int numUphill = 0;
    int numReject = 0;
    int bestCost, currentCost;
    int deltaCost, lastCost;
    lastCost = bestCost = INT_MAX;
    while (true) {
        numStep = numUphill = numReject = 0;
        while (true) {
            #ifdef BSTAR_DEBUG
            printf("start one iteration\n");
            printf("T: %d\n", temperature);
            printf("numStep: %d\n", numStep);
            printf("numUphill: %d\n", numUphill);
            printf("numReject: %d\n", numReject);
            #endif
            randomPerturbSeed(currentState, seed);
            perturbState(currentState, seed);
            currentState.pack(_db);
            ++numStep;
            currentCost = cost(currentState);
            deltaCost = currentCost - lastCost;
            #ifdef BSTAR_DEBUG
            printf("  currentCost: %d\n", currentCost);
            printf("  deltaCost: %d\n", deltaCost);
            printf("  bestCost: %d\n", bestCost);
            printf("  accept rate: %f\n", exp(-1 * deltaCost / temperature));
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
                printf("here\n");
                printf("history size: %lu\n", history.size());
                printf("history capacity: %lu\n", history.capacity());
                history.emplace_back(seed);
                printf("here1\n");
                lastCost = currentCost;
                if (cost(currentState) < bestCost) {
                    bestState = history.size() - 1;
                    bestCost = currentCost;
                }
            }
            else {
                #ifdef BSTAR_DEBUG
                printf("reject\n");
                #endif
                ++numReject;
            }
            // getchar();
            printf("end\n");
            if (numUphill > N || numStep > 2 * N) break;
        }
        temperature *= bstarParam.gamma;
        if (float(numReject) / numStep > 0.95 || temperature < bstarParam.epsilon) break;
    }

    printf("here\n");

    // assert(bestState < (int)history.size());
    // for (int i = 0; i <= bestState; ++i) {
    //     perturbState(currentStateCopy, history[i]);
    // }
    //
    // currentStateCopy.print();
    // currentStateCopy.pack(_db);
    // printf("here1\n");
    // currentStateCopy.dumpFile("output/input.layout", _db);
    // printf("here2\n");

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
        // printf("rotation\n");
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
        if (state.leftChild(1) != State::Node::nullNode && state.rightChild(1) == State::Node::nullNode) {
            seed._insertDir = PerturbSeed::InsertRight;
        }
        else if (state.leftChild(1) == State::Node::nullNode && state.rightChild(1) != State::Node::nullNode) {
            seed._insertDir = PerturbSeed::InsertLeft;
        }
        else {
            if (drand() <= 0.5) {
                seed._insertDir = PerturbSeed::InsertLeft;
            }
            else {
                seed._insertDir = PerturbSeed::InsertRight;
            }
        }
    }
}
int Bstar::cost(State& state) {
    return (state.size()[0] * state.size()[1]) + state.wireLength();
}
