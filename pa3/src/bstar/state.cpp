
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

void State::rotate(int a) {
    _rotationV[a] = !_rotationV[a];
};
void State::swap(int a, int b) {
    std::swap(_nodeV[a], _nodeV[b]);
    rename(a, b);
    rename(b, a);
};
void State::deleteInsert(int a, int b, PerturbSeed::InsertDir dir) {
    if (leftChild(a) == Node::nullNode) {
        if (leftChild(parent(a)) == a) {
            connectLeftChild(parent(a), rightChild(a));
        }
        else if(rightChild(parent(a)) == a) {
            connectRightChild(parent(a), rightChild(a));
        }
        else assert(false);
    }
    else {
        int handle = leftChild(a);
        int handleOriginalRightChild = rightChild(leftChild(a));
        int nextHandleOriginalRightChild;
        if (leftChild(parent(a)) == a) {
            connectLeftChild(parent(a), handle);
        }
        else if(rightChild(parent(a)) == a) {
            connectRightChild(parent(a), handle);
        }
        connectRightChild(handle, rightChild(a));
        while (leftChild(handle) != Node::nullNode) {
            nextHandleOriginalRightChild = rightChild(leftChild(handle));
            connectRightChild(leftChild(handle), handleOriginalRightChild);
            handle = leftChild(handle);
            handleOriginalRightChild = nextHandleOriginalRightChild;
        }
        connectLeftChild(handle, handleOriginalRightChild);
    }

    if (dir == PerturbSeed::InsertLeft) {
        connectLeftChild(a, leftChild(b));
        connectRightChild(a, Node::nullNode);
        connectLeftChild(b, a);
    }
    else if(dir == PerturbSeed::InsertRight) {
        connectRightChild(a, rightChild(b));
        connectLeftChild(a, Node::nullNode);
        connectRightChild(b, a);
    }
};
void State::rename(int a, int b) {
    // printf("i: %d\n", i);
    // printf("parent id: %d\n", parent(i));
    if (parent(a) == a) {
        setParent(a, b);
    }
    else if (leftChild(parent(a)) == b) {
        setLeftChild(parent(a), a);
    }
    else if (rightChild(parent(a)) == b) {
        setRightChild(parent(a), a);
    }

    if (leftChild(a) == a) {
        setLeftChild(a, b);
    }
    else if (parent(leftChild(a)) == b) {
        setParent(leftChild(a), a);
    }

    if (rightChild(a) == a) {
        setRightChild(a, b);
    }
    else if (parent(rightChild(a)) == b) {
        setParent(rightChild(a), a);
    }
};
void State::pack(DB& db) {
    printf("here01\n");
    _size.set(0,0);
    Point s = db.block(root()).size();
    if (_rotationV[root()]) s.swapXY();

    _locationV[root()][0] = 0;
    _locationV[root()][2] = s[0];
    _locationV[root()][1] = 0;
    _locationV[root()][3] = s[1];
    printf("here02\n");
    list<int> contourL;
    printf("here03\n");
    contourL.emplace_front(root());
    updateSize(_locationV[root()]);
    #ifdef STATE_DEBUG
    printf("id: %d\n", root());
    printf("  rect: %s\n", _locationV[root()].str().c_str());
    printContour();
    #endif
    auto it = contourL.begin();
    assert(contourL.size() == 1);
    printf("here04\n");
    pack(db, root(), contourL, it);
    printf("here05\n");
    calculateWireLength(db);
    printf("here06\n");
};
void State::pack(DB& db, int id, list<int>& contourL, const list<int>::iterator& it) {
    // printf("Pack id: %d, iterator id: %d\n", id, *it);
    int leftChildId, rightChildId;
    leftChildId = leftChild(id);
    rightChildId = rightChild(id);
    // printf("  leftChildId: %d\n", leftChildId);
    // printf("  rightChildId: %d\n", rightChildId);
    if (leftChildId != Node::nullNode) {
        Point s = db.block(leftChildId).size();
        if (_rotationV[leftChildId]) s.swapXY();
        auto recursiveIt = next(it);

        _locationV[leftChildId][0] = _locationV[id][2];
        _locationV[leftChildId][2] = _locationV[leftChildId][0] + s[0];
        _locationV[leftChildId][1] = findY(leftChildId, contourL, recursiveIt);
        _locationV[leftChildId][3] = _locationV[leftChildId][1] + s[1];
        updateSize(_locationV[leftChildId]);
        #ifdef STATE_DEBUG
        printf("  left id: %d\n", leftChildId);
        printf("    rect: %s\n", _locationV[leftChildId].str().c_str());
        printContour();
        #endif
        pack(db, leftChildId, contourL, recursiveIt);
        // printf("come back to %d\n", id);
    }

    if (rightChildId != Node::nullNode) {
        Point s = db.block(rightChildId).size();
        if (_rotationV[rightChildId]) s.swapXY();
        auto recursiveIt = it;

        _locationV[rightChildId][0] = _locationV[id][0];
        _locationV[rightChildId][2] = _locationV[rightChildId][0] + s[0];
        _locationV[rightChildId][1] = findY(rightChildId, contourL, recursiveIt);
        _locationV[rightChildId][3] = _locationV[rightChildId][1] + s[1];
        updateSize(_locationV[rightChildId]);
        #ifdef STATE_DEBUG
        printf("  right id: %d\n", rightChildId);
        printf("    rect: %s\n", _locationV[rightChildId].str().c_str());
        printContour();
        #endif
        pack(db, rightChildId, contourL, recursiveIt);
    }
}
void State::updateSize(const Rect& size) {
    _size[0] = max(_size[0], size[2]);
    _size[1] = max(_size[1], size[3]);
}
int State::findY(int id, list<int>& contourL, list<int>::iterator& it) {
    // printf("find y id: %d\n", id);
    // printf("list id: %d\n", *it);
    Rect& handle = _locationV[id];
    list<int>::iterator start, end;
    start = end = it;
    int maxY = 0;
    while (end != contourL.end() && _locationV[*end][2] <= handle[2]) {
        maxY = max(maxY, _locationV[*end][3]);
        ++end;
    }
    if (end != contourL.end() && _locationV[*end][0] < handle[2]) {
        maxY = max(maxY, _locationV[*end][3]);
    }
    start = contourL.erase(start, end);
    it = contourL.insert(start, id);
    return maxY;
}
void State::calculateWireLength(DB& db) {
    _wireLength = 0;
    for (int i = 0; i < db.netSize(); ++i) {
        // printf("net terminal size: %d\n", db.net(i).terminalSize());
        calculateWireLength(db.net(i));
    }

};
void State::calculateWireLength(Net& net) {
    printf("net id: %d\n", net.id());
    Point coor;
    int minX, minY, maxX, maxY;
    minX = minY = INT_MAX;
    maxX = maxY = 0;
    printf("net blockSize: %d\n", net.blockSize());
    printf("net blockSize: %lu\n", net._blockPV.size());
    for (int i = 0; i < net._blockPV.size(); ++i) {
        printf("id: %d\n", net._blockPV[i]->id());
    }
    printf("here\n");
    printf("net blockSize: %lu\n", net._blockPV.size());
    for (int i = 0; i < net.blockSize(); ++i) {
        coor = _locationV[net.blockP(i)->id()].center();
        // printf("i: %d\n", i);
        minX = min(minX, coor[0]);
        minY = min(minY, coor[1]);
        maxX = max(maxX, coor[0]);
        maxY = max(maxY, coor[1]);
        // printf("maxX: %d, maxY: %d, minX: %d, minY: %d\n", maxX, maxY, minX, minY);
    }
    
    printf("net terminalSize: %d\n", net.terminalSize());
    for (int i = 0; i < net.terminalSize(); ++i) {
        coor = net.terminalP(i)->coor();
        minX = min(minX, coor[0]);
        minY = min(minY, coor[1]);
        maxX = max(maxX, coor[0]);
        maxY = max(maxY, coor[1]);
    }
    // printf("cost: %d\n", maxX + maxY - minX - minY);
    _wireLength += maxX + maxY - minX - minY;
}
void State::print(int i, int level, int type) {
    printf("%*s", level, "  ");
    if (type == 1) {
        printf("L:");
    }
    else if (type == 2) {
        printf("R:");
    }
    printf("%d\n", i);
    // getchar();
    if (_nodeV[i].leftChild != Node::nullNode) {
        assert(parent(leftChild(i) == i));
        print(_nodeV[i].leftChild, level + 1, 1);
    }
    if (_nodeV[i].rightChild != Node::nullNode) {
        assert(parent(rightChild(i) == i));
        print(_nodeV[i].rightChild, level + 1, 2);
    }
    // getchar();
};
void State::dumpFile(const string& name, DB& db) {
    FILE* file = fopen(name.c_str(), "w");
    for (int i = 0; i < (int)_locationV.size(); ++i) {
        fprintf(file, "%d\n", i);
        fprintf(file, "(\n");
        _locationV[i].dumpFile(file, 1);
        fprintf(file, ")\n");
    }

    fprintf(file, "Fixed_OutLine\n");
    fprintf(file, "(\n");
    Rect(Point(0,0), db.size()).dumpFile(file, 3);
    fprintf(file, ")\n");
    fclose(file);
};
void State::printContour(list<int>& contourL) {
    printf("Ccontour:\n");
    for (auto it = contourL.begin(); it != contourL.end(); ++it) {
        printf("  id: %d\n", *it);
    }
}
