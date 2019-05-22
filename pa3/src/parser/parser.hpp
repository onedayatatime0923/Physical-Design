
#ifndef parser__parser_h
#define parser__parser_h

#include "db/db.hpp"
#include "bstar/bstar.hpp"
#include "argparse/argparse.hpp"

class Parser{
public:
    Parser                  (Argparse& opt) : _opt(opt) {};
    void        parse       (DB& db);
    void        dumpFile    (DB& db, Bstar& bstar, double time);

private:

    Argparse& _opt;

};


#endif
