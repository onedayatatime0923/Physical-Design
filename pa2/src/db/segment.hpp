
#ifndef data__segment_h
#define data__segment_h

#include "misc/global.hpp"

class Segment3D {
public:
    Segment3D(DType x1 = 0, DType y1 = 0, DType z1 = 0, DType x2 = 0, DType y2 = 0, DType z2 = 0)
        : _p1(min(x1, x2), min(y1, y2), min(z1, z2)), _p2(max(x1, x2), max(y1, y2), max(z1, z2)) {}
    Segment3D(const Point3D& p1, const Point3D& p2) 
        : _p1( min(p1[0], p2[0]), min(p1[1], p2[1]), min(p1.layer(), p2.layer()) ),
        _p2( max(p1[0], p2[0]), max(p1[1], p2[1]), max(p1.layer(), p2.layer()) ) {}

    // Basic access functions
    Point3D&        point1()            { return _p1;                           }
    Point3D&        point2()            { return _p2;                           }
    const Point3D&  point1()    const   { return _p1;                           }
    const Point3D&  point2()    const   { return _p2;                           }
    DType       layer   ()      const   { return min(_p1.layer(), _p2.layer()); }
    DType       x1      ()      const   { return _p1[0];                        }
    DType       x2      ()      const   { return _p2[0];                        }
    DType       y1      ()      const   { return _p1[1];                        }
    DType       y2      ()      const   { return _p2[1];                        }
    DType       z1      ()      const   { return _p1.layer();                   }
    DType       z2      ()      const   { return _p2.layer();                   }
    DType       length  ()      const   { return Point3D::Mdistance(_p1, _p2);  }


    bool        check   ()      const   { return int(_p1[0] != _p2[0]) + int(_p1[1] != _p2[1]) + int(_p1[2] != _p2[2]) == 1; }
    
    // operators
    bool             operator ==  (const Segment3D& s)              const { return _p1 == s._p1 && _p2 == s._p2;                           }
    bool             operator !=  (const Segment3D& s)              const { return !(*this == s);                                          }
    bool             operator <   (const Segment3D& s)              const { if (_p1 != s._p1) return _p1 < s._p1; else return _p2 < s._p2; }
    void             operator =   (const Segment3D& s)                    { _p1 = s._p1; _p2 = s._p2;                                      }
    
    DType&       operator[] (const int& i)      { if(i == 0)        return _p1[0];
                                                  else if (i == 1)  return _p1[1];
                                                  else if (i == 2)  return _p1[2];
                                                  else if (i == 3)  return _p2[0];
                                                  else if (i == 4)  return _p2[1];
                                                  else if (i == 5)  return _p2[2];
                                                  else assert(false); }
    const DType& operator[] (const int& i) const{ if(i == 0)        return _p1[0];
                                                  else if (i == 1)  return _p1[1];
                                                  else if (i == 2)  return _p1[2];
                                                  else if (i == 3)  return _p2[0];
                                                  else if (i == 4)  return _p2[1];
                                                  else if (i == 5)  return _p2[2];
                                                  else assert(false); }
    string      str         ()              const { return ((_p1.str()) + "-" + (_p2.str())); }


    

private:
    Point3D     _p1;
    Point3D     _p2;
};
#endif
