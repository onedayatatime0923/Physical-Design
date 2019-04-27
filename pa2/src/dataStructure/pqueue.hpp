/**CFile****************************************************************

  FileName    [pqueue.hpp]

  SystemName  [NTUidRoute: Initial Detailed Routing System.]

  PackageName [dataStructure usage.]

  Synopsis    [dataStructure usage.]

  Author      []
  
  Affiliation [NTU]

  Date        [Ver. 1.0. Started - Dec 6, 2019.]

  Revision    []

***********************************************************************/

#ifndef ds__pqueue_h
#define ds__pqueue_h

#include "dsGlobal.hpp"
#include <ext/pb_ds/priority_queue.hpp>


template<typename T, typename Cmp = less<T> >
using PQueue = __gnu_pbds::priority_queue<T, Cmp>;

template<typename T, typename Cmp = less<T> >
using BinaryHeap = __gnu_pbds::priority_queue<T, Cmp, __gnu_pbds::binary_heap_tag>;

template<typename T, typename Cmp = less<T> >
using BinomialHeap = __gnu_pbds::priority_queue<T, Cmp, __gnu_pbds::binomial_heap_tag>;

template<typename T, typename Cmp = less<T> >
using RCBinomialHeap = __gnu_pbds::priority_queue<T, Cmp, __gnu_pbds::rc_binomial_heap_tag>;

template<typename T, typename Cmp = less<T> >
using PairingHeap = __gnu_pbds::priority_queue<T, Cmp, __gnu_pbds::pairing_heap_tag>;

template<typename T, typename Cmp = less<T> >
using ThinHeap = __gnu_pbds::priority_queue<T, Cmp, __gnu_pbds::thin_heap_tag>;


#endif // PQUEUE_HPP
