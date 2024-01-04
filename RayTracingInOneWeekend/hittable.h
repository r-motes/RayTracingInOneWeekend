#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"

class material;// hit_recordクラスとmaterialクラスで循環参照が生じている。そのための処理。


//structはclassと異なり, デフォでpublic指定になっている。違いはそれだけ。
struct hit_record {
    point3 p;// 衝突点
    vec3 normal;// 衝突点の法線
    shared_ptr<material> mat_ptr;// 衝突した物体のmaterial
    double t;// 衝突したrayの長さ(a + tb)
    double u;// テクスチャ座標
    double v;
    bool front_face;// 衝突した面の法線の表裏

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;// (条件式) ? trueの場合 : falseの場合
    }
};

class hittable {//レイが衝突しうる物体を表す抽象クラス
public://このクラスにはコンストラクタは存在していない(継承で使う用)(必要ない)
    virtual ~hittable() {}//デストラクタ
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec
    ) const = 0;// 抽象クラス
    virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
};

#endif