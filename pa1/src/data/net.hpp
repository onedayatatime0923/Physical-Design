

#ifndef data__net_h
#define data__net_h

#include "misc/global.hpp"

class Cell;

class Net {
public:
    Net(const string& name) : _name(name) {};

    const string&   name        ()  { return _name; }
    int             id          ()  { return _id; }
    int             cellPSize   ()  { return _cellPV.size(); }
    Cell*           cellP       (int i )    { return _cellPV[i]; }

    // setter
    void    setId       (int id)        { _id = id; }
    void    addCell     (Cell* cell)    { _cellPV.push_back(cell); }
private:
    string              _name;
    int                 _id;
    vector<Cell*>       _cellPV;
};
#endif
