#ifndef VAE_IMPL_LISTENER
#define VAE_IMPL_LISTENER

#include "./config.hpp"
#include "../../external/tklb/src/util/TMemory.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/audio/resampler/TResampler.hpp"
#include "../../include/VAE/vae_device_info.hpp"
#include "portaudio.h"

namespace VAE::Impl {

	struct ListnerShared {
		tklb::ResamplerTpl<Config::Sample> resampler;
		tklb::AudioBufferTpl<Config::Sample> buffer;
	};

	static int AudioCallback(
		const void *in, void *out,
		unsigned long frames,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *data
	) {
		// Prevent unused variable warnings.
		(void) timeInfo;
		(void) statusFlags;
		(void) in;
		ListnerShared& shared = *reinterpret_cast<ListnerShared*>(data);
		float* outBuffer = reinterpret_cast<float*>(out);
		return paContinue;
	}

	static void StreamFinished(void* data) {
		ListnerShared& shared = *reinterpret_cast<ListnerShared*>(data);
	}

	inline int PAInitialized = 0;

	class ListenerImpl {
		PaStream *mStream = nullptr;
		ListnerShared mShared; // Data shared with the audio thread
		bool mInitialized = false;

		void cleanUp() {
			if (!mInitialized || mStream == nullptr) {
				return;
			}

			PaError err = Pa_StopStream(mStream);
			TKLB_ASSERT(err == paNoError)

			err = Pa_CloseStream(mStream);
			TKLB_ASSERT(err == paNoError)
			if (PAInitialized == 0) {
				TKLB_ASSERT(false)
				return;
			}

			PAInitialized--;
			if (PAInitialized == 0) {
				Pa_Terminate();
			}
			mInitialized = false;
		}

		void setUp() {
			if (mInitialized) { return; }
			mInitialized = true;
			if (PAInitialized == 0) {
				PaError err = Pa_Initialize();
				if (err != paNoError) {
					TKLB_ASSERT(false)
					return;
				}
			}
			PAInitialized++;
		}
	public:
		~ListenerImpl() {
			cleanUp();
		}

		bool openDevice() {
			setUp();
			DeviceInfo device;
			device.id = Pa_GetDefaultOutputDevice();
			return openDevice(device);
		}

		bool openDevice(const DeviceInfo& device) {
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
				mStream, &StreamFinished
			);

			if (err != paNoError) { cleanUp(); return false; }

			err = Pa_StartStream(mStream);

			if (err != paNoError) { cleanUp(); return false; }

			return true;
		}

		unsigned int getDeviceCount() {
			setUp();
			return Pa_GetDeviceCount();
		}



	};
}

#endif // VAE_IMPL_LISTENER
