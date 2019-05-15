#ifndef ds__hash_h
#define ds__hash_h

#include "dsGlobal.hpp"
#include <ext/pb_ds/assoc_container.hpp>
#include <sparsehash/dense_hash_set>
#include <sparsehash/dense_hash_map>
#include <sparsehash/sparse_hash_set>
#include <sparsehash/sparse_hash_map>


template<typename Key, typename Hash_Fn = typename __gnu_pbds::detail::default_hash_fn<Key>::type>
using DenseHashSet = google::dense_hash_set<Key, Hash_Fn>;

template<typename Key, typename Value, typename Hash_Fn = typename __gnu_pbds::detail::default_hash_fn<Key>::type>
using DenseHashMap = google::dense_hash_map<Key, Value, Hash_Fn>;

template<typename Key, typename Hash_Fn = typename __gnu_pbds::detail::default_hash_fn<Key>::type>
using SparseHashSet = google::sparse_hash_set<Key, Hash_Fn>;

template<typename Key, typename Value, typename Hash_Fn = typename __gnu_pbds::detail::default_hash_fn<Key>::type>
using SparseHashMap = google::sparse_hash_map<Key, Value, Hash_Fn>;

#endif // HASH_HPP
