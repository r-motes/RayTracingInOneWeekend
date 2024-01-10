#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "surface_texture.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// openmp
#include <omp.h>

#include <iostream>
#include <fstream>


// ray飛ばしてレンダリング
color ray_color(const ray& r, const color& background, const hittable& world, int depth) {

    hit_record rec;

    // 反射回数が一定よりも多くなったら、その時点で追跡をやめる
    if (depth <= 0)
        return color(0, 0, 0);

    // レイがどのオブジェクトとも交わらないなら、背景色を返す
    if (!world.hit(r, 0.001, infinity, rec)) {
        return background;
    }

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))// mat_ptrのメンバ関数が呼び出される。つまりマテリアルに応じたscatter関数が呼び出され、returnされるbool値は異なる。
    //if (!(*rec.mat_ptr).scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);

}


// オブジェクトとカメラ情報をセットで持つようにする
struct SceneData
{
    hittable_list objects;
    // camera param
    point3 lookfrom;
    point3 lookat;
    vec3 vup;
    double vfov;
    double dist_to_focus;
    double aperture;
};


// シーン一覧

// 大規模シーン定義
SceneData random_scene() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(
        make_shared<solid_color>(0.2, 0.3, 0.1),
        make_shared<solid_color>(0.9, 0.9, 0.9)
        );

    objects.add(
        make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker))
    );

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(make_shared<solid_color>(albedo));
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    objects.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    objects.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    objects.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<lambertian>(make_shared<solid_color>(0.4, 0.2, 0.1));
    objects.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    objects.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));


    SceneData ret;
    ret.objects = objects;
    ret.lookfrom = vec3(13, 2, 3);
    ret.lookat = vec3(0, 0, 0);
    ret.vup = vec3(0, 1, 0);
    ret.vfov = 20.0;
    ret.dist_to_focus = 10.0;
    ret.aperture = 0.0;

    return ret;
}


// 球2つの接点拡大シーン
SceneData two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(
        make_shared<solid_color>(0.2, 0.3, 0.1),
        make_shared<solid_color>(0.9, 0.9, 0.9)
        );

    objects.add(
        make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker))
    );
    objects.add(
        make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker))
    );

    SceneData ret;
    ret.objects = objects;
    ret.lookfrom = vec3(13, 2, 3);
    ret.lookat = vec3(0, 0, 0);
    ret.vup = vec3(0, 1, 0);
    ret.vfov = 20.0;
    ret.dist_to_focus = 10.0;
    ret.aperture = 0.0;

    return ret;
}

//パーリンノイズシーン
SceneData two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>();
    objects.add(make_shared<sphere>(
        point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext))
    );
    objects.add(make_shared<sphere>(
        point3(0, 2, 0), 2, make_shared<lambertian>(pertext))
    );

    SceneData ret;
    ret.objects = objects;
    ret.lookfrom = vec3(13, 2, 3);
    ret.lookat = vec3(0, 0, 0);
    ret.vup = vec3(0, 1, 0);
    ret.vfov = 20.0;
    ret.dist_to_focus = 10.0;
    ret.aperture = 0.0;

    return ret;
}


// 地球儀シーン
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("../resources/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);
    return hittable_list(globe);
}


// 長方形を光源とする
SceneData simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(
        point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext))
    );
    objects.add(make_shared<sphere>(
        point3(0, 2, 0), 2, make_shared<lambertian>(pertext))
    );

    auto difflight = make_shared<diffuse_light>(make_shared<solid_color>(4, 4, 4));
    objects.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));


    SceneData ret;
    ret.objects = objects;
    ret.lookfrom = vec3(13, 2, 3);
    ret.lookat = vec3(0, 0, 0);
    ret.vup = vec3(0, 1, 0);
    ret.vfov = 20.0;
    ret.dist_to_focus = 10.0;
    ret.aperture = 0.0;

    return ret;
}


