/*
 * Renderer.h
 *
 *  Created on: Nov 27, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_RENDERER_H_
#define TRACERAPTOR_RENDERER_H_

#include <Logger.h>
#include <string>
#include <thread>

#include "Image.h"
#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "UtilMath.h"
#include "Material.h"


namespace traceraptor {

class Renderer {
	Hitable *random_scene() const {
	    const int n = 500;
	    Hitable **list = new Hitable*[n+1];
	    std::shared_ptr<Material> mat(new Lambertian(Vec3(0.5, 0.5, 0.5)));
	    list[0] =  new Sphere(Vec3(0,-1000,0), 1000, mat);
	    int i = 1;
	    for (int a = -11; a < 11; a++) {
	        for (int b = -11; b < 11; b++) {
	            float choose_mat = random01();
	            Vec3 center(a+0.9*random01(),0.2,b+0.9*random01());
	            if ((center-Vec3(4,0.2,0)).length() > 0.9) {
	                if (choose_mat < 0.8) {
	                	Vec3 color = Vec3(random01()*random01(), random01()*random01(), random01()*random01());
	                	std::shared_ptr<Material> diffuse(new Lambertian(color));
	                    list[i++] = new Sphere(center, 0.2, diffuse);
	                }
	                else if (choose_mat < 0.95) {
	                	std::shared_ptr<Material> metal(new Metal(Vec3(0.5*(1 + random01()), 0.5*(1 + random01()), 0.5*(1 + random01())),  0.5*random01()));
	                    list[i++] = new Sphere(center, 0.2, metal);
	                }
	                else {
	                	std::shared_ptr<Material> glass(new Dielectric(1.5));
	                    list[i++] = new Sphere(center, 0.2, glass);
	                }
	            }
	        }
	    }

	    std::shared_ptr<Material> glass(new Dielectric(1.5));
	    list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, glass);
	    std::shared_ptr<Material> diffuse(new Lambertian(Vec3(0.4, 0.2, 0.1)));
	    list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, diffuse);
	    std::shared_ptr<Material> metal(new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
	    list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, metal);

	    return new HitableList(list,i);
	}

	Vec3 color(const Ray& r, Hitable *world, int current_ray_bounce, int max_ray_bounce) {
			hit_record rec;
			if (world->hit(r, 0.001, MAXFLOAT, rec)) {
				Ray scattered;
				Vec3 attenuation;
				if (current_ray_bounce < max_ray_bounce &&
						rec.material->scatter(r, rec, attenuation, scattered)) {
					return attenuation*color(scattered, world, current_ray_bounce+1, max_ray_bounce);
				} else {
					return Vec3(0, 0, 0);
				}
			} else {
				Vec3 unit_direction = unit_vector(r.direction());
				float t = 0.5*(unit_direction.y() + 1.0);
				return lerp(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0), t);
			}
		}

public:
	Renderer(int width, int height, int ns, int max_ray_bounce) : width(width), height(height), ns(ns), max_ray_bounce(max_ray_bounce) {}

	void render_rectangle(int current_thread, int number_threads, const Camera &camera, Hitable *world, Image *image) {
		int height_per_thread = height / number_threads;
		int thread_height = (current_thread*height_per_thread) + height_per_thread;

		for (int j = current_thread; j < thread_height; j++) {
			for (int i = 0; i < width; i++) {
				Logger::log_debug("Iteration [" + std::to_string(i) + ", " + std::to_string(j) +"] out of [" + std::to_string(width) + ", " + std::to_string(height) + "]");
				Vec3 col(0,0,0);
				for (int s=0; s < ns; s++) {
					float u = float(i + random01()) / float(width);
					float v = float(j + random01()) / float(height);
					Ray r = camera.get_ray(u, v);
					col += color(r, world, 0, max_ray_bounce);
				}
				col /= ns;
				col = Vec3(std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]));
				image->setPixel(i, j, col);
			}
		}
	}

	void render_scene(const Camera& camera, Hitable *world, std::string filename, const int number_threads = 1) {
		Image image(width, height);

		Logger::log_debug("Beginning ray tracing");

		std::vector<std::thread> renderThreads(number_threads);
		for(int i = 0 ; i < number_threads; ++i) {
			renderThreads[i] = std::thread(&Renderer::render_rectangle,
					this,
					i,
					number_threads,
					camera,
					world,
					&image);
		}

		for(int i = 0 ; i < number_threads; ++i) {
			renderThreads[i].join();
		}

		Logger::log_debug("Image writing");
		int result = image.writePNGfile(filename);
		if (result < 0) {
			Logger::log_debug("cannot create image file");
		}
	}

	void render_random_scene(const Camera& camera, std::string filename, int number_threads = 1) {
		Hitable *world = random_scene();
		render_scene(camera, world, filename, number_threads);
	}

	int width;
	int height;
	int ns;
	int max_ray_bounce;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_RENDERER_H_ */
