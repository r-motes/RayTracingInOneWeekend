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
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }//

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
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);


    hittable_list world;
    world.add(make_shared<sphere>(
        point3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.7, 0.3, 0.3))));
    world.add(make_shared<sphere>(
        point3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));
    world.add(make_shared<sphere>(
        point3(1, 0, -1), 0.5, make_shared<metal>(color(.8, .6, .2))));
    world.add(make_shared<sphere>(
        point3(-1, 0, -1), 0.5, make_shared<metal>(color(.8, .8, .8))));


    camera cam;// カメラインスタンス生成

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






//// practice
//// ---------------------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include "ray.h"
//#include "vec3.h"
//#include "hittable.h"
//
//
//class motegi {
//public:
//	motegi() {}// default constructor
//
//	motegi(int const age_)// user difined constructor
//		: age(age_) {}
//
//	virtual ~motegi() {}// デストラクタは delete p; を実行してオブジェクトを削除した時に実行される
//
//
//private:// member functions
//	bool undertwenty(int age) {
//		return age < 20;
//	}
//
//
//public:// member functions
//	double half_age(int const age) {
//		if (undertwenty(age)) {// privateメンバ関数は同一のクラス内からしかアクセス出来ない
//			std::cout << "under 20 years old\n";
//		}
//		else {
//			std::cout << "not under 20 years old\n";
//		}
//		return static_cast<double>(age / 2);// ここのstatic_castは書いても書かなくても
//	}
//
//public:// member variables
//	int age;
//};
//
//
//class Myclass {
//int private_num;//classの場合, public指定しないとデフォでprivateになる
//
//public:
//	int public_num;
//};
//
//
//struct Mystruct {
//	// デフォルトコンストラクタ
//	Mystruct() : name("Unknown"), age(0) {}
//
//	// パラメータつきコンストラクタ
//	Mystruct(const std::string& n, int a) : name(n), age(a) {}
//
//
//	// メンバ変数
//	std::string const name;
//	int const age;
//
//	// メンバ関数
//	double onethird(double num) {
//		return static_cast<double>(num / 3.0);
//	}
//	void display() {
//		std::cout << "name：" << name << "\nage：" << age << std::endl;
//	}
//};
//
//
//void twice(int* a) {
//	if (a == nullptr) {
//		std::cout << "nullptrが返されました\n";
//		return;
//	}
//	std::cout << "address：" << a << std::endl;
//	*a *= 2;
//}
//
//
//void twice(int& a) {
//	std::cout << "address：" << &a << std::endl;
//	a *= 2;
//}
//
//
//
//
//
//
//
//int main(void) {
//	int const myage = 23;
//	std::cout << "input age is " << myage << std::endl;
//
//	motegi myMotegi(myage);//インスタンス(オブジェクト)生成
//
//	double const myhalf_age = myMotegi.half_age(myage);
//	std::cout << "half age is " << myhalf_age << std::endl;
//
//
//	point3 origin = {0,0,0};
//	vec3 direction = { 2,3,4 };//ユーザー定義コンストラクタでインスタンスを生成時は初期化
//	ray sample_one(origin, direction);
//	std::cout << sample_one.origin() << std::endl;
//	std::cout << sample_one.direction() << std::endl;
//	std::cout << sample_one.at(0.2) << std::endl;
//
//
//	ray sample_two;//デフォルトコンストラクタある場合はインスタンスを生成するとき()不要
//	sample_two.orig = point3{ 0,0,0 };// あとからメンバ変数を設定
//	sample_two.dir = vec3{ 3,4,5 };
//	std::cout << sample_two.origin() << std::endl;
//	std::cout << sample_two.direction() << std::endl;
//	std::cout << sample_two.at(0.2) << std::endl;
//
//
//	Myclass myclass;
//	myclass.public_num = 2;//メンバ変数がpublicになっているため実行可
//	//myclass.private_num = 2;//メンバ変数がprivateになっているため実行不可
//
//
//	Mystruct mystruct1;
//	std::cout << mystruct1.name << std::endl;
//	std::cout << mystruct1.age << std::endl;
//	mystruct1.display();
//
//
//	Mystruct mystruct2("shunya", 22);
//	std::cout << mystruct2.name << std::endl;
//	std::cout << mystruct2.age << std::endl;
//	mystruct2.display();
//	std::cout << mystruct2.onethird(22) << std::endl;
//
//	vec3 v1 = { 1, 2, 3 };
//	vec3 v2 = { 2, 3, 4 };
//	v1 += v2;
//	v2 *= 2;
//	std::cout << "v1：" << v1 << std::endl;
//	std::cout << "v2：" << v2 << std::endl;
//	vec3 sum = v1 + v2;
//	std::cout << "sum：" << sum << std::endl;
//	std::cout << "v1.length()：" << v1.length() << std::endl;
//	std::cout << "v1.length_squared()：" << v1.length_squared() << std::endl;
//	std::cout << "dot(v1, v2)：" << dot(v1, v2) << std::endl;
//
//	// int型ベクトルのエイリアスの生成
//	using Intvector = std::vector<int>;//#include <vector>
//	Intvector numarray;
//	numarray = { 1, 2, 3, 4, 5 };
//	//std::cout << numarray << std::endl;//これは出力できない
//
//
//
//
//	// ポインタはメモリアドレスを持つ。つまりnullを持つ可能性がある。
//	// 参照は、元の変数の別名のような扱いであり、元の変数がある上で生成される。
//	// そのため、エラーを起こす可能性が低い。関数の引数で渡すことで値渡しのようにコピーを生成する必要もなく、
//	// ポインタのように直接渡した変数の値を直接操作できるが、エラーの危険性が低い。
//	// 基本的にはc++における関数の引数は参照を利用すべき。
//	// int程度なら(const int a)で複製してもいいが、引数が重い場合は(const A& a)のように参照で渡すといい。
//
//	twice(nullptr);
//
//	int v = 16;
//	int* p = &v;// int変数vのアドレスを持つポインタpを生成
//	std::cout << v << std::endl;
//	twice(p);
//	std::cout << v << std::endl;
//
//	int m = 15;
//	std::cout << m << std::endl;
//	twice(m);
//	std::cout << m << std::endl;
//
//
//	return 0;
//}