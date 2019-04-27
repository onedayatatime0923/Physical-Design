
#ifndef dr__drAstar_h
#define ds__drAstar_h

#include "db/db.hpp"

class DrAstar {
public:
  DrAstar(DB& db) : _db(db) {};
  ~DrAstar() {}

  void route();
  void clear();

private:
    struct AstarNode;
    struct AstarNodeComp0;
    struct AstarNodeComp1;

    void    init                ();

    int     routeNet            (Net& net);
    void    initComponent       (Net& net);
    void    initPoint           (const Point& p, set<pair<int, int>>& sameComp);
    void    pushComponentSV     ();

    bool    findPathAss         ();
    void    findNeighbors       (const Point3D& p, vector<Point3D>& vRet) const;
    void    nearestPointOnTrack (DType x, Track* trackP, DType& upper, DType& lower) const;
    void    upDownNeighbor      (const Point3D& p, const int& layerId, vector<Point3D>& vRet) const;

    void    backtrace           (AstarNode* u);
    int     backtrace           (AstarNode* x, int i);
    int     minAreaOverhead     (int layer ,int length, bool hasSpace);

    void    connectNodeToPin    (AstarNode* currentNode);
    void    addRoutedWireOrVia  (const Point3D& start, const Point3D& current);
    void    addPatch            (const Point3D& start, const Point3D& current,const Point3D& parent);
    void    addVia              (const Point3D& a, const Point3D& b);
    void    connectPointToPin   (const Point3D& point, Pin* pinP);
    EPathDir    pathDir         (const Point3D& p1, const Point3D& p2) const;
    int     directLength(Point3D &p1, Point3D &p2, bool preferred);
    
    
    bool    prefDir        (const Point3D& p1, const Point3D& p2) const;
    bool    connectGuide   (const Point3D& p) const;
    Via*    chooseVia       (const Point3D& p1, const Point3D& p2) const;
    // DType   costG(const Point3D& u, const Point3D& v) const;
    DType   costG(AstarNode* u,AstarNode* v, int groupId);
    DType   costH(const Point3D& u, const Point3D& v) const;


    bool    onGrid  (const Point3D& p) const;
    bool    onTrack (DType x, Track* track) const;

    // member

    DB&         _db;
    Net*        _netP;
    // Astar Parameter 
    /////////////////////////////////////////////////////
    struct AstarParam {
      DType viaCost = 250;
      DType minAreaCost = 800;
      DType jogCost = 2;
    } _astarParam;



    typedef DenseHashSet<Point3D, Point3D::Hasher>              PointSetType;
    typedef DenseHashMap<Point3D, AstarNode*, Point3D::Hasher>  PointMapType;


    vector<PointSetType>    _componentSV;
    vector<PointMapType>    _totalPoint2NodeMV;     // layerId -> point hashMap

    int                     _srcCompId;
    UFDS                    _componentDS;           // size = _componentSV.size();


    struct AstarNode {
        AstarNode(const Point3D &co = Point3D(), const int c = -1) : coor(co), componentId(c), pinP(nullptr), segmentP(nullptr), upperbound(0), lowerbound(0) { initialize(); }

        Point3D             coor;                                   // set when neibor node is selected
        AstarNode*          parent[2];                              // set when neibor node is selected
        int                 costG[2];                               // set when neibor node is selected
        int                 costF[2];                               // set when neibor node is selected

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

};
#endif