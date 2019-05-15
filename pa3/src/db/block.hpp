
#ifndef db__block_h
#define db__block_h

#include "dbGlobal.hpp"
#include "point.hpp"


class Block {
public:
    Block(int id) : _id(id) {};


    int             id      () { return _id; }
    const string&   name    () { return _name; }
    const Point&    size    () { return _size; }

    bool            rotatable() { return _rotatable; }

    void            rotate  () { _size.swapXY(); }

    void            print   ();

    // setter

    void            setName         (const string& name)    { _name = name; }
    void            setSize         (int x, int y)          { _size.set(x,y); }
    void            setRotatable    (bool b)                { _rotatable = b; }
    

private:
    int             _id;
    string          _name;

    Point           _size;
    bool            _rotatable;
};

#endif
