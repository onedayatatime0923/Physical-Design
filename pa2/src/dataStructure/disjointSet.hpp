/*
	Author : Akshay Pratap Singh
	Email-Id : pratapakshay0@gmail.com
*/

/********   Union-Find Disjoint Set Implementation   *****/

#include <vector>
using namespace std;
class UFDS {
private:
	vector<int> p, rank, setSizes;
	int numSets;
public:
	UFDS() {};
	UFDS(int N) { init(N); }
    void init(int N) {
		numSets = N;
		rank.assign(N, 0);
		p.assign(N, 0);
		for (int i = 0; i < N; i++)
			p[i] = i;
		setSizes.assign(N, 1);
	}
	int find(int i) {
		return (p[i] == i) ? i : p[i] = find(p[i]);
	}
	bool isSameSet(int i, int j) {
		return find(i) == find(j);
	}
	void merge(int i, int j) {
		if (!isSameSet(i, j)) {
			int x = find(i), y = find(j);
			if (rank[x] > rank[y]) {
				setSizes[find(x)] += setSizes[find(y)];
				p[y] = x;
			} else {
				setSizes[find(y)] += setSizes[find(x)];
				p[x] = y;
				if (rank[x] == rank[y])
					rank[y]++;
			}
			numSets--;
		}
	}
	int setSize(int i) {
		return setSizes[find(i)];
	}
	int nSets() {
		return numSets;
	}
};
/******** ************* *********** ********** **********/

/*** How to use above UFDS class **/

/*
	Here , 'N' is the no. of disjoint sets initially
	UFDS uf(N) : Creates a object of UFDS class having 'N' disjoint sets
	findSet(i) : Return in which set the element 'i' is
	isSameSet(i,j) : Return true if element 'i' and 'j' are of same
					 set else false
	unionSet(i,j) : Return none while makes a union of two seperate set
					of 'i' and 'j'
	setSize(i) : Return the size of set of which 'i' is a part
	numDisjointSets() : Retrun the no. of total disjoint sets at present
*/
/*****************************/
