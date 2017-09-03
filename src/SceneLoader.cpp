/*
 * SceneLoader.cpp
 *
 *  Created on: May 1, 2017
 *      Author: bogdan
 */

#include <SceneLoader.h>
#include <Logger.h>
#include <Vec.h>
#include <BVH.h>
#include <shapes/Triangle.h>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tiny_obj_loader.h>

namespace traceraptor {

using namespace std;

SceneLoader::SceneLoader() { }

std::shared_ptr<Material> ConstructMaterial(std::vector<tinyobj::material_t> materials, size_t index) {
	if (index >= materials.size()){
		return nullptr;
	}

	std::shared_ptr<Material> resultingMaterial;
	tinyobj::material_t material = materials[index];
//	TODO use for caching : material.name
	if (material.emission[0] > 0.0f || material.emission[1] > 0.0f || material.emission[2] > 0.0f)
	{
		resultingMaterial = std::make_shared<Emissive>(
				std::make_shared<ConstantTexture>(Vec3f{
			material.emission[0],
			material.emission[1],
			material.emission[2]
		}));
	} else if (material.diffuse[0] > 0.0f || material.diffuse[1] > 0.0f || material.diffuse[2] > 0.0f) {
		resultingMaterial = std::make_shared<Lambertian>(
				std::make_shared<ConstantTexture>(Vec3f{
			material.diffuse[0],
			material.diffuse[1],
			material.diffuse[2]
		}));
	} else if (material.specular[0] > 0.0f || material.specular[1] > 0.0f || material.specular[2] > 0.0f) {
		resultingMaterial = std::make_shared<Metal>(
				Vec3f{
			material.specular[0],
			material.specular[1],
			material.specular[2]
		}, 1.0f);
	}

	return resultingMaterial;
}

std::unique_ptr<vector<shared_ptr<Primitive>>> SceneLoader::Load(string const& inputFileName) {
	std::unique_ptr<std::vector<shared_ptr<Primitive>>> loadedShapes = std::make_unique<std::vector<std::shared_ptr<Primitive>>>();

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

	printf("# of shapes    : %ld\n", shapes.size());
	printf("# of materials : %ld\n", materials.size());
    
    std::shared_ptr<std::vector<Vec3f>> vertices = std::make_shared<std::vector<Vec3f>>(attrib.vertices.size()/3);
    for(size_t i=0, index_vert=0; i < attrib.vertices.size(); i += 3, index_vert++) {
        Vec3f& vertex = vertices->at(index_vert);
        vertex[0] = (float)attrib.vertices[i];
        vertex[1] = (float)attrib.vertices[i+1];
        vertex[2] = (float)attrib.vertices[i+2];
        
        printf("added vertex %f, %f, %f at index %zu\n", vertex[0], vertex[1], vertex[2], index_vert);
    }
    
    std::shared_ptr<std::vector<Vec3f>> normals = std::make_shared<std::vector<Vec3f>>(attrib.normals.size()/3);
    for(size_t i=0, index_normal= 0 ; i < attrib.normals.size(); i += 3, index_normal++) {
        Vec3f& normal = normals->at(index_normal);
        normal[0] = (float)attrib.normals[i];
        normal[1] = (float)attrib.normals[i+1];
        normal[2] = (float)attrib.normals[i+2];
    }
    
    std::shared_ptr<std::vector<Vec2f>> uvs = std::make_shared<std::vector<Vec2f>>(attrib.texcoords.size()/2);
    for(size_t i=0, index_uv = 0; i < attrib.texcoords.size(); i += 2, index_uv++) {
        Vec2f& uv = uvs->at(index_uv);
        uv[0] = (float)attrib.texcoords[i];
        uv[1] = (float)attrib.texcoords[i+1];
    }
    
    int totalTriangles = 0;
    for (size_t shape_index = 0; shape_index < shapes.size(); shape_index++) {
        totalTriangles += shapes[shape_index].mesh.num_face_vertices.size();
    }
    std::shared_ptr<std::vector<TriangleIndex>> triangles =  std::make_shared<std::vector<TriangleIndex>>(totalTriangles);
    
    int triangleIndex = 0;
    
	for (size_t shape_index = 0; shape_index < shapes.size(); shape_index++) {
		tinyobj::shape_t currentShape = shapes[shape_index];
		assert((currentShape.mesh.indices.size() % 3) == 0);
        printf("shape[%ld].name = %s\n", shape_index, shapes[shape_index].name.c_str());
        printf("Size of shape[%ld].indices: %ld\n", shape_index, shapes[shape_index].mesh.indices.size());
        printf("Size of shape[%ld].material_ids: %ld\n", shape_index, shapes[shape_index].mesh.material_ids.size());
        
        size_t index_offset = 0;
        for (size_t f = 0; f < currentShape.mesh.num_face_vertices.size(); f++) {
            int fv = currentShape.mesh.num_face_vertices[f];
            
            TriangleIndex& triangleIndices = triangles->at(triangleIndex);
            // Loop over vertices in the face.
            for (int v = 0; v < fv; v++) {
                int vertex_index = index_offset + v;
                tinyobj::index_t idx = currentShape.mesh.indices[vertex_index];
                triangleIndices.vertexIndices[v] = idx.vertex_index;
                triangleIndices.normalIndices[v] = idx.normal_index;
                
                printf("add vi %d, ", idx.vertex_index);
            }
            
            printf("add triangle %d \n", triangleIndex);
            triangleIndex++;
            index_offset += fv;
        }
	}
    
    std::unique_ptr<std::vector<shared_ptr<Shape>>> triangleMesh = CreateTriangleMesh(triangles, vertices, normals, uvs);
    
    std::shared_ptr<Material> material = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(Vec3f{0.65f, 0.05f, 0.05f}));
    for(shared_ptr<Shape> shape : *triangleMesh) {
        std::shared_ptr<GeometricPrimitive> trianglePrimitive = std::make_shared<GeometricPrimitive>(shape, material);
        loadedShapes->push_back(trianglePrimitive);
    }

	return loadedShapes;
}

SceneLoader::~SceneLoader() {

}

} /* namespace traceraptor */
