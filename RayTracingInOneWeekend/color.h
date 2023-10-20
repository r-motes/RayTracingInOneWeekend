#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

// �s�N�Z���J���[�ƃT���v�����������Ŏ󂯎��A�Ō�ɃT���v�����Ŋ��邱�Ƃŕ��ς����B
void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // �F�̍��v���T���v���̐��Ŋ���Agamma = 2.0 �̃K���}�␳���s��
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);// (scale*r)�ŕ��ϒl��������l��sqrt()��1/2�悷�邾���B�o�͋P�x=(���͋P�x)^(1/�K���}�l)
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // �e������ [0,255] �ɕϊ����ďo�͂���
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif