
#ifndef data__net_h
#define data__net_h

#include "dbGlobal.hpp"
#include "segment.hpp"


class Net {
public:
    Net(int index, int id, const string& name) : _index(index), _id(id), _name(name) {};

    int             index   () { return _index; }
    int             id      () { return _id; }
    const string&   name    () { return _name; }

    int             pointSize   ()  { return _pointV.size(); }
    pair<Point, Point>&         point       (int i)  { return _pointV[i]; }
    const pair<Point, Point>&   point       (int i)  const { return _pointV[i]; }

    set<Segment3D>&             segmentS()  { return _segmentS; }


    void    print   ();

    // setter

    void            pushPoint(int x1, int y1, int x2, int y2) { _pointV.emplace_back(make_pair(Point(x1,y1), Point(x2, y2))); }
    void            pushSegment(const Segment3D& s) { _segmentS.insert(s); }
    

private:
    int                 _index;
    int                 _id;
    string              _name;

    vector<pair<Point, Point>>      _pointV;
    set<Segment3D>                  _segmentS;
};

#endif
