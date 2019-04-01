
#include "fm.hpp"

FM::FM(Data& data) : _data(data) {
    int maxP = 0;
    for (int i = 0; i < data.cellPSize(); ++i) {
        if (data.cellP(i)->netPSize() > maxP) {
            maxP = data.cellP(i)->netPSize();
        }
    }
    // printf("maxP: %d\n", maxP);
    _bucketList.initialize(maxP);

    _blockState.resize(2);
    _blockState[0] = 0;
    _blockState[1] = 0;

    _blockNetState.resize(2);
    _blockNetState[0].resize(data.netPSize());
    _blockNetState[1].resize(data.netPSize());
};

int FM::solve() {
    initializeCellBlock();
    initializeBlockState();
    initializeCellGain();
    initializeBucketList();

    int cost = initializeCost();
    #ifdef DEBUG
    _data.printCell();
    _bucketList.print();
    printf("cost: %d\n", cost);
    getchar();
    #endif

    pass();
    int times = 1;
    while (_bestUpdateCost > 0) {
        cost -= _bestUpdateCost;
        pass();
        ++times;
    }
    printf("iteration times: %d\n", times);
    return cost;
}

void FM::initializeCellBlock() {
    Cell* cellP;
    for (int i = 0; i < _data.cellPSize() / 2; ++i) {
        cellP = _data.cellP(i);
        cellP->block() = 0;
    }
    for (int i = _data.cellPSize() / 2; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        cellP->block() = 1;
    }
};
void FM::assignCell() {
    for (int i = 0; i < _data.cellPSize(); ++i) {
        cellP->block() = cellP->finalBlock();
    }
}
void FM::initializeBlockState() {
    _blockState[0] = 0;
    _blockState[1] = 0;
    for (int i = 0; i < cellP->netPSize(); ++i) {
        _blockNetState[0][i].first.clear();
        _blockNetState[0][i].second.clear();
        _blockNetState[1][i].first.clear();
        _blockNetState[1][i].second.clear();
    }
    Cell* cellP;
    for (int i = 0; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        ++_blockState[cellP->block()];
        for (int j = 0; j < cellP->netPSize(); ++j) {
            _blockNetState[cellP->block()][cellP->netP(j)->id()].first.emplace(cellP);
        }

    }
    assert(checkBlockState());
};
void FM::initializeCellGain() {
    Cell* cellP;
    int from, to;
    for (int i = 0; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        cellP->gain() = 0;
        from = cellP->block();
        to = from ^ 1;
        for (int j = 0; j < cellP->netPSize(); ++j) {
            if (_blockNetState[from][cellP->netP(j)->id()].first.size() == 1)
                ++cellP->gain();
            if (_blockNetState[to][cellP->netP(j)->id()].first.size() == 0)
                --cellP->gain();
        }
    }
};
void FM::initializeBucketList() {
    for (int i = 0; i < _data.cellPSize(); ++i) {
        _bucketList.insert(_data.cellP(i));
    }
}
int FM::initializeCost() {
    int res = 0;
    for (int i = 0; i < _data.netPSize(); ++i) {
        if (_blockNetState[0][i].first.size() != 0 && _blockNetState[1][i].first.size() != 0) {
            ++res;
        }
    }
    return res;
}

