#include <string>

#include "Logger.h"
#include "Vec3.h"
#include "Hitable.h"
#include "Sphere.h"
#include "Renderer.h"
#include "BVH.h"
#include "Texture.h"

using namespace traceraptor;

std::vector<std::shared_ptr<Hitable>> create_random_scene() {
	const int n = 489;
	std::vector<std::shared_ptr<Hitable>> list(n);
	std::shared_ptr<Material> mat_ground(new Lambertian(
			std::make_shared<CheckerTexture>(
					std::make_shared<ConstantTexture>(Vec3(0.5, 0.5, 0.5)),
					std::make_shared<ConstantTexture>(Vec3(1,1,1)),
					10)));
	list[0] =  std::shared_ptr<Hitable>(new Sphere(Vec3(0,-1000,0), 1000, mat_ground));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random01();
			Vec3 center(a+0.9*random01(),0.2,b+0.9*random01());
			if (choose_mat < 0.8) {
				Vec3 color = Vec3(random01()*random01(), random01()*random01(), random01()*random01());
				std::shared_ptr<Material> diffuse(new Lambertian(std::make_shared<ConstantTexture>(color)));
				list[i++] = std::shared_ptr<Hitable>(new Sphere(center, 0.2, diffuse));
			}
			else if (choose_mat < 0.95) {
				std::shared_ptr<Material> metal(new Metal(Vec3(0.5*(1 + random01()), 0.5*(1 + random01()), 0.5*(1 + random01())),  0.5*random01()));
				list[i++] = std::shared_ptr<Hitable>(new Sphere(center, 0.2, metal));
			}
			else {
				std::shared_ptr<Material> glass(new Dielectric(1.5));
				list[i++] = std::shared_ptr<Hitable>(new Sphere(center, 0.2, glass));
			}
		}
	}

	std::shared_ptr<Material> glass(new Dielectric(1.5));
	list[i++] = std::shared_ptr<Hitable>(new Sphere(Vec3(0, 1, 0), 1.0, glass));
	std::shared_ptr<Material> glass2(new Dielectric(1.5));
	list[i++] = std::shared_ptr<Hitable>(new Sphere(Vec3(0, 1, 0), -0.95, glass2));
	std::shared_ptr<Material> diffuse(new Lambertian(std::make_shared<ConstantTexture>(Vec3(0.4, 0.2, 0.1))));
	list[i++] = std::shared_ptr<Hitable>(new Sphere(Vec3(-4, 1, 0), 1.0, diffuse));
	std::shared_ptr<Material> metal(new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
	list[i++] = std::shared_ptr<Hitable>(new Sphere(Vec3(4, 1, 0), 1.0, metal));

	return list;
}

void random_setup(std::string filename){
	const int width = 800;
	const int height = 400;
	const int ns = 500;
	const int MAX_RAY_BOUNCE = 6;

	Vec3 lookfrom(13,2,3);
	Vec3 lookat(0,0,0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	Camera camera(lookfrom, lookat, Vec3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);

	Renderer renderer(width, height, ns, MAX_RAY_BOUNCE);
	std::vector<std::shared_ptr<Hitable>> world_objects = create_random_scene();
	BVH world(world_objects);

	renderer.render_scene(camera, world, filename, 4);
}

void manual_setup(std::string filename) {
	const int width = 800;
	const int height = 400;
	const int ns = 100;
	const int MAX_RAY_BOUNCE = 50;

	Vec3 lookfrom(-7,3,-4);
	Vec3 lookat(0,0,0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	Camera camera(lookfrom, lookat, Vec3(0,1,0), 20, float(width)/float(height), aperture, dist_to_focus);

	std::vector<std::shared_ptr<Hitable>> list(6);
    list[0] = std::shared_ptr<Hitable>(new Sphere(Vec3(0,0,0),
    		0.5,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3(0.8, 0.3, 0.3))))));
    list[1] = std::shared_ptr<Hitable>(new Sphere(Vec3(0,-1000.5,0),
    		1000,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3(0.5, 0.5, 0.5))))));
    list[2] = std::shared_ptr<Hitable>(new Sphere(Vec3(1,0,0),
    		0.5,
			std::shared_ptr<Material>(new Metal(Vec3(0.7, 0.6, 0.5), 0.2))));
    list[3] = std::shared_ptr<Hitable>(new Sphere(Vec3(-1,0,0),
    		0.5,
			std::shared_ptr<Material>(new Dielectric(1.2))));
    list[4] = std::shared_ptr<Hitable>(new Sphere(Vec3(-1,0,0),
    		-0.45,
			std::shared_ptr<Material>(new Dielectric(1.2))));
    list[5] = std::shared_ptr<Hitable>(new Sphere(Vec3(1,0,-1),
    		0.5,
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3(0.4, 0.2, 0.1))))));
    BVH world(list);

    Renderer renderer(width, height, ns, MAX_RAY_BOUNCE);
    renderer.render_scene(camera, world, filename, 4);
}

int main(int argc, char* argv[]) {
	std::string filename = (argc < 2) ? "output.png" : argv[1];

//	manual_setup(filename);
	random_setup(filename);

    return 0;
}
