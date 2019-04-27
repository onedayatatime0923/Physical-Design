

#include "net.hpp"

void Net::print() {
    printf("Net index: %d, id: %d, name: %s\n", _index, _id, _name.c_str());
    printf("Point:");
    for (int i = 0; i < (int)_pointV.size(); ++i) {
        if (i % 5 == 0) printf("\n");
        printf(" %s", _pointV[i].str().c_str());
    }
    printf("\n");
}
