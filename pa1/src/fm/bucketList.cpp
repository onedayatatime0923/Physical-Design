
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
        if (gain < 0) {
            return NULL;
        }
    }
    return cellList(gain).front();
}
void BucketList::erase(Cell* cellP) {
    list<Cell*>& cellL = cellList(cellP->gain());
    assert(cellP->iterator() != end());
    cellL.erase(cellP->iterator());
    cellP->iterator() = end();
    decreaseMaxGain();
}
void BucketList::update(Cell* cellP) {
    list<Cell*>& cellL = cellList(cellP->gain());
    cellL.erase(cellP->iterator());
    iterator it = insert(cellP);
    cellP->iterator() = it;
};
void BucketList::increaseMaxGain(Cell* cellP) {
    if (cellP->gain() > _maxGain) {
        _maxGain = cellP->gain();
    }
};
void BucketList::decreaseMaxGain() {
    while (cellList(_maxGain).size() == 0) {
        --_maxGain;
    }
};
void BucketList::print() {
    printf("max Gain: %d\n", _maxGain);
    for (int i = _n; i >= -1 * _n; --i) {
        list<Cell*>& cellL = cellList(i);
        printf("gain: %d\n", i);
        printf("here\n");
        for (auto it = cellL.begin(); it != cellL.end(); ++it) {
            printf("  cell name: %s\n", (*it)->name().c_str());
        }
        printf("here end\n");
    }

}
