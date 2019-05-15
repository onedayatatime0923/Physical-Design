
#include "state.hpp"


void State::print() {
    assert(parent(dummy()) == Node::nullNode);
    assert(rightChild(dummy()) == Node::nullNode);
    assert(parent(root()) == dummy());
    // printf("dummy parent %d\n", dummy().parent);
    // printf("dummy leftChild %d\n", dummy().leftChild);
    // printf("dummy rightChild %d\n", dummy().rightChild);
    print(root(), 1);
};

void State::print(int i, int level) {
    printf("%*s%d\n", level, " ", i);
    // getchar();
    if (_nodeV[i].leftChild != Node::nullNode) {
        assert(parent(leftChild(i) == i));
        print(_nodeV[i].leftChild, level + 1);
    }
    if (_nodeV[i].rightChild != Node::nullNode) {
        assert(parent(rightChild(i) == i));
        print(_nodeV[i].rightChild, level + 1);
    }
    // getchar();
};
