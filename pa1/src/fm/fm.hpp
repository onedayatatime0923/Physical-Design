
#ifndef fm__fm_h
#define fm__fm_h

#include "misc/global.hpp"

#include "data/data.hpp"
#include "bucketList.hpp"

class FM {
public:
    FM(Data& data);

    

    void    solve();

private:

    void    initializeCellBlock();
    void    initializeCellGain();
    void    initializeBucketList();

    void    pass();
    void    moveBaseCell(Cell* baseCell);

    bool    checkBlockState();



    Data&                           _data;
    BucketList                      _bucketList;
    vector<int>                     _blockState;
    vector<vector<pair<set<Cell*>, set<Cell*>>>>  _blockNetState; // first for free, second for locked
    int                             _bestUpdateCost;

};

#endif
