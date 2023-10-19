#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <cstdlib>// rand()を使える

//#include <random>// 標準の乱数生成ヘッダー


// using
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// 定数
const double infinity = std::numeric_limits<double>::infinity();// そういうやつ
const double pi = 3.1415926535897932385;// 数学定数は別ヘッダーで定義しておくとまとまっていい。

// ユーティリティ関数
inline double degrees_to_radians(double degrees) {// 度数からラジアンへ
	return degrees * pi / 180;
}

inline double random_double() {// 引数無しの場合は0から1
	// [0,1) の実数乱数を返す
	return rand() / (RAND_MAX + 1.0);// rand()は0からRAND_MAXの値を返す。
}

inline double random_double(double min, double max) {// 引数を与えた場合はその区間で乱数を生成。
	// [min,max) の実数乱数を返す
	return min + (max - min) * random_double();
}

//inline double random_double() {//<random>を使用する場合
//	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//	static std::mt19937 generator;
//	return generator(distribution);
//}


inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}


// 共通ヘッダー（まとめてここでincludeしておく＝rtweekend.hをincludeすれば最低限okな状態になる）
#include "ray.h"
#include "vec3.h"
#include "color.h"

#endif