
#include "capacityTable.hpp"

float CapacityTable::cost(const Point3D& p1, const Point3D& p2) {
    Point3D query = min(p1, p2);
    if (p1[0] == p2[0] && p1[2] == p2[2]) {
        return cost(_verCapVV[query[0]][query[1]]);
    }
    else if (p1[1] == p2[1] && p1[2] == p2[2]) {
        return cost(_horCapVV[query[0]][query[1]]);
    }
    else if (p1[0] == p2[0] && p1[1] == p2[1]) {
        return 0;
    }
    else assert(false);

    assert(false);
};
void CapacityTable::setHor(int i) {
    assert(_size != Point(-1, -1));
    _horCap = i;
    _horCapVV.clear();
    _horCapVV.resize(_size[0], {(long unsigned)_size[1], std::make_pair(i, 0)});
};
void CapacityTable::setVer(int i) {
    assert(_size != Point(-1, -1));
    _verCap = i;
    _verCapVV.clear();
    _verCapVV.resize(_size[0], {(long unsigned)_size[1], std::make_pair(i, 0)});
};
void CapacityTable::addSegment(const Point3D& p1, const Point3D& p2) {
    Point3D query = min(p1, p2);
    if (p1[0] == p2[0] && p1[2] == p2[2]) {
        ++_verCapVV[query[0]][query[1]].second;
    }
    else if (p1[1] == p2[1] && p1[2] == p2[2]) {
        ++_horCapVV[query[0]][query[1]].second;
    }
    else if (p1[0] == p2[0] && p1[1] == p2[1]) {
    }
    else assert(false);

    assert(false);
};
void CapacityTable::print() {
    printf("CapacityTable\n");
    printf("  hor default: %d\n", _horCap);
    printf("  hor table\n");
    for (int i = 0; i < _horCapVV.size(); ++i) {
        printf("row %d:", i);
        for (int j = 0; j < _horCapVV[i].size(); ++j) {
            printf(" %d", _horCapVV[i][j].first);
        }
        printf("\n");
    }
    printf("  ver default: %d\n", _verCap);
    printf("  ver table\n");
    for (int i = 0; i < _verCapVV.size(); ++i) {
        printf("row %d:", i);
        for (int j = 0; j < _verCapVV[i].size(); ++j) {
            printf(" %d", _verCapVV[i][j].first);
        }
        printf("\n");
    }
    printf(" %f %f\n", cost({0,0,0}, {0,1,0}), cost({0,1,0}, {1,1,0}));
    addSegment({0,0,0}, {0,1,0});
    printf(" %f %f\n", cost({0,0,0}, {0,1,0}), cost({0,1,0}, {1,1,0}));
}
