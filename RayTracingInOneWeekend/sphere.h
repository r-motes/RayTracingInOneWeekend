#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {}// 中心座標と半径

    virtual bool hit(
        const ray& r, double tmin, double tmax, hit_record& rec
    ) const;

public:
    point3 center;// 中心座標
    double radius;// 半径
};

bool sphere::hit(// sphere classのhit関数の詳細
    const ray& r, double t_min, double t_max, hit_record& rec
) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;// 判別式

    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;// 解
        if (temp < t_max && temp > t_min) {
            rec.t = temp;// 解をhit_recordに記録
            rec.p = r.at(rec.t);// 衝突点
            vec3 outward_normal = (rec.p - center) / radius;// 衝突点の法線(球だから)(外向き)(単位ベクトル)
            rec.set_face_normal(r, outward_normal);
            return true;
        }
        temp = (-half_b + root) / a;// もう一方の解(どちらかしかif文を通らない？もう一方はtemp<0となり無効？)
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }
    }
    return false;
}

#endif