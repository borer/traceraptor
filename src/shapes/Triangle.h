/*
 * Triangle.h
 *
 *  Created on: Feb 17, 2017
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_TRIANGLE_H_
#define TRACERAPTOR_TRIANGLE_H_

#include <Shape.h>
#include <Material.h>
#include <BBox.h>

namespace traceraptor {
    
struct TriangleIndex {
    size_t vertexIndices[3];
    size_t normalIndices[3];
};

std::unique_ptr<std::vector<std::shared_ptr<Shape>>> CreateTriangleMesh(std::shared_ptr<std::vector<TriangleIndex>> triangles,
                                                           std::shared_ptr<std::vector<Vec3f>> p,
                                                           std::shared_ptr<std::vector<Vec3f>> n,
                                                           std::shared_ptr<std::vector<Vec2f>> uv);

struct TriangleMesh {
	TriangleMesh(std::shared_ptr<std::vector<TriangleIndex>> triangles,
                 std::shared_ptr<std::vector<Vec3f>> V,
                 std::shared_ptr<std::vector<Vec3f>> N,
                 std::shared_ptr<std::vector<Vec2f>> uv);

	std::shared_ptr<std::vector<TriangleIndex>> triangles;
    std::shared_ptr<std::vector<Vec3f>> V;
    std::shared_ptr<std::vector<Vec3f>> N;
    std::shared_ptr<std::vector<Vec2f>> uv;
};

class Triangle: public Shape {
public:
	Triangle(std::shared_ptr<TriangleMesh> mesh, int triNumber);

	virtual bool Intersect(const Ray &r, IntersectionInfo &rec) const;
	virtual BBox get_bbox() const;
	virtual UV get_uv(const Vec3f& point) const;

private:
	std::shared_ptr<TriangleMesh> mesh;
	TriangleIndex triangleIndex;

	std::shared_ptr<BBox> bounds;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_TRIANGLE_H_ */
