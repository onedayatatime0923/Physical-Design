
#include "data.hpp"

Net* Data::net(const string& name) {
    map<string, Net>::iterator it = _netM.find(name);
    if (it != _netM.end())
        return &it->second;
    else {
        Net* res = &_netM.insert({name, name}).first->second;
        _netPV.emplace_back(res);
        _netPV.back()->setId(_netPV.size() - 1);
        return res;
    }
}
Cell* Data::cell(const string& name) {
    map<string, Cell>::iterator it = _cellM.find(name);
    if (it != _cellM.end())
        return &it->second;
    else {
        Cell* res =  &_cellM.insert({name, name}).first->second;
        _cellPV.emplace_back(res);
        _cellPV.back()->setId(_cellPV.size() - 1);
        return res;
    }
}
void Data::spread() {
    _netPV.reserve(_netM.size());
    for (map<string, Net>::iterator it = _netM.begin(); it != _netM.end(); ++it) {
        _netPV.emplace_back(&it->second);
        _netPV.back()->setId(_netPV.size() - 1);
    }
    _cellPV.reserve(_cellM.size());
    for (map<string, Cell>::iterator it = _cellM.begin(); it != _cellM.end(); ++it) {
        _cellPV.emplace_back(&it->second);
        _cellPV.back()->setId(_cellPV.size() - 1);
    }

};
void Data::print() {
    for (int i = 0; i < (int)_netPV.size(); ++i) {
        printf("Net name: %d, %s\n", _netPV[i]->id(), _netPV[i]->name().c_str());
        for (int j = 0; j < (int)_netPV[i]->cellPSize(); ++j) {
            printf("  cell: %s\n", _netPV[i]->cellP(j)->name().c_str());
        }
    }
    printCell();
};
void Data::printCell() {
    for (int i = 0; i < (int)_cellPV.size(); ++i) {
        printf("Cell name: %s\n", _cellPV[i]->name().c_str());
        for (int j = 0; j < (int)_cellPV[i]->netPSize(); ++j) {
            printf("  net: %s\n", _cellPV[i]->netP(j)->name().c_str());
        }
        printf("  block: %d\n", _cellPV[i]->block());
        printf("  gain: %d\n", _cellPV[i]->gain());
    }
};
