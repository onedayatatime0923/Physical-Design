
#ifndef KDTREE_HPP
#define KDTREE_HPP

#include "dsGlobal.hpp"
#include "nanoflann.hpp"
#include "geometry/point.hpp"


class PointCloud2D {
public:
	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return _pointV.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
    //
    const Point&  point       (const size_t i)                        const   { return _pointV[i]; }
	inline DType kdtree_get_pt  (const size_t idx, const size_t dim)    const   {
		if (dim == 0) return _pointV[idx][0];
		else return _pointV[idx][1];
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

    // utils
    void clear      ()                                              { _pointV.clear(); }
    void add        (const DType x, const DType y)                  { _pointV.emplace_back(x, y); }
    void reserve    (size_t n)                                      { _pointV.reserve(n);            }
private:
	vector<Point>  _pointV;
};
class PointCloud3D {
public:
	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return _pointV.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
    //
    const Point3D&  point       (const size_t i)                        const   { return _pointV[i]; }
	inline DType kdtree_get_pt  (const size_t idx, const size_t dim)    const   {
		if (dim == 0) return _pointV[idx][0];
		else if (dim == 1) return _pointV[idx][1];
		else return _pointV[idx][2];
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

    // utils
    void clear      ()                                              { _pointV.clear(); }
    void add        (const DType x, const DType y, const DType z)   { _pointV.emplace_back(x, y, z); }
    void reserve    (size_t n)                                      { _pointV.reserve(n);            }
private:
	vector<Point3D>  _pointV;
};
// k2d types

typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L1_Adaptor<DType, PointCloud2D >, PointCloud2D, 2> KDTree_L1_2D;
typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<DType, PointCloud2D >, PointCloud2D, 2> KDTree_L2_2D;

// k3d types

typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L1_Adaptor<DType, PointCloud3D >, PointCloud3D, 3> KDTree_L1_3D;
typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<DType, PointCloud3D >, PointCloud3D, 3> KDTree_L2_3D;

class KDTree2D {
public:
    KDTree2D    () : _kdtree(2, _pointCloud) {}
    ~KDTree2D   () {}

    // utils
    void clear()                                                { _pointCloud.clear();                      }
    void insert(const Point& p)                                 { _pointCloud.add(p[0], p[1]);              }
    void insert(const DType x, const DType y)                   { _pointCloud.add(x, y);                    }
    void buildIndex()                                           { _kdtree.buildIndex();                     }

    // search
    void nearestSearch(const Point& queryP, Point& ret, DType& dist) {
        size_t retIndex;
        nanoflann::KNNResultSet<DType> resultSet(1);
        resultSet.init(&retIndex, &dist);
        DType qt[2] = {queryP[0], queryP[1]};
        _kdtree.findNeighbors(resultSet, qt, nanoflann::SearchParams(10));
        ret = _pointCloud.point(retIndex);
    }
    void knnSearch(const Point& queryP, const size_t nSearch, vector<Point >& resV, vector<DType>& disV) {
        resV.resize(nSearch);
        disV.resize(nSearch, 0);
        vector<size_t> retIndexV(nSearch);
        nanoflann::KNNResultSet<DType> resultSet(nSearch);
        resultSet.init(&retIndexV[0], &disV[0]);
        DType qt[2] = {queryP[0], queryP[1]};
        _kdtree.findNeighbors(resultSet, qt, nanoflann::SearchParams(10));
        for (int i = 0; i < (int)retIndexV.size(); ++i)
            resV[i] = _pointCloud.point(retIndexV[i]);
    }
private:
    PointCloud2D _pointCloud;
    KDTree_L1_2D _kdtree;
};
class KDTree3D {
public:
    KDTree3D    (DType zDirectWeight = 1) : _zDirectWeight(zDirectWeight), _kdtree(3, _pointCloud) {}
    ~KDTree3D   () {}

    // utils
    void setZDirectWeight   (DType z)                           { _zDirectWeight = z;                                   }
    void clear()                                                { _pointCloud.clear();                                  }
    void insert(const Point3D& p)                               { _pointCloud.add(p[0], p[1], p[2] * _zDirectWeight);   }
    void insert(const DType x, const DType y, const DType z)    { _pointCloud.add(x, y, z * _zDirectWeight);            }
    void buildIndex()                                           { _kdtree.buildIndex();                                 }

    // search
    void nearestSearch(const Point3D& queryP, Point3D& ret, DType& dist) {
        size_t retIndex;
        nanoflann::KNNResultSet<DType> resultSet(1);
        resultSet.init(&retIndex, &dist);
        DType qt[3] = {queryP[0], queryP[1], queryP[2] * _zDirectWeight};
        _kdtree.findNeighbors(resultSet, qt, nanoflann::SearchParams(10));
        ret = _pointCloud.point(retIndex);
        ret[2] /= _zDirectWeight;
    }
    void knnSearch(const Point3D& queryP, const size_t nSearch, vector<Point3D >& resV, vector<DType>& disV) {
        resV.resize(nSearch);
        disV.resize(nSearch, 0);
        vector<size_t> retIndexV(nSearch);
        nanoflann::KNNResultSet<DType> resultSet(nSearch);
        resultSet.init(&retIndexV[0], &disV[0]);
        DType qt[3] = {queryP[0], queryP[1], queryP[2] * _zDirectWeight};
        _kdtree.findNeighbors(resultSet, qt, nanoflann::SearchParams(10));
        for (int i = 0; i < (int)retIndexV.size(); ++i) {
            resV[i] = _pointCloud.point(retIndexV[i]);
            resV[i][2] /= _zDirectWeight;
        }
    }
private:
    DType           _zDirectWeight;
    PointCloud3D    _pointCloud;
    KDTree_L1_3D    _kdtree;
};



#endif // KDTREE_HPP
