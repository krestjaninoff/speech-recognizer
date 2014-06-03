#ifndef AUDIODATACOMMAND_H_
#define AUDIODATACOMMAND_H_

#include <Context.h>
#include <string>

#ifdef __MINGW32__
#include <io.h>
#endif

using namespace std;

namespace yazz {
namespace command {

/**
 * Split a sample into several words and store them into files
 */
class AudioDataCommand {
public:

	/**
	 * Read Wav data from the file
	 */
	static bool readData(Context& context, const char* inputFile);

	/**
	 * Split the data and store it into the specific folder
	 */
	static bool splitIntoFiles(Context& context, const char* outputFolder);

private:
	static const char* outputFolder;
	static const char* OUTPUT_FOLDER_DEFAULT;

	static bool initOutputDirectory(const string& outputFolder);
};

} /* namespace command */
} /* namespace yazz */

#endif /* AUDIODATACOMMAND_H_ */
