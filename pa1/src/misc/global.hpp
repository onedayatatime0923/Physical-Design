
#ifndef misc__global_h
#define misc__global_h

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <string>
#include <vector>

#pragma GCC diagnostic ignored "-Wunused-result"
////////////////////////////////////////////////////////////////////////
///                         NAMESPACES                               ///
////////////////////////////////////////////////////////////////////////


using namespace std;
constexpr double    MEMORY_SCALE = 1024.0;
constexpr double    TIME_SCALE   = 1000000.0;

////////////////////////////////////////////////////////////////////////
///                         helper function                          ///
////////////////////////////////////////////////////////////////////////


void showMemoryUsage(bool peak = false);
double getPeakMemoryUsage();
double getCurrMemoryUsage();


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
