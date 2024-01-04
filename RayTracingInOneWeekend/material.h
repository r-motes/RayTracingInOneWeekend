#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"


// �V�����b�N�̋ߎ�
double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}


// ���ۃN���X
class material {
public:
    // virtual�̓|���t�H�[���Y�����T�|�[�g���邽�߂̏d�v�ȊT�O
    virtual ~material() {}// �f�X�g���N�^

    // �����h���֐��B���ł͓��e���`���Ȃ��֐��̂��ƂŁA�����1�ł��܂ރN���X�𒊏ۃN���X�Ƃ���
    // ���e���`���Ȃ����߁A�h���N���X�ŕK���I�[�o�[���C�h����K�v������B
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0; // const �� =0 �͕ʁB
                // const�̓I�u�W�F�N�g�̏�Ԃ�ύX���Ȃ����Ƃ������B(=�����o�֐���ύX���Ȃ�)
                // =0 �͏������z�֐��ł��邱�Ƃ�\���A�h���N���X���ŕK���������邱�Ƃ�v������K�{�L�@�B
};


// �����}�e���A��
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(// �����p�����������z�֐�scatter���`
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);// ���˕����Ɩ@���������甽�˕���������
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());// ray�N���X�ɏՓ˓_p�ƎU�����C�̕�����ݒ�
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);// �@�����������ɏՓ˂����ꍇ��0��Ԃ��B
    }

public:
    color albedo;
    double fuzz;// �U�����I�Ȃ���(�l���傫���قǎU�����傫��)
};



// �����o�[�g���˂̎����͎��2�B
// 1�́A�S�Ẵ��C���U�������A�S�Ĕ��˗�R�Ō���������B
// ����1�́A�m���I��ray�������Ȃ��ŎU�������A����ȊO�͋z���Ƃ���B
// �ȉ��͑O�ҁB
class lambertian : public material {// ���ۃN���X��material���p��
public:
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(// �����p�����������z�֐�scatter���`
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction, r_in.time());// ray�N���X�ɏՓ˓_p�ƎU�����C�̕�����ݒ�
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};



// �U�d�̃}�e���A��(���܂��l��)
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
        if (etai_over_etat * sin_theta > 1.0) { // �S���˂̏���
            vec3 reflected = reflect(unit_direction, rec.normal);// ���ˏ���
            scattered = ray(rec.p, reflected);
            return true;
        }
        double reflect_prob = schlick(cos_theta, etai_over_etat);// ���܂̏ꍇ�̓V�����b�N�̋ߎ���p����
        if (random_double() < reflect_prob) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);// ���܏���
        scattered = ray(rec.p, refracted);
        return true;
    }

public:
    double ref_idx;
};



#endif