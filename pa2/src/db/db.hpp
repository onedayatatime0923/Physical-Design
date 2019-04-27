
#ifndef db__db_h
#define db__db_h

#include "dbGlobal.hpp"
#include "point.hpp"
#include "capacityTable.hpp"
#include "net.hpp"


class DB {
public:

    CapacityTable& capacityTable()  { return _capacityTable; }

    // setter
    void setSize    (int x, int, y) { _size.set(x,y); }
    Net& pushNet    (int index, int id, const string& name) { _netV.emplace_back(index, id, nmae); return _netV.back(); }

private:
    Point               _size;
    CapacityTable       _capacityTable;
    vector<Net>         _netV;
};

#endif
