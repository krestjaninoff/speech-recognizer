/**
 * Common project's settings
 */
#ifndef COMMON_H_
#define COMMON_H_

#define __GXX_EXPERIMENTAL_CXX0X__ 1

#include <stdio.h>
#include <iostream>
#include <sstream>

namespace wtm {

	#define DEBUG_ENABLED true
	#define DEBUG(...) if (DEBUG_ENABLED) {fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}

	#define UNUSED(expr) do { (void)(expr); } while (0)

	#define toString( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

} // namespace wtm

#endif /* COMMON_H_ */
