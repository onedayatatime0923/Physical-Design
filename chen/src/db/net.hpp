
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
    Point&          point       (int i)  { return _pointV[i]; }
    const Point&    point       (int i)  const { return _pointV[i]; }

    int             segmentSize   ()  { return _segmentV.size(); }
    Segment3D&       segment       (int i)  { return _segmentV[i]; }
    const Segment3D& segment       (int i)  const { return _segmentV[i]; }


    void    print   ();

    // setter

    void            pushPoint(int x, int y) { _pointV.emplace_back(x, y); }
    void            pushSegment(const Segment3D& s) { _segmentV.emplace_back(s); }
    

private:
    int                 _index;
    int                 _id;
    string              _name;

    vector<Point>       _pointV;
    vector<Segment3D>   _segmentV;
};

#endif
