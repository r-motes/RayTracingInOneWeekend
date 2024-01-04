#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>// shared_ptrを使える
#include <vector>// 要素が増えると自動的にメモリを確保する。

using std::shared_ptr;// これ自体はただのnamespaceの宣言(memoryをincludeすることで使えるようになる)
using std::make_shared;
// 普通のpointerは、例えばあるポインタpが他のポインタ変数qにコピーされて、元のポインタpがfree(p)で解放された場合、
// コピーされたポインタqは壊れる(segmentation fault)
// しかしshared_ptrであれば、コピーなどされた場合に内部でカウント+1、使われなくなった場合にカウント-1が起こり、
// カウント0の場合に勝手に解放されるため、上記の普通のポインタで起こるエラーが起こることがない。
// make_sharedはshared_ptrのためのもので、make_shared<型>(コンストラクタの初期化)でshared_ptrを生成する。
// このとき。普通のポインタでいうところのmalloc(4*sizeof(int))のようなことが行なわれている。
// また、賢いのでコンストラクタの引数として新しいインスタンスをアロケート・初期化できる。c++ならではの機能。
// 例：shared_ptr<double> double_ptr = make_shared<double>(0.37);
// 例：shared_ptr<sphere> sphere_ptr = make_shared<sphere>(point3(0, 0, 0), 1.0); ←centerとradius



class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }//コンストラクタ(hittable型のsharedポインタで初期化)

    void clear() { objects.clear(); }// object"s"をクリアする
    void add(shared_ptr<hittable> object) { objects.push_back(object); }// object"s"の最後にobjectを追加している

    virtual bool hit(
        const ray& r, double tmin, double tmax, hit_record& rec
    ) const;

    bool bounding_box(
        double t0, double t1, aabb& output_box
    ) const;

public:
    std::vector<shared_ptr<hittable>> objects;
};



bool hittable_list::hit(
    const ray& r, double t_min, double t_max, hit_record& rec
) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(double t0, double t1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(t0, t1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}


#endif