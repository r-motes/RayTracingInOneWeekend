#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"

class sphere : public hittable {// デストラクタは継承している
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m)// コンストラクタでメンバ関数を設定
        : center(cen), radius(r), mat_ptr(m) {}

    virtual bool hit(// 純粋仮想関数のhitは以下で定義している
        const ray& r, double tmin, double tmax, hit_record& rec
    ) const;

    bool bounding_box(
        double t0, double t1, aabb& output_box
    ) const;

public:
    point3 center;// 中心座標
    double radius;// 半径
    shared_ptr<material> mat_ptr;// マテリアル(ポインタ)
};


// uv座標計算
void get_sphere_uv(const vec3& p, double& u, double& v) {
    auto phi = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
}


bool sphere::hit(// sphere classのhit関数を定義
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
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);//sphereのuv座標を計算
            vec3 outward_normal = (rec.p - center) / radius;// 衝突点の法線(球だから)(外向き)(単位ベクトル)
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;// マテリアルのポインタをhit_record構造体(rec)に追加。
            return true;
        }
        temp = (-half_b + root) / a;// もう一方の解(どちらかしかif文を通らない？もう一方はtemp<0となり無効？)
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);//sphereのuv座標を計算
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;// マテリアルのポインタをhit_record構造体(rec)に追加。
            return true;
        }
    }
    return false;
}


bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
    output_box = aabb(center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}



#endif