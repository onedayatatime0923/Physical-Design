#ifndef bstar__state_h
#define bstar__state_h

// #define STATE_DEBUG

#include "bstarGlobal.hpp"
#include "db/db.hpp"

class Bstar;

class PerturbSeed{
    public:
        PerturbSeed() : _operation(OpNull), _insertDir(InsertNull) { _operand.reserve(2); }

        enum Operation { OpRotation, OpSwap, OpDeleteInsert, OpNull, };

        enum InsertDir { InsertLeft, InsertRight, InsertNull, };

        // for rotation
        int                 rotationId() { assert(_operand.size() == 1); return _operand[0]; }
        // for swap
        const vector<int>&  swapId() { assert(_operand.size() == 2); return _operand; }
        // for delete insert
        int                 deleteId () { assert(_operand.size() == 2); return _operand[0]; }
        int                 insertId () { assert(_operand.size() == 2); return _operand[1]; }
        InsertDir           insertDir() { return _insertDir; }

        friend Bstar;
    private:
        vector<int> _operand;
        Operation   _operation;
        InsertDir   _insertDir;
};
class State{
public:
    // for dummy root vertex
    State           (int n) : _nodeV(n + 1), _locationV(n) { _rotationV.assign(n, false); };
    ~State          () {}

    void    print   ();
    friend Bstar;
private:

    struct Node{
        Node() :  parent(-1), leftChild(-1), rightChild(-1) {};
        
        static const int nullNode = -1;

        int parent;
        int leftChild;
        int rightChild;
    };

    int     blockSize   () { return _nodeV.size() - 1; }
    int     dummy       () { return _nodeV.size() - 1; }
    int     root        () { return _nodeV.back().leftChild; }
    int     parent      (int i) { return _nodeV[i].parent; }
    int     leftChild   (int i) { return _nodeV[i].leftChild; }
    int     rightChild  (int i) { return _nodeV[i].rightChild; }

    void    setParent   (int i, int p) { _nodeV[i].parent = p; }
    void    setLeftChild(int i, int l) { _nodeV[i].leftChild = l; }
    void    setRightChild(int i, int r) { _nodeV[i].rightChild = r; }
    void    connectLeftChild(int p, int c) { setLeftChild(p,c); if (c != Node::nullNode) setParent(c,p); }
    void    connectRightChild(int p, int c) { setRightChild(p,c); if (c != Node::nullNode) setParent(c,p); }
    
    void    rotate      (int a);

    void    swap        (int a, int b);
    void    rename      (int a, int b);

    void    deleteInsert(int a, int b, PerturbSeed::InsertDir dir);

    void    pack        (DB& db);
    void    pack        (DB& db, int id, list<int>& contourL, const list<int>::iterator& it);
    void    updateSize  (const Rect& size);
    int     findY       (int id, list<int>& contourL, list<int>::iterator& it);

    void    calculateWireLength(DB& db);
    void    calculateWireLength(Net& net);

    void    print       (int i, int level, int type = 0);
    void    dumpFile    (const string& name, DB& db);
    void    printContour(list<int>& contourL);

    const Point&    size() { return _size; }
    int             wireLength() { return _wireLength; }

    // state information
    vector<Node>    _nodeV;
    vector<bool>    _rotationV;

    // being filled by packing
    vector<Rect>    _locationV;
    Point           _size;
    int             _wireLength;
};


#endif

