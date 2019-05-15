
#ifndef db__block_h
#define db__block_h

#include "dbGlobal.hpp"
#include "point.hpp"


class Block {
public:
    Block(int id) : _id(id) {};

    enum Direction { Normal = 1, Rotate = 2, NormalAndRotate = 3, };

    int             id      () { return _id; }
    const string&   name    () { return _name; }
    const Point&    size    () { return _size; }
    Direction&      dir     () { return _dir; }

    bool            rotatable() { return _dir == NormalAndRotate; }

    void    print   ();

    // setter

    void            setName         (const string& name)    { _name = name; }
    void            setSize         (int x, int y)          { _size.set(x,y); }
    void            setDir          (Direction d)           { _dir = d; }
    

private:
    int                 _id;
    string              _name;

    Point               _size;
    Direction           _dir;
};

#endif
