#ifndef _VAE_PORTAUDIO
#define _VAE_PORTAUDIO

#include "./vae_device.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"

#include "../../../external/portaudio/include/portaudio.h"
#include "../../../external/portaudio/src/common/pa_debugprint.h"

namespace vae { namespace core {
	/**
	 * @brief Portaudio backend implementation.
	 */
	class DevicePortaudio final : public Device {
		PaStream *mStream = nullptr;
		bool mInitialized = false;

		void cleanUp() {
			VAE_PROFILER_SCOPE_NAMED("Cleanup portaudio")
			if (!mInitialized || mStream == nullptr) {
				return;
			}

			{
				VAE_PROFILER_SCOPE_NAMED("Stop portaudio stream")
				PaError err = Pa_StopStream(mStream);
				VAE_ASSERT(err == paNoError)
			}
			{
				VAE_PROFILER_SCOPE_NAMED("Close portaudio stream")
				PaError err = Pa_CloseStream(mStream);
				VAE_ASSERT(err == paNoError)
			}
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
			(void) timeInfo;	// Prevent unused variable warnings.
			(void) statusFlags;	// Prevent unused variable warnings.
			auto inFloat = static_cast<const float*>(in);
			auto outFloat = static_cast<float*>(out);
			static_cast<AudioThreadWorker*>(data)
				->swapBufferInterleaved(inFloat, outFloat, frames);
			return paContinue;
		}

		static void StreamFinished(void* data) {
			(void) data;	// Prevent unused variable warnings.
		}

	public:
		VAE_PROFILER_OVERLOAD_NEW()

		DevicePortaudio(
			Backend& backend, const EngineConfig& config
		) : Device(backend, config) { }

		~DevicePortaudio() { cleanUp(); }

		bool openDevice(DeviceInfo& device) override {
			const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(device.id);

			if (deviceInfo == nullptr) {
				VAE_ASSERT(false)
				return false;
			}

			device.channelsIn = tklb::clamp<int>(device.channelsIn, 0, StaticConfig::MaxChannels);
			device.channelsOut = tklb::clamp<int>(device.channelsOut, 0, StaticConfig::MaxChannels);

			PaStreamParameters inputParameters;
			inputParameters.device = device.id;
			inputParameters.sampleFormat = paFloat32;
			inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;
			inputParameters.hostApiSpecificStreamInfo = NULL;
			VAE_ASSERT(device.channelsIn <= Uint(deviceInfo->maxInputChannels))
			inputParameters.channelCount = device.channelsIn;

			PaStreamParameters outputParameters;
			outputParameters.device = device.id;
			outputParameters.sampleFormat = paFloat32;
			outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
			outputParameters.hostApiSpecificStreamInfo = NULL;
			VAE_ASSERT(device.channelsOut <= Uint(deviceInfo->maxOutputChannels))
			outputParameters.channelCount = device.channelsOut;

			if (device.bufferSize == 0) {
				device.bufferSize = mConfig.preferredBufferSize;
			}

			PaError err;
			{
				VAE_PROFILER_SCOPE_NAMED("Open stream portaudio")
				err = Pa_OpenStream(
					&mStream,
					0 < inputParameters.channelCount ? &inputParameters : NULL,
					0 < outputParameters.channelCount ? &outputParameters : NULL,
					mConfig.internalSampleRate, // try getting internal samplerate
					device.bufferSize,
					paClipOff, // no clipping, device will do that
					AudioCallback,
					&mWorker
				);

			}

			if (err != paNoError) {
				VAE_ASSERT(false)
				cleanUp();
				return false;
			}

			err = Pa_SetStreamFinishedCallback(
				mStream, StreamFinished
			);

			if (err != paNoError) {
				VAE_ASSERT(false)
				cleanUp();
				return false;
			}

			// Might have gotten different samplerate
			const PaStreamInfo* streamInfo = Pa_GetStreamInfo(mStream);
			init(
				Uint(streamInfo->sampleRate),
				device.channelsIn, device.channelsOut,
				device.bufferSize // Pa doesn't provide any info, so we assume we got what we wanted
			);

			{
				VAE_PROFILER_SCOPE_NAMED("Start stream portaudio")
				err = Pa_StartStream(mStream);
			}

			if (err != paNoError) {
				VAE_ASSERT(false)
				cleanUp();
				return false;
			}
			postInit();
			return true;
		}

		bool closeDevice() override {
			cleanUp();
			return true;
		}
	};

	class BackendPortAudio final : public Backend {
		static void debugLog(const char* message) {
			VAE_DEBUG("PortAudio: %s", message)
		}

		BackendPortAudio() {
			/**
			 * Should set up logging, but most of PortAudios underlying
			 * APIs don't care about this and print to stdout
			 */
			PaUtil_SetDebugPrintFunction(&debugLog);
			PaError err = Pa_Initialize();
			if (err != paNoError) {
				VAE_ASSERT(false)
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
				VAE_ASSERT(false)
				return DeviceInfo();
			}
			DeviceInfo info;
			info.id = index;
			info.channelsIn = deviceInfo->maxInputChannels;
			info.channelsOut = deviceInfo->maxOutputChannels;
			info.sampleRate = Uint(deviceInfo->defaultSampleRate);
			tklb::memory::stringCopy(info.name, deviceInfo->name, sizeof(DeviceInfo::name));
			tklb::memory::stringCopy(info.api, getName(), sizeof(DeviceInfo::api), false);
			return info;
		};

		const char* getName() const override  { return "portaudio"; };

		DeviceInfo getDefaultOutputDevice() override {
			return getDevice(Pa_GetDefaultOutputDevice());
		};

		DeviceInfo getDefaultInputDevice() override {
			return getDevice(Pa_GetDefaultInputDevice());
		};

		Device* createDevice(const EngineConfig& config) override {
			return new DevicePortaudio(*this, config);
		}
	};

} } // namespace vae::core

#endif // _VAE_PORTAUDIO
