
#include "drAstar.hpp"

void DrAstar::route() {
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
}
int DrAstar::routeNet() {
    Net& net = *_netP;
    printf("net name: %s\n", net.name().c_str());
    #ifdef DRASTAR_DEBUG
    printf("Connecting %s\n", net.name().c_str());
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
    int originalSize = _componentDS.nSets();

    while (_componentDS.nSets() > 1) {
        // Determine source netCompId
        // cout<<"// Determine source netCompId"<<endl;
        _srcCompId = _componentDS.find(0);

        if (findPathAss()) {
            // printf("Net %s path found. (%d / %d)\n", _netP->name().c_str(), _componentDS.nSets(), originalSize);
        } else {
            printf("Net %s\n", net.name().c_str());
            printf("ApPair failed. QQ\n");
            assert(false);
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


    for (int i = 0; i < (int)net.pointSize(); ++i) {
        initPoint(net.point(i));
    }

    _componentDS.init(_componentSV.size());
    #ifdef DRASTAR_DEBUG
    printf("after init component\n");
    for (int i = 0; i < (int)_componentSV.size(); ++i) {
        printf("component: %d\n", i);
        for (auto it = _componentSV[i].begin(); it != _componentSV[i].end(); ++it) {
            printf("  point: %s\n", it->str().c_str());
        }
    }
    getchar();
    #endif
}
void DrAstar::initPoint(const Point& p) {
    int componentId = _componentSV.size();
    pushComponentSV();

    Point3D point3D(p, 1);
    _componentSV[componentId].insert(point3D);
    if (_totalPoint2NodeMV[1].count(point3D) == 0) {
        AstarNode* newNode = new AstarNode(point3D, componentId);
        _totalPoint2NodeMV[1][point3D] = newNode;
    }
    else {
        auto it = _totalPoint2NodeMV[point3D.layer()].find(point3D);
        int id = it->second->componentId;
        if (id == -1) {
            it->second->componentId = componentId;
        }else assert(false);
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
    KDTree2D kdtree[2];
    // kdtree[0].setZDirectWeight(_astarParam.viaCost);
    // kdtree[1].setZDirectWeight(_astarParam.viaCost);
    for (int componentId = 0, nComps = _componentSV.size(); componentId < nComps; ++componentId) {
        // printf("componentId: %d\n", componentId);
        // printf("_componentSV[componentId]): %d\n", _componentDS.find(componentId));
        for (const Point3D& p : _componentSV[componentId]) {
            if (_componentDS.find(componentId) == _srcCompId) {
                // printf("insert into 0: %s\n", p.str().c_str());
                kdtree[0].insert(p.point());
            } else {
                // printf("insert into 1: %s\n", p.str().c_str());
                kdtree[1].insert(p.point());
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
        Point pNearest;
        int dNearest;
        int costF;
        for (const Point3D& p : _componentSV[componentId]) {
            // printf("componentId: %d\n", componentId);
            // printf("_componentDS.find(componentId): %d\n", _componentDS.find(componentId));
            // printf("srcCompId: %d\n", _srcCompId);
            if (_componentDS.find(componentId) == _srcCompId) {
                kdtree[1].nearestSearch(p.point(), pNearest, dNearest);
                costF = (0 + dNearest);
                // assert(dNearest == costH(p, pNearest));
            }
            else {
                kdtree[0].nearestSearch(p.point(), pNearest, dNearest);
                costF = (0 + dNearest);
                // assert(dNearest == costH(p, pNearest));
            }
            auto iter = _totalPoint2NodeMV[p.layer()].find(p);
            assert(iter != _totalPoint2NodeMV[p.layer()].end());
            AstarNode* node = iter->second;
            if (_componentDS.find(componentId) == _srcCompId) {
                node->costG[0] = 0;
                node->costF[0] = costF;
                relax(node, 0, kdtree, heap0, heap1, iterMap0, iterMap1);
            } else {
                node->costG[1] = 0;
                node->costF[1] = costF;
                relax(node, 1, kdtree, heap0, heap1, iterMap0, iterMap1);
            }
        }
    }
    // cout<<"end "<<endl;


    while ( !heap0.empty() && !heap1.empty() ) {
    // Extract min
        AstarNode* currentNode = nullptr;
        int groupId = 0; // 0 or 1. 0: src to tar; 1: tar to src
        bool pathFound = false;
        #ifdef DRASTAR_DEBUG
        printf("heap0 top: %s\n", heap0.top()->coor.str().c_str());
        printf("heap0 cost: %d\n", heap0.top()->costF[0]);
        printf("heap1 top: %s\n", heap1.top()->coor.str().c_str());
        printf("heap1 cost: %d\n", heap1.top()->costF[1]);
        #endif
        if (heap0.top()->costF[0] < heap1.top()->costF[1]) {
            currentNode = heap0.top();
            heap0.pop();
            groupId = 0;
            pathFound = currentNode->explored[1];
        } 
        else {
            currentNode = heap1.top();
            heap1.pop();
            groupId = 1;
            pathFound = currentNode->explored[0];
        }
        #ifdef DRASTAR_DEBUG
        printf("groupId: %d\n", groupId);
        printf("current coor: %s\n", currentNode->coor.str().c_str());
        #endif

        // path found
        if (pathFound) {
            backtrace(currentNode);
            return true;
        }

        relax(currentNode, groupId, kdtree, heap0, heap1, iterMap0, iterMap1);
    }
    // printf("heap0 empty: %d\n", heap0.empty());
    // printf("heap1 empty: %d\n", heap1.empty());
    return false;
}
void DrAstar::relax(AstarNode* currentNode, int groupId, KDTree2D(&kdtree)[2], DrAstar::NodePHeap0Type& heap0, DrAstar::NodePHeap1Type& heap1, DrAstar::IterMap0Type& iterMap0, DrAstar::IterMap1Type& iterMap1) {
    // printf("relax\n");
    // find neighbors
    if (currentNode->adjV.empty()) {
        vector<Point3D > vAdjPts;
        findNeighbors(currentNode->coor, vAdjPts);
        for (Point3D& adjPoint : vAdjPts) {
            #ifdef DRASTAR_DEBUG
            printf("adj point: %s\n", adjPoint.str().c_str());
            #endif
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

    for (AstarNode* neiNode : currentNode->adjV) {
        // skip explored node
        if (neiNode->explored[groupId]) continue;

        // skip invalid neighbors

        float newCostG = currentNode->costG[groupId] + costG(currentNode, neiNode, groupId);

        // need to update
        if (newCostG < neiNode->costG[groupId]) {
            #ifdef DRASTAR_DEBUG
            printf("neighbor point: %s\n", neiNode->coor.str().c_str());
            #endif
            // set parent
            neiNode->parent[groupId] = currentNode;

            // set cost
            int dNearest;
            Point pNearest;

            kdtree[groupId ^ 1].nearestSearch(neiNode->coor.point(), pNearest, dNearest);
            
            int new_costF = newCostG + (dNearest);
            #ifdef DRASTAR_DEBUG
            printf("costG: %d, costH: %d\n", newCostG, dNearest);
            #endif

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
void DrAstar::findNeighbors(const Point3D& p, vector<Point3D>& retV) const {
    // Same layer
    // printf("point: %s\n", p.str().c_str());
    int layer = p.layer();

    if (layer == 1) {
        if (p[0] > 0) {
            retV.emplace_back(p[0] - 1, p[1], p[2]);
        }
        if (p[0] < _db.size()[0] - 1) {
            retV.emplace_back(p[0] + 1, p[1], p[2]);
        }
        retV.emplace_back(p[0], p[1], 2);
    }
    else if (layer == 2) {
        if (p[1] > 0) {
            retV.emplace_back(p[0], p[1] - 1, p[2]);
        }
        if (p[1] < _db.size()[1] - 1) {
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
    _db.capacityTable().addSegment(start, current);
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
float DrAstar::costG(AstarNode* u, AstarNode* v, int groupId)  {
    return abs(u->coor[0] - v->coor[0]) + abs(u->coor[1] - v->coor[1]) + (abs(u->coor.layer() - v->coor.layer()) * _astarParam.viaCost)
        + _db.capacityTable().cost(u->coor, v->coor);
}

int DrAstar::costH(const Point3D& u, const Point3D& v) const {
    return abs(u[0] - v[0]) + abs(u[1] - v[1]) + (abs(u.layer() - v.layer()) * _astarParam.viaCost);
}


void DrAstar::clear() {
    for (unsigned i = 0; i < _totalPoint2NodeMV.size(); ++i) {
        for (auto n : _totalPoint2NodeMV[i]) {
            delete n.second;
        }
    }
}

