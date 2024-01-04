#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>// shared_ptr���g����
#include <vector>// �v�f��������Ǝ����I�Ƀ��������m�ۂ���B

using std::shared_ptr;// ���ꎩ�̂͂�����namespace�̐錾(memory��include���邱�ƂŎg����悤�ɂȂ�)
using std::make_shared;
// ���ʂ�pointer�́A�Ⴆ�΂���|�C���^p�����̃|�C���^�ϐ�q�ɃR�s�[����āA���̃|�C���^p��free(p)�ŉ�����ꂽ�ꍇ�A
// �R�s�[���ꂽ�|�C���^q�͉���(segmentation fault)
// ������shared_ptr�ł���΁A�R�s�[�Ȃǂ��ꂽ�ꍇ�ɓ����ŃJ�E���g+1�A�g���Ȃ��Ȃ����ꍇ�ɃJ�E���g-1���N����A
// �J�E���g0�̏ꍇ�ɏ���ɉ������邽�߁A��L�̕��ʂ̃|�C���^�ŋN����G���[���N���邱�Ƃ��Ȃ��B
// make_shared��shared_ptr�̂��߂̂��̂ŁAmake_shared<�^>(�R���X�g���N�^�̏�����)��shared_ptr�𐶐�����B
// ���̂Ƃ��B���ʂ̃|�C���^�ł����Ƃ����malloc(4*sizeof(int))�̂悤�Ȃ��Ƃ��s�Ȃ��Ă���B
// �܂��A�����̂ŃR���X�g���N�^�̈����Ƃ��ĐV�����C���X�^���X���A���P�[�g�E�������ł���Bc++�Ȃ�ł͂̋@�\�B
// ��Fshared_ptr<double> double_ptr = make_shared<double>(0.37);
// ��Fshared_ptr<sphere> sphere_ptr = make_shared<sphere>(point3(0, 0, 0), 1.0); ��center��radius



class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }//�R���X�g���N�^(hittable�^��shared�|�C���^�ŏ�����)

    void clear() { objects.clear(); }// object"s"���N���A����
    void add(shared_ptr<hittable> object) { objects.push_back(object); }// object"s"�̍Ō��object��ǉ����Ă���

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