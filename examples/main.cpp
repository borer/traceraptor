#include <Vec.h>
#include <string>
#include <chrono>

#include <Logger.h>
#include <shapes/Sphere.h>
#include <shapes/Triangle.h>
#include <Renderer.h>
#include <BVH.h>
#include <Texture.h>
#include <SceneLoader.h>
#include <integrators/Integrator.h>
#include <Shape.h>
#include <Primitive.h>

using namespace traceraptor;

std::vector<std::shared_ptr<Primitive>> create_random_scene() {
	const int n = 489;
	std::vector<std::shared_ptr<Primitive>> list(n);
	std::shared_ptr<Material> mat_ground(new Lambertian(
			std::make_shared<CheckerTexture>(
					std::make_shared<ConstantTexture>(Vec3f{0.5, 0.5, 0.5}),
					std::make_shared<ConstantTexture>(Vec3f{1,1,1}),
					10)));
	list[0] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{0,-1000,0}, 1000)), mat_ground);
	int i = 1;
	Sampler sampler;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = sampler.random01f();
			Vec3f center{a+0.9f*sampler.random01f(), 0.2f, b+0.9f*sampler.random01f()};
			if (choose_mat < 0.8) {
				Vec3f color = Vec3f{sampler.random01f()*sampler.random01f(), sampler.random01f()*sampler.random01f(), sampler.random01f()*sampler.random01f()};
				std::shared_ptr<Material> diffuse(new Lambertian(std::make_shared<ConstantTexture>(color)));
				list[i++] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(center, 0.2)), diffuse);
			}
			else if (choose_mat < 0.95) {
				Vec3f color{0.5f*(1 + sampler.random01f()), 0.5f*(1 + sampler.random01f()), 0.5f*(1 + sampler.random01f())};
				std::shared_ptr<Material> metal(new Metal(color,  0.5*sampler.random01f()));
				list[i++] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(center, 0.2)), metal);
			}
			else {
				std::shared_ptr<Material> glass(new Dielectric(1.5));
				list[i++] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(center, 0.2)), glass);
			}
		}
	}

	std::shared_ptr<Material> glass(new Dielectric(1.5));
	list[i++] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(
			new Sphere(Vec3f{0, 1, 0}, 1.0)), glass);
	std::shared_ptr<Material> glass2(new Dielectric(1.5));
	list[i++] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(
			new Sphere(Vec3f{0, 1, 0}, -0.95)), glass2);
	std::shared_ptr<Material> diffuse(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.4, 0.2, 0.1})));
	list[i++] = std::make_shared<GeometricPrimitive>(
			std::shared_ptr<Shape>(new Sphere(Vec3f{-4, 1, 0}, 1.0)), diffuse);
	std::shared_ptr<Material> metal(new Metal(Vec3f{0.7, 0.6, 0.5}, 0.0));
	list[i++] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(
			new Sphere(Vec3f{4, 1, 0}, 1.0)), metal);

	return list;
}

void random_setup(std::string filename){
	const int width = 800;
	const int height = 400;
	const int ns = 500;
	const int MAX_RAY_BOUNCE = 6;

	Vec3f lookfrom{13,2,3};
	Vec3f lookat{0,0,0};
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	Camera camera(lookfrom, lookat, Vec3f{0,1,0}, 20, float(width)/float(height), aperture, dist_to_focus);

	SimpleIntegrator simpleIntegrator(true, MAX_RAY_BOUNCE);
	Renderer renderer(width, height, ns, simpleIntegrator);
	std::vector<std::shared_ptr<Primitive>> world_objects = create_random_scene();
	BVH world(world_objects);

	renderer.render_scene(camera, world, filename, 4);
}

