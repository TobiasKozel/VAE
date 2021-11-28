#ifndef _VAE_RTAUDIO
#define _VAE_RTAUDIO

#include "./vae_device.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"

#include "../../../external/rtaudio/RtAudio.h"

namespace vae { namespace core {

	class DeviceRtaudio final : public Device {
		RtAudio mAudio; // Rt Device instance

		bool cleanUp() {
			if (mAudio.isStreamRunning()) {
				auto result = mAudio.stopStream();
				if (result != RTAUDIO_NO_ERROR) {
					VAE_ASSERT(false)
					return false;
				}
			}
			if (mAudio.isStreamOpen()) {
				mAudio.closeStream();
			}
			return true;
		}

		/**
 		* @brief Function called from the RtAudio thread.
 		*/
		static int AudioCallback(
			void* out, void* in, unsigned int frames,
			double streamTime, RtAudioStreamStatus status, void* data
		) {
			(void) streamTime;	// Prevent unused variable warnings.
			(void) status;		// Prevent unused variable warnings.
			auto inFloat = static_cast<const float*>(in);
			auto outFloat = static_cast<float*>(out);
			static_cast<AudioThreadWorker*>(data)
				->swapBufferInterleaved(inFloat, outFloat, frames);
			return 0;
		}


	public:
		DeviceRtaudio(
			Backend& backend, EngineConfig& config
		) : Device(backend, config) { };

		~DeviceRtaudio() {
			cleanUp();
		}

		bool openDevice(DeviceInfo& device) override {
			if (mBackend.getDeviceCount() < Size(device.id)) {
				TKLB_ASSERT(false)
				return false;
			}

			RtAudio::StreamParameters inParams, outParams;
			inParams.deviceId = device.id;
			inParams.nChannels = device.channelsIn;
			outParams.deviceId = device.id;
			outParams.nChannels = device.channelsOut;
			if (device.bufferSize == 0) {
				device.bufferSize = mConfig.preferredBufferSize;
			}

			auto result = mAudio.openStream(
				outParams.nChannels ? &outParams : nullptr,
				inParams.nChannels  ? &inParams  : nullptr,
				RTAUDIO_FLOAT32, device.sampleRate,
				&device.bufferSize, &AudioCallback, &mWorker
			);

			if (result != RTAUDIO_NO_ERROR) {
				return false;
			}

			device.sampleRate = mAudio.getStreamSampleRate();
			device.channelsIn = inParams.nChannels;
			device.channelsOut = outParams.nChannels;

			init(
				device.sampleRate, device.channelsIn, device.channelsOut,
				device.bufferSize // RtAudio writes back to this on openStream
			);

			result = mAudio.startStream();
			if (result != RTAUDIO_NO_ERROR) {
				cleanUp();
				return false;
			}

			postInit();

			return true;
		}

		bool closeDevice() override { return cleanUp(); }
	};

	class BackendRtAudio final : public Backend {
		RtAudio mAudio; // This instance is only used for probing
		BackendRtAudio() { }

		~BackendRtAudio() { }
	public:
		static BackendRtAudio& instance() {
			static BackendRtAudio backend;
			return backend;
		}

		unsigned int getDeviceCount() override {
			return mAudio.getDeviceCount();
		}

		DeviceInfo getDevice(unsigned int index) override {
			auto rtInfo = mAudio.getDeviceInfo(index);
			TKLB_ASSERT(rtInfo.probed) // device needs to be probed

			DeviceInfo info;
			info.id = index;
			info.channelsIn = rtInfo.inputChannels;
			info.channelsOut = rtInfo.outputChannels;
			info.sampleRate = Uint(rtInfo.preferredSampleRate);
			tklb::memory::stringCopy(info.name, rtInfo.name.c_str(), sizeof(DeviceInfo::name));
			tklb::memory::stringCopy(info.api, getName(), sizeof(DeviceInfo::api), false);
			return info;
		};

		const char* getName() override  { return "rtaudio"; };

		DeviceInfo getDefaultInputDevice() override {
			return getDevice(mAudio.getDefaultInputDevice());
		};

		DeviceInfo getDefaultOutputDevice() override {
			return getDevice(mAudio.getDefaultOutputDevice());
		};

		Device* createDevice(EngineConfig& config) override {
			return TKLB_NEW(DeviceRtaudio, *this, config);
		}
	};
} } // namespace vae::core

#endif // _VAE_RTAUDIO
