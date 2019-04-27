
#include "drAstar.hpp"

void DrAstar::route() {
    // printf("Connecting %s\n", _net.name().c_str());

    // _net.clearAllRoutedWires();
    // init();
    init();

    for (int i = 0; i < _db.netSize(); ++i) {
        _netP = &_db.net(i);
        routeNet();
    }
}
void DrAstar::init() {
    _totalPoint2NodeMV.clear();
    _totalPoint2NodeMV.resize(3);
    for (PointMapType& pointMap : _totalPoint2NodeMV) {
        pointMap.set_empty_key(Point3D(-1, -1, -1));
    }
    Rect guide;
    int layerStart;
    int layerEnd;
}
int DrAstar::routeNet() {
    Net& net = *_netP;
    #ifdef DRASTAR_DEBUG
    printf("start to route net: %s\n", net.name().c_str());
    for (int i = 0; i < (int)net.pointSize(); ++i) {
        printf("point: %s\n", net.point(i).str().c_str());
    }
    getchar();
    #endif

    initComponent();

    if(_componentDS.nSets() == 1) {
        return -1; // which means undo
    }
    // int originalSize = _componentDS.nSets();

    while (_componentDS.nSets() > 1) {
        // Determine source netCompId
        // cout<<"// Determine source netCompId"<<endl;
        _srcCompId = _componentDS.find(0);

        if (findPathAss()) {
            // printf("ApPair path found. (%d / %d)\n", _componentDS.nSets(), originalSize);
        } else {
            printf("Net %s\n", _net.name().c_str());
            printf("ApPair failed. QQ\n");
            assert(!_forceRoute);
            _apP->clearRoutedWires();
            return 0; // which means fail
        }
    }
    // printf("Net %s path found. (%d / %d)\n", _net.name().c_str(), _apPairNum, _net.apPairSize() * 2);
    return 1; // which means success
}
void DrAstar::initComponent() {
    Net& net = *_netP;
    // initialize
    _componentSV.clear();

    for (unsigned i = 0; i < _totalPoint2NodeMV.size(); ++i) {
        for (auto n : _totalPoint2NodeMV[i]) {
            n.second->resetComponentId();
        }
    }

    // add to totalPoint2NodeMV and componentSV
    set<pair<int, int>> sameComp;
    // printf("init\n");
    // printf("size: %d\n", (int) _componentVP->size());

    for (int i = 0; i < (int)net.pointSize(); ++i) {
        initPoint(net.point(i), sameComp);
    }

    _componentDS.clear();
    _componentDS.init(_componentSV.size());
    for (auto& it : sameComp) {
        // printf("id: %d, %d\n", it.first, it.second);
        _componentDS.merge(it.first, it.second);
    }
}
void DrAstar::initPoint(const Point& p, set<pair<int, int>>& sameComp) {
    int componentId = _componentSV.size();
    pushComponentSV();

    _componentSV[componentId].insert(point);
    if (_totalPoint2NodeMV[1].count(p) == 0) {
        AstarNode* newNode = new AstarNode(p, componentId);
        _totalPoint2NodeMV[1][p] = newNode;
    }
    else {
        auto it = _totalPoint2NodeMV[p.layer()].find(p);
        int id = it->second->componentId;
        if (id == -1) {
            it->second->componentId = componentId;
        }else if (sameComp.find({min(id, componentId), max(id, componentId)}) == sameComp.end()) {
            sameComp.emplace(min(id, componentId), max(id, componentId));
        }
    }
}
void DrAstar::pushComponentSV() {
    // printf("here\n");
    _componentSV.emplace_back();
    _componentSV.back().set_empty_key(Point3D(-1, -1, -1));
    _componentSV.back().set_deleted_key(Point3D(INT_MAX, INT_MAX, INT_MAX));
    // printf("here end\n");
}
bool DrAstar::findPathAss() {
    // construct kd tree
    // cout<<"// construct kd tree"<<endl;
    KDTree3D kdtree[2];
    kdtree[0].setZDirectWeight(_astarParam.viaCost);
    kdtree[1].setZDirectWeight(_astarParam.viaCost);
    for (int componentId = 0, nComps = _componentSV.size(); componentId < nComps; ++componentId) {
        // printf("componentId: %d\n", componentId);
        // printf("_componentSV[componentId]): %d\n", _componentDS.find(componentId));
        for (const Point3D& p : _componentSV[componentId]) {
            if (_componentDS.find(componentId) == _srcCompId) {
                // printf("insert into 0: %s\n", p.str().c_str());
                kdtree[0].insert(p);
            } else {
                // printf("insert into 1: %s\n", p.str().c_str());
                kdtree[1].insert(p);
            }
        }
    }
    kdtree[0].buildIndex();
    kdtree[1].buildIndex();

    // reinitialize total node
    // cout<<"// reinitialize total node"<<endl;
    for (unsigned i = 0; i < _totalPoint2NodeMV.size(); ++i) {
        for (auto n : _totalPoint2NodeMV[i]) {
            n.second->initialize();
        }
    }

    // construct heap
    NodePHeap0Type heap0;
    NodePHeap1Type heap1;
    IterMap0Type iterMap0;
    IterMap1Type iterMap1;
    iterMap0.set_empty_key(nullptr);
    iterMap1.set_empty_key(nullptr);

    // construct terminal point
    // cout<<"// construct terminal point"<<endl;
    for (int componentId = 0, nComps = _componentSV.size(); componentId < nComps; ++componentId) {
        Point3D pNearest;
        DType dNearest;
        DType costF;
        for (const Point3D& p : _componentSV[componentId]) {
            // cout<<"node " << p <<endl;
            // printf("componentId: %d\n", componentId);
            // printf("_componentDS.find(componentId): %d\n", _componentDS.find(componentId));
            // printf("srcCompId: %d\n", _srcCompId);
            if (_componentDS.find(componentId) == _srcCompId) {
                // cout<<"near"<<endl;
                kdtree[1].nearestSearch(p, pNearest, dNearest);
                // cout<<"pNearest "<< pNearest << " " << costH(p, pNearest) <<endl;
                costF = (0 + dNearest);
                assert(dNearest == costH(p, pNearest));
            }
            else {
                // cout<<"near1"<<endl;
                kdtree[0].nearestSearch(p, pNearest, dNearest);
                // cout<<"pNearest "<< pNearest << " " << costH(p, pNearest) << endl;
                costF = (0 + dNearest);
                assert(dNearest == costH(p, pNearest));
            }
            auto iter = _totalPoint2NodeMV[p.layer()].find(p);
            assert(iter != _totalPoint2NodeMV[p.layer()].end());
            AstarNode* node = iter->second;
            if (_componentDS.find(componentId) == _srcCompId) {
                node->costG[0] = 0;
                node->costF[0] = costF;
                iterMap0[node] = heap0.push(node);
            } else {
                node->costG[1] = 0;
                node->costF[1] = costF;
                iterMap1[node] = heap1.push(node);
            }
        }
    }
    // cout<<"end "<<endl;


    while ( !heap0.empty() && !heap1.empty() ) {
    // Extract min
        AstarNode* currentNode = nullptr;
        int groupId = 0; // 0 or 1. 0: src to tar; 1: tar to src
        bool pathFound = false;
        // printf("heap0 top: %s\n", heap0.top()->coor.str().c_str());
        // printf("heap0 cost: %d\n", heap0.top()->costF[0]);
        // printf("heap1 top: %s\n", heap1.top()->coor.str().c_str());
        // printf("heap1 cost: %d\n", heap1.top()->costF[1]);
        if (heap0.top()->costF[0] < heap1.top()->costF[1]) {
            currentNode = heap0.top();
            // cout<<" 0 current coor: " << currentNode->coor << " " << currentNode->costF[0] << endl;
            heap0.pop();
            groupId = 0;
            pathFound = currentNode->explored[1];
        } 
        else {
            currentNode = heap1.top();
            // cout<<" 1 current coor: " << currentNode->coor << " " << currentNode->costF[1]  << endl;
            heap1.pop();
            groupId = 1;
            pathFound = currentNode->explored[0];
        }
        // printf("groupId: %d\n", groupId);
        // cout<<"current coor: " << currentNode->coor  << endl;

        // path found
        if (pathFound) {
            backtrace(currentNode);
            return true;
        }

        // find neighbors
        if (currentNode->adjV.empty()) {
            vector<Point3D > vAdjPts;
            // printf("  currentNode node: %s\n", currentNode->coor.str().c_str());
            findNeighbors(currentNode->coor, vAdjPts);
            for (Point3D& adjPoint : vAdjPts) {
                // printf("adj point: %s\n", adjPoint.str().c_str());
                auto iter = _totalPoint2NodeMV[adjPoint.layer()].find(adjPoint);
                if (iter == _totalPoint2NodeMV[adjPoint.layer()].end()) {
                    AstarNode* newNode = new AstarNode(adjPoint);
                    _totalPoint2NodeMV[adjPoint.layer()][adjPoint] = newNode;
                    currentNode->adjV.emplace_back(newNode);
                } else {
                    // assert(false);
                    currentNode->adjV.emplace_back(iter->second);
                }
            }
        }

        // relaxation
        for (AstarNode* neiNode : currentNode->adjV) {
            // skip explored node
            if (neiNode->explored[groupId]) continue;

            // skip invalid neighbors

            DType newCostG = currentNode->costG[groupId] + costG(currentNode, neiNode, groupId);

            // need to update
            if (newCostG < neiNode->costG[groupId]) {
                // printf("neighbor point: %s\n", neiNode->coor.str().c_str());
                // set parent
                neiNode->parent[groupId] = currentNode;

                // set cost
                DType dNearest;
                Point3D pNearest;
                kdtree[groupId ^ 1].nearestSearch(neiNode->coor, pNearest, dNearest);
                
                DType new_costF = newCostG + (dNearest);

                // printf("dNearest: %d\n", dNearest);
                // printf("pNearest: %s\n", pNearest.str().c_str());

                neiNode->costG[groupId] = newCostG;
                neiNode->costF[groupId] = new_costF;

                if (groupId == 0) {
                    auto it = iterMap0.find(neiNode);
                    if (it != iterMap0.end())
                        heap0.modify(it->second, neiNode);
                    else
                        iterMap0[neiNode] = heap0.push(neiNode);
                }
                else {
                    auto it = iterMap1.find(neiNode);
                    if (it != iterMap1.end())
                        heap1.modify(it->second, neiNode);
                    else
                        iterMap1[neiNode] = heap1.push(neiNode);
                }
            }
        }
        // set explored
        currentNode->explored[groupId] = true;
    }
    // printf("heap0 empty: %d\n", heap0.empty());
    // printf("heap1 empty: %d\n", heap1.empty());
    return false;
}
void DrAstar::findNeighbors(const Point3D& p, vector<Point3D>& retV) const {
    // Same layer
    // printf("point: %s\n", p.str().c_str());
    int layer = p.layer();

    if (layer == 1) {
        if (p[0] > 0) {
            retV.emplace_back(p[0] - 1, p[1], p[2]);
        }
        if (p[0] < db.size()[0] - 1) {
            retV.emplace_back(p[0] + 1, p[1], p[2]);
        }
        retV.emplace_back(p[0], p[1], 2);
    }
    else if (layer == 2) {
        if (p[1] > 0) {
            retV.emplace_back(p[0], p[1] - 1, p[2]);
        }
        if (p[1] < db.size()[1] - 1) {
            retV.emplace_back(p[0], p[1] + 1, p[2]);
        }
        retV.emplace_back(p[0], p[1], 1);
    }
}
void DrAstar::backtrace(AstarNode* u) {
    // cout<< "curent : " <<u->coor <<endl;
    int srcCompId = backtrace(u, 0);
    int tarCompId = backtrace(u, 1);
    assert(srcCompId == _srcCompId);
    // cout<<"here back" <<endl;

    _componentDS.merge(srcCompId, tarCompId);
}
int DrAstar::backtrace(AstarNode* x, int i) {
    if (x->parent[i] == nullptr) {
        assert(x->componentId != -1);
        return _componentDS.find(x->componentId);
    }

    Point3D startPoint = x->coor;
    AstarNode* currentNode = x->parent[i];
    AstarNode* parentNode = currentNode->parent[i];

    // handle the spetial criterion
    assert(startPoint.layer() == currentNode->coor.layer() || startPoint.point() == currentNode->coor.point());

    EPathDir start2currentDir;
    EPathDir current2parentDir = pathDir(startPoint, currentNode->coor);
    while (true) {
        if (parentNode == nullptr) {
            // printf("false\n");
            addRoutedWireOrVia(startPoint, currentNode->coor);
            // printf("starPoint: %s, currentNode->coor: %s\n", startPoint.str().c_str(), currentNode->coor.str().c_str());

            assert(currentNode->componentId != -1);
            return _componentDS.find(currentNode->componentId);
        }

        start2currentDir = current2parentDir;
        current2parentDir = pathDir(currentNode->coor, parentNode->coor);

        // printf("start2currentDir: %d\n", start2currentDir);
        // printf("current2parentDir: %d\n", current2parentDir);
        if (current2parentDir != start2currentDir || current2parentDir == ZF || current2parentDir == ZR) {
            addRoutedWireOrVia(startPoint, currentNode->coor);
            startPoint = currentNode->coor;
            // printf("starPoint: %s, currentNode->coor: %s, parentNode->coor: %s\n", startPoint.str().c_str(), currentNode->coor.str().c_str(), parentNode->coor.str().c_str());
        }
        if (currentNode->componentId == -1) {
            _componentSV[_srcCompId].insert(currentNode->coor);
            currentNode->componentId = _srcCompId;
        }

        currentNode = parentNode;
        parentNode = currentNode->parent[i];
    }
    assert(false);
    return 0;
};
void DrAstar::addRoutedWireOrVia(const Point3D& start, const Point3D& current) {
    // printf("start: %s\n", start.str().c_str());
    // printf("current: %s\n", current.str().c_str());
    // printf("_apP name: %s\n", _apP->name().c_str());
    Net& net = *_netP;
    assert(Segment3D(start,current).check());
    net.pushSegment({start, current});
}
EPathDir DrAstar::pathDir(const Point3D& p1, const Point3D& p2) const {
    // From p1 to p2
    // assert( int(p1[0] != p2[0]) + int(p1[1] != p2[1]) + int(p1.layer() != p2.layer()) == 1);
    if (p1.layer() != p2.layer()) {
        if (p1.layer() < p2.layer()) return ZR;
        else if (p1.layer() > p2.layer()) return ZF;
        else { assert(false); }
    }
    else if (p1[0] != p2[0]) {
        if (p1[0] < p2[0]) return XR;
        else if (p1[0] > p2[0]) return XF;
        else { assert(false); }
    } else if (p1[1] != p2[1]) {
        if (p1[1] < p2[1]) return YR;
        else if (p1[1] > p2[1]) return YF;
        else { assert(false); }
    } 
    else assert(false);
}
DType DrAstar::costG(AstarNode* u, AstarNode* v, int groupId)  {
    return (abs(u->coor[0] - v->coor[0]) + abs(u->coor[1] - v->coor[1]) + (abs(u->coor.layer() - v->coor.layer()) * _astarParam.viaCost) 
}

DType DrAstar::costH(const Point3D& u, const Point3D& v) const {
    return abs(u[0] - v[0]) + abs(u[1] - v[1]) + (abs(u.layer() - v.layer()) * _astarParam.viaCost);
}


void DrAstar::clear() {
    for (unsigned i = 0; i < _totalPoint2NodeMV.size(); ++i) {
        for (auto n : _totalPoint2NodeMV[i]) {
            delete n.second;
        }
    }
}
