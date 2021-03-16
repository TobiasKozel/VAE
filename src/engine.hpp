#include "../external/tklb/types/THeapBuffer.h"

#include "./types.hpp"
#include "./world.hpp"
#include "./voice.hpp"
#include "./listener.hpp"
#include "./soundbank.hpp"

namespace VAE {
	class Core {
		tklb::HeapBuffer<Voice*> mVoices;
		tklb::HeapBuffer<Listener*> mListener;

		public:
			void init() {

			}

			void play(const AudioBuffer& sound) {
				auto voice = new Voice(&sound);
				mVoices.push(voice);
			}

			/**
			 * \brief Audio callback needed if the audio engine is not handling the audio device.
			 * \param frameCount The number of frames to process
			 * \param result The resulting samples will be stored here.
			 * In case this is a nullptr nothing will be written into it and
			 * The result can be retrieved from the corresponding Listener object instead.
			 */
			void process(size_t frameCount, Sample** result) {
				for (size_t l = 0; l < mListener.size(); l++) {
					for (size_t i = 0; i < mVoices.size(); i++) {

					}
				}

				for (size_t i = 0; i < mVoices.size();) {
					if (!mVoices[i]->tick(frameCount)) {
						mVoices.remove(i);
					} else {
						i++;
					}
				}
			}
	};
}
