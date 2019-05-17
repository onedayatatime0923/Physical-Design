#ifndef db__rect_h
#define db__rect_h

#include "dbGlobal.hpp"
#include "point.hpp"



class Rect{
public:
    Rect                     () {}
    Rect                     (const Point& p1, const Point& p2) : _buttomLeft(p1), _topRight(p2) {};
    Rect                     (const int& x1, const int& y1, const int& x2, const int& y2): _buttomLeft(x1, y1), _topRight(x2, y2){};
    Rect                     (const Point& p, const int& x1, const int& y1, const int& x2, const int& y2): _buttomLeft(p[0]-x1, p[1]-y1), _topRight(p[0]+x2, p[1]+y2){};

    void         set         (const int x1, const int y1, const int x2, const int y2) { _buttomLeft.set(x1, y1); _topRight.set(x2, y2); }
    void         set         (const Point& p1, const Point& p2) { _buttomLeft = p1; _topRight = p2; }
   
    // basic acess functions
    const Point& buttomLeft  () const { return _buttomLeft;     }
    const Point& topRight    () const { return _topRight;       }
    // operators
    int&       operator[]  (const int i)          { if(i == 0)      return _buttomLeft[0];
                                                        else if(i == 1) return _buttomLeft[1];
                                                        else if(i == 2) return _topRight[0];
                                                        else            return _topRight[1];}
                                                        
    const int& operator[]  (const int i)    const { if(i == 0)      return _buttomLeft[0];
                                                        else if(i == 1) return _buttomLeft[1];
                                                        else if(i == 2) return _topRight[0];
                                                        else            return _topRight[1];}
    bool         operator == (const Rect& r1) const  { return (_buttomLeft == r1._buttomLeft && _topRight == r1._topRight); }
    bool         operator != (const Rect& p)  const  { return (_buttomLeft != p._buttomLeft || _topRight != p._topRight); }
    bool         operator <  (const Rect& p)  const  { return (_buttomLeft < p._buttomLeft) || (_buttomLeft == p._buttomLeft && _topRight < p._topRight);   }
    bool         operator >  (const Rect& p)  const  { return (_buttomLeft > p._buttomLeft) || (_buttomLeft == p._buttomLeft && _topRight > p._topRight);   }
    void         operator =  (const Rect& p)         { set(p._buttomLeft, p._topRight);                                     }
    Rect         operator +  (const Rect& p)  const  { return Rect(_buttomLeft + p._buttomLeft, _topRight + p._topRight);   }
    Rect         operator -  (const Rect& p)  const  { return Rect(_buttomLeft - p._buttomLeft, _topRight - p._topRight);   }
    Rect&        operator += (const Rect& p)         { _buttomLeft += p._buttomLeft; _topRight += p._topRight; return *this;}
    Rect&        operator -= (const Rect& p)         { _buttomLeft -= p._buttomLeft; _topRight -= p._topRight; return *this;}
    Rect&        operator *= (const int& i)          { _buttomLeft *= i; _topRight *= i; return *this; }
    Rect&        operator /= (const int& i)          { _buttomLeft /= i; _topRight /= i; return *this; }
    

    // utils
    void    shift       (const Point& size)                    { _topRight+= size; _buttomLeft+= size; };
    
    int     halfPerimeter   ()                          const { return (_topRight[1] - _buttomLeft[1]) + (_topRight[0] - _buttomLeft[0]); }
    int     area            ()                          const { return (_topRight[0] - _buttomLeft[0]) * (_topRight[1] - _buttomLeft[1]); }
    Point   center          ()                          const;
    Rect    spandHor        (const int& d)            const { return Rect(_buttomLeft[0] - (d/2), _buttomLeft[1], _topRight[0] + (d/2), _topRight[1]); }
    Rect    spandVer        (const int& d)            const { return Rect(_buttomLeft[0], _buttomLeft[1] - (d/2), _topRight[0], _topRight[1] + (d/2)); }
    Rect    spandBoth       (const int& d)            const { return Rect(_buttomLeft[0] - (d/2), _buttomLeft[1] - (d/2), _topRight[0] + (d/2), _topRight[1] + (d/2)); }
    Rect    spand           (const int& d, bool dir)  const { return (dir == 0)? spandHor(d): spandVer(d); }

    // print function
    string  str             ()                          const { return ((_buttomLeft.str()) + "-" + (_topRight.str())); }
    void    dumpfile    (FILE* file)const { fprintf(file, "%d %d 1 %d %d 1\n", _buttomLeft[0], _buttomLeft[1], _topRight[0], _topRight[1]); }


    // static functions
    static int              location    (const Rect& r1, const Rect& r2, bool y = true);
    static pair<int, int>   overlapRect (int xy2, int xy1, int query_xy2, int query_xy1);
    static Rect             overlapRect (const Rect& r1, const Rect& r2);
    static int              overlapLength(const Rect& r1, const Rect& r2, bool y = true);
    static pair<int, int>   unionRect   (int xy2, int xy1, int query_xy2, int query_xy1);
    static Rect             unionRect   (const Rect& r1, const Rect& r2);
    static bool             horPlaced   (const Rect& r1, const Rect& r2) { return r1[1] == r2[1] && r1[3] == r2[3]; }
    static bool             verPlaced   (const Rect& r1, const Rect& r2) { return r1[0] == r2[0] && r1[2] == r2[2]; }
    static int              Mdistance           (const Point &p, const Rect& r);
    static int              nearestMdistance    (const Rect& r1, const Rect &r2);
    static int              centerMdistance     (const Rect &r1, const Rect &r2);
    static bool             isConnect   (const Rect& r1, const Point& p) { return (p[0] >= r1[0] && p[0] <= r1[2] && p[1] >= r1[1] && p[1] <= r1[3]); }

private:
    Point _buttomLeft;
    Point _topRight;
};

struct VerCmp {
    bool operator ()(const Rect& gd1, const Rect& gd2) {
        if (gd1[0] != gd2[0]) return gd1[0] < gd2[0] ;
        if (gd1[1] != gd2[1]) return gd1[1] < gd2[1] ;
        if (gd1[2] != gd2[2]) return gd1[2] < gd2[2] ;
        return gd1[3] < gd2[3];
        // return ((gd1[0] < gd2[0]) || (gd1[0] == gd2[0] && gd1[1] < gd2[1]));
    }
} ;

struct HorCmp {
    bool operator () (const Rect& gd1, const Rect& gd2) {
        if (gd1[1] != gd2[1])
            return gd1[1] < gd2[1];
        if (gd1[0] != gd2[0])
            return gd1[0] < gd2[0];
        if (gd1[3] != gd2[3])
            return gd1[3] < gd2[3];
        return gd1[2] < gd2[2];
    }
} ;




#endif