void FM::pass() {
    _totalUpdateCost = 0;
    _bestUpdateCost = 0;
    while (true) {
        #ifdef DEBUG
        printf("start iteration\n");
        getchar();
        #endif
        if (_bucketList.empty()) return;
        Cell* maxCellP = _bucketList.max();
        #ifdef DEBUG
        printf("maxCellP name: %s\n", maxCellP->name().c_str());
        #endif
        int from, to;
        from = maxCellP->block();
        to = from ^ 1;
        while (!_data.balanced(_blockState[from] - 1) || !_data.balanced(_blockState[to] + 1)) {
            maxCellP = _bucketList.next(maxCellP);
            #ifdef DEBUG
            printf("maxCellP name: %s\n", maxCellP->name().c_str());
            #endif
            if (maxCellP == NULL) return;
            from = maxCellP->block();
            to = from ^ 1;
        }
        _bucketList.erase(maxCellP);
        --_blockState[from];
        ++_blockState[to];
        moveBaseCell(maxCellP);
        maxCellP->block() = to;
        
        _totalUpdateCost += maxCellP->gain();
        if (_totalUpdateCost > _bestUpdateCost) {
            _bestUpdateCost = _totalUpdateCost;
            for (int i = 0; i < _data.cellPSize(); ++i) {
                _data.cellP(i)->finalBlock() = _data.cellP(i)->block();
            }
        }



        #ifdef DEBUG
        _data.printCell();
        _bucketList.print();
        printf("total update cost: %d\n", _totalUpdateCost);
        printf("best update cost: %d\n", _bestUpdateCost);
        printf("finish iteration\n");
        getchar();
        #endif
    }
    assert(_totalUpdateCost = 0);
}
void FM::moveBaseCell(Cell* baseCell) {
    int from = baseCell->block();
    int to = from ^ 1;
    int netId;
    for (int i = 0; i < baseCell->netPSize(); ++i) {
        netId = baseCell->netP(i)->id();
        #ifdef DEBUG
        printf("update net: %s\n", _data.netP(netId)->name().c_str());
        printf("update to\n");
        #endif
        if (_blockNetState[to][netId].second.size() == 0) {
            if (_blockNetState[to][netId].first.size() == 0) {
                set<Cell*>& cellPS= _blockNetState[from][netId].first;
                #ifdef DEBUG
                printf("update to 0\n");
                #endif
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    if (cellLocked(*it)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (*it)->name().c_str());
                    #endif
                    _bucketList.erase(*it);
                    ++(*it)->gain();
                    (*it)->iterator() = _bucketList.insert(*it);
                }
            }
            else if (_blockNetState[to][netId].first.size() == 1) {
                set<Cell*>& cellPS= _blockNetState[to][netId].first;
                #ifdef DEBUG
                printf("update to 1\n");
                #endif
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    if (cellLocked(*it)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (*it)->name().c_str());
                    printf("cell gain: %d\n", (*it)->gain());
                    #endif
                    _bucketList.erase(*it);
                    --(*it)->gain();
                    (*it)->iterator() = _bucketList.insert(*it);
                }
            }
        }

        _blockNetState[from][netId].first.erase(baseCell);
        _blockNetState[to][netId].second.emplace(baseCell);
        
        #ifdef DEBUG
        printf("update from\n");
        #endif
        if (_blockNetState[from][netId].second.size() == 0) {
            if (_blockNetState[from][netId].first.size() == 0) {
                set<Cell*>& cellPS= _blockNetState[to][netId].first;
                #ifdef DEBUG
                printf("update from 0\n");
                #endif
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    if (cellLocked(*it)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (*it)->name().c_str());
                    #endif
                    _bucketList.erase(*it);
                    --(*it)->gain();
                    (*it)->iterator() = _bucketList.insert(*it);
                }
            }
            else if (_blockNetState[from][netId].first.size() == 1) {
                set<Cell*>& cellPS= _blockNetState[from][netId].first;
                #ifdef DEBUG
                printf("update from 1\n");
                #endif
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    if (cellLocked(*it)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (*it)->name().c_str());
                    #endif
                    _bucketList.erase(*it);
                    ++(*it)->gain();
                    (*it)->iterator() = _bucketList.insert(*it);
                }
            }
        }
    }
}
bool FM::cellLocked(Cell* cellP) {
    return cellP->iterator() == _bucketList.end();
}
bool FM::checkBlockState() {
    // printf("_blockState.first: %d\n", _blockState.first);
    // printf("_blockState.second: %d\n", _blockState.second);
    if (_blockState[0] + _blockState[1] != _data.cellPSize()) return false;
    // printf("here\n");
    if (!_data.balanced(_blockState[0])) return false;
    // printf("here\n");
    if (!_data.balanced(_blockState[1])) return false;
    // printf("here\n");
    // printf("_blockState[0] size: %d\n", (int)_blockNetState[0].size());
    // printf("_blockState[1] size: %d\n", (int)_blockNetState[1].size());
    if (_data.netPSize() != (int)_blockNetState[0].size()) return false;
    if (_data.netPSize() != (int)_blockNetState[1].size()) return false;
    for (int i = 0; i < (int)_blockNetState.size(); ++i) {
        // printf("_blockNetState[0][i].first: %d\n", _blockNetState[0][i].first);
        // printf("_blockNetState[0][i].second: %d\n", _blockNetState[1][i].second);
        // printf("_blockNetState[1][i].first: %d\n", _blockNetState[0][i].first);
        // printf("_blockNetState[1][i].second: %d\n", _blockNetState[1][i].second);
        if (_data.netP(i)->cellPSize() != (int)(_blockNetState[0][i].first.size() + _blockNetState[0][i].second.size() + _blockNetState[1][i].first.size() + _blockNetState[1][i].second.size()))
            return false;
    }
    return true;
}


