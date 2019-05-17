#ifndef bstar__bstar_h
#define bstar__bstar_h

#define BSTAR_DEBUG

#include "bstarGlobal.hpp"
#include "state.hpp"
#include "db/db.hpp"

class Bstar{
public:
    Bstar   (DB& db) : _db(db) {};

    void    solve           ();
private:
    struct BstarParam {
        int     startTemperature  = INT_MAX;
        float   numStepCoefficient = 5;
        float   gamma = 0.8;
        int     epsilon = 1000;
    } bstarParam;
    void    initState       (State& state);
    void    perturbState    (State& state, PerturbSeed& seed);
    void    calculateCost   (State& state);

    Block&  block           (int i) { return _db.block(i); }
    void    randomPerturbSeed(State& state, PerturbSeed& seed);

    int     cost            (State& state);


    DB&     _db;
};


#endif

