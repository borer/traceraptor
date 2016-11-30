/*
 * HitableList.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_HITABLELIST_H_
#define TRACERAPTOR_HITABLELIST_H_

#include <vector>
#include "Hitable.h"
#include "BBox.h"

namespace traceraptor {

class HitableList : public Hitable {
public:
	HitableList(Hitable **list, int n) {
		this->list = list;
		list_size = n;
		bb = new BBox();
		for(int i=0 ; i < list_size; ++i) {
			bb->expandToInclude(list[i]->get_bbox());
		}
	}
	virtual bool hit(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const;
	virtual BBox get_bbox() const;
	virtual Vec3 get_centroid() const;

	virtual ~HitableList() {
		delete bb;
	}

	Hitable	**list;
	int list_size;
	BBox *bb;
};

bool HitableList::hit(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const {
	IntersectionInfo temp_rec;
	bool hit_anything = false;
	double closest_so_far = tmax;
	for (int i = 0; i < list_size ; i++) {
		if (list[i]->hit(r, tmin, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

BBox HitableList::get_bbox() const {
	return *bb;
}

Vec3 HitableList::get_centroid() const {
    return Vec3(0,0,0);
}

} /* namespace traceraptor */

#endif /* TRACERAPTOR_HITABLELIST_H_ */
