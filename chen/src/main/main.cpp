
#include "misc/global.hpp"
#include "parser/parser.hpp"
#include "db/db.hpp"
#include "dr/drAstar.hpp"

using namespace std;

int main(int argc, char** argv) {

    DB db; 
    SelfParser::parse(argc, argv, db);
    AStar::Generator astar(db);
    for (int i = 0; i < db.netSize(); ++i) {
        Net& n = db.net(i);
        printf("net: %s\n", n.name().c_str());
        for (int j = 0; j < n.pointSize(); ++j) {
            Point3D s(n.point(j).first, 1);
            Point3D t(n.point(j).second, 1);
            astar.findPath(n, s, t);
        }
    }
    // db.print();
    SelfParser::dumpFile(argc, argv, db);
    
}




