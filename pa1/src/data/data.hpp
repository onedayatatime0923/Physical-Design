
#ifndef data__data_h
#define data__data_h

#include "misc/global.hpp"

#include "net.hpp"
#include "cell.hpp"

class Data {
public:
    Net*    net     (const string& name);
    int     netPSize()          { return _netPV.size(); }
    Net*    netP    (int i )    { return _netPV[i]; }
    Cell*   cell    (const string& name);
    int     cellPSize()         { return _cellPV.size(); }
    Cell*   cellP   (int i )    { return _cellPV[i]; }
    float&  balanceDegree()     { return _balanceDegree; }

    bool    balanced(int i)     { return ((1-_balanceDegree)/2) * _cellPV.size() <= i && i <= ((1+_balanceDegree)/2) * _cellPV.size(); }


    void    spread  ();

    void    print   ();
    void    printCell   ();

private:
    map<string, Net>    _netM;
    vector<Net*>        _netPV;

    map<string, Cell>   _cellM;
    vector<Cell*>       _cellPV;

    float _balanceDegree;
};

#endif
