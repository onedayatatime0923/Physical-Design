
#include "db.hpp"

void DB::pushBlock(const string& name, int x, int y) {
    Block& b = _blockM.emplace(name, _blockM.size()).first->second;
    _blockPV.emplace_back(&b);
    b.setName(name);
    b.setSize(x,y);
    if (x <= _size[0] && y <= _size[1] && x <= _size[1] && y <= _size[0]) {
        b.setRotatable(true);
    }
    else if (x <= _size[0] && y <= _size[1]) {
        b.setRotatable(false);
        // printf("Normal\n");
        // getchar();
    }
    else if (x <= _size[1] && y <= _size[0]) {
        b.setRotatable(false);
        b.rotate();
        // printf("Rotate\n");
        // getchar();
    }
};
void DB::pushTerminal(const string& name, int x, int y) {
    Terminal& t = _terminalM.emplace(name, _terminalM.size()).first->second;
    _terminalPV.emplace_back(&t);
    t.setName(name);
    t.setCoor(x,y);
};
void DB::pushPin(const string& name) {
    auto blockIt = _blockM.find(name);
    if (blockIt != _blockM.end()) {
        _netV.back().pushBlockP(&(blockIt->second));
        return;
    }
    auto terminalIt = _terminalM.find(name);
    if (terminalIt != _terminalM.end()) {
        _netV.back().pushTerminalP(&(terminalIt->second));
        return;
    }
    assert(false);
};
void DB::print() {
    printf("DataBase\n");
    printf("  size: %s\n", _size.str().c_str());
    printf("  block: %lu\n", _blockPV.size());
    for (int i = 0; i < (int)_blockPV.size(); ++i) {
        _blockPV[i]->print();
    }
    printf("  terminal: %lu\n", _terminalPV.size());
    for (int i = 0; i < (int)_terminalPV.size(); ++i) {
        _terminalPV[i]->print();
    }
    printf("  net: %lu\n", _netV.size());
    for (int i = 0; i < (int)_netV.size(); ++i) {
        _netV[i].print();
    }
};
