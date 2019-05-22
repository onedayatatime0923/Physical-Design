#ifndef __TREE_H__
#define __TREE_H__

#include <iostream>
#include <vector>
#include "routingdb.hpp"

using namespace std;

namespace SelfParser {
    class RoutingTree
    {
        public:
        RoutingTree() {;}
        ~RoutingTree() {;}

        int ManhattanDistance(Pin& p1, Pin& p2);
        void MinimumSpanningTreeConstruction();
        void ShowInfo();

        private:
        void MinimumSpanningTreeConstruction(Net& n);
    };
}

#endif
