#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
// �A�N�Z�X�w��q�ipublic��class�̊O����A�N�Z�X��, private��class��������̂݉�, protected�̓N���X�����Ⴕ���͔h���N���X����̂݉�.�j
public:
	ray() {}

	ray(const point3& origin, const vec3& direction)
		: orig(origin), dir(direction) {}

	point3 origin() const {
		return orig;
	}

	vec3 direction() const {
		return dir;
	}

	point3 at(double t) const {
		return orig + t * dir;
	}


public:
	point3 orig;
	vec3 dir;

};


#endif