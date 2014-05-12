#include <iostream>
#include <stdlib.h>
#include "common.h"
#include "command/CommandProcessor.h"

using namespace std;
using namespace wtm::command;

/**
 * Entry point
 */
int main(int argc, char** argv) {

	CommandProcessor processor(argc, argv);
	processor.process();

	return EXIT_SUCCESS;
}
