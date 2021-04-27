#ifndef VAEZ_PORTAUDIO
#define VAEZ_PORTAUDIO

#include "./vae_backend.hpp"
#include "../config.hpp"

#include "portaudio.h"

namespace VAE { namespace core {

	/**
	 * @brief Portaudio backend implementation.
	 */
	class BackendPortaudio final : public Backend {

		static int PAInitialized(int change = 0) {
			static int Count = 0;
			Count += change;
			return Count;
		}

		struct AudioThreadShared {
			Backend& backend;
			Backend::AudioBuffer bufferTo;
			Backend::AudioBuffer bufferFrom;
		};

		PaStream *mStream = nullptr;
		// Data shared with the audio thread
		AudioThreadShared mShared;
		bool mInitialized = false;

		void cleanUp() {
			if (!mInitialized || mStream == nullptr) {
				return;
			}

			PaError err = Pa_StopStream(mStream);
			TKLB_ASSERT(err == paNoError)

			err = Pa_CloseStream(mStream);
			TKLB_ASSERT(err == paNoError)
			if (PAInitialized() == 0) {
				TKLB_ASSERT(false)
				return;
			}

			if (PAInitialized(-1) == 0) {
				Pa_Terminate();
			}
			mInitialized = false;
		}

		void setUp() {
			if (mInitialized) { return; }
			mInitialized = true;
			if (PAInitialized() == 0) {
				PaError err = Pa_Initialize();
				if (err != paNoError) {
					TKLB_ASSERT(false)
					return;
				}
			}
			PAInitialized(+1);
		}

		/**
		 * @brief Function called from the PortAudio thread.
		 */
		static int AudioCallback(
			const void *in, void *out, unsigned long frames,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags, void *data
		) {
			// Prevent unused variable warnings.
			(void) timeInfo;
			(void) statusFlags;

			AudioThreadShared& shared = *reinterpret_cast<AudioThreadShared*>(data);

			float* outBuffer = reinterpret_cast<float*>(out);
			const float* inBuffer = reinterpret_cast<const float*>(in);

			shared.bufferFrom.setFromInterleaved(inBuffer, frames, shared.backend.getChannelsIn());
			TKLB_ASSERT(shared.bufferFrom.validSize() == frames)
			shared.backend.callback(shared.bufferFrom, shared.bufferTo);
			shared.bufferTo.putInterleaved(outBuffer, shared.bufferFrom.validSize());

			return paContinue;
		}

		static void StreamFinished(void* data) {
			AudioThreadShared& shared = *reinterpret_cast<AudioThreadShared*>(data);
		}

	public:

		BackendPortaudio() : mShared({ *this }) { }

		BackendPortaudio(SyncCallback& callback) :
			Backend(callback), mShared({ *this }) { }

		~BackendPortaudio() {
			cleanUp();
		}

		bool openDevice() override {
			setUp();
			DeviceInfo device;
			device.id = Pa_GetDefaultOutputDevice();
			return openDevice(device);
		}

		bool openDevice(const DeviceInfo& device) override {
			setUp();
			const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(device.id);
			PaStreamParameters outputParameters;
			outputParameters.channelCount = 2; // only stereo
			outputParameters.sampleFormat = paFloat32;
			outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
			outputParameters.hostApiSpecificStreamInfo = NULL;

			PaError err = Pa_OpenStream(
				&mStream,
				NULL, // no input
				&outputParameters,
				Config::SampleRate,
				device.bufferSize,
				paClipOff, // no clipping, device will do that
				AudioCallback,
				&mShared
			);

			if (err != paNoError) { cleanUp(); return false; }

			err = Pa_SetStreamFinishedCallback(
				mStream, StreamFinished
			);

			if (err != paNoError) { cleanUp(); return false; }

			err = Pa_StartStream(mStream);

			if (err != paNoError) { cleanUp(); return false; }

			return true;
		}

		unsigned int getDeviceCount() override {
			setUp();
			return Pa_GetDeviceCount();
		}

		DeviceInfo getDevice(unsigned int id) {
			// TODO tklb
			TKLB_ASSERT(false)
			return DeviceInfo();
		};

		bool closeDevice() override {
			cleanUp();
			return true;
		}
	};

} } // namespace VAE::core

#endif // VAEZ_PORTAUDIO
