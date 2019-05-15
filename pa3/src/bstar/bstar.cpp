
#include "bstar.hpp"

void Bstar::solve() {
    State currentState(_db.blockSize());
    initState(currentState);
    State bestState = currentState;
    currentState.print();
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
        assert(block(seed.rotationId()).rotatable());
        block(seed.rotationId()).rotate();
    }
    else if (seed._operation == PerturbSeed::OpSwap) {
        swap(_nodeV[seed.swapId()[0]], _nodeV[seed.swapId()[1]]);
    }
    else if (seed._operation == PerturbSeed::OpSwap) {
        swap(_nodeV[seed.swapId()[0]], _nodeV[seed.swapId()[1]]);
    }
}
void Bstar::randomPerturbSeed(PerturbSeed& seed) {
    // seed._operation = PerturbSeed::OpRotation;
    // seed._operand.clear();
    // seed._operand.emplace_back(0);
    // 
    // seed._operation = PerturbSeed::OpSwap;
    // seed._operand.clear();
    // seed._operand.emplace_back(0);
    // seed._operand.emplace_back(1);
    //
    seed._operation = PerturbSeed::OpSwap;
    seed._operand.clear();
    seed._operand.emplace_back(0);
    seed._operand.emplace_back(1);
}
