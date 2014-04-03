/*
 * Common objects for work with audio
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <string>
#include <list>
#include <stdint.h>

namespace wtm {
namespace audio {

/**
 * WAV header
 */
typedef struct wav_hdr_t {
    char                RIFF[4];        // RIFF Header
    unsigned long       ChunkSize;      // RIFF Chunk Size
    char                WAVE[4];        // WAVE Header

    char                fmt[4];         // FMT header
    unsigned long       Subchunk1Size;  // Size of the fmt chunk
    unsigned short      AudioFormat;    // Audio format 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio
    unsigned long       SamplesPerSec;  // Sampling Frequency in Hz
    unsigned long       bytesPerSec;    // bytes per second
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short      bitsPerSample;  // Number of bits per sample

    char                Subchunk2ID[4]; // "data"  string
    unsigned long       Subchunk2Size;  // Sampled data length

} wav_hdr_t;

/**
 * WAV data
 */
typedef std::list<int16_t> wav_data_t;

} // namespace
} // namespace

#endif /* AUDIO_H_ */
