#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"

class material;// hit_record�N���X��material�N���X�ŏz�Q�Ƃ������Ă���B���̂��߂̏����B


//struct��class�ƈقȂ�, �f�t�H��public�w��ɂȂ��Ă���B�Ⴂ�͂��ꂾ���B
struct hit_record {
    point3 p;// �Փ˓_
    vec3 normal;// �Փ˓_�̖@��
    shared_ptr<material> mat_ptr;// �Փ˂������̂�material
    double t;// �Փ˂���ray�̒���(a + tb)
    bool front_face;// �Փ˂����ʂ̖@���̕\��

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;// (������) ? true�̏ꍇ : false�̏ꍇ
    }
};

class hittable {//���C���Փ˂����镨�̂�\�����ۃN���X
public://���̃N���X�ɂ̓R���X�g���N�^�͑��݂��Ă��Ȃ�(�p���Ŏg���p)(�K�v�Ȃ�)
    virtual ~hittable() {}//�f�X�g���N�^
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec
    ) const = 0;// ���ۃN���X
};

#endif