/*
 * Traceraptor.h
 *
 *  Created on: Jun 25, 2017
 *      Author: bogdan
 */

#ifndef SRC_TRACERAPTOR_H_
#define SRC_TRACERAPTOR_H_

//#define TRACERAPTOR_FLOAT_AS_DOUBLE
#ifdef TRACERAPTOR_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif  // TRACERAPTOR_FLOAT_AS_DOUBLE

#define UNUSED(expr) (void)(expr)

#endif /* SRC_TRACERAPTOR_H_ */
