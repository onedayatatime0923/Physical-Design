

#ifndef data__net_h
#define data__net_h

#include "misc/global.hpp"

class Cell;

class Net {
public:
    Net(const string& name) : _name(name) {};

    const string&   name        ()  { return _name; }
    int             cellPSize   ()  { return _cellPV.size(); }
    Cell*           cellP       (int i )    { return _cellPV[i]; }

    // setter
    void    addCell     (Cell* cell) { _cellPV.push_back(cell); }
private:
    string              _name;
    vector<Cell*>       _cellPV;
};
#endif
