#ifndef bstar__bstar_h
#define bstar__bstar_h

// #define BSTAR_DEBUG

#include "bstarGlobal.hpp"
#include "state.hpp"
#include "db/db.hpp"

class Bstar{
public:
    Bstar   (DB& db) : _db(db) {};

    void    solve           ();
private:
    struct BstarParam {
        int     startTemperature  = 100000;
        float   numStepCoefficient = 30;
        float   gamma = 0.95;
        int     epsilon = 1;
        float   aspectRationCost = 1000000000;

    } bstarParam;

    void    initState       (State& state);
    void    perturbState    (State& state, PerturbSeed& seed);
    void    calculateCost   (State& state);

    Block&  block           (int i) { return _db.block(i); }
    void    randomPerturbSeed(State& state, PerturbSeed& seed);

    float   cost            (State& state);


    DB&     _db;
};


#endif

