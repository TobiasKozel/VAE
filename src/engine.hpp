#include "../external/tklb/types/THeapBuffer.h"
#include "../external/tklb/util/TAssert.h"

#include "./config.hpp"
#include "./world.hpp"
#include "./voice.hpp"
#include "./listener.hpp"
#include "./soundbank.hpp"

namespace VAE {
	class Core {
		unsigned int mSampleRate = 0;
		size_t mVoiceLimit = 0;
		tklb::HeapBuffer<Voice> mVoices;
		tklb::HeapBuffer<Listener> mListener;

		public:
			void init(int sampleRate = Config::SampleRate) {
				mSampleRate = sampleRate;
			}

			Voice* play(const AudioBuffer& sound) {
				mVoices.push(Voice(&sound));
				return voice;
			}

			/**
			 * \brief Audio callback needed if the audio engine is not handling the audio device.
			 * \param frameCount The number of frames to process
			 * \param output The resulting samples will be stored here.
			 * In case this is a nullptr nothing will be written into it and
			 * The result can be retrieved from the corresponding Listener object instead.
			 * \param input Input buffer. nullptr if no input channels are needed.
			 */
			void process(size_t frameCount, Config::Sample** output, Config::Sample** input) {
				TKLB_ASSERT(mSampleRate != Config::SampleRate); // TODO: resample the output

				if (mVoiceLimit) {
					if ()
				}
				for (size_t l = 0; l < mListener.size(); l++) {
					for (size_t i = 0; i < mVoices.size(); i++) {

					}
				}

				for (size_t i = 0; i < mVoices.size();) {
					Voice* v = mVoices[i];
					if (!v->tick(frameCount)) {
						if (mVoices.remove(i)) {
							delete v;
						}; // will put the last voice at the current index
					} else {
						i++; // only iterate if no voice was removed
					}
				}
			}
	};
}
