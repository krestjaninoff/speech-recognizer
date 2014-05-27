#ifndef VIZUALIZATIONCOMMAND_H_
#define VIZUALIZATIONCOMMAND_H_

#include <stdio.h>
#include <string>
#include "Context.h"
#include "../visual/Painter.h"

using namespace std;
using namespace yazz::visual;

namespace yazz {
namespace command {

/**
 * Create a diagram and store it into a file
 */
class VisualizationCommand {
public:

	static bool createDiagram(Context& context, const char* outputFile) {
		cout << "Creating a diagram..." << endl;

		// Check pre-requirements
		if (NULL == context.wavData) {
			cout << "Input data is not specified :(" << endl;
			return false;
		}

		// Determine results file name
		string file = OUTPUT_FILE_DEFAULT;
		if (NULL != outputFile) {
			string outputFileStr(outputFile);
			file = outputFileStr;
		}
		cout << "Diagram file is: " << file << "..." << endl;

		// Create the Processor
		audio::Processor* processor = new Processor(context.wavData);
		context.processor = processor;

		// Split wav data into words
		processor->split();

		// Draw the diagram
		Painter::drawFrames(context.processor, file.c_str());

		cout << "Complete!" << endl;
		return true;
	};

private:
	static const char* outputFile;
	static const char* OUTPUT_FILE_DEFAULT;
};

const char* VisualizationCommand::OUTPUT_FILE_DEFAULT = "diagram.png";

} /* namespace command */
} /* namespace yazz */

#endif /* VIZUALIZATIONCOMMAND_H_ */
