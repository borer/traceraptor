/*
 * BVH2.h
 *
 *  Created on: Jun 11, 2017
 *      Author: bogdan
 */

#ifndef SRC_ACCELERATORS_BVHACCEL_H_
#define SRC_ACCELERATORS_BVHACCEL_H_

#include <Primitive.h>
#include <BBox.h>

namespace traceraptor {

struct BVHBuildNode;
struct BVHPrimitiveInfo;
struct LinearBVHNode;

class BVHAccel : public Aggregate {
public:
	enum class SplitMethod { SAH, Middle, EqualCounts };

	BVHAccel(const std::vector<std::shared_ptr<Primitive>> &p,
			int maxPrimsInNode = 1,
			SplitMethod splitMethod = SplitMethod::SAH);
	BBox GetBounds() const;
	Vec3f GetCentroid() const;
	bool Intersect(const Ray &ray, IntersectionInfo &rec) const;
	bool IntersectP(const Ray &ray) const;
	~BVHAccel();

private:
	std::shared_ptr<BVHBuildNode> recursiveBuild(
        std::vector<BVHPrimitiveInfo> &primitiveInfo,
        int start, int end, int *totalNodes,
        std::vector<std::shared_ptr<Primitive>> &orderedPrims);

    int flattenBVHTree(std::shared_ptr<BVHBuildNode> node, int *offset);

    const int maxPrimsInNode;
    const SplitMethod splitMethod;
    std::vector<std::shared_ptr<Primitive>> primitives;
    LinearBVHNode *nodes = nullptr;
};

} /* namespace traceraptor */

#endif /* SRC_ACCELERATORS_BVHACCEL_H_ */
