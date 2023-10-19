#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {}// ���S���W�Ɣ��a

    virtual bool hit(
        const ray& r, double tmin, double tmax, hit_record& rec
    ) const;

public:
    point3 center;// ���S���W
    double radius;// ���a
};

bool sphere::hit(// sphere class��hit�֐��̏ڍ�
    const ray& r, double t_min, double t_max, hit_record& rec
) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;// ���ʎ�

    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;// ��
        if (temp < t_max && temp > t_min) {
            rec.t = temp;// ����hit_record�ɋL�^
            rec.p = r.at(rec.t);// �Փ˓_
            vec3 outward_normal = (rec.p - center) / radius;// �Փ˓_�̖@��(��������)(�O����)(�P�ʃx�N�g��)
            rec.set_face_normal(r, outward_normal);
            return true;
        }
        temp = (-half_b + root) / a;// ��������̉�(�ǂ��炩����if����ʂ�Ȃ��H���������temp<0�ƂȂ薳���H)
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