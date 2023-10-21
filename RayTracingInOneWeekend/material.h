#ifndef MATERIAL_H
#define MATERIAL_H


// ���ۃN���X
class material {
public:
    // virtual�̓|���t�H�[���Y�����T�|�[�g���邽�߂̏d�v�ȊT�O
    virtual ~material() {}// �f�X�g���N�^

    // �����h���֐��B���ł͓��e���`���Ȃ��֐��̂��ƂŁA�����1�ł��܂ރN���X�𒊏ۃN���X�Ƃ���
    // ���e���`���Ȃ����߁A�h���N���X�ŕK���I�[�o�[���C�h����K�v������B
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const =0; // const �� =0 �͕ʁB
                // const�̓I�u�W�F�N�g�̏�Ԃ�ύX���Ȃ����Ƃ������B(=�����o�֐���ύX���Ȃ�)
                // =0 �͏������z�֐��ł��邱�Ƃ�\���A�h���N���X���ŕK���������邱�Ƃ�v������K�{�L�@�B
};



// �����}�e���A��
class metal : public material {
public:
    metal(const color& a) : albedo(a) {}

    virtual bool scatter(// �����p�����������z�֐�scatter���`
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);// ���˕����Ɩ@���������甽�˕���������
        scattered = ray(rec.p, reflected);// ray�N���X�ɏՓ˓_p�ƎU�����C�̕�����ݒ�
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);// �@�����������ɏՓ˂����ꍇ��0��Ԃ��B
    }

public:
    color albedo;
};



// �����o�[�g���˂̎����͎��2�B
// 1�́A�S�Ẵ��C���U�������A�S�Ĕ��˗�R�Ō���������B
// ����1�́A�m���I��ray�������Ȃ��ŎU�������A����ȊO�͋z���Ƃ���B
// �ȉ��͑O�ҁB
class lambertian : public material {// ���ۃN���X��material���p��
public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(// �����p�����������z�֐�scatter���`
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);// ray�N���X�ɏՓ˓_p�ƎU�����C�̕�����ݒ�
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};



#endif