/*
 * Primitive.h
 *
 *  Created on: May 28, 2017
 *      Author: bogdan
 */

#ifndef SRC_PRIMITIVE_H_
#define SRC_PRIMITIVE_H_

#include <Shape.h>
#include <Material.h>
#include <BBox.h>
#include <Intersection.h>
#include <Transform.h>

namespace traceraptor {

class Primitive {
public:
	virtual const std::shared_ptr<Material> GetMaterial() const = 0;
	virtual bool Intersect(const Ray &ray, IntersectionInfo &rec) const = 0;
	virtual bool IntersectP(const Ray &ray) const = 0;
	virtual BBox GetBounds() const = 0;
	virtual Vec3f GetCentroid() const = 0;
	virtual ~Primitive() {}
};

class GeometricPrimitive : public Primitive {
  public:
	GeometricPrimitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material) :
		shape(shape),
		material(material) { }

	virtual bool IntersectP(const Ray &ray) const {
		return shape->IntersectP(ray);
	}

	virtual bool Intersect(const Ray &ray, IntersectionInfo &intersectionInfo) const {
		bool isIntersected = shape->Intersect(ray, intersectionInfo);
		if (isIntersected){
			ray.tMax = intersectionInfo.t;
            intersectionInfo.shape = shape;
            intersectionInfo.material = material;
		} else {
			intersectionInfo.shape = nullptr;
			intersectionInfo.material = nullptr;
		}

		return isIntersected;
	}

	virtual const std::shared_ptr<Material> GetMaterial() const {
		return material;
	}

	virtual BBox GetBounds() const {
		return shape->get_bbox();
	}

	virtual Vec3f GetCentroid() const {
		BBox bbox = shape->get_bbox();
		return bbox.GetCentroid();
	}

	virtual ~GeometricPrimitive() { }

private:
	std::shared_ptr<Shape> shape;
	std::shared_ptr<Material> material;
};

class TransformedPrimitive : public Primitive {
  public:
    TransformedPrimitive(std::shared_ptr<Primitive> primitive,
                         const Transform &PrimitiveToWorld)
        : primitive(primitive), PrimitiveToWorld(PrimitiveToWorld) {}

    bool IntersectP(const Ray &ray) const {
        Ray transformedRay = Inverse(PrimitiveToWorld)(ray);
        return primitive->IntersectP(transformedRay);
    }

    virtual bool Intersect(const Ray &ray, IntersectionInfo &intersectionInfo) const{
    	if (PrimitiveToWorld.IsIdentity()) {
    		return primitive->Intersect(ray, intersectionInfo);
    	} else {
    		Ray transformedRay = Inverse(PrimitiveToWorld)(ray);
    		if (!primitive->Intersect(transformedRay, intersectionInfo)) return false;
    		ray.tMax = transformedRay.tMax;
    		PrimitiveToWorld(intersectionInfo);
    		return true;
    	}
    }

    virtual const std::shared_ptr<Material> GetMaterial() const { return primitive->GetMaterial(); }

    virtual BBox GetBounds() const {
    	return PrimitiveToWorld(primitive->GetBounds());
    }

    virtual Vec3f GetCentroid() const {
    	return PrimitiveToWorld.TransformPoint(primitive->GetCentroid());
    }

  private:
    std::shared_ptr<Primitive> primitive;
    const Transform PrimitiveToWorld;
};

class Aggregate : public Primitive {
  public:
	virtual BBox GetBounds() const {
		return BBox(Vec3f({0.0f, 0.0f, 0.0f}));
	}

	virtual const std::shared_ptr<Material> GetMaterial() const {
		return nullptr;
	}

	virtual Vec3f GetCentroid() const {
		return Vec3f({1.0f, 1.0f, 1.0f});
	}

	virtual ~Aggregate() { }
};

} /* namespace traceraptor */

#endif /* SRC_PRIMITIVE_H_ */
