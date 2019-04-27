
#include "misc/global.hpp"
#include "parser/parser.hpp"
#include "db/db.hpp"
#include "dr/drAstar.hpp"

using namespace std;

int main(int argc, char** argv) {

    DB db; 
    SelfParser::parse(argc, argv, db);
    DrAstar drAstar(db);
    drAstar.route();
    SelfParser::dumpFile(argc, argv, db);
    // db.print();
    
}




