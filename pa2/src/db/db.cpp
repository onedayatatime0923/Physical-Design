
#include "db.hpp"

void DB::print() {
    printf("DataBase!\n");
    printf("size: %s\n", _size.str().c_str());
    _capacityTable.print();
    for (int i = 0; i < (int)_netV.size(); ++i) {
        _netV[i].print();
    }
};
