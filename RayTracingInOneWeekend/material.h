#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"


// シュリックの近似
double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}


// 抽象クラス
class material {
public:
    // virtualはポリフォーリズムをサポートするための重要な概念
    virtual ~material() {}// デストラクタ

    // 純粋派生関数。基底では内容を定義しない関数のことで、これを1つでも含むクラスを抽象クラスという
    // 内容を定義しないため、派生クラスで必ずオーバーライドする必要がある。
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0; // const と =0 は別。
                // constはオブジェクトの状態を変更しないことを示す。(=メンバ関数を変更しない)
                // =0 は純粋仮想関数であることを表し、派生クラス内で必ず実装することを要求する必須記法。
};


// 金属マテリアル
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(// 引き継いだ純粋仮想関数scatterを定義
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);// 入射方向と法線方向から反射方向を決定
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());// rayクラスに衝突点pと散乱レイの方向を設定
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);// 法線が裏向きに衝突した場合は0を返す。
    }

public:
    color albedo;
    double fuzz;// 散乱率的なもの(値が大きいほど散乱が大きい)
};



// ランバート反射の実装は主に2つ。
// 1つは、全てのレイを散乱させ、全て反射率Rで減衰させる。
// もう1つは、確率的にrayを減衰なしで散乱させ、それ以外は吸収とする。
// 以下は前者。
class lambertian : public material {// 抽象クラスのmaterialを継承
public:
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(// 引き継いだ純粋仮想関数scatterを定義
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction, r_in.time());// rayクラスに衝突点pと散乱レイの方向を設定
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};



// 誘電体マテリアル(屈折を考慮)
class dielectric : public material {
public:
    dielectric(double ri) : ref_idx(ri) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        attenuation = color(1.0, 1.0, 1.0);
        double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        if (etai_over_etat * sin_theta > 1.0) { // 全反射の条件
            vec3 reflected = reflect(unit_direction, rec.normal);// 反射処理
            scattered = ray(rec.p, reflected);
            return true;
        }
        double reflect_prob = schlick(cos_theta, etai_over_etat);// 屈折の場合はシュリックの近似を用いる
        if (random_double() < reflect_prob) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);// 屈折処理
        scattered = ray(rec.p, refracted);
        return true;
    }

public:
    double ref_idx;
};



#endif