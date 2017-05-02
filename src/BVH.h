/*
 * BVH.h
 *
 *  Created on: Nov 30, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_BVH_H_
#define TRACERAPTOR_BVH_H_

#include <Primitive.h>
#include <vector>
#include "BBox.h"
#include "Ray.h"
#include "Logger.h"

namespace traceraptor {

struct BVHFlatNode {
  BBox bbox;
  unsigned int start, num_primitives, rightOffset;
};

class BVH {
	static const float default_tmin;
	static const float default_tmax;

	unsigned int nNodes, nLeafs, leafSize;
	std::vector<std::shared_ptr<Primitive>> build_prims;

	void build();

	std::vector<BVHFlatNode> flatTree;

public:
	BVH(std::vector<std::shared_ptr<Primitive>> &objects, unsigned int leafSize = 4);
	bool getIntersection(const Ray& ray, IntersectionInfo &intersection, bool occlusion) const;

	~BVH();
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_BVH_H_ */
