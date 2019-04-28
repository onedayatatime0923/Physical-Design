
#ifndef dr__drAstar_h
#define ds__drAstar_h

// #define DRASTAR_DEBUG

#include "db/db.hpp"
#include "dataStructure/pqueue.hpp"
#include "dataStructure/hash.hpp"

namespace AStar
{

    using CoordinateList = vector<Point3D>;

    struct Node {
        float G, H;
        Point3D coordinates;
        Node *parent;

        Node(Point3D coord_, Node *parent_ = nullptr);
        float getScore() const { return G + H; }
    };
    struct NodeCompareCoor {
        bool operator () (const Node* n1, const Node* n2) {
            return n1->coordinates < n2->coordinates;
        }
    };
    struct NodeCompareCost {
        bool operator () (const Node* n1, const Node* n2) {
            if      (n1->getScore() != n2->getScore()) return n1->getScore() > n2->getScore();
            else if (n1->G != n2->G) return n1->G < n2->G;
            else return false;
        }
    };
    using NodeSet = set<Node*, NodeCompareCoor>;
    typedef PairingHeap<Node*, NodeCompareCost>                 NodePHeapType;
    typedef map<Node*, NodePHeapType::point_iterator>           NodeIterMapType;

    class Generator {

        void findNeighbors(const Point3D& p, vector<Point3D>& retV) const;
        bool detectCollision(Point3D coordinates_);
        Node* findNodeOnList(NodeSet& nodes_, Point3D coordinates_);
        void releaseNodes(NodeSet& nodes_);
        void releaseNodes(NodeIterMapType& nodes_);

    public:
        Generator(DB& db_) : db(db_) {};
        void findPath(Net& net, Point3D source_, Point3D target_);
        void addCollision(Point3D coordinates_);
        void removeCollision(Point3D coordinates_);
        void clearCollisions();

    private:
        DB& db;
    };

}




#endif
