
#include "capacityTable.hpp"

int CapacityTable::horCapacity(const Point& p) {
    auto it = _horCapM.find(p);
    if (it == _horCapM.end()) { 
        return _horCap;
    }
    else {
        return it->second;
    }
};
int CapacityTable::verCapacity(const Point& p) {
    auto it = _verCapM.find(p);
    if (it == _verCapM.end()) { 
        return _verCap;
    }
    else {
        return it->second;
    }
};
int CapacityTable::capacity(const Point3D& p, Direction d) {
    Point query(p.point());
    switch (d) {
        case UP:
            return verCapacity(query);
        case DOWN:
            query[1] -= 1;
            return verCapacity(query);
        case RIGHT:
            return horCapacity(query);
        case LEFT:
            query[0] -= 1;
            return horCapacity(query);
        default:
            assert(false);
    }
    assert(false);
};
void CapacityTable::print() {
    printf("CapacityTable\n");
    printf("  hor default: %d\n", _horCap);
    printf("  hor adjustment\n");
    for (auto it = _horCapM.begin(); it != _horCapM.end(); ++it) {
        printf("    %s, %d\n", it->first.str().c_str(), it->second);
    }
    printf("  ver default: %d\n", _verCap);
    printf("  ver adjustment\n");
    for (auto it = _verCapM.begin(); it != _verCapM.end(); ++it) {
        printf("    %s, %d\n", it->first.str().c_str(), it->second);
    }
}
