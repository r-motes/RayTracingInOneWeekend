#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
	// �f�t�H���g�R���X�g���N�^
	vec3() : e{0,0,0} {}

	// �������R���X�g���N�^
	vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

	// �e�����ɃA�N�Z�X���邽�߂̃����o�֐�(vec.x()�I�Ȃ��)
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// �P�����Z�q '-' �̃I�[�o�[���[�h
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

	// �Y�������Z�q '[]' �̃I�[�o�[���[�h
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }
	
	// ����������Z�q '+=' �̃I�[�o�[���[�h
	vec3& operator += (const vec3 &v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// ����������Z�q '*=' �̃I�[�o�[���[�h
	vec3& operator *= (const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	// ����������Z�q '/=' �̃I�[�o�[���[�h
	vec3& operator /= (const double t) {
		return *this *= 1 / t;
	}

	// �x�N�g���̒������v�Z���郁���o�֐�
	double length() const {
		return sqrt(length_squared());
	}

	// �x�N�g���̒�����2����v�Z���郁���o�֐�
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// �����_����vec3�𐶐�
	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max),
			random_double(min, max),
			random_double(min, max));
	}

public:
	// �����o�ϐ��Ƃ���3�̗v�f�����z��
	double e[3];
};


// vec3 �̌^�G�C���A�X(�����̌^��l�ɐV�������O������d�g�݁��V�������O�̃R�s�[)
//c++11�ȍ~�ł�using���A�ȑO�ł�typedef��p����B
using point3 = vec3; // 3D�_
using color = vec3; // RGB�F




// vec3 ���[�e�B���e�B�֐�
// �R���p�C���ɑ΂��A�C�����C���W�J���Ă���
// �C�����C���W�J�Ƃ́A�������֐��Ȃǂ��p�ɂɌĂяo�����ꍇ�A
// �Ăяo���̃I�[�o�[�w�b�h�����\�Ɉ��e����^����\�������邽�߁A
// �֐��̖{�̂��Ăяo�����̃R�[�h�ɖ��ߍ���ł��܂����@�B
// ��ʂɂ̓w�b�_�[�t�@�C���ɏ�����邱�Ƃ������A�N���X�̃����o�֐��Ȃǂ�
// �p�ɂɌĂ΂��ꍇ�Ainline��p���邱�ƂŐ��\���オ���҂ł���B

// vec3��std::cout�ŏo�͉\�ɂ��Ă���
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// �ȉ���vec3�̌v�Z���`
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

// dot(v1, v2)
inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// ���K��
inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}


// �P�ʋ����Ƀ����_��(cos�̕��z)
vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);// xyz�̊e�v�f��-1����1�̊ԂŃ����_���Ȓl�����vec3�𐶐�����
		if (p.length_squared() >= 1) continue;// �傫����1�𒴂���Ȃ������x�T���v������i���p�@�j
		return p;
	}
}


// ���S�ɋ��ʏ�ɂ�����@�����z
vec3 random_unit_vector() {
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r * cos(a), r * sin(a), z);
}


vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0)
		return in_unit_sphere; // in_unit_sphere �� normal �Ɠ��������ɂ���
	else
		return -in_unit_sphere;
}

// ���˃x�N�g���̌v�Z
vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

#endif