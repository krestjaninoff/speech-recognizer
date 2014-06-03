#include <Painter.h>
#include <Processor.h>
#include <VisualizationCommand.h>
#include <WavData.h>
#include <iostream>
#include <string>

using namespace std;
using namespace yazz::visual;

namespace yazz {
namespace command {

	const char* VisualizationCommand::OUTPUT_FILE_DEFAULT = "diagram.png";

	bool VisualizationCommand::createDiagram(Context& context, const char* outputFile) {
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
	}

} /* namespace command */
} /* namespace yazz */
