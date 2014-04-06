/**
 * A module for reading a WAV file into a PCM-data structure
 *
 * @see http://en.wikipedia.org/wiki/WAV
 * @see http://en.wikipedia.org/wiki/Linear_pulse-code_modulation
 * @see https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 */
#ifndef WAV_READER_H_
#define WAV_READER_H_

#include <string>
#include "audio.h"

namespace wtm {
namespace audio {

/**
 * Reads Wav data from a file
 */
class WavReader {

public:
	static TWavDataPtr readFromFile(const std::string& file);

private:
	static void checkData(const TWavHeader& wav_header);
};


} // namespace audio
} // namespace wtm

#endif /* WAV_READER_H_ */
