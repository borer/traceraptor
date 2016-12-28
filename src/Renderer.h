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
	RenderChunk(int x, int y, int max_x, int max_y) : min_x(x), min_y(y), max_x(max_x), max_y(max_y) {}

	int min_x,min_y;
	int max_x, max_y;
	std::atomic_flag is_processed = ATOMIC_FLAG_INIT;
};

class Renderer {

	std::vector<std::shared_ptr<RenderChunk>> calculate_render_chuncks(int size = 16)
	{
		std::vector<std::shared_ptr<RenderChunk>> render_chunks;

		for(int i = 0; i < width; i+=size) {
			for(int j = 0; j < height; j+=size) {
				int max_width = i+size > width ? width : i+size;
				int max_height = j+size > height ? height : j+size;
				render_chunks.push_back(std::make_shared<RenderChunk>(i,j,max_width,max_height));
			}
		}

		return render_chunks;
	}

	Vec3 skybox_shade(const Ray& ray, Vec3 &top_color, Vec3 &bottom_color) {
		Vec3 unit_direction = unit_vector(ray.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return lerp(top_color, bottom_color, t);
	}

	Vec3 shade(const Ray& ray, const BVH &world, int current_ray_bounce, bool renderSkyBox) {
		IntersectionInfo rec;
			if (world.getIntersection(ray, rec, false)) {
				Ray scattered;
				Vec3 attenuation;
				Vec3 emitted = rec.material->emitted(rec.uv.u, rec.uv.v, rec.hit_point);

				if (current_ray_bounce < max_ray_bounce &&
						rec.material->scatter(ray, rec, attenuation, scattered)) {

					return emitted + attenuation*shade(scattered, world, current_ray_bounce+1, renderSkyBox);
				} else {
					return emitted;
				}
			} else {
				if (renderSkyBox) {
					Vec3 top_color(1.0, 1.0, 1.0);
					Vec3 bottom_color(0.5, 0.7, 1.0);
					return skybox_shade(ray, top_color, bottom_color);
				} else{
					Vec3 black_color(0, 0, 0);
					return black_color;
				}
			}
		}

public:
	Renderer(int width, int height, int ns, int max_ray_bounce, bool render_skybox) :
		width(width), height(height), ns(ns), max_ray_bounce(max_ray_bounce), render_skybox(render_skybox) {}

	void render_chunk(std::shared_ptr<RenderChunk> chunk, const Camera &camera, const BVH &world, Image &image) {
		for (int i = chunk->min_x; i < chunk->max_x; i++) {
			for (int j = chunk->min_y; j < chunk->max_y; j++) {
				Vec3 color(0,0,0);
				for (int s=0; s < ns; s++) {
					float u = float(i + random01()) / float(width);
					float v = float(j + random01()) / float(height);
					Ray r = camera.get_ray(u, v);
					color += shade(r, world, 0, render_skybox);
				}
				color /= ns;
				color.clamp01();
				color = Vec3(std::sqrt(color[0]), std::sqrt(color[1]), std::sqrt(color[2]));
				image.setPixel(i, j, color);
			}
		}
	}

	void render_worker(const std::vector<std::shared_ptr<RenderChunk>> &chunks, const Camera &camera, const BVH &world, Image &image) {
		int chunks_size = chunks.size();
		for(int i = 0; i < chunks_size; i++) {
			if (!chunks[i]->is_processed.test_and_set()) {
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
	bool render_skybox;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_RENDERER_H_ */
