#ifndef SURFACE_TEXTURE_H
#define SURFACE_TEXTURE_H

#include "rtweekend.h"
#include "stb_image.h"
#include "texture.h"

#include <iostream>


// テクスチャ画像読み込み
class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;

    image_texture()
        : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(
            filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '"
                << filename
                << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    virtual color value(double u, double v, const vec3& p) const {
        // テクスチャのデータがない場合には、そのことが分かるようにシアン色を返す。
        if (data == nullptr)
            return color(0, 1, 1);

        // 入力されたテクスチャ座標を [0,1] で切り捨てる。
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);  // v を反転させて画像の座標系に合わせる。

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // 整数座標をさらに切り捨てる (テクスチャ座標は 1.0 になってはいけない)。
        if (i >= width)  i = width - 1;
        if (j >= height) j = height - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return color(color_scale * pixel[0],
            color_scale * pixel[1],
            color_scale * pixel[2]);
        // return color(u, v, 0.0);//DEBUG
    }

private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};


#endif