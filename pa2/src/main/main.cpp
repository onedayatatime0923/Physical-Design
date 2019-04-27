
#include "misc/global.hpp"
#include "parser/parser.hpp"
#include "db/db.hpp"

using namespace std;

int main(int argc, char** argv) {

    DB db; 
    SelfParser::parse(argc, argv, db);
}




