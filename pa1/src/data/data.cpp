
#include "data.hpp"

Net* Data::net(const string& name) {
    auto it = _netM.find(name);
    if (it != _netM.end())
        return &it->second;
    else {
        return &_netM.emplace(name, name).first->second;
    }
}
Cell* Data::cell(const string& name) {
    auto it = _cellM.find(name);
    if (it != _cellM.end())
        return &it->second;
    else {
        return &_cellM.emplace(name, name).first->second;
    }
}
void Data::spread() {
    _netPV.reserve(_netM.size());
    for (auto it = _netM.begin(); it != _netM.end(); ++it) {
        _netPV.emplace_back(&it->second);
    }
    _cellPV.reserve(_cellM.size());
    for (auto it = _cellM.begin(); it != _cellM.end(); ++it) {
        _cellPV.emplace_back(&it->second);
    }

};
void Data::print() {
    for (int i = 0; i < (int)_netPV.size(); ++i) {
        printf("Net name: %s\n", _netPV[i]->name().c_str());
        for (int j = 0; j < (int)_netPV[i]->cellPSize(); ++j) {
            printf("  cell: %s\n", _netPV[i]->cellP(j)->name().c_str());
        }
    }
    for (int i = 0; i < (int)_cellPV.size(); ++i) {
        printf("Cell name: %s\n", _cellPV[i]->name().c_str());
        for (int j = 0; j < (int)_cellPV[i]->netPSize(); ++j) {
            printf("  net: %s\n", _cellPV[i]->netP(j)->name().c_str());
        }
    }

};
