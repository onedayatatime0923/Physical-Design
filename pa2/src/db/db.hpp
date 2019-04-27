
#ifndef db__db_h
#define db__db_h

#include "dbGlobal.hpp"
#include "point.hpp"
#include "capacityTable.hpp"
#include "net.hpp"


class DB {
public:

    CapacityTable& capacityTable()  { return _capacityTable; }

    int             netSize   ()        { return _netV.size(); }
    Net&            net       (int i)   { return _netV[i]; }
    const Net&      net       (int i)  const { return _netV[i]; }
    // setter
    void setSize    (int x, int y) { _size.set(x,y); }
    void pushNet    (int index, int id, const string& name) { _netV.emplace_back(index, id, name); }
    void pushPoint  (int x, int y) { _netV.back().pushPoint(x,y); }
    
    void print      ();

private:
    Point               _size;
    CapacityTable       _capacityTable;
    vector<Net>         _netV;
};

#endif
