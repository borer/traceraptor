/*
 * Triangle.h
 *
 *  Created on: Feb 17, 2017
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_TRIANGLE_H_
#define TRACERAPTOR_TRIANGLE_H_

#include <Primitive.h>
#include "Material.h"
#include "BBox.h"

namespace traceraptor {

std::vector<std::shared_ptr<Primitive>> CreateTriangleMesh(int nTriangles,
                                                           const std::vector<int> vertexIndices,
                                                           int nVertices,
                                                           const std::vector<Vec3f> p,
                                                           const std::vector<Vec3f> n,
                                                           const std::vector<Vec2f> uv,
                                                           std::shared_ptr<Material> obj_material);

struct TriangleMesh {
	TriangleMesh(int nTriangles,
                 const std::vector<int> vertexIndices,
                 int nVertices,
                 const std::vector<Vec3f> V,
                 const std::vector<Vec3f> N,
                 const std::vector<Vec2f> uv,
                 std::shared_ptr<Material> obj_material);

	const int nTriangles, nVertices;
	std::vector<int> vertexIndices;
	std::unique_ptr<Vec3f[]> v; //vertices
	std::unique_ptr<Vec3f[]> n; //normals
	std::unique_ptr<Vec2f[]> uv;

	std::shared_ptr<Material> material;
};

class Triangle: public Primitive {
public:
	Triangle(std::shared_ptr<TriangleMesh> mesh, int triNumber);

	virtual bool Intersect(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const;
	virtual BBox get_bbox() const;
	virtual Vec3f get_centroid() const;
	virtual UV get_uv(const Vec3f& point) const;

private:
	std::shared_ptr<TriangleMesh> mesh;
	const int *vi;

	std::shared_ptr<BBox> bounds;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_TRIANGLE_H_ */
