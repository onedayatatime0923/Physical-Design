

int CapacityTable::horCapacity(const Point& p) {
    auto it = _horCapM.find(p);
    if (it = _horCapM.end()) { 
        return _horCap;
    }
    else {
        return it->second;
    }
};
int CapacityTable::verCapacity(const Point& p) {
    auto it = _verCapM.find(p);
    if (it = _verCapM.end()) { 
        return _verCap;
    }
    else {
        return it->second;
    }
};
    int     verCapacity     (const Point& p);
    int     capacity        (const Point3D& p, Direction d);
