
#ifndef data__point_h
#define data__point_h

#include "misc/global.hpp"

class Point{
public:
    Point              ():_x(0), _y(0){};
    Point              (const DType& x, const DType& y): _x(x), _y(y){};

    void         set        (const DType& x, const DType& y) {_x = x; _y = y;};
    void         setX       (const DType& x) {_x = x;};
    void         setY       (const DType& y) {_y = y;};

    DType&       operator[] (const int& i)                   { if(i == 0) return _x;
                                                               else if(i==1) return _y;
                                                               else assert(false); }
    const DType& operator[] (const int& i) const             { if(i == 0) return _x;
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
    Point&          operator *=  (const int& i)                { _x *= i; _y *= i; return *this; }
    Point           operator *   (const int& i)                { return Point(_x * i, _y * i);   }
    Point&          operator /=  (const int& i)                { _x /= i; _y /= i; return *this; }
    Point           operator /   (const int& i)                { return Point(_x / i, _y / i);   }

    // comparators
    struct CompareX { bool operator() (const Point& p1, const Point& p2) { return p1._x != p2._x ? p1._x < p2._x : p1._y < p2._y; } };
    struct CompareY { bool operator() (const Point& p1, const Point& p2) { return p1._y != p2._y ? p1._y < p2._y : p1._x < p2._x; } };

    static DType        Mdistance       (const Point& p1, const Point& p2);
    string              str             () const { return (to_string(_x) + "-" + to_string(_y)); }
private:
    DType _x;
    DType _y;
};

class Point3D{
public:
	Point3D (): _point(0,0), _layer(0) {};
	Point3D (const Point& x, DType layer) : _point(x), _layer(layer) {};
	Point3D (DType x, DType y, DType layer) : _point(x,y), _layer(layer) {};

	Point&          point       ()                            { return _point; }
	const Point&    point       ()                      const { return _point; }
	DType           layer       ()                      const { return _layer; }
	DType&          layer       ()                            { return _layer; }
        
	void         	set        	(const DType& x, const DType& y, const DType& layer) {_point.set(x,y); _layer= layer; };
	void         	setX       	(const DType& x) {_point[0] = x;};
	void         	setY       	(const DType& y) {_point[1] = y;};
	void         	setLayer   	(const DType& layer)      { _layer = layer; }
	void         	setXY      	(const DType& x, const DType& y) {_point.set(x,y); }

	DType&       operator[] (const int& i)                   { if(i == 0)       return _point[0];
															   else if (i == 1) return _point[1]; 
															   else if (i == 2) return _layer;  
															   else assert(false); }

	const DType& operator[] (const int& i) const             { if(i == 0)       return _point[0];
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

    static DType        Mdistance       (const Point3D& p1, const Point3D& p2);
    string              str             () const { return (to_string(_point[0]) + "-" + to_string(_point[1]) + "-" + to_string(_layer)); }


    private:
        Point   _point;
        DType   _layer;
};

inline DType Point::Mdistance(const Point& p1, const Point& p2) {
    return abs(p1._x - p2._x) + abs(p1._y - p2._y);
};

inline DType Point3D::Mdistance(const Point3D& p1, const Point3D& p2) {
    return (p1._layer > p2._layer? p1._layer - p2._layer : p2._layer - p1._layer) + Point::Mdistance(p1._point, p2._point);
};

#endif
