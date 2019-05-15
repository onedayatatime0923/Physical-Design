
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
    if (seed._operation == OpRotation) {
        assert(block(seed.rotationId()).rotatable());
    state._nodeV.back().leftChild = 0;
    for (int i = 0; i < state.blockSize(); ++i) {
        if (2 * i + 1 < state.blockSize()) {
            state._nodeV[i].leftChild = 2 * i + 1;
            state._nodeV[2 * i + 1]. parent = i;
        }
        if (2 * i + 2 < state.blockSize()) {
            state._nodeV[i].rightChild = 2 * i + 2;
            state._nodeV[2 * i + 2]. parent = i;
        }
    }
}
void Bstar::randomPerturbSeed(PerturbSeed& seed) {
    seed._operation = OpRotation;
    seed._operand.clear();
    seed._operand.emplace_back(0);
}
