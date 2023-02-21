#ifndef _VAE_ALSA
#define _VAE_ALSA

#include "./vae_device.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

namespace vae { namespace core {

	class DeviceAlsa final : public Device {
		snd_pcm_t* mPcm;
		size_t pos;

		bool cleanUp() {
			VAE_PROFILER_SCOPE_NAMED("Cleanup Alsa")
			snd_pcm_close(mPcm);
			return true;
		}

		// /**
 		// * @brief Function called from the RtAudio thread.
 		// */
		// static int AudioCallback(
		// 	void* out, void* in, unsigned int frames,
		// 	double streamTime, RtAudioStreamStatus status, void* data
		// ) {
		// 	(void) streamTime;	// Prevent unused variable warnings.
		// 	(void) status;		// Prevent unused variable warnings.
		// 	auto inFloat = static_cast<const float*>(in);
		// 	auto outFloat = static_cast<float*>(out);
		// 	static_cast<AudioThreadWorker*>(data)
		// 		->swapBufferInterleaved(inFloat, outFloat, frames);
		// 	return 0;
		// }



	public:
		DeviceAlsa(
			Backend& backend, const EngineConfig& config
		) : Device(backend, config) { };

		~DeviceAlsa() {
			cleanUp();
		}

		bool openDevice(DeviceInfo& device) override {
			if (mBackend.getDeviceCount() < Size(device.id)) {
				VAE_ERROR("Failed to open deivce with index out of bounds")
				return false;
			}
			if (snd_pcm_open(
				&mPcm,
				"default",	// ASCI name for PCM handle ?
				SND_PCM_STREAM_PLAYBACK,
				0			// Mode (e.g  SND_PCM_NONBLOCK)
			)) {
				return false;

			}
			return snd_pcm_set_params(
				mPcm,
				SND_PCM_FORMAT_FLOAT,
				SND_PCM_ACCESS_RW_INTERLEAVED,
				device.channelsOut,
				device.sampleRate,
				1,	// allow alsa software resampling
				100000		// latency 100ms
			);

			device.channelsIn = tklb::clamp<int>(device.channelsIn, 0, StaticConfig::MaxChannels);
			device.channelsOut = tklb::clamp<int>(device.channelsOut, 0, StaticConfig::MaxChannels);
			RtAudio::StreamParameters inParams, outParams;
			inParams.deviceId = device.id;
			inParams.nChannels = device.channelsIn;
			outParams.deviceId = device.id;
			outParams.nChannels = device.channelsOut;
			if (device.bufferSize == 0) {
				device.bufferSize = mConfig.preferredBufferSize;
			}

			RtAudioErrorType result;
			{
				VAE_PROFILER_SCOPE_NAMED("Open RtAudio stream")
				result = mAudio.openStream(
					outParams.nChannels ? &outParams : nullptr,
					inParams.nChannels  ? &inParams  : nullptr,
					RTAUDIO_FLOAT32, device.sampleRate,
					&device.bufferSize, &AudioCallback, &mWorker
				);
			}

			if (result != RTAUDIO_NO_ERROR) {
				VAE_ERROR("Failed to open RtAudio device with code %i", result)
				return false;
			}

			device.sampleRate = mAudio.getStreamSampleRate();
			device.channelsIn = inParams.nChannels;
			device.channelsOut = outParams.nChannels;

			init(
				device.sampleRate, device.channelsIn, device.channelsOut,
				device.bufferSize // RtAudio writes back to this on openStream
			);
			{
				VAE_PROFILER_SCOPE_NAMED("Start RtAudio stream")
				result = mAudio.startStream();
			}

			if (result != RTAUDIO_NO_ERROR) {
				VAE_ERROR("Failed to open RtAudio device with code %i", result)
				cleanUp();
				return false;
			}

			postInit();

			return true;
		}

		bool closeDevice() override { return cleanUp(); }
	};

	class BackendAlsa final : public Backend {
		BackendAlsa() { }

		~BackendAlsa() { }
	public:
		VAE_PROFILER_OVERLOAD_NEW()
		static BackendAlsa& instance() {
			static BackendAlsa backend;
			return backend;
		}

		unsigned int getDeviceCount() override {
			// return mAudio.getDeviceCount();
		}

		DeviceInfo getDevice(unsigned int index) override {
			const auto rtInfo = mAudio.getDeviceInfo(index);
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

		const char* getName() const override  { return "rtaudio"; };

		DeviceInfo getDefaultInputDevice() override {
			return getDevice(mAudio.getDefaultInputDevice());
		};

		DeviceInfo getDefaultOutputDevice() override {
			return getDevice(mAudio.getDefaultOutputDevice());
		};

		Device* createDevice(const EngineConfig& config) override {
			return new DeviceAlsa(*this, config);
		}
	};
} } // namespace vae::core

#endif // _VAE_ALSA
