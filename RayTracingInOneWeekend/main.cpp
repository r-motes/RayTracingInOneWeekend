#include <iostream>
// #include <fstream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

// g—p‚·‚énamespace‚ÌéŒ¾(ex: std::cout ¨ cout)
using namespace std;


int main() {
	const int image_width = 10;
	const int image_height = 10;

	// out file stream
	//std::ofstream ofs("image.ppm");
	//if (!ofs)
	//{
	//	std::cout << "failed to open image file\n";
	//}

	std::cout << "p3\n" << image_width << ' ' << image_height << "\n255\n";
	//ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {

			color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
			write_color(std::cout, pixel_color);

			// ofs << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}

	std::cerr << "\nDone.\n";

	//ofs.close();

}