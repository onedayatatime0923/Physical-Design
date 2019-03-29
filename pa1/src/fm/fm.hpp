
#ifndef fm__fm_h
#define fm__fm_h

#include "misc/global.hpp"

#include "data/data.hpp"
#include "bucketList.hpp"

class FM {
public:
    FM(Data& data) : _data(data) {};

    


private:

    void    initializeCellBlock();
    void    initializeCellGain();
    void    initializebucketList();

    BucketList                      _bucketList;
    vector<vector<pair<int, int>>>  _blockState;
    int                             _bestUpdateCost;

};

#endif
