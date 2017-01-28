/*
 * Renderer.h
 *
 *  Created on: Jan 28, 2017
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_RAY_TRACING_STATISTICS_H_
#define TRACERAPTOR_RAY_TRACING_STATISTICS_H_

#ifdef TRACERAPTOR_STATISTICS
#define INCREMENT_PRIMARY_RAY_STATISTICS RayTracingStatistics::getInstance().numberPrimaryRays++;
#define INCREMENT_NUMBER_PRIMITIVES_STATISTICS RayTracingStatistics::getInstance().totalNumberPrimitives++;
#define INCREMENT_RAY_PRIMITIVES_TEST_STATISTICS RayTracingStatistics::getInstance().numberRayPrimitiveTests++;
#define INCREMENT_RAY_PRIMITIVES_INTERSECTIONS_STATISTICS RayTracingStatistics::getInstance().numberRayPrimitiveIntersections++;
#else
#define INCREMENT_PRIMARY_RAY_STATISTICS
#define INCREMENT_NUMBER_PRIMITIVES_STATISTICS
#define INCREMENT_RAY_PRIMITIVES_TEST_STATISTICS
#define INCREMENT_RAY_PRIMITIVES_INTERSECTIONS_STATISTICS
#endif

#include <atomic>
#include <string>

namespace traceraptor {

class RayTracingStatistics {

private:
	RayTracingStatistics() {}

	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::system_clock::duration elapsed_time;
public:
	static RayTracingStatistics& getInstance()
	{
		static RayTracingStatistics instance;
		return instance;
	}
	RayTracingStatistics(RayTracingStatistics const&) = delete;
	void operator=(RayTracingStatistics const&) = delete;

	void start_time_tracking() {
		start = std::chrono::system_clock::now();
	}

	void end_time_tracking() {
		end = std::chrono::system_clock::now();
		elapsed_time = end-start;
	}

	std::string generate_report() {
		std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time);
		long long elapsed_minutes = seconds.count() / 60;
		long long elapsed_seconds = seconds.count() % 60;
		float successRate = (float(RayTracingStatistics::getInstance().numberRayPrimitiveIntersections)/float(RayTracingStatistics::getInstance().numberRayPrimitiveTests))*100;

		std::string report = "=====Report=====\n";
		report += "Rendering duration : " + std::to_string(elapsed_minutes) + "mins, " + std::to_string(elapsed_seconds) + "secs\n";
		report += "Total primitives : " + std::to_string(RayTracingStatistics::getInstance().totalNumberPrimitives) + "\n";
		report += "Total primary rays : " + std::to_string(RayTracingStatistics::getInstance().numberPrimaryRays) + "\n";
		report += "Total ray-primitive tests : " + std::to_string(RayTracingStatistics::getInstance().numberRayPrimitiveTests) +"\n";
		report += "Total ray-primitive intersections : " + std::to_string(RayTracingStatistics::getInstance().numberRayPrimitiveIntersections) + "\n";
		report += "ray-primitive intersection success rate : " + std::to_string(successRate) + " %\n";

		return report;
	}

	std::atomic_int totalNumberPrimitives;
	std::atomic_int numberPrimaryRays;
	std::atomic_int numberRayPrimitiveTests;
	std::atomic_int numberRayPrimitiveIntersections;
};

}

#endif /* TRACERAPTOR_RAY_TRACING_STATISTICS_H_ */
