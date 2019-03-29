
#include "bucketList.hpp"
#include "data/cell.hpp"

BucketList::iterator BucketList::insert(Cell* cellP) {
    list<Cell*> cellL = cellList(cellP->gain());
    cellL.emplace_front(cellP);
    if (cellP->gain() > _maxGain) {
        _maxGain = cellP->gain();
    }
    cellP->iterator() = cellL.begin();
    return cellL.begin();
};
Cell* BucketList::popMax() {
    Cell* res = max();
    list<Cell*> cellL = cellList(_maxGain);
    cellL.pop_front();
    while (cellList(_maxGain).size() == 0) {
        --_maxGain;
    }
    return res;
};
void BucketList::update(Cell* cellP) {
    list<Cell*> cellL = cellList(cellP->gain());
    cellL.erase(cellP->iterator());
    iterator it = insert(cellP);
    cellP->iterator() = it;
};
