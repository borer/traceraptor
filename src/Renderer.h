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
#include "BVH.h"


namespace traceraptor {

class Renderer {

	Vec3 shade(const Ray& ray, const BVH &world, int current_ray_bounce) {
		IntersectionInfo rec;
			if (world.getIntersection(ray, rec, false)) {
				Ray scattered;
				Vec3 attenuation;
				if (current_ray_bounce < max_ray_bounce &&
						rec.material->scatter(ray, rec, attenuation, scattered)) {
					return attenuation * shade(scattered, world, current_ray_bounce+1);
				} else {
					return Vec3(0, 0, 0);
				}
			} else {
				Vec3 unit_direction = unit_vector(ray.direction());
				float t = 0.5*(unit_direction.y() + 1.0);
				return lerp(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0), t);
			}
		}

public:
	Renderer(int width, int height, int ns, int max_ray_bounce) : width(width), height(height), ns(ns), max_ray_bounce(max_ray_bounce) {}

	void render_rectangle(int current_thread, int number_threads, const Camera &camera, const BVH &world, Image &image) {
		int height_per_thread = height / number_threads;
		int thread_height = (current_thread*height_per_thread) + height_per_thread;

		for (int j = current_thread; j < thread_height; j++) {
			for (int i = 0; i < width; i++) {
//				Logger::log_debug("Iteration [" + std::to_string(i) + ", " + std::to_string(j) +"] out of [" + std::to_string(width) + ", " + std::to_string(height) + "]");
				Vec3 col(0,0,0);
				for (int s=0; s < ns; s++) {
					float u = float(i + random01()) / float(width);
					float v = float(j + random01()) / float(height);
					Ray r = camera.get_ray(u, v);
					col += shade(r, world, 0);
				}
				col /= ns;
				col = Vec3(std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]));
				image.setPixel(i, j, col);
			}
		}
	}

	void render_scene(const Camera& camera, const BVH &world, const std::string &filename, const int number_threads = 1) {
		Image image(width, height);

		Logger::log_debug("Beginning ray tracing");

		std::vector<std::thread> renderThreads(number_threads);
		for(int i = 0 ; i < number_threads; ++i) {
			Logger::log_debug("Starting render thread " + std::to_string(i));
			renderThreads[i] = std::thread(&Renderer::render_rectangle,
					this,
					i,
					number_threads,
					std::ref(camera),
					std::ref(world),
					std::ref(image));
		}

		for(int i = 0 ; i < number_threads; ++i) {
			renderThreads[i].join();
			Logger::log_debug("Render thread " + std::to_string(i) + " has finished");
		}

		Logger::log_debug("Image writing");
		int result = image.writePNGfile(filename);
		if (result < 0) {
			Logger::log_error("Cannot create image file");
		}
	}

	int width;
	int height;
	int ns;
	int max_ray_bounce;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_RENDERER_H_ */
