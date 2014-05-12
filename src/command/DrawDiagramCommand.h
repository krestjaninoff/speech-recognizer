#ifndef DRAWDIAGRAMCOMMAND_H_
#define DRAWDIAGRAMCOMMAND_H_

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
	DrawDiagramCommand(const string& outputFile) : outputFile(outputFile) {};

	void execute(Context& context) {

		string file = outputDefault;
		if (!outputFile.empty()) {
			file = outputFile;
		}

		Painter::drawFrames(context.splitter, file.c_str());
	};

private:
	const string& outputFile;
	const string outputDefault = "diagram.png";
};

} /* namespace command */
} /* namespace wtm */

#endif /* DRAWDIAGRAMCOMMAND_H_ */
