/*
 * Util.h
 *
 *  Created on: Nov 26, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_UTIL_H_
#define TRACERAPTOR_UTIL_H_

#include <iostream>
#include <string>

namespace traceraptor {

class Logger {
public:
	static void log_debug(const char* message) {
		std::cout << message << std::endl;
	}

	static void log_debug(std::string message) {
		std::cout << message << std::endl;
	}
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_UTIL_H_ */
