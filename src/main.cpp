#include <Logger.h>
#include <string>

#include "Image.h"
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "UtilMath.h"
#include "Material.h"

using namespace traceraptor;

Vec3 color(const Ray& r, Hitable *world, int current_ray_bounce, int MAX_RAY_BOUNCE) {
	hit_record rec;
	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		Ray scattered;
		Vec3 attenuation;
		if (current_ray_bounce < MAX_RAY_BOUNCE &&
				rec.material_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, current_ray_bounce+1, MAX_RAY_BOUNCE);
		} else {
			return Vec3(0, 0, 0);
		}
	} else {
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return lerp(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0), t);
	}
}

Hitable *random_scene() {
    int n = 500;
    Hitable **list = new Hitable*[n+1];
    list[0] =  new Sphere(Vec3(0,-1000,0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random01();
            Vec3 center(a+0.9*random01(),0.2,b+0.9*random01());
            if ((center-Vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                	Vec3 color = Vec3(random01()*random01(), random01()*random01(), random01()*random01());
                    list[i++] = new Sphere(center, 0.2, new Lambertian(color));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new Sphere(center, 0.2,
                            new Metal(Vec3(0.5*(1 + random01()), 0.5*(1 + random01()), 0.5*(1 + random01())),  0.5*random01()));
                }
                else {  // glass
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

    return new HitableList(list,i);
}

int main(int argc, char* argv[]) {
	std::string fileName = (argc < 2) ? "output.png" : argv[1];

    const int width = 1200;
    const int height = 800;
    const int ns = 1000;
    const int MAX_RAY_BOUNCE = 50;
    Image image(width, height);

    Hitable *world = random_scene();
    Vec3 lookfrom(13,2,3);
    Vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    Camera camera(lookfrom, lookat, Vec3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);
    Logger::log_debug("Beginning ray tracing");
    for (int j = 0; j < height; j++) {
    	for (int i = 0; i < width; i++) {
    		Logger::log_debug("Iteration [" + std::to_string(i) + ", " + std::to_string(j) +"] out of [" + std::to_string(width) + ", " + std::to_string(height) + "]");
    		Vec3 col(0,0,0);
    		for (int s=0; s < ns; s++) {
    			float u = ((float)i + random01())/(float)width;
    			float v = ((float)j + random01())/(float)height;
    			Ray r = camera.get_ray(u, v);
    			col += color(r, world, 0, MAX_RAY_BOUNCE);
    		}
    		col /= ns;
    		col = Vec3(std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]));
    		image.setPixel(i, j, col);
    	}
    }

    Logger::log_debug("Beginning image writing");
    int result = image.writePNGfile(fileName);
    if (result < 0) {
    	Logger::log_debug("cannot create image file");
    	return -1;
    }

    return 0;
}
