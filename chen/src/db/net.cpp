

#include "net.hpp"

void Net::print() {
    printf("Net index: %d, id: %d, name: %s\n", _index, _id, _name.c_str());
    printf("Point:");
    for (int i = 0; i < (int)_pointV.size(); ++i) {
        if (i % 5 == 0) printf("\n");
        printf(" %s-%s", _pointV[i].first.str().c_str(), _pointV[i].second.str().c_str());
    }
    printf("\n");
    printf("Segment:");
    int i = 0;
    for (auto it = _segmentS.begin(); it != _segmentS.end(); ++it) {
        if (++i % 5 == 0) printf("\n");
        printf(" %s", it->str().c_str());
    }
    printf("\n");
}
