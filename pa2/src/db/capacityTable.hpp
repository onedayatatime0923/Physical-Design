
#ifndef data__capacityTable_h
#define data__capacityTable_h

#include "dbGlobal.hpp"
#include "point.hpp"


class CapacityTable {
public:
    int     horCapacity     (const Point& p);
    int     verCapacity     (const Point& p);
    int     capacity        (const Point3D& p, Direction d);

    // setter
    void    setHor          (int i) { _horCap = i; };
    void    setHor          (const Point& p, int i) { _horCapM[p] = i; };
    void    setVer          (int i) { _verCap = i; };
    void    setVer          (const Point& p, int i) { _verCapM[p] = i; };

    void    print   ();

private:
    int                 _horCap;
    int                 _verCap;

    map<Point, int>     _horCapM;
    map<Point, int>     _verCapM;
};

#endif
