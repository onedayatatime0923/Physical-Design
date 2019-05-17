
#ifndef db__point_h
#define db__point_h

#include "dbGlobal.hpp"

class Point{
public:
    Point              ():_x(0), _y(0){};
    Point              (const int& x, const int& y): _x(x), _y(y){};

    void        set         (const int& x, const int& y) {_x = x; _y = y;};
    void        setX        (const int& x) {_x = x;};
    void        setY        (const int& y) {_y = y;};
    void        swapXY      () { int tmp = _x; _x = _y; _y = tmp; }
    float       ratio       () const { return (float)_x / (float)_y; }

    int&       operator[] (const int& i)                   { if(i == 0) return _x;
                                                               else if(i==1) return _y;
                                                               else assert(false); }
    const int& operator[] (const int& i) const             { if(i == 0) return _x;
                                                               else if(i==1) return _y;
                                                               else assert(false); }
    // operators
    bool            operator ==  (const Point& p)       const  { return _x == p._x && _y == p._y;                    }
    bool            operator !=  (const Point& p)       const  { return _x != p._x || _y != p._y;                    }
    bool            operator <   (const Point& p)       const  { return (_x < p._x) || (_x == p._x && _y < p._y);   }
    bool            operator >   (const Point& p)       const  { return (_x > p._x) || (_x == p._x && _y > p._y);   }
    void            operator =   (const Point& p)              { set(p._x, p._y);                                   }
    Point           operator +   (const Point& p)       const  { return Point(_x + p._x, _y + p._y);                }
    Point           operator -   (const Point& p)       const  { return Point(_x - p._x, _y - p._y);                }
    Point&          operator +=  (const Point& p)              { _x += p._x; _y += p._y; return *this; }
    Point&          operator -=  (const Point& p)              { _x -= p._x; _y -= p._y; return *this; }
    Point           operator *   (const int& i)                { return Point(_x * i, _y * i);   }
    Point&          operator *=  (const int& i)                { _x *= i; _y *= i; return *this; }
    Point           operator /   (const int& i)                { return Point(_x / i, _y / i);   }
    Point&          operator /=  (const int& i)                { _x /= i; _y /= i; return *this; }

    // comparators
    struct CompareX { bool operator() (const Point& p1, const Point& p2) { return p1._x != p2._x ? p1._x < p2._x : p1._y < p2._y; } };
    struct CompareY { bool operator() (const Point& p1, const Point& p2) { return p1._y != p2._y ? p1._y < p2._y : p1._x < p2._x; } };

    static int          Mdistance       (const Point& p1, const Point& p2);
    string              str             () const { return (std::to_string(_x) + "-" + std::to_string(_y)); }
private:
    int _x;
    int _y;
};

class Point3D{
public:
	Point3D (): _point(0,0), _layer(0) {};
	Point3D (const Point& x, int layer) : _point(x), _layer(layer) {};
	Point3D (int x, int y, int layer) : _point(x,y), _layer(layer) {};

	Point&          point       ()                            { return _point; }
	const Point&    point       ()                      const { return _point; }
	int             layer       ()                      const { return _layer; }
	int&            layer       ()                            { return _layer; }
        
	void         	set        	(const int& x, const int& y, const int& layer) {_point.set(x,y); _layer= layer; };
	void         	setX       	(const int& x) {_point[0] = x;};
	void         	setY       	(const int& y) {_point[1] = y;};
	void         	setLayer   	(const int& layer)      { _layer = layer; }
	void         	setXY      	(const int& x, const int& y) {_point.set(x,y); }

	int&       operator[] (const int& i)                   { if(i == 0)       return _point[0];
															   else if (i == 1) return _point[1]; 
															   else if (i == 2) return _layer;  
															   else assert(false); }

	const int& operator[] (const int& i) const             { if(i == 0)       return _point[0];
															   else if (i == 1) return _point[1]; 
															   else if (i == 2) return _layer;  
															   else assert(false); }
        // operators
	bool            operator ==  (const Point3D& p)       const  { return _point[0] == p._point[0] && _point[1] == p._point[1] && _layer == p._layer;           }
	bool            operator !=  (const Point3D& p)       const  { return _point[0] != p._point[0] || _point[1] != p._point[1] || _layer != p._layer;           }
	bool            operator <   (const Point3D& p)       const  { return (_point[0] < p._point[0]) || (_point[0] == p._point[0] && _point[1] < p._point[1]) || (_point[0] == p._point[0] && _point[1] == p._point[1] && _layer < p._layer);   };
	bool            operator >   (const Point3D& p)       const  { return (_point[0] > p._point[0]) || (_point[0] == p._point[0] && _point[1] > p._point[1]) || (_point[0] == p._point[0] && _point[1] == p._point[1] && _layer > p._layer);   };
	void            operator =   (const Point3D& p)              { set(p._point[0], p._point[1], p._layer);                                   }
	Point3D           operator +   (const Point3D& p)       const  { return Point3D(_point[0] + p._point[0], _point[1] + p._point[1], _layer + p._layer);                }
	Point3D           operator -   (const Point3D& p)       const  { return Point3D(_point[0] - p._point[0], _point[1] - p._point[1], _layer - p._layer);                }
	Point3D&          operator +=  (const Point3D& p)              { _point[0] += p._point[0]; _point[1] += p._point[1]; _layer += p._layer; return *this; }
	Point3D&          operator -=  (const Point3D& p)              { _point[0] -= p._point[0]; _point[1] -= p._point[1]; _layer -= p._layer; return *this; }
	Point3D&          operator *=  (const int& i)                { _point[0] *= i; _point[1] *= i; _layer *= i; return *this; }
	Point3D&          operator /=  (const int& i)                { _point[0] /= i; _point[1] /= i; _layer /= i; return *this; }

    // comparators

    static int      Mdistance       (const Point3D& p1, const Point3D& p2);
    string          str             () const { return (to_string(_point[0]) + "-" + to_string(_point[1]) + "-" + to_string(_layer)); }

	struct Hasher {
        size_t operator()(const Point3D& p) const {
        size_t h1 = hash<int>{}(p._point[0]);
        size_t h2 = hash<int>{}(p._point[1]);
        size_t h3 = hash<int>{}(p._layer);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

    private:
        Point   _point;
        int   _layer;
};

inline int Point::Mdistance(const Point& p1, const Point& p2) {
    return abs(p1._x - p2._x) + abs(p1._y - p2._y);
};

inline int Point3D::Mdistance(const Point3D& p1, const Point3D& p2) {
    return (p1._layer > p2._layer? p1._layer - p2._layer : p2._layer - p1._layer) + Point::Mdistance(p1._point, p2._point);
};

#endif
