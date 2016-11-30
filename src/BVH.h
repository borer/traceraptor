/*
 * BVH.h
 *
 *  Created on: Nov 30, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_BVH_H_
#define TRACERAPTOR_BVH_H_

#include "BBox.h"
#include <vector>
#include "Ray.h"
#include "Hitable.h"
#include "Logger.h"

namespace traceraptor {

struct BVHFlatNode {
  BBox bbox;
  unsigned int start, num_primitives, rightOffset;
};

class BVH {
	unsigned int nNodes, nLeafs, leafSize;
	std::vector<std::shared_ptr<Hitable>> build_prims;

	void build();

	std::vector<BVHFlatNode> flatTree;

public:
	BVH(std::vector<std::shared_ptr<Hitable>> &objects, unsigned int leafSize = 4);
	bool getIntersection(const Ray& ray, IntersectionInfo &intersection, bool occlusion) const;

	~BVH();
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_BVH_H_ */