void manual_setup(std::string filename) {
	const int width = 800;
	const int height = 400;
	const int ns = 100;
	const int MAX_RAY_BOUNCE = 50;

	Vec3f lookfrom{-7,3,-4};
	Vec3f lookat{0,0,0};
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	Camera camera(lookfrom, lookat, Vec3f{0,1,0}, 20, float(width)/float(height), aperture, dist_to_focus);

	std::vector<std::shared_ptr<Primitive>> list(7);
    
    std::shared_ptr<GeometricPrimitive> baseMateSphere = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{0.f,0.0, 0.f}, 0.5)),
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.8f, 0.3f, 0.3f}))));
    Transform offsetTransform = Translate(Vec3f({0.0f, 0.5f, -1.0f}));

    list[0] = baseMateSphere;
    list[1] = std::make_shared<TransformedPrimitive>(baseMateSphere, offsetTransform);
	list[2] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{0.f,-1000.5f,0.f}, 1000)),
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.5, 0.5, 0.5}))));
	list[3] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{1.f,0.f,0.f}, 0.5)),
			std::shared_ptr<Material>(new Metal(Vec3f{0.7f, 0.6f, 0.5f}, 0.2)));
	list[4] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{-1.f,0.f,0.f}, 0.5)),
			std::shared_ptr<Material>(new Dielectric(1.2)));
	list[5] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{-1,0,0}, -0.45)),
			std::shared_ptr<Material>(new Dielectric(1.2)));
	list[6] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{1,0,-1}, 0.5)),
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.4, 0.2, 0.1}))));

    BVH world(list);
    SimpleIntegrator simpleIntegrator(true, MAX_RAY_BOUNCE);
    Renderer renderer(width, height, ns, simpleIntegrator);
    renderer.render_scene(camera, world, filename, 4);
}

void manual_setup_light(std::string filename) {
	const int width = 800;
	const int height = 400;
	const int ns = 50;
	const int MAX_RAY_BOUNCE = 8;

	Vec3f lookfrom{-7,3,-4};
	Vec3f lookat{0,0,0};
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	Camera camera(lookfrom, lookat, Vec3f{0,1,0}, 20, float(width)/float(height), aperture, dist_to_focus);

	std::vector<std::shared_ptr<Primitive>> list(6);
    list[0] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{0,1,0}, 0.5)),
			std::shared_ptr<Material>(new Emissive(std::make_shared<ConstantTexture>(Vec3f{0.5, 1, 1}))));
    list[1] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{0,-1000.5,0}, 1000)),
   			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.5, 0.5, 0.5}))));
    list[2] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{1,0,0}, 0.5)),
			std::shared_ptr<Material>(new Metal(Vec3f{0.7, 0.6, 0.5}, 0.2)));
    list[3] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{-1,0,0}, 0.5)),
			std::shared_ptr<Material>(new Dielectric(1.2)));
    list[4] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{-1,0,0}, -0.48)),
			std::shared_ptr<Material>(new Dielectric(1.2)));
    list[5] = std::make_shared<GeometricPrimitive>(std::shared_ptr<Shape>(new Sphere(Vec3f{1,0,-1},	0.5)),
			std::shared_ptr<Material>(new Lambertian(std::make_shared<ConstantTexture>(Vec3f{0.4, 0.2, 0.1}))));
    BVH world(list);
    SimpleIntegrator simpleIntegrator(true, MAX_RAY_BOUNCE);
    Renderer renderer(width, height, ns, simpleIntegrator);
    renderer.render_scene(camera, world, filename, 4);
}

void manual_triangle(std::string filename) {
	const int width = 500;
	const int height = 600;
	const int ns = 30;
	const int MAX_RAY_BOUNCE = 8;

	Vec3f lookfrom{0,1,4.5f};
	Vec3f lookat{0,0,0};
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 35;

	Camera camera(lookfrom, lookat, Vec3f{0,1,0}, vfov, float(width)/float(height), aperture, dist_to_focus);

	SceneLoader sceneLoader;
//	vector<shared_ptr<Primitive>> scene = sceneLoader.Load("cornell_box.obj");
    vector<shared_ptr<Primitive>> scene = sceneLoader.Load("simple-box.obj");

    BVH world(scene);
    SimpleIntegrator simpleIntegrator(true, MAX_RAY_BOUNCE);
    Renderer renderer(width, height, ns, simpleIntegrator);
    renderer.render_scene(camera, world, filename, 4);
}

int main(int argc, char* argv[]) {
	std::string filename = (argc < 2) ? "output.png" : argv[1];

	RayTracingStatistics::getInstance().start_time_tracking();

//	manual_setup(filename);
//	random_setup(filename);
//	manual_setup_light(filename);
	manual_triangle(filename);

	RayTracingStatistics::getInstance().end_time_tracking();
	std::string report = RayTracingStatistics::getInstance().generate_report();
	Logger::log_debug(report);

    return 0;
}