// 空のコーネルボックスの作成
SceneData cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(make_shared<solid_color>(.65, .05, .05));
    auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
    auto green = make_shared<lambertian>(make_shared<solid_color>(.12, .45, .15));
    auto light = make_shared<diffuse_light>(make_shared<solid_color>(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(183, 373, 197, 362, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 =
        make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 =
        make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);

    SceneData ret;
    ret.objects = objects;
    ret.lookfrom = vec3(278, 278, -800);
    ret.lookat = vec3(278, 278, 0);
    ret.vup = vec3(0, 1, 0);
    ret.vfov = 40.0;
    ret.dist_to_focus = 10.0;
    ret.aperture = 0.0;

    return ret;
}


//  煙がかった直方体のレンダリング
SceneData cornell_smoke() {
    hittable_list objects;

    auto red = make_shared<lambertian>(make_shared<solid_color>(.65, .05, .05));
    auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
    auto green = make_shared<lambertian>(make_shared<solid_color>(.12, .45, .15));
    auto light = make_shared<diffuse_light>(make_shared<solid_color>(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 =
        make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 =
        make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));

    objects.add(
        make_shared<constant_medium>(box1, 0.01, make_shared<solid_color>(0, 0, 0))
    );
    objects.add(
        make_shared<constant_medium>(box2, 0.01, make_shared<solid_color>(1, 1, 1))
    );


    SceneData ret;
    ret.objects = objects;
    ret.lookfrom = vec3(278, 278, -800);
    ret.lookat = vec3(278, 278, 0);
    ret.vup = vec3(0, 1, 0);
    ret.vfov = 40.0;
    ret.dist_to_focus = 10.0;
    ret.aperture = 0.0;

    return ret;
}


// 最終シーン
SceneData final_scene() {
    hittable_list boxes1;
    auto ground =
        make_shared<lambertian>(make_shared<solid_color>(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(make_shared<solid_color>(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material =
        make_shared<lambertian>(make_shared<solid_color>(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(
        center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(
        point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 10.0)));

    auto boundary = make_shared<sphere>(
        point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(
        boundary, 0.2, make_shared<solid_color>(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(
        point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(
        boundary, .0001, make_shared<solid_color>(1, 1, 1)));

    auto emat = make_shared<lambertian>(
        make_shared<image_texture>("../resources/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(
        point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
        vec3(-100, 270, 395)
        )
    );

    SceneData ret;
    ret.objects = objects;
    ret.lookfrom = vec3(278, 278, -800);
    ret.lookat = vec3(278, 278, 0);
    ret.vup = vec3(0, 1, 0);
    ret.vfov = 40.0;
    ret.dist_to_focus = 10.0;
    ret.aperture = 0.0;

    return ret;
}



int main() {
    
    SceneData scene = final_scene();
    
    constexpr int image_width = 500;
    constexpr int image_height = 500;
    const int samples_per_pixel = 128;

    const double aspect_ratio = image_width / image_height;
    const int max_depth = 50;
    const color background(0, 0, 0);
    camera cam(
        scene.lookfrom, scene.lookat, scene.vup, scene.vfov, aspect_ratio, scene.aperture, scene.dist_to_focus, 0.0, 1.0
    );// カメラインスタンス生成


    //ppm用（並列化用バッファを用意）
    //std::vector<std::vector<color>> image(image_height, std::vector<color>(image_width));
    //std::ofstream ofs("image.ppm");
    //ofs << "P3\n" << image_width << " " << image_height << "\n255\n";

    //png用（並列化用バッファを用意）
    std::vector<unsigned char> image(image_width * image_height * 3, 255);

    
    const int threadsNum = omp_get_max_threads(); // 使える最大のスレッド数を得る
    std::cerr << " threads num : " << threadsNum << "\n";
    omp_set_num_threads(threadsNum);
#pragma omp parallel for
    for (int j = image_height - 1; j >= 0; --j) {
       
         // 並列化すると表示がおかしくなる
        if (omp_get_thread_num() == threadsNum - 1)
        {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        }

        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {// サンプル数だけループする
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, scene.objects, max_depth);
                
            }

            // ppmで出力
            //image[j][i] = postProcess(pixel_color, samples_per_pixel);

            // pngで出力
            vec3 color = postProcess(pixel_color, samples_per_pixel);
            image[image_width * image_height * 3 - 1 - ((image_width * j + i) * 3 + 2)] = static_cast<int>(256 * clamp(color[0], 0.0, 0.999));
            image[image_width * image_height * 3 - 1 - ((image_width * j + i) * 3 + 1)] = static_cast<int>(256 * clamp(color[1], 0.0, 0.999));
            image[image_width * image_height * 3 - 1 - ((image_width * j + i) * 3 + 0)] = static_cast<int>(256 * clamp(color[2], 0.0, 0.999));
        }
    }

    // ppmで出力
    //std::cerr << "\n\n";
    //for (int j = image_height - 1; j >= 0; --j) {
    //    std::cerr << "\routput scanlines remaining: " << j << ' ' << std::flush;
    //    for (int i = 0; i < image_width; ++i) {
    //            outputColor(ofs, image[j][i]);
    //    }
    //}
    //ofs.close();


    // pngで出力
    stbi_write_png("output.png", image_width, image_height, 3, image.data(), image_width * 3);

    std::cerr << "\nDone.\n";


#ifndef NDEBUG // DEBUG
    system("output.png");
#else // RELEASE
    system("output.png");
#endif

}
