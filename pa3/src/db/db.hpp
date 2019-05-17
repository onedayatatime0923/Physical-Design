
#ifndef db__db_h
#define db__db_h

#include "dbGlobal.hpp"
#include "point.hpp"
#include "rect.hpp"
#include "terminal.hpp"
#include "block.hpp"
#include "net.hpp"


class DB {
public:

    Point&          size        ()          { return _size; }
    int             blockSize   ()          { return _blockPV.size(); }
    Block&          block       (int i)     { return *_blockPV[i]; }
    const Block&    block       (int i) const { return *_blockPV[i]; }

    int             terminalSize()          { return _terminalPV.size(); }
    Terminal&       terminal    (int i)     { return *_terminalPV[i]; }
    const Terminal& terminal    (int i) const { return *_terminalPV[i]; }

    int             netSize     ()        { return _netV.size(); }
    Net&            net         (int i)   { return _netV[i]; }
    const Net&      net         (int i)  const { return _netV[i]; }

    // setter
    void            setSize     (int x, int y)  { _size.set(x,y); }
    void            pushBlock   (const string& name, int x, int y);
    void            pushTerminal(const string& name, int x, int y);
    void            pushNet     () { _netV.emplace_back(_netV.size()); }
    void            pushPin     (const string& name);
    
    void print      ();

private:
    Point                   _size;
    map<string, Block>      _blockM;
    vector<Block*>          _blockPV;
    map<string, Terminal>   _terminalM;
    vector<Terminal*>       _terminalPV;
    vector<Net>             _netV;
};

#endif
