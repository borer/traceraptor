#include "Triangle.h"

#include "RayTracingStatistics.h"
#include <vector>

namespace traceraptor {

TriangleMesh::TriangleMesh(std::shared_ptr<std::vector<TriangleIndex>> triangles,
                           std::shared_ptr<std::vector<Vec3f>> V,
                           std::shared_ptr<std::vector<Vec3f>> N,
                           std::shared_ptr<std::vector<Vec2f>> uv)
		:
        triangles(triangles),
        V(V),
        N(N),
        uv(uv) {
            //TODO: Transform mesh vertices to world space
        }

Triangle::Triangle(std::shared_ptr<TriangleMesh> mesh, int triangleNumber) {

    this->mesh = mesh;
	triangleIndex = mesh->triangles->at(triangleNumber); //this is wrong, we need to map triangle number to index

    printf("get triangle vertex index %zu , %zu, %zu \n", triangleIndex.vertexIndices[0],
                                                           triangleIndex.vertexIndices[1],
                                                           triangleIndex.vertexIndices[2]);
    
	const Vec3f& v0 = mesh->V->at(triangleIndex.vertexIndices[0]);
	const Vec3f& v1 = mesh->V->at(triangleIndex.vertexIndices[1]);
	const Vec3f& v2 = mesh->V->at(triangleIndex.vertexIndices[2]);

	Vec3f min, max;
	min = component_min(v0, v1);
	min = component_min(min, v2);
	max = component_max(v0, v1);
	max = component_max(max, v2);
	bounds = std::make_shared<BBox>(min, max);
};

bool Triangle::Intersect(const Ray &r, IntersectionInfo &rec) const {
	INCREMENT_RAY_PRIMITIVES_TEST_STATISTICS;

    const Vec3f& v0 = mesh->V->at(triangleIndex.vertexIndices[0]);
    const Vec3f& v1 = mesh->V->at(triangleIndex.vertexIndices[1]);
    const Vec3f& v2 = mesh->V->at(triangleIndex.vertexIndices[2]);
    
	Vec3f edge1 = v1 - v0;
	Vec3f edge2 = v2 - v0;

	Vec3f pvec = cross(r.dir, edge2);
	float det = dot(edge1, pvec);

	// check determinant and exit if triangle and ray are parallel
	// TODO: add EPSILONS
	if (det == 0) return false;
	float inv_det = 1.0f / det;

	// compute and check first bricentric coordinated
	auto tvec = r.o - v0;
//	auto u = dot(tvec, pvec) * inv_det;
//	if (u < 0 || u > 1) return false;

	// compute and check second bricentric coordinated
	auto qvec = cross(tvec, edge1);
//	auto v = dot(r.dir, qvec) * inv_det;
//	if (v < 0 || u + v > 1) return false;

	// compute and check ray parameter
	auto t = dot(edge2, qvec) * inv_det;
	if (t < Ray::default_tmin || t > r.tMax) return false;

	rec.t = t;
	rec.hit_point = r.point_at_parameter(rec.t);
	rec.normal = normalize(cross(edge1, edge2));
    rec.uv = get_uv(rec.normal);
	rec.hit_something = true;
	INCREMENT_RAY_PRIMITIVES_INTERSECTIONS_STATISTICS

	return true;
}

BBox Triangle::get_bbox() const {
	return BBox(bounds->pMin, bounds->pMax);
}

UV Triangle::get_uv(const Vec3f& point) const {
	UNUSED(point);
	return UV(0,0);
}

std::unique_ptr<std::vector<std::shared_ptr<Shape>>> CreateTriangleMesh(std::shared_ptr<std::vector<TriangleIndex>> triangles,
                                                       std::shared_ptr<std::vector<Vec3f>> V,
                                                       std::shared_ptr<std::vector<Vec3f>> N,
                                                       std::shared_ptr<std::vector<Vec2f>> uv) {

    std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(triangles,V,N,uv);
    size_t totalTriangles = triangles->size();
    std::unique_ptr<std::vector<std::shared_ptr<Shape>>> tris = std::make_unique<std::vector<std::shared_ptr<Shape>>>();
    for (size_t i = 0; i < totalTriangles; ++i){
        std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(mesh, i);
        tris->push_back(triangle);
    }

    return tris;
}

} /* namespace traceraptor */
