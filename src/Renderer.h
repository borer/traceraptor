/*
 * Renderer.h
 *
 *  Created on: Nov 27, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_RENDERER_H_
#define TRACERAPTOR_RENDERER_H_

#include <string>
#include <thread>

#include <Logger.h>
#include <MathUtil.h>
#include <Vec.h>
#include <Image.h>
#include <Ray.h>
#include <Camera.h>
#include <Material.h>
#include <accelerators/BVH.h>
#include <RayTracingStatistics.h>
#include <integrators/Integrator.h>
#include <Shape.h>

namespace traceraptor {

class RenderChunk {
public:
	RenderChunk(int x, int y, int max_x, int max_y) : min_x(x), min_y(y), max_x(max_x), max_y(max_y) {}

	int min_x,min_y;
	int max_x, max_y;
	std::atomic_flag is_processed = ATOMIC_FLAG_INIT;
};

class Renderer {

public:
	Renderer(int width, int height, int ns, const Integrator& integrator) :
		width(width),
		height(height),
		ns(ns),
		integrator(integrator) { }

	void render_chunk(std::shared_ptr<RenderChunk> chunk, const Camera& camera, const Aggregate& world, Image& image) {
		float sampler_seed = chunk->min_x + chunk->min_y;
		Sampler sampler(sampler_seed);
		for (int i = chunk->min_x; i < chunk->max_x; i++) {
			for (int j = chunk->min_y; j < chunk->max_y; j++) {
				RGBColor color{0,0,0};
				for (int s=0; s < ns; s++) {
					INCREMENT_PRIMARY_RAY_STATISTICS
					float u = float(i + sampler.random01f()) / float(width);
					float v = float(j + sampler.random01f()) / float(height);
					Ray r = camera.get_ray(u, v, sampler);
					color += integrator.Li(r, world, sampler);
				}
				color /= (float)ns;
                RGBColor compressedColor = color.CompressGamma();
				image.setPixel(i, j, compressedColor);
			}
		}
	}

	void render_worker(const std::vector<std::shared_ptr<RenderChunk>>& chunks, const Camera& camera, const Aggregate& world, Image& image) {
		int chunks_size = chunks.size();
		for(int i = 0; i < chunks_size; i++) {
			if (!chunks[i]->is_processed.test_and_set()) {
//				Logger::log_debug("Rendering chunk " + std::to_string(i) + " out of " + std::to_string(chunks_size));
				render_chunk(chunks[i], camera, world, image);
			}
		}
	}

	void render_scene(const Camera& camera, const Aggregate& world, const std::string& filename, const int number_threads = 1) {
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

private:

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

	int width;
	int height;
	int ns;
	const Integrator& integrator;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_RENDERER_H_ */
