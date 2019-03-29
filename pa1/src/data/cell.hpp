

#ifndef data__cell_h
#define data__cell_h

#include "misc/global.hpp"
#include "net.hpp"
#include "fm/bucketList.hpp"

class Cell {
public:
    Cell(const string& name) : _name(name) {};

    const string&   name        ()  { return _name; }
    int             netPSize    ()  { return _netPV.size(); }
    Net*            netP        (int i )    { return _netPV[i]; }
    BucketList::iterator& iterator()        { return _iterator; }
    int&            block       ()  { return _block; }
    int&            gain        ()  { return _gain; }



    // setter
    void    addNet      (Net* net) { _netPV.push_back(net); }

private:
    string              _name;
    vector<Net*>        _netPV;

    // for fm
    BucketList::iterator    _iterator;
    int                     _block;
    int                     _gain;
    
};

#endif
