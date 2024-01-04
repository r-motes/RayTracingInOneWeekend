#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) { _min = a; _max = b; }//区間(3次元のAABBは2点の座標で決定できる)

    point3 min() const { return _min; }
    point3 max() const { return _max; }

    //// AABBの衝突関数
    //bool hit(const ray& r, double tmin, double tmax) const {
    //    for (int a = 0; a < 3; a++) {// xyzの3次元分ループ
    //        auto t0 = fmin((_min[a] - r.origin()[a]) / r.direction()[a],
    //            (_max[a] - r.origin()[a]) / r.direction()[a]);
    //        auto t1 = fmax((_min[a] - r.origin()[a]) / r.direction()[a],
    //            (_max[a] - r.origin()[a]) / r.direction()[a]);
    //        tmin = fmax(t0, tmin);// 各次元の区間の最小
    //        tmax = fmin(t1, tmax);// 各次元の区間の最大
    //        if (tmax <= tmin)
    //            return false;// 衝突なし(途中で矛盾が生じたらその時点でfalseを返す)
    //    }
    //    return true;// 衝突あり
    //}

    // ↑の最適化
    inline bool hit(const ray& r, double tmin, double tmax) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin)
                return false;
        }
        return true;
    }

    point3 _min;
    point3 _max;
};



// 動的物体のAABBを計算
aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
        fmax(box0.max().y(), box1.max().y()),
        fmax(box0.max().z(), box1.max().z()));

    return aabb(small, big);
}

#endif