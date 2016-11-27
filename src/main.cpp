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
#include "Renderer.h"

using namespace traceraptor;

void random_setup(std::string filename){
	    const int width = 800;
	    const int height = 400;
	    const int ns = 10;
	    const int MAX_RAY_BOUNCE = 6;

	    Vec3 lookfrom(13,2,3);
	    Vec3 lookat(0,0,0);
	    float dist_to_focus = 10.0;
	    float aperture = 0.1;

	    Camera camera(lookfrom, lookat, Vec3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);

	    Renderer renderer(width, height, ns, MAX_RAY_BOUNCE);
	    renderer.render_random_scene(camera, filename, 4);
}

void manual_setup(std::string filename) {
	const int width = 800;
	const int height = 400;
	const int ns = 500;
	const int MAX_RAY_BOUNCE = 50;

	Vec3 lookfrom(-7,3,-4);
	Vec3 lookat(0,0,0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	Camera camera(lookfrom, lookat, Vec3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);

    Hitable *list[6];
    list[0] = new Sphere(Vec3(0,0,0), 0.5, std::shared_ptr<Material>(new Lambertian(Vec3(0.8, 0.3, 0.3))));
    list[1] = new Sphere(Vec3(0,-100.5,0), 100, std::shared_ptr<Material>(new Lambertian(Vec3(0.5, 0.5, 0.5))));
    list[2] = new Sphere(Vec3(1,0,0), 0.5, std::shared_ptr<Material>(new Metal(Vec3(0.7, 0.6, 0.5), 0.2)));
    list[3] = new Sphere(Vec3(-1,0,0), 0.5, std::shared_ptr<Material>(new Dielectric(1.2)));
    list[4] = new Sphere(Vec3(-1,0,0), -0.45, std::shared_ptr<Material>(new Dielectric(1.2)));
    list[5] = new Sphere(Vec3(1,0,-1), 0.5, std::shared_ptr<Material>(new Lambertian(Vec3(0.4, 0.2, 0.1))));
    Hitable *world = new HitableList(list, 6);

    Renderer renderer(width, height, ns, MAX_RAY_BOUNCE);
    renderer.render_scene(camera, world, filename, 4);
}

int main(int argc, char* argv[]) {
	std::string filename = (argc < 2) ? "output.png" : argv[1];

	manual_setup(filename);

    return 0;
}
