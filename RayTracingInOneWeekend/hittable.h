#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"


//structはclassと異なり, デフォでpublic指定になっている。違いはそれだけ。
struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};

class hittable {
public://このクラスにはコンストラクタは存在していない
    virtual ~hittable() {}//デストラクタ
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec
    ) const = 0;
};

#endif