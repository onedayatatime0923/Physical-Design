
#include "fm.hpp"

FM::FM(Data& data) : _data(data) {
    int maxP = 0;
    for (int i = 0; i < data.cellPSize(); ++i) {
        if (data.cellP(i)->netPSize() > maxP) {
            maxP = data.cellP(i)->netPSize();
        }
    }
    // printf("maxP: %d\n", maxP);
    _bucketList.resize(2);
    _bucketList[0].initialize(maxP);
    _bucketList[1].initialize(maxP);

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
    // for (int i = 0; i < _blockNetState[0].size(); ++i) {
    //     printf("block 0 net: %s, size: %d\n", _data.netP(i)->name().c_str(), _blockNetState[0][i].first);
    // }
    // for (int i = 0; i < _blockNetState[1].size(); ++i) {
    //     printf("block 1 net: %s, size: %d\n", _data.netP(i)->name().c_str(), _blockNetState[1][i].first);
    // }
    initializeBucketList();

    int cost = initializeCost();
    int times = 0;
    printf("start iteration\n");
    printf("cost: %d\n", cost);
    #ifdef DEBUG
    _data.printCell();
    _bucketList[0].print();
    _bucketList[1].print();
    getchar();
    #endif


    pass();
    cost -= _bestUpdateCost;
    ++times;
    printf("after one iteration, cost: %d\n", cost);
    while (_bestUpdateCost > 0) {
        assignCell();
        initializeBlockState();
        initializeCellGain();
        initializeBucketList();
        pass();
        cost -= _bestUpdateCost;
        ++times;
        printf("after one iteration, cost: %d\n", cost);
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
    Cell* cellP;
    for (int i = 0; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        cellP->block() = cellP->finalBlock();
    }
}
void FM::initializeBlockState() {
    _blockState[0] = 0;
    _blockState[1] = 0;
    for (int i = 0; i < _data.netPSize(); ++i) {
        _blockNetState[0][i].first = 0;
        _blockNetState[0][i].second = 0;
        _blockNetState[1][i].first = 0;
        _blockNetState[1][i].second = 0;
    }
    Cell* cellP;
    for (int i = 0; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        ++_blockState[cellP->block()];
        for (int j = 0; j < cellP->netPSize(); ++j) {
            ++_blockNetState[cellP->block()][cellP->netP(j)->id()].first;
        }

    }
    assert(checkBlockState());
    assert(false);
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
            if (_blockNetState[from][cellP->netP(j)->id()].first == 1)
                ++cellP->gain();
            if (_blockNetState[to][cellP->netP(j)->id()].first == 0)
                --cellP->gain();
        }
    }
};
void FM::initializeBucketList() {
    _bucketList[0].clear();
    _bucketList[1].clear();
    Cell* cellP;
    for (int i = 0; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        _bucketList[cellP->block()].insert(_data.cellP(i));
    }
}
int FM::initializeCost() {
    int res = 0;
    for (int i = 0; i < _data.netPSize(); ++i) {
        if (_blockNetState[0][i].first != 0 && _blockNetState[1][i].first != 0) {
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

        bool from0To1, from1To0;

        from0To1 = (_data.balanced(_blockState[0] - 1) && _data.balanced(_blockState[1] + 1));
        from1To0 = (_data.balanced(_blockState[1] - 1) && _data.balanced(_blockState[0] + 1));

        Cell* maxCellP;
        if (from0To1 && !from1To0) {
            if (!_bucketList[0].empty())
                maxCellP = _bucketList[0].max();
            else return;
        }
        else if (!from0To1 && from1To0) {
            if (!_bucketList[1].empty())
                maxCellP = _bucketList[1].max();
            else return;
        }
        else if (from0To1 && from1To0) {
            if (!_bucketList[0].empty() && !_bucketList[1].empty())
                maxCellP = _bucketList[0].max()->gain() > _bucketList[1].max()->gain()? _bucketList[0].max(): _bucketList[1].max();
            else if (!_bucketList[0].empty() && _bucketList[1].empty())
                maxCellP = _bucketList[0].max();
            else if (_bucketList[0].empty() && !_bucketList[1].empty())
                maxCellP = _bucketList[1].max();
            else return;

        }else
            return;


        #ifdef DEBUG
        printf("from0To1: %d\n", from0To1);
        printf("from1To0: %d\n", from1To0);
        cout << maxCellP << endl;
        printf("maxCellP name: %s\n", maxCellP->name().c_str());
        #endif
        int from, to;
        from = maxCellP->block();
        to = from ^ 1;

        _bucketList[from].erase(maxCellP);
        --_blockState[from];
        ++_blockState[to];
        moveBaseCell(maxCellP);
        maxCellP->block() = to;

        _bucketList[from].decreaseMaxGain();
        _bucketList[to].decreaseMaxGain();
        
        _totalUpdateCost += maxCellP->gain();
        if (_totalUpdateCost > _bestUpdateCost) {
            _bestUpdateCost = _totalUpdateCost;
            for (int i = 0; i < _data.cellPSize(); ++i) {
                _data.cellP(i)->finalBlock() = _data.cellP(i)->block();
            }
        }



        #ifdef DEBUG
        _data.printCell();
        _bucketList[0].print();
        _bucketList[1].print();
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
        if (_blockNetState[to][netId].second == 0) {
            if (_blockNetState[to][netId].first == 0) {
                #ifdef DEBUG
                printf("update to 0\n");
                #endif
                Cell* cellP;
                for (int i = 0; i < _data.netP(netId)->cellPSize(); ++i) {
                    cellP = _data.netP(netId)->cellP(i);
                    if (cellP->block() == to || cellLocked(cellP)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (cellP)->name().c_str());
                    #endif
                    _bucketList[from].increase(cellP);
                }
            }
            else if (_blockNetState[to][netId].first == 1) {
                #ifdef DEBUG
                printf("update to 1\n");
                #endif
                Cell* cellP;
                for (int i = 0; i < _data.netP(netId)->cellPSize(); ++i) {
                    cellP = _data.netP(netId)->cellP(i);
                    if (cellP->block() == from || cellLocked(cellP)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (cellP)->name().c_str());
                    #endif
                    _bucketList[to].decrease(cellP);
                }
            }
        }

        --_blockNetState[from][netId].first;
        ++_blockNetState[to][netId].second;
        
        #ifdef DEBUG
        printf("update from\n");
        #endif
        if (_blockNetState[from][netId].second == 0) {
            if (_blockNetState[from][netId].first == 0) {
                #ifdef DEBUG
                printf("update from 0\n");
                #endif
                Cell* cellP;
                for (int i = 0; i < _data.netP(netId)->cellPSize(); ++i) {
                    cellP = _data.netP(netId)->cellP(i);
                    if (cellP->block() == from || cellLocked(cellP)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (cellP)->name().c_str());
                    #endif
                    _bucketList[to].decrease(cellP);
                }
            }
            else if (_blockNetState[from][netId].first == 1) {
                #ifdef DEBUG
                printf("update from 1\n");
                #endif
                Cell* cellP;
                for (int i = 0; i < _data.netP(netId)->cellPSize(); ++i) {
                    cellP = _data.netP(netId)->cellP(i);
                    if (cellP->block() == to || cellLocked(cellP)) continue;
                    #ifdef DEBUG
                    printf("cell name: %s\n", (cellP)->name().c_str());
                    #endif
                    _bucketList[from].increase(cellP);
                }
            }
        }
    }
}
bool FM::cellLocked(Cell* cellP) {
    return (cellP->iterator() == _bucketList[0].end() || cellP->iterator() == _bucketList[1].end());
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
    for (int i = 0; i < _data.netPSize(); ++i) {
        // printf("_blockNetState[0][i].first: %d\n", _blockNetState[0][i].first);
        // printf("_blockNetState[0][i].second: %d\n", _blockNetState[1][i].second);
        // printf("_blockNetState[1][i].first: %d\n", _blockNetState[0][i].first);
        // printf("_blockNetState[1][i].second: %d\n", _blockNetState[1][i].second);
        if (_data.netP(i)->cellPSize() != (int)(_blockNetState[0][i].first + _blockNetState[0][i].second + _blockNetState[1][i].first + _blockNetState[1][i].second))
            return false;
    }
    return true;
}


