#ifndef bstar__bstar_h
#define bstar__bstar_h

#include "bstarGlobal.hpp"
#include "state.hpp"
#include "db/db.hpp"

class Bstar{
public:
    Bstar   (DB& db) : _db(db) {};

    void    solve           ();
private:
    void    initState       (State& state);
    void    perturbState    (State& state);
    void    calculateCost   (State& state);

    Block&  block           (int i) { return _db.block(i); }


    DB&     _db;
};


#endif

