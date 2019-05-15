#include "rect.hpp"

Point Rect::center() const { 
    int x = (_topRight[0] + _buttomLeft[0])/2;  
    int y = (_topRight[1] + _buttomLeft[1])/2;
    Point p(x, y);
    return p;
}
int Rect::location(const Rect& r1, const Rect& r2, bool y) {
    if (r1._buttomLeft[y] > r2._topRight[y]) //input in my left side
        return -1;
    else if (r1._topRight[y] < r2._buttomLeft[y]) // inside
        return 1;
    else //right
        return 0;
};

pair<int, int> Rect::overlapRect(int xy1, int xy2, int query_xy1, int query_xy2) {   
    //xy2>>xy1
    //in range  被包在裡面
    if(xy1>=query_xy1 && xy2<=query_xy2)
        return pair<int, int>(xy1 ,xy2);
    //not in 橫跨
    else if(xy1<=query_xy1 && xy2>=query_xy2)
        return pair<int, int>(query_xy1 ,query_xy2);
    //half 在上面或是在右邊
    else if(xy1 >= query_xy1 && xy2 >= query_xy2 && xy1 <= query_xy2)
        return pair<int, int>(xy1 ,query_xy2);
    //half 在下面或是在左邊
    else if(xy1 <= query_xy1 && xy2 <= query_xy2 && query_xy1 <= xy2)
        return pair<int, int>(query_xy1 ,xy2);
    else{
        // cout<<endl<<"幹你娘找到囉"<<endl;
        // cout<<xy2<<" "<<xy1<<" "<<query_xy2<<" "<<query_xy1<<endl;
        return pair<int, int>(-1 ,-1);;
    }
}
Rect Rect::overlapRect(const Rect& r1, const Rect& r2) {
    pair<int, int> p1 = overlapRect(r1[0], r1[2], r2[0], r2[2]);
    pair<int, int> p2 = overlapRect(r1[1], r1[3], r2[1], r2[3]);
    if (p1 != pair<int, int> (-1,-1) && p2 != pair<int, int> (-1,-1))
        return Rect(p1.first, p2.first, p1.second, p2.second);
    else
        return Rect(-1, -1, -1, -1);
}
int Rect::overlapLength(const Rect& r1, const Rect& r2, bool y) {
    if (r1._buttomLeft[y] <= r2._buttomLeft[y] && r1._topRight[y] >= r2._topRight[y])
        return (r2._topRight[y] - r2._buttomLeft[y]);
    else if (r1._buttomLeft[y] >= r2._buttomLeft[y] && r1._topRight[y] <= r2._topRight[y])
        return (r1._topRight[y] - r1._buttomLeft[y]);
    else if (r1._buttomLeft[y] >= r2._buttomLeft[y] && r1._topRight[y] >= r2._topRight[y] && r1._buttomLeft[y] < r2._topRight[y])
        return (r2._topRight[y] - r1._buttomLeft[y]);
    else if (r1._buttomLeft[y] <= r2._buttomLeft[y] && r1._topRight[y] <= r2._topRight[y] && r1._buttomLeft[y] > r2._topRight[y])
        return (r1._buttomLeft[y] - r2._topRight[y]);
    else
        return 0;
};
pair<int, int> Rect::unionRect(int xy1, int xy2, int query_xy1, int query_xy2) {   
    //xy1>>xy2
    //in range  被包在裡面
    if(xy1<=query_xy1&&xy2>=query_xy2)
        return pair<int, int>(xy1 ,xy2);
    //not in 橫跨
    else if(xy1>=query_xy1&&xy2<=query_xy2)
        return pair<int, int>(query_xy1 ,query_xy2);
    else if(xy1<=query_xy1&&xy2<=query_xy2)
        return pair<int, int>(xy1 ,query_xy2);
    else if(xy1>=query_xy1&&xy2>=query_xy2)
        return pair<int, int>(query_xy1 ,xy2);
    else assert(false);
    //half 在上面或是在右邊
    // else if(xy1>=query_xy1&&xy2>=query_xy2&&xy1<query_xy2)
    //     return pair<int, int>(query_xy1 ,xy2);
    // //half 在下面或是在左邊
    // else if(xy1<=query_xy1&&xy2<=query_xy2&&xy2>query_xy1)
    //     return pair<int, int>(xy1 ,query_xy2);
    // else{
    //     cout<<endl<<"幹你娘找到囉"<<endl;
    //     // cout<<xy1<<" "<<xy2<<" "<<query_xy1<<" "<<query_xy2<<endl;
    //     return pair<int, int>(0 ,0);;
    // }
}
Rect Rect::unionRect(const Rect& r1, const Rect& r2) {
    pair<int, int> p1 = unionRect(r1[0], r1[2], r2[0], r2[2]);
    pair<int, int> p2 = unionRect(r1[1], r1[3], r2[1], r2[3]);
    return Rect(p1.first, p2.first, p1.second, p2.second);
}
int Rect::Mdistance(const Point &p, const Rect& r) { 
    int dx, dy;
    if (p[0] > r._topRight[0]) dx = p[0] - r._topRight[0]; 
    else if (p[0] < r._buttomLeft[0]) dx = r._buttomLeft[0] - p[0];
    else dx = 0;
    if (p[1] > r._topRight[1]) dy = p[1] - r._topRight[1]; 
    else if (p[1] < r._buttomLeft[1]) dy = r._buttomLeft[1] - p[1];
    else dy = 0;
    return dx + dy;
}
int Rect::nearestMdistance(const Rect& r1, const Rect &r2) { 
    int dx, dy;
    if (r2[0] > r1._topRight[0]) dx = r2[0] - r1._topRight[0]; 
    else if (r2[2] < r1._buttomLeft[0]) dx =r1._buttomLeft[0] - r2[2];
    else dx = 0;
    if (r2[1] > r1._topRight[1]) dy = r2[1] - r1._topRight[1]; 
    else if (r2[3] < r1._buttomLeft[1]) dy =r1._buttomLeft[1] - r2[3];
    else dy = 0;
    return dx + dy;
}
int Rect::centerMdistance(const Rect &r1, const Rect &r2) {
    Point p1 = r1.center(); Point p2 = r2.center();
    return Point::Mdistance(p1, p2);
}
