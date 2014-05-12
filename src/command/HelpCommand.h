#ifndef HELPCOMMAND_H_
#define HELPCOMMAND_H_

#include <iostream>
#include "ICommand.h"

using namespace std;

namespace wtm {
namespace command {

/**
 * Show a manual
 */
class HelpCommand : ICommand {
public:
	void execute(Context& context) {
		UNUSED(context);

		cout << "Help info" << endl;
	};
};

} /* namespace command */
} /* namespace wtm */

#endif /* HELPCOMMAND_H_ */
