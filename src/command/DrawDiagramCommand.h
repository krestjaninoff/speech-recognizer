#ifndef DRAWDIAGRAMCOMMAND_H_
#define DRAWDIAGRAMCOMMAND_H_

#include <stdio.h>
#include <string>
#include "ICommand.h"
#include "../visual/Painter.h"

using namespace std;
using namespace wtm::visual;

namespace wtm {
namespace command {

/**
 * Create a diagram and store it into a file
 */
class DrawDiagramCommand : ICommand {
public:
	DrawDiagramCommand(const char* outputFile) : outputFile(outputFile) {};

	bool execute(Context& context) {
		cout << "Creating a diagram..." << endl;

		// Check pre-requirements
		if (NULL == context.wavData) {
			cout << "Input data is not specified :(" << endl;
			return false;
		}

		// Determine results file name
		string file = outputDefault;
		if (NULL != outputFile) {
			string outputFileStr(outputFile);
			file = outputFileStr;
		}
		cout << "Diagram file is: " << file << "..." << endl;

		// Create the splitter
		audio::Splitter* splitter = new Splitter(context.wavData);
		context.splitter = splitter;

		// Split wav data into words
		splitter->split();

		// Draw the diagram
		Painter::drawFrames(context.splitter, file.c_str());

		cout << "Complete!" << endl;
		return true;
	};

private:
	const char* outputFile;
	const string outputDefault = "diagram.png";
};

} /* namespace command */
} /* namespace wtm */

#endif /* DRAWDIAGRAMCOMMAND_H_ */
