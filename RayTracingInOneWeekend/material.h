#ifndef MATERIAL_H
#define MATERIAL_H


// 抽象クラス
class material {
public:
    // virtualはポリフォーリズムをサポートするための重要な概念
    virtual ~material() {}// デストラクタ

    // 純粋派生関数。基底では内容を定義しない関数のことで、これを1つでも含むクラスを抽象クラスという
    // 内容を定義しないため、派生クラスで必ずオーバーライドする必要がある。
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const =0; // const と =0 は別。
                // constはオブジェクトの状態を変更しないことを示す。(=メンバ関数を変更しない)
                // =0 は純粋仮想関数であることを表し、派生クラス内で必ず実装することを要求する必須記法。
};



// 金属マテリアル
class metal : public material {
public:
    metal(const color& a) : albedo(a) {}

    virtual bool scatter(// 引き継いだ純粋仮想関数scatterを定義
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);// 入射方向と法線方向から反射方向を決定
        scattered = ray(rec.p, reflected);// rayクラスに衝突点pと散乱レイの方向を設定
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);// 法線が裏向きに衝突した場合は0を返す。
    }

public:
    color albedo;
};



// ランバート反射の実装は主に2つ。
// 1つは、全てのレイを散乱させ、全て反射率Rで減衰させる。
// もう1つは、確率的にrayを減衰なしで散乱させ、それ以外は吸収とする。
// 以下は前者。
class lambertian : public material {// 抽象クラスのmaterialを継承
public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(// 引き継いだ純粋仮想関数scatterを定義
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);// rayクラスに衝突点pと散乱レイの方向を設定
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};



#endif