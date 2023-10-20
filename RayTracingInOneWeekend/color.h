#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

// ピクセルカラーとサンプル数を引数で受け取り、最後にサンプル数で割ることで平均を取る。
void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // 色の合計をサンプルの数で割り、gamma = 2.0 のガンマ補正を行う
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);// (scale*r)で平均値を取った値をsqrt()で1/2乗するだけ。出力輝度=(入力輝度)^(1/ガンマ値)
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // 各成分を [0,255] に変換して出力する
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif