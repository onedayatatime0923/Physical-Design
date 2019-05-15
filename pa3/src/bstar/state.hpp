#ifndef bstar__state_h
#define bstar__state_h

#include "bstarGlobal.hpp"

class Bstar;

class State{
public:
    // for dummy root vertex
    State           (int n) : _nodeV(n + 1) {};

    void    print   ();
    friend Bstar;
private:

    struct Node{
        Node() :  parent(-1), leftChild(-1), rightChild(-1) {};
        
        static const unsigned nullNode = (1 << 10) - 1;

        unsigned parent:10;
        unsigned leftChild:10;
        unsigned rightChild:10;
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
    

    void    print       (int i, int level);

    vector<Node>    _nodeV;
    int             _wireLength;
    int             _area;
};

class PerturbSeed{
    public:
        enum Operation { OpRotation, };

        // for rotation
        int rotationId() { assert(_operand.size() == 1); return _operand[0]; }

        friend Bstar;
    private:
        Operation   _operation;
        vector<int> _operand;
}

#endif

