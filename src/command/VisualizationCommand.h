#ifndef VIZUALIZATIONCOMMAND_H_
#define VIZUALIZATIONCOMMAND_H_

#include "Context.h"

namespace yazz {
namespace command {

/**
 * Create a diagram and store it into a file
 */
class VisualizationCommand {
public:

	static bool createDiagram(Context& context, const char* outputFile);

private:
	static const char* outputFile;
	static const char* OUTPUT_FILE_DEFAULT;
};

} /* namespace command */
} /* namespace yazz */

#endif /* VIZUALIZATIONCOMMAND_H_ */
