#ifndef VAEZ_PORTAUDIO
#define VAEZ_PORTAUDIO

#include "./vae_device.hpp"
#include "../vae_config.hpp"

// #include "portaudio.h"
#include "../../../external/portaudio/include/portaudio.h"

namespace vae { namespace core {
	/**
	 * @brief Portaudio backend implementation.
	 */
	class DevicePortaudio final : public Device {

		/**
		 * @brief Stuct with data shared with audio thread.
		 * Mutex is in the Device class
		 */
		struct AudioThreadShared {
			Device& device;
			// buffer for interleaving and deinterleaving
			Device::AudioBuffer bufferTo;
			Device::AudioBuffer bufferFrom;

			AudioThreadShared(Device& d) : device(d) { }
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
			mInitialized = false;
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

			if (in != nullptr) {
				const float* inBuffer = reinterpret_cast<const float*>(in);
				shared.bufferFrom.setFromInterleaved(inBuffer, frames, shared.device.getChannelsIn());
				TKLB_ASSERT(shared.bufferFrom.validSize() == frames)
			}

			if (out != nullptr) {
				// make sure to inform the callback about the amount of samples needed
				shared.bufferTo.setValidSize(frames);
			}

			shared.device.callback(shared.bufferFrom, shared.bufferTo);

			if (out != nullptr) {
				float* outBuffer = reinterpret_cast<float*>(out);
				shared.bufferTo.putInterleaved(outBuffer, shared.bufferTo.validSize());
			}

			return paContinue;
		}

		static void StreamFinished(void* data) {
			AudioThreadShared& shared = *reinterpret_cast<AudioThreadShared*>(data);
		}

	public:

		DevicePortaudio(Backend& backend) : mShared(*this), Device(backend) { }

		~DevicePortaudio() { cleanUp(); }

		bool openDevice(DeviceInfo& device) override {
			const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(device.id);

			if (deviceInfo == nullptr) {
				TKLB_ASSERT(false)
				return false;
			}

			PaStreamParameters inputParameters;
			inputParameters.device = device.id;
			inputParameters.sampleFormat = paFloat32;
			inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;
			inputParameters.hostApiSpecificStreamInfo = NULL;
			TKLB_ASSERT(device.channelsIn <= uint(deviceInfo->maxInputChannels))
			inputParameters.channelCount = device.channelsIn;

			PaStreamParameters outputParameters;
			outputParameters.device = device.id;
			outputParameters.sampleFormat = paFloat32;
			outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
			outputParameters.hostApiSpecificStreamInfo = NULL;
			TKLB_ASSERT(device.channelsOut <= uint(deviceInfo->maxOutputChannels))
			outputParameters.channelCount = device.channelsOut;

			PaError err = Pa_OpenStream(
				&mStream,
				0 < inputParameters.channelCount ? &inputParameters : NULL,
				0 < outputParameters.channelCount ? &outputParameters : NULL,
				Config::SampleRate, // try getting internal samplerate
				device.bufferSize,
				paClipOff, // no clipping, device will do that
				AudioCallback,
				&mShared
			);

			if (err != paNoError) {
				TKLB_ASSERT(false)
				cleanUp();
				return false;
			}

			err = Pa_SetStreamFinishedCallback(
				mStream, StreamFinished
			);

			if (err != paNoError) {
				TKLB_ASSERT(false)
				cleanUp();
				return false;
			}

			// Might have gotten different samplerate
			const PaStreamInfo* streamInfo = Pa_GetStreamInfo(mStream);
			init(uint(streamInfo->sampleRate), device.channelsIn, device.channelsOut);

			// Setup deinterleave buffers
			if (0 < inputParameters.channelCount) {
				mShared.bufferFrom.resize(device.bufferSize, inputParameters.channelCount);
				mShared.bufferFrom.sampleRate = device.sampleRate;
			}

			if (0 < outputParameters.channelCount) {
				mShared.bufferTo.resize(device.bufferSize, outputParameters.channelCount);
				mShared.bufferTo.sampleRate = device.sampleRate;
			}

			err = Pa_StartStream(mStream);

			if (err != paNoError) {
				TKLB_ASSERT(false)
				cleanUp();
				return false;
			}

			return true;
		}

		bool openDevice(bool input = false) override {
			DeviceInfo device =
				input ? mBackend.getDefaultInputDevice() : mBackend.getDefaultOutputDevice();
			return openDevice(device);
		}

		bool closeDevice() override {
			cleanUp();
			return true;
		}
	};

	class BackendPortAudio final : public Backend {
		BackendPortAudio() {
			PaError err = Pa_Initialize();
			if (err != paNoError) {
				TKLB_ASSERT(false)
				return;
			}
		}

		~BackendPortAudio() {
			Pa_Terminate();
		}
	public:
		static BackendPortAudio& instance() {
			static BackendPortAudio backend;
			return backend;
		}

		unsigned int getDeviceCount() override {
			return Pa_GetDeviceCount();
		}

		DeviceInfo getDevice(unsigned int index) override {
			const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(index);
			if (deviceInfo == nullptr) {
				TKLB_ASSERT(false)
				return DeviceInfo();
			}
			DeviceInfo info;
			info.id = index;
			info.channelsIn = deviceInfo->maxInputChannels;
			info.channelsOut = deviceInfo->maxOutputChannels;
			info.sampleRate = uint(deviceInfo->defaultSampleRate);
			tklb::memory::stringCopy(info.name, deviceInfo->name, sizeof(DeviceInfo::name));
			tklb::memory::stringCopy(info.api, getName(), sizeof(DeviceInfo::api), false);
			return info;
		};

		const char* getName() override  { return "portaudio"; };

		DeviceInfo getDefaultOutputDevice() override {
			return getDevice(Pa_GetDefaultOutputDevice());
		};

		DeviceInfo getDefaultInputDevice() override {
			return getDevice(Pa_GetDefaultInputDevice());
		};

		Device* createDevice() override {
			return TKLB_NEW(DevicePortaudio, *this);
		}
	};

} } // namespace vae::core

#endif // VAEZ_PORTAUDIO
