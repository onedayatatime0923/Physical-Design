
#ifndef fm__fm_h
#define fm__fm_h

#include "misc/global.hpp"

#include "data/data.hpp"
#include "bucketList.hpp"

// #define DEBUG

class FM {
public:
    FM(Data& data);

    

    int     solve();

private:

    void    initializeCellBlock ();
    void    assignCell          ();
    void    initializeBlockState();
    void    initializeCellGain  ();
    void    initializeBucketList();
    int     initializeCost      ();

    void    pass                ();
    void    moveBaseCell        (Cell* baseCell);

    bool    cellLocked          (Cell* cellP);
    bool    checkBlockState     ();



    Data&                           _data;
    vector<BucketList>              _bucketList;
    vector<int>                     _blockState;
    vector<vector<pair<int, int>>>  _blockNetState; // first for free, second for locked

    int                             _totalUpdateCost;
    int                             _bestUpdateCost;

};

#endif
