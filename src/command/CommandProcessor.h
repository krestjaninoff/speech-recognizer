#ifndef COMMANDPROCESSOR_H_
#define COMMANDPROCESSOR_H_

#include <getopt.h>
#include "ICommand.h"
#include "DrawDiagramCommand.h"
#include "SplitWordsCommand.h"
#include "HelpCommand.h"

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
	 * Commands map
	 */
	static struct option longOptions[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "input", required_argument, NULL, 'i' },
		{ "draw", optional_argument, NULL, 'd' },
		{ "split", optional_argument, NULL, 's' },
		{0, 0, 0, 0}};

	static char* optionsStop = "h:id:is:";

	/**
	 * Current execution context
	 */
	Context* context;

	/**
	 * Load an audio file
	 */
	void readWavData(const string& source);
};

} /* namespace command */
} /* namespace wtm */

#endif /* COMMANDPROCESSOR_H_ */
