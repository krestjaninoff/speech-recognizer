/**
 * Common project's settings
 */
#ifndef COMMON_H_
#define COMMON_H_

namespace wtm {

	// Debugging mode
	#define DEBUG_ENABLED true
	#define DEBUG(...) if (DEBUG_ENABLED) {fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}

	#define toString( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

} // namespace wtm

#endif /* COMMON_H_ */
