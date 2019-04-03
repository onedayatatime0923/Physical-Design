
#include "bucketList.hpp"
#include "data/cell.hpp"

BucketList::iterator BucketList::insert(Cell* cellP) {
    list<Cell*>& cellL = cellList(cellP->gain());
    cellL.emplace_front(cellP);
    cellP->iterator() = cellL.begin();

    increaseMaxGain(cellP);
    return cellL.begin();
};
Cell* BucketList::next(Cell* cellP) {
    int gain = cellP->gain();
    list<Cell*>& cellL = cellList(gain);
    iterator it = cellP->iterator();
    assert(it != end());
    ++it;
    if (it != cellL.end()) return *it;
    --gain;
    while(cellList(gain).size() == 0) {
        --gain;
        if (gain < -1 * _n) {
            return NULL;
        }
    }
    return cellList(gain).front();
}
void BucketList::erase(Cell* cellP) {
    // printf("gain: %d\n", cellP->gain());
    list<Cell*>& cellL = cellList(cellP->gain());
    assert(cellP->iterator() != end());
    // printf("gain: %d\n", cellP->gain());
    // printf("max Gain: %d\n", _maxGain);
    // printf("cellL size: %d\n", (int)cellList(_maxGain).size());
    // auto tmp = end();
    // for (auto it = cellList(_maxGain).begin(); it != cellList(_maxGain).end(); ++it) {
    //     printf("cellL name: %s\n", (*it)->name().c_str());
    // }
    // printf("end\n");
    // printf("bool: %d\n", cellP->iterator() == tmp);
    cellL.erase(cellP->iterator());
    cellP->iterator() = end();
    // printf("max Gain: %d\n", _maxGain);
    // printf("cellL size: %d\n", (int)cellList(_maxGain).size());
    // for (auto it = cellList(_maxGain).begin(); it != cellList(_maxGain).end(); ++it) {
    //     printf("cellL name: %s\n", (*it)->name().c_str());
    // }
    // printf("end\n");
}
void BucketList::increaseMaxGain(Cell* cellP) {
    if (cellP->gain() > _maxGain) {
        _maxGain = cellP->gain();
    }
};
void BucketList::decreaseMaxGain() {
    while (_maxGain >= (-1 * _n) && cellList(_maxGain).size() == 0) {
        --_maxGain;
    }
};
void BucketList::increase(Cell* cellP) {
    erase(cellP);
    ++cellP->gain();
    cellP->iterator() = insert(cellP);
};
void BucketList::decrease(Cell* cellP) {
    erase(cellP);
    --cellP->gain();
    cellP->iterator() = insert(cellP);
};
void BucketList::clear() {
    for (int i = 0; i < (int)_cellLV.size(); ++i) {
        _cellLV.clear();
    }
    _maxGain = (-1 * _n) - 1;
}
void BucketList::print() {
    printf("max Gain: %d\n", _maxGain);
    for (int i = _n; i >= -1 * _n; --i) {
        list<Cell*>& cellL = cellList(i);
        printf("gain: %d\n", i);
        // printf("here\n");
        auto it = cellL.begin();
        // printf("weird\n");
        it = cellL.end();
        // printf("weird\n");
        for (auto it = cellL.begin(); it != cellL.end(); ++it) {
            // cerr<< (*it)<<endl;
            printf("  cell name: %s\n", (*it)->name().c_str());
        }
        // printf("here end\n");
    }

}
