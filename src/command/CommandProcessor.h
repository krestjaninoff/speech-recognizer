#ifndef COMMANDPROCESSOR_H_
#define COMMANDPROCESSOR_H_

#include <getopt.h>
#include "ICommand.h"
#include "ReadWavDataCommand.h"
#include "DrawDiagramCommand.h"
#include "SplitWordsCommand.h"
#include "ModelCommand.h"

namespace wtm {
namespace command {

/**
 * This processor creates commands based on input parameters
 * <p>
 * Big commands (the ones which requires a lot of code) must be moved into its own class.
 * As for little (helper like) commands, I think we can store them as a private methods of this class.
 *
 * @see http://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example
 */
class CommandProcessor {
public:
	CommandProcessor(int argc, char** argv):
		argc(argc), argv(argv) {

		this->context = new Context();
	};

	~CommandProcessor() {
		delete this->context;
	}

	bool process();

private:

	// Input data
	int argc;
	char** argv;

	/**
	 * Current execution context
	 */
	Context* context;

	void printVersion();
	void printHelp();
};

} /* namespace command */
} /* namespace wtm */

#endif /* COMMANDPROCESSOR_H_ */
