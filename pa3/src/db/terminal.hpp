
#ifndef db__terminal_h
#define db__terminal_h

#include "dbGlobal.hpp"
#include "point.hpp"


class Terminal {
public:
    Terminal(int id) : _id(id) {};

    int             id      () { return _id; }
    const string&   name    () { return _name; }
    const Point&    coor    () { return _coor; }


    void    print   ();

    // setter

    void            setName         (const string& name)    { _name = name; }
    void            setCoor         (int x, int y)          { _coor.set(x,y); }
    

private:
    int                 _id;
    string              _name;

    Point               _coor;
};

#endif
