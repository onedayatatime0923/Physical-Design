
#ifndef data__capacityTable_h
#define data__capacityTable_h

#include "dbGlobal.hpp"
#include "point.hpp"


class CapacityTable {
public:
    CapacityTable() : _size(-1, -1) {};
    float   cost            (const Point3D& p1, const Point3D& p2);

    // setter
    void    setSize         (const Point& s) { _size = s; };
    void    setHor          (int i);
    void    setHor          (const Point& p, int i) { _horCapVV[p[0]][p[1]].first = i; }
    void    setVer          (int i);
    void    setVer          (const Point& p, int i) { _verCapVV[p[0]][p[1]].first = i; }
    void    addSegment      (const Point3D& p1, const Point3D& p2);

    void    print   ();

private:
    float   cost            (pair<int, int> p) { return 0; } //pow(2, float(p.second - p.first)); }
    int                 _horCap;
    int                 _verCap;
    Point               _size;

    vector<vector<pair<int, int>>>  _horCapVV;
    vector<vector<pair<int, int>>>  _verCapVV;
};

#endif
