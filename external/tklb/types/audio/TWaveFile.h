#ifndef TKLB_AUDIOFILE
#define TKLB_AUDIOFILE

#include "./TAudioBuffer.h"

#define DR_WAV_IMPLEMENTATION
#include "../../external/dr_wav.h"

namespace tklb {

namespace wave {

	/**
	 * @brief Decode wav from memory or file path
	 * @param path The path or the wav file buffer if length is 0
	 * @param out The buffer to store the result in
	 * @param length The length of the wav file buffer if not reading from file
	 */
	template <typename T>
	bool load(const char* path, AudioBufferTpl<T>& out, size_t length = 0) {
		drwav wav;
		if (length == 0) {
			if (!drwav_init_file(&wav, path, nullptr)) {
				return false;
			}
		} else {
			if (!drwav_init_memory(&wav, path, length, nullptr)) {
				return false;
			}
		}
		float* sampleData = new float[size_t(wav.totalPCMFrameCount) * size_t(wav.channels)];

		if (sampleData == nullptr) { return false; }

		length = drwav_read_pcm_frames_f32(
			&wav, wav.totalPCMFrameCount, sampleData
		);

		if (length == 0) {
			free(sampleData);
			return false;
		}
		out.sampleRate = wav.sampleRate;
		out.resize(length, wav.channels);
		out.setFromInterleaved(sampleData, length, wav.channels);
		out.setValidSize(length);
		free(sampleData);
		drwav_uninit(&wav);
	}

	template <typename T>
	bool write(const AudioBufferTpl<T>& in, char* path, size_t length = 0) {
		// TODO
		TKLB_ASSERT(false);
	}

}

} // namespace

#endif // TKLB_AUDIOFILE
