#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"


//struct��class�ƈقȂ�, �f�t�H��public�w��ɂȂ��Ă���B�Ⴂ�͂��ꂾ���B
struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};

class hittable {
public://���̃N���X�ɂ̓R���X�g���N�^�͑��݂��Ă��Ȃ�
    virtual ~hittable() {}//�f�X�g���N�^
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec
    ) const = 0;
};

#endif