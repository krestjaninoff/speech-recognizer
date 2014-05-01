#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include "Context.h"

namespace wtm {
namespace command {

/**
 * Template for commands
 * <p>
 * For small (helper like) commands you can use private methods of CommandProcessor class.
 */
class ICommand {
public:
	virtual ~ICommand() {}

	/**
	 * The place for command's logic
	 */
	virtual void execute(Context& context) = 0;
};

} /* namespace command */
} /* namespace wtm */

#endif /* ICOMMAND_H_ */
