#include "rtweekend.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <fstream>


color ray_color(const ray& r, const hittable& world, int depth) {

    // 反射回数が一定よりも多くなったら、その時点で追跡をやめる
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))// mat_ptrのメンバ関数が呼び出される。つまりマテリアルに応じたscatter関数が呼び出され、returnされるbool値は異なる。
            return attenuation * ray_color(scattered, world, depth - 1);// 散乱する場合はこのif内の処理をする。基本的には減衰と再帰。
        return color(0, 0, 0);// 散乱しない場合(0,0,0)を返して終了
    }

    // 衝突市内場合は背景としてy座標(高さ)に応じたcolorが返される
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main() {
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    
    std::ofstream ofs("image.ppm");

    ofs << "P3\n" << image_width << " " << image_height << "\n255\n";

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2. - vertical / 2. - vec3(0, 0, focal_length);


    hittable_list world;
    world.add(make_shared<sphere>(
        point3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.7, 0.3, 0.3))));
    world.add(make_shared<sphere>(// 誘電体球の半径を負に設定すると、ジオメトリは変わらず法線だけが反転する。これを使うと中空の球を作ることができる。
        point3(1, 0, -1), 0.5, make_shared<dielectric>(1.5))); 
    world.add(make_shared<sphere>(
        point3(-1, 0, -1), 0.5, make_shared<metal>(color(0.8, 0.8, 0.8), 0.9)));

    world.add(make_shared<sphere>(// 下のまるで床のような球(半径100)
        point3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));


    camera cam(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 40, aspect_ratio);// カメラインスタンス生成


    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {// サンプル数だけループする
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(ofs, pixel_color, samples_per_pixel);
        }
    }

    ofs.close();
    std::cerr << "\nDone.\n";

    // i_view32.exe image.ppm　を実行したい
    system("\"C:\\Program Files (x86)\\IrfanView\\i_view32.exe\" C:\\Users\\motes\\source\\repos\\RayTracingInOneWeekend\\RayTracingInOneWeekend\\image.ppm");
}
