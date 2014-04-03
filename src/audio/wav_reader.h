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
 * Read Wav data from a file
 */
wav_data_t readWav(std::string file);

} // namespace audio
} // namespace wtm

#endif /* WAV_READER_H_ */
