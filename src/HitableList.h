/*
 * HitableList.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_HITABLELIST_H_
#define TRACERAPTOR_HITABLELIST_H_

#include "Hitable.h"

namespace traceraptor {

class HitableList : public Hitable {
public:
	HitableList(Hitable **list, int n) {this->list = list; list_size = n;}
	virtual bool hit(const Ray &r, float tmin, float tmax, hit_record &rec) const;
	virtual ~HitableList() {}

	Hitable **list;
	int list_size;
};

bool HitableList::hit(const Ray &r, float tmin, float tmax, hit_record &rec) const {
	hit_record temp_rec;
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

} /* namespace traceraptor */

#endif /* TRACERAPTOR_HITABLELIST_H_ */
