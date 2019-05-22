
#ifndef dr__drAstar_h
#define ds__drAstar_h

// #define DRASTAR_DEBUG

#include "db/db.hpp"
#include "dataStructure/hash.hpp"
#include "dataStructure/disjointSet.hpp"
#include "dataStructure/pqueue.hpp"
#include "dataStructure/kdtree.hpp"

enum EPathDir {
    XF = 0,
    XR = 1,
    YF = 2,
    YR = 3,
    ZF = 4,
    ZR = 5,
    NONE = 6
};
class DrAstar {
public:
    DrAstar(DB& db) : _db(db) {};
    ~DrAstar() { clear(); }

    void route(Net& net);
    void clear();

    struct AstarNode {
        AstarNode(const Point3D &co = Point3D(), const int c = -1) : coor(co), componentId(c) { initialize(); }

        Point3D             coor;                                   // set when neibor node is selected
        AstarNode*          parent[2];                              // set when neibor node is selected
        float               costG[2];                               // set when neibor node is selected
        float               costF[2];                               // set when neibor node is selected

        int                 componentId; // -1 for non-comp points  // set when backtrace

        bool                explored[2];                            // set when selected
        vector<AstarNode*>  adjV;                                   // set when selected

        void initialize() {
            parent[0] = parent[1] = nullptr;
            costG[0] = costG[1] = costF[0] = costF[1] = INT_MAX;
            explored[0] = explored[1] = false;
        }
        void resetComponentId() {
            componentId = -1;
        }
    };
    struct AstarNodeComp0 {
        bool operator () (const AstarNode* n1, const AstarNode* n2) {
            if      (n1->costF[0] != n2->costF[0]) return n1->costF[0] > n2->costF[0];
            else if (n1->costG[0] != n2->costG[0]) return n1->costG[0] < n2->costG[0];
            else return false;
        }
    };
  
    struct AstarNodeComp1 {
        bool operator () (const AstarNode* n1, const AstarNode* n2) {
            if      (n1->costF[1] != n2->costF[1]) return n1->costF[1] > n2->costF[1];
            else if (n1->costG[1] != n2->costG[1]) return n1->costG[1] < n2->costG[1];
            else return false;
        }
    };
    typedef PairingHeap<AstarNode*, AstarNodeComp0>             NodePHeap0Type;
    typedef PairingHeap<AstarNode*, AstarNodeComp1>             NodePHeap1Type;
    typedef DenseHashMap<AstarNode*, NodePHeap0Type::point_iterator> IterMap0Type;
    typedef DenseHashMap<AstarNode*, NodePHeap1Type::point_iterator> IterMap1Type;
private:

    void    init                ();

    int     routeNet            ();
    void    initComponent       ();
    void    initPoint           (const Point& p);
    void    pushComponentSV     ();

    bool    findPathAss         ();
    void    relax               (AstarNode* currentNode, int groupId, KDTree3D(&kdtree)[2], NodePHeap0Type& heap0, NodePHeap1Type& heap1, IterMap0Type& iterMap0, IterMap1Type& iterMap1);
    void    findNeighbors       (const Point3D& p, vector<Point3D>& vRet) const;

    void    backtrace           (AstarNode* u);
    int     backtrace           (AstarNode* x, int i);

    void    addRoutedWireOrVia  (const Point3D& start, const Point3D& current);
    EPathDir    pathDir         (const Point3D& p1, const Point3D& p2) const;
    
    float   costG(AstarNode* u,AstarNode* v, int groupId);
    int     costH(const Point3D& u, const Point3D& v) const;


    // member

    DB&         _db;
    Net*        _netP;
    // Astar Parameter 
    /////////////////////////////////////////////////////
    struct AstarParam {
      int viaCost = 1;
    } _astarParam;



    typedef DenseHashSet<Point3D, Point3D::Hasher>              PointSetType;
    typedef DenseHashMap<Point3D, AstarNode*, Point3D::Hasher>  PointMapType;


    vector<PointSetType>    _componentSV;
    vector<PointMapType>    _totalPoint2NodeMV;     // layerId -> point hashMap

    int                     _srcCompId;
    UF                      _componentDS;           // size = _componentSV.size();



};
#endif
