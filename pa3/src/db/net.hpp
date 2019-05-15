
#ifndef db__net_h
#define db__net_h

#include "dbGlobal.hpp"
#include "terminal.hpp"
#include "block.hpp"


class Net {
public:
    Net(int id) : _id(id) {};

    int             id      () { return _id; }

    int             terminalSize()         { return _terminalPV.size(); }
    Terminal*       terminalP   (int i)     { return _terminalPV[i]; }
    const Terminal* terminalP   (int i) const { return _terminalPV[i]; }

    int             blockSize   ()         { return _blockPV.size(); }
    Block*          blockP      (int i)     { return _blockPV[i]; }
    const Block*    blockP      (int i) const { return _blockPV[i]; }


    void    print   ();

    // setter

    void            pushTerminalP   (Terminal* tP) { _terminalPV.emplace_back(tP); }
    void            pushBlockP      (Block* bP) { _blockPV.emplace_back(bP); }
    

private:
    int                 _id;

    vector<Terminal*>   _terminalPV;
    vector<Block*>      _blockPV;
};

#endif
