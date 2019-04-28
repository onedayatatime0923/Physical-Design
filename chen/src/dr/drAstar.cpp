
#include "drAstar.hpp"


AStar::Node::Node(Point3D coordinates_, Node *parent_) {
    parent = parent_;
    coordinates = coordinates_;
    G = H = 0;
}



void AStar::Generator::findPath(Net& net, Point3D source_, Point3D target_) {
    Node *current = new Node(source_);
    NodePHeapType openPHeap;
    NodeIterMapType openIterMap;
    NodeSet closedSet;
    // openSet.insert(current);
    openIterMap[current] = openPHeap.push(current);

    while (!openPHeap.empty()) {
        current = openPHeap.top();
        // current = *openSet.begin();
        // for (auto node : openSet) {
        //     if (node->getScore() <= current->getScore()) {
        //         current = node;
        //     }
        // }
        // printf("current point: %s\n", current->coordinates.str().c_str());
        // getchar();

        if (current->coordinates == target_) {
            break;
        }

        closedSet.insert(current);
        openPHeap.pop();
        openIterMap.erase(current);
        // openSet.erase(current);

        vector<Point3D > coorV;
        findNeighbors(current->coordinates, coorV);
        for (int i = 0; i < coorV.size(); ++i) {
            Point3D newCoordinates(coorV[i]);
            if (detectCollision(newCoordinates) ||
                findNodeOnList(closedSet, newCoordinates)) {
                continue;
            }

            float totalCost = current->G + 1 + db.capacityTable().cost(current->coordinates, newCoordinates);

            // printf("newCoordinates: %s\n", newCoordinates.str().c_str());
            Node successor(newCoordinates, current);
            auto it = openIterMap.find(&successor);
            if (it == openIterMap.end()) {
                Node* nextNode = new Node(newCoordinates, current);
                nextNode->G = totalCost;
                nextNode->H = Point3D::Mdistance(nextNode->coordinates, target_);
                // openSet.insert(nextNode);
                openIterMap[nextNode] = openPHeap.push(nextNode);
            }
            else if (totalCost < it->first->G) {
                it->first->parent = current;
                it->first->G = totalCost;
                openPHeap.modify(it->second, it->first);
            }
        }
    }

    CoordinateList path;
    net.pushSegment({current->coordinates, current->parent->coordinates});
    while (current->parent != nullptr) {
        net.pushSegment({current->coordinates, current->parent->coordinates});
        db.capacityTable().addSegment(current->coordinates, current->parent->coordinates);
        current = current->parent;
    }

    releaseNodes(openIterMap);
    releaseNodes(closedSet);

}

void AStar::Generator::findNeighbors(const Point3D& p, vector<Point3D>& retV) const {
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
AStar::Node* AStar::Generator::findNodeOnList(NodeSet& nodes_, Point3D coordinates_) {
    Node n(coordinates_);
    auto it = nodes_.find(&n);
    if (it == nodes_.end()) return nullptr;
    else return *it;
}

void AStar::Generator::releaseNodes(NodeSet& nodes_) {
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
        delete *it;
    }
}
void AStar::Generator::releaseNodes(NodeIterMapType& nodes_) {
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
        delete it->first;
    }
}

bool AStar::Generator::detectCollision(Point3D coordinates_) {
    if (coordinates_[0] < 0 || coordinates_[0] >= db.size()[0] ||
        coordinates_[1] < 0 || coordinates_[1] >= db.size()[1])
        return true;
    return false;
}

