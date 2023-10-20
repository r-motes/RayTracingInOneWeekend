#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class material;


//structはclassと異なり, デフォでpublic指定になっている。違いはそれだけ。
struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;// (条件式) ? trueの場合 : falseの場合
    }
};

class hittable {//レイが衝突しうる物体を表す抽象クラス
public://このクラスにはコンストラクタは存在していない(継承で使う用)
    virtual ~hittable() {}//デストラクタ
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec
    ) const = 0;
};

#endif