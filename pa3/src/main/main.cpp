
#include "misc/global.hpp"
#include "argparse/argparse.hpp"
#include "parser/parser.hpp"
#include "db/db.hpp"
#include "bstar/bstar.hpp"

using namespace std;

int main(int argc, char** argv) {

    Argparse opt(argc, argv);
    printf("apha: %f\n", opt.apha());
    printf("inputBlock: %s\n", opt.inputBlock().c_str());
    printf("inputNet: %s\n", opt.inputNet().c_str());
    printf("output: %s\n", opt.output().c_str());
    
    DB db; 
    {
        Parser parser;
        parser.parse(opt.inputBlock(), opt.inputNet(), db);
    }
    // db.print();
    {
        Bstar bstar(db);
        bstar.solve();
    }
}




