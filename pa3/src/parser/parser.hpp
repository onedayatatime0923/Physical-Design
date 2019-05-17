
#ifndef parser__parser_h
#define parser__parser_h

#include "db/db.hpp"
#include "argparse/argparse.hpp"

class Parser{
public:
    Parser                  (Argparse& opt) : _opt(opt) {};
    void        parse       (DB& db);

private:

    Argparse& _opt;

};


#endif
