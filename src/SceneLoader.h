/*
 * SceneLoader.h
 *
 *  Created on: May 1, 2017
 *      Author: bogdan
 */

#ifndef SRC_SCENELOADER_H_
#define SRC_SCENELOADER_H_

#include <Primitive.h>
#include <string>
#include <vector>

namespace traceraptor {

using namespace std;

class SceneLoader {
public:
	SceneLoader();
	vector<shared_ptr<Primitive>> Load(string const& inputFileName);
	virtual ~SceneLoader();
};

} /* namespace traceraptor */

#endif /* SRC_SCENELOADER_H_ */
