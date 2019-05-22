
#include <stdio.h>
#include <vector>
#include "../dataStructure/disjointSet.hpp"

using namespace std;

int main(int argc, char** argv) {
    
    UFDS ds;
    ds.init(10);
    // ds.init(3);


    ds.unionSet(1,2);
    printf("isSame: %d\n", ds.isSameSet(1,2));
    printf("number: %d\n", ds.numDisjointSets());


}




