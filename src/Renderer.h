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
#include "Sphere.h"
#include "Camera.h"
#include "UtilMath.h"
#include "Material.h"
#include "BVH.h"


namespace traceraptor {

class RenderChunk {
public:
	RenderChunk(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

	int x,y;
	int width, height;
	std::atomic_flag is_available = ATOMIC_FLAG_INIT;
};

class Renderer {

	std::vector<std::shared_ptr<RenderChunk>> calculate_render_chuncks(int size = 16)
	{
		std::vector<std::shared_ptr<RenderChunk>> render_chunks;

		for(int i = 0; i < width; i+=size) {
			for(int j = 0; j < height; j+=size) {
				int chunk_width = i+size > width ? width-i : i+size;
				int chunk_height = j+size > height ? height-j : j+size;
				render_chunks.push_back(std::make_shared<RenderChunk>(i,j,chunk_width,chunk_height));
			}
		}

		return render_chunks;
	}

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

	void render_chunk(std::shared_ptr<RenderChunk> chunk, const Camera &camera, const BVH &world, Image &image) {
		for (int i = chunk->x; i < chunk->width; i++) {
			for (int j = chunk->y; j < chunk->height; j++) {
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

	void render_worker(const std::vector<std::shared_ptr<RenderChunk>> &chunks, const Camera &camera, const BVH &world, Image &image) {
		int chunks_size = chunks.size();
		for(int i = 0; i < chunks_size; i++) {
			if (chunks[i]->is_available.test_and_set()) {
//				Logger::log_debug("Rendering chunk " + std::to_string(i) + " out of " + std::to_string(chunks_size));
				render_chunk(chunks[i], camera, world, image);
			}
		}
	}

	void render_scene(const Camera& camera, const BVH &world, const std::string &filename, const int number_threads = 1) {
		Image image(width, height);

		Logger::log_debug("Beginning ray tracing");

		std::vector<std::shared_ptr<RenderChunk>> chunks = calculate_render_chuncks(32);

		std::vector<std::thread> renderThreads(number_threads);
		for(int i = 0 ; i < number_threads; ++i) {
			Logger::log_debug("Starting render thread " + std::to_string(i));
			renderThreads[i] = std::thread(&Renderer::render_worker,
					this,
					std::ref(chunks),
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
