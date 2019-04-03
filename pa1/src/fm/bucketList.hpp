
#ifndef fm__bucketList_h
#define fm__bucketList_h

#include "misc/global.hpp"


class Cell;

class BucketList {
public:
    BucketList(int n) { initialize(n); }
    BucketList() {};
    typedef list<Cell*>::iterator iterator;

    void        initialize(int n)   { _cellLV.resize((2*n) + 1); _maxGain = (-1 * n) - 1; _n = n; }
    iterator    insert  (Cell* cellP);
    bool        empty   ()  { return _maxGain == (-1 * _n) -1; }
    Cell*       max     () { assert(cellList(_maxGain).size() > 0); return cellList(_maxGain).front(); }
    Cell*       next    (Cell* cellP);
    void        erase   (Cell* cellP);
    void        increase(Cell* cellP);
    void        decrease(Cell* cellP);

    iterator    end     () { return _cellLV[0].end(); }

    void        clear   ();

    void        print   ();

    void        decreaseMaxGain();
private:

    void        increaseMaxGain(Cell* cellP);

    list<Cell*>&    cellList(int gain)  { assert(gain >= -1 * _n && gain <= _n); return _cellLV[gain + _n]; }

    vector<list<Cell*> > _cellLV;
    int                 _maxGain;
    int                 _n;
};

#endif

