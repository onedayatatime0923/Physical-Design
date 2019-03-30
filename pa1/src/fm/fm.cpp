
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

void FM::solve() {
    initializeCellBlock();
    initializeCellGain();
    initializeBucketList();

    _data.printCell();
    _bucketList.print();
    getchar();

    pass();
    printf("finish\n");
    _data.printCell();
    getchar();
}

void FM::initializeCellBlock() {
    Cell* cellP;
    for (int i = 0; i < _data.cellPSize() / 2; ++i) {
        cellP = _data.cellP(i);
        cellP->block() = 0;
        ++_blockState[0];
        for (int j = 0; j < cellP->netPSize(); ++j) {
            _blockNetState[0][cellP->netP(j)->id()].first.emplace(cellP);
        }

    }
    for (int i = _data.cellPSize() / 2; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        cellP->block() = 1;
        ++_blockState[1];
        for (int j = 0; j < cellP->netPSize(); ++j) {
            _blockNetState[1][cellP->netP(j)->id()].first.emplace(cellP);
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
void FM::pass() {
    while (true) {
        printf("start iteration\n");
        getchar();
        Cell* maxCellP = _bucketList.max();
        printf("maxCellP name: %s\n", maxCellP->name().c_str());
        if (maxCellP == NULL) return;
        int from, to;
        from = maxCellP->block();
        to = from ^ 1;
        while (!_data.balanced(_blockState[from] - 1) || !_data.balanced(_blockState[to] + 1)) {
            maxCellP = _bucketList.next(maxCellP);
            printf("maxCellP name: %s\n", maxCellP->name().c_str());
            if (maxCellP == NULL) return;
            from = maxCellP->block();
            to = from ^ 1;
        }
        _bucketList.erase(maxCellP);
        --_blockState[from];
        ++_blockState[to];
        moveBaseCell(maxCellP);
        maxCellP->block() = to;
        _data.printCell();
        _bucketList.print();
        printf("finish iteration\n");
        getchar();
    }
}
void FM::moveBaseCell(Cell* baseCell) {
    int from = baseCell->block();
    int to = from ^ 1;
    int netId;
    for (int i = 0; i < baseCell->netPSize(); ++i) {
        netId = baseCell->netP(i)->id();
        if (_blockNetState[to][netId].second.size() == 0) {
            if (_blockNetState[to][netId].first.size() == 0) {
                set<Cell*>& cellPS= _blockNetState[from][netId].first;
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    ++(*it)->gain();
                    _bucketList.update(*it);
                }
            }
            else if (_blockNetState[to][netId].first.size() == 1) {
                set<Cell*>& cellPS= _blockNetState[to][netId].first;
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    --(*it)->gain();
                    _bucketList.update(*it);
                }
            }
        }

        _blockNetState[from][netId].first.erase(baseCell);
        _blockNetState[to][netId].second.emplace(baseCell);
        
        if (_blockNetState[from][netId].second.size() == 0) {
            if (_blockNetState[from][netId].first.size() == 0) {
                set<Cell*>& cellPS= _blockNetState[to][netId].first;
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    --(*it)->gain();
                    _bucketList.update(*it);
                }
            }
            else if (_blockNetState[to][netId].first.size() == 1) {
                set<Cell*>& cellPS= _blockNetState[from][netId].first;
                for (auto it = cellPS.begin(); it != cellPS.end(); ++it) {
                    ++(*it)->gain();
                    _bucketList.update(*it);
                }
            }
        }
    }
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


