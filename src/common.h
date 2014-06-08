/**
 * Common project's settings
 */

#ifndef COMMON_H_
#define COMMON_H_

#define __GXX_EXPERIMENTAL_CXX0X__ 1

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <limits.h>

namespace yazz {

	// Build information

	#define PROJECT_NAME "YAZZ"
	#define PROJECT_VERSION "0.0.1"


	// Helpers

	#define DEBUG_ENABLED true
	#define DEBUG(...)  \
		do { if (DEBUG_ENABLED) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } } while (0)

	#define UNUSED(expr) do { (void)(expr); } while (0)

	#define EOL "\n"
	#define toString( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


	// Test settings

	const double EPS_TEST = 1e-4;

} // namespace yazz

#endif /* COMMON_H_ */
