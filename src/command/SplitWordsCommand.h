#ifndef SPLITWORDSCOMMAND_H_
#define SPLITWORDSCOMMAND_H_

#include <iostream>
#include "ICommand.h"
#include "../audio/Word.h"
#include "../audio/Splitter.h"

using namespace std;
using namespace wtm::audio;

namespace wtm {
namespace command {

/**
 * Split a sample into several words and store them into files
 */
class SplitWordsCommand : public ICommand {
public:
	SplitWordsCommand(const string& outputFolder) : outputFolder(outputFolder) {};

	void execute(Context& context) {
		int counter = 1;

		// Create the splitter
		audio::SplitterPtr splitter = audio::SplitterPtr(new Splitter(context.wavData));
		context.splitter = splitter;

		// Split wav data into words
		splitter->split();

		// Save results
		DEBUG("Words: %d", splitter->getWords()->size());
		for (vector<Word*>::const_iterator word = splitter->getWords()->begin();
				word != splitter->getWords()->end(); ++word) {

			string fileName = "sampleFile." + toString(counter) + ".wav";
			cout << fileName << endl;

			splitter->saveWordAsAudio(fileName, *(*word));
			counter++;
		}
	};

private:
	const string& outputFolder;
	static const string& outputDefault = "out";
};

} /* namespace command */
} /* namespace wtm */

#endif /* SPLITWORDSCOMMAND_H_ */
