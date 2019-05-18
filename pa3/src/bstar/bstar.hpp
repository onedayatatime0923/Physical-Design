#ifndef bstar__bstar_h
#define bstar__bstar_h

// #define BSTAR_DEBUG

#include "bstarGlobal.hpp"
#include "state.hpp"
#include "db/db.hpp"

class Bstar{
public:
    Bstar   (DB& db) : _db(db), _finalState(_db.blockSize()) {};

    void    solve           ();
    
    const Point&    finalSize       () { return _finalState.size(); }
    int             finalWireLength () { return _finalState.wireLength(); }
    int             finalLocationSize() { return _finalState.blockSize(); }
    const Rect&     finalLocation   (int i) { return _finalState.location(i); }
private:
    struct BstarParam {
        int     normSample = 20;
        float   startTemperature  = 10000000;
        float   numStepCoefficient = 20;
        float   gamma = 0.85;
        float   epsilon = 0.001;
        float   aspectRationCostStart = 0.03;
        float   aspectRationPower = 1;
        float   aspectRationCostIterationIncrememt = 1;
        float   aspectRationCostIncrememt = 1.00000;
        float   aspectRationCostDecrement = 0.99;
        float   quitCondition = 0.95;

    } bstarParam;

    void    anealing        (State& state);
    void    norm            (State& state);
    void    initState       (State& state);
    void    perturbState    (State& state, PerturbSeed& seed);
    void    calculateCost   (State& state);

    Block&  block           (int i) { return _db.block(i); }
    void    randomPerturbSeed(State& state, PerturbSeed& seed);

    float   costWithRatio   (State& state);
    float   costWithoutRatio(State& state);
    bool    check           (State& state);


    DB&     _db;
    State   _finalState;

    float   _RNorm;
    float   _aspectRationCost;
};


#endif

