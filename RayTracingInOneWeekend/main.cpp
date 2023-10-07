//#include "ray.h"
//#include "color.h"
//#include "vec3.h"
//
//#include <iostream>
//#include <fstream>
//
//
//double hit_sphere(const point3& center, double radius, const ray& r) {
//    vec3 oc = r.origin() - center; // ���̒��Scenter���王�_origin�ւ̃x�N�g��
//    auto a = dot(r.direction(), r.direction());
//    auto half_b = dot(oc, r.direction());
//    auto c = oc.length_squared() - radius * radius;
//    auto discriminant = half_b * half_b - a * c;// ���ʎ�
//    if (discriminant < 0) {// ray���Փ˂��Ȃ��ꍇ, t=-1.0�ŕԂ�
//        return -1.0;
//    }
//    else {// ray���Փ˂���ꍇ, t�̒l��Ԃ�
//        return (-half_b - sqrt(discriminant)) / a;
//    }
//}
//
//color ray_color(const ray& r) {
//    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
//    if (t > 0.0) {// t�����Ȃ�O���ŏՓ˂��Ă��邽��, �@����RGB�Ƃ��ďo�͂���
//        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
//        return 0.5 * color( N.x()+1, N.y()+1, N.z()+1 );
//    }
//    vec3 unit_direction = unit_vector(r.direction());
//    t = 0.5 * (unit_direction.y() + 1.0);// t��-1.0�Ȃ�Փ˂��Ă��Ȃ�����, y���W(����)�Ɉˑ������O���f�[�V�����̔w�i���o��
//    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}
//
//int main() {
//    const auto aspect_ratio = 16.0 / 9.0;
//    const int image_width = 200;
//    const int image_height = static_cast<int>(image_width / aspect_ratio);
//    
//    std::ofstream ofs("image.ppm");
//
//    ofs << "P3\n" << image_width << " " << image_height << "\n255\n";
//
//    auto viewport_height = 2.0;
//    auto viewport_width = aspect_ratio * viewport_height;
//    auto focal_length = 1.0;
//
//    auto origin = point3(0, 0, 0);
//    auto horizontal = vec3(viewport_width, 0, 0);
//    auto vertical = vec3(0, viewport_height, 0);
//    auto lower_left_corner =
//        origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
//
//    for (int j = image_height - 1; j >= 0; --j) {
//        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
//        for (int i = 0; i < image_width; ++i) {
//            auto u = double(i) / (image_width - 1);
//            auto v = double(j) / (image_height - 1);
//            // �C���X�^���X��������ۂɈ��������ꍇ. �R���X�g���N�^. ray.h������Ɨ���������.
//            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
//            color pixel_color = ray_color(r);
//            write_color(ofs, pixel_color);
//        }
//    }
//
//    ofs.close();
//    std::cerr << "\nDone.\n";
//
//    // i_view32.exe image.ppm�@�����s������
//    system("\"C:\\Program Files (x86)\\IrfanView\\i_view32.exe\" C:\\Users\\motes\\source\\repos\\RayTracingInOneWeekend\\RayTracingInOneWeekend\\image.ppm");
//
//}





// --------------------------------------------------------
#include <iostream>
#include "ray.h"
#include "vec3.h"
#include "hittable.h"


class motegi {
public:
	motegi() {}// default constructor

	motegi(int const age_)// user difined constructor
		: age(age_) {}

	virtual ~motegi() {}// destructor


private:// member functions
	bool undertwenty(int age) {
		return age < 20;
	}


public:// member functions
	double half_age(int const age) {
		if (undertwenty(age)) {// private�����o�֐��͓���̃N���X�����炵���A�N�Z�X�o���Ȃ�
			std::cout << "under 20 years old\n";
		}
		else {
			std::cout << "not under 20 years old\n";
		}
		return static_cast<double>(age / 2);// ������static_cast�͏����Ă������Ȃ��Ă�
	}

public:// member variables
	int age;
};


class Myclass {
int private_num;//class�̏ꍇ, public�w�肵�Ȃ��ƃf�t�H��private�ɂȂ�

public:
	int public_num;
};




int main(void) {
	int const myage = 23;
	std::cout << "input age is " << myage << std::endl;

	motegi myMotegi(myage);//�C���X�^���X(�I�u�W�F�N�g)����

	double const myhalf_age = myMotegi.half_age(myage);
	std::cout << "half age is " << myhalf_age << std::endl;


	point3 origin = {0,0,0};
	vec3 direction = { 2,3,4 };//���[�U�[��`�R���X�g���N�^�ŃC���X�^���X�𐶐����͏�����
	ray sample_one(origin, direction);
	std::cout << sample_one.origin() << std::endl;
	std::cout << sample_one.direction() << std::endl;
	std::cout << sample_one.at(0.2) << std::endl;


	ray sample_two;//�f�t�H���g�R���X�g���N�^����ꍇ�̓C���X�^���X�𐶐�����Ƃ�()�s�v
	sample_two.orig = point3{ 0,0,0 };// ���Ƃ��烁���o�ϐ���ݒ�
	sample_two.dir = vec3{ 3,4,5 };
	std::cout << sample_two.origin() << std::endl;
	std::cout << sample_two.direction() << std::endl;
	std::cout << sample_two.at(0.2) << std::endl;


	Myclass myclass;
	myclass.public_num = 2;//�����o�ϐ���public�ɂȂ��Ă��邽�ߎ��s��
	//myclass.private_num = 2;//�����o�ϐ���private�ɂȂ��Ă��邽�ߎ��s�s��
}