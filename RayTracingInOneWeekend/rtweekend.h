#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <cstdlib>// rand()���g����

//#include <random>// �W���̗��������w�b�_�[


// using
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// �萔
const double infinity = std::numeric_limits<double>::infinity();// �����������
const double pi = 3.1415926535897932385;// ���w�萔�͕ʃw�b�_�[�Œ�`���Ă����Ƃ܂Ƃ܂��Ă����B

// ���[�e�B���e�B�֐�
inline double degrees_to_radians(double degrees) {// �x�����烉�W�A����
	return degrees * pi / 180;
}

inline double random_double() {// ���������̏ꍇ��0����1
	// [0,1) �̎���������Ԃ�
	return rand() / (RAND_MAX + 1.0);// rand()��0����RAND_MAX�̒l��Ԃ��B
}

inline double random_double(double min, double max) {// ������^�����ꍇ�͂��̋�Ԃŗ����𐶐��B
	// [min,max) �̎���������Ԃ�
	return min + (max - min) * random_double();
}

//inline double random_double() {//<random>���g�p����ꍇ
//	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//	static std::mt19937 generator;
//	return generator(distribution);
//}


inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}


// ���ʃw�b�_�[�i�܂Ƃ߂Ă�����include���Ă�����rtweekend.h��include����΍Œ��ok�ȏ�ԂɂȂ�j
#include "ray.h"
#include "vec3.h"
#include "color.h"

#endif