/*
 * SceneLoader.cpp
 *
 *  Created on: May 1, 2017
 *      Author: bogdan
 */

#include <SceneLoader.h>
#include "Logger.h"
#include "Vec.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

namespace traceraptor {

using namespace std;

SceneLoader::SceneLoader() { }

vector<shared_ptr<Primitive>> SceneLoader::Load(string const& inputFileName) {
	vector<shared_ptr<Primitive>> loadedShapes;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputFileName.c_str());

	if (!ret) {
		Logger::log_error("Could not load .obj file: " + inputFileName);
	}

	if (!err.empty()) {
		Logger::log_error(err);
	}

	printf("# of shapes    : %ld", shapes.size());
	printf("# of materials : %ld", materials.size());

	for (size_t i = 0; i < shapes.size(); i++) {
		tinyobj::shape_t currentShape = shapes[i];
		assert((currentShape.mesh.indices.size() % 3) == 0);

		int numTriangles = currentShape.mesh.num_face_vertices.size();
		int numVertices = currentShape.mesh.indices.size();
		std::shared_ptr<Material> redMaterial = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3f{0.65f, 0.05f, 0.05f}));

		int* vertexIndices = new int[numVertices];
		for(int i =0 ; i < numVertices; i++) {
			vertexIndices[i] = currentShape.mesh.indices[i].vertex_index;
		}

		Vec3f* vertices = new Vec3f[attrib.vertices.size()/3];
		for(size_t i=0 ; i < attrib.vertices.size(); i += 3) {
			Vec3f vertex = vertices[i];
			vertex[0] = (float)attrib.vertices[i];
			vertex[1] = (float)attrib.vertices[i+1];
			vertex[2] = (float)attrib.vertices[i+2];
		}

		Vec3f* normals = new Vec3f[attrib.normals.size()/3];
		for(size_t i=0 ; i < attrib.normals.size(); i += 3) {
			normals[i][0] = (float)attrib.normals[i];
			normals[i][1] = (float)attrib.normals[i+1];
			normals[i][2] = (float)attrib.normals[i+2];
		}

		Vec2f* uvs = new Vec2f[attrib.texcoords.size()/2];
		for(size_t i=0 ; i < attrib.texcoords.size(); i += 2) {
			uvs[i][0] = (float)attrib.texcoords[i];
			uvs[i][1] = (float)attrib.texcoords[i+1];
		}

		vector<shared_ptr<Primitive>> shape = CreateTriangleMesh(numTriangles, vertexIndices, numVertices, vertices, normals, uvs, redMaterial);

		for(size_t i=0; i < shape.size(); i++) {
			loadedShapes.push_back(shape[i]);
		}

		printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
//		printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
//		printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());

//		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
//			printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
//		}

		delete[] vertexIndices;
		delete[] vertices;
		delete[] normals;
		delete[] uvs;
	}

	return loadedShapes;
}

SceneLoader::~SceneLoader() {
	// TODO Auto-generated destructor stub
}

} /* namespace traceraptor */
