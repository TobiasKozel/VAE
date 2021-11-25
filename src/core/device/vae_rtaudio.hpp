#ifndef _VAE_RTAUDIO
#define _VAE_RTAUDIO

#include "./vae_device.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"

#include "../../../external/rtaudio/RtAudio.h"

namespace vae { namespace core {

	class DeviceRtaudio final : public Device {

		/**
		 * @brief Stuct with data shared with audio thread.
		 * Mutex is in the Device class
		 */
		struct AudioThreadShared {
			Device& device;
			// buffer for interleaving and deinterleaving
			AudioBuffer bufferTo;
			AudioBuffer bufferFrom;
			size_t underruns = 0;
			AudioThreadShared(Device& d) : device(d) { }
		};

		RtAudio mAudio; // Rt Device instance
		AudioThreadShared mShared; // Data shared with the audio thread

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
			AudioThreadShared& shared = *reinterpret_cast<AudioThreadShared*>(data);

			if (status) { shared.underruns++; }

			(void) streamTime;

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

			return 0;
		}


	public:
		DeviceRtaudio(
			Backend& backend, EngineConfig& config
		) : mShared(*this), Device(backend, config) { };

		~DeviceRtaudio() {
			cleanUp();
		}

		bool openDevice(DeviceInfo& device) override {
			if (mBackend.getDeviceCount() < device.id) {
				TKLB_ASSERT(false)
				return false;
			}

			RtAudio::StreamParameters inParams, outParams;
			inParams.deviceId = device.id;
			inParams.nChannels = device.channelsIn;
			outParams.deviceId = device.id;
			outParams.nChannels = device.channelsOut;

			auto result = mAudio.openStream(
				outParams.nChannels ? &outParams : nullptr,
				inParams.nChannels  ? &inParams  : nullptr,
				RTAUDIO_FLOAT32, device.sampleRate,
				&device.bufferSize, &AudioCallback, &mShared
			);

			if (result != RTAUDIO_NO_ERROR) {
				return false;
			}

			device.sampleRate = mAudio.getStreamSampleRate();
			device.channelsIn = inParams.nChannels;
			device.channelsOut = outParams.nChannels;

			init(device.sampleRate, device.channelsIn, device.channelsOut);

			// Setup deinterleave buffers
			if (0 < inParams.nChannels) {
				mShared.bufferFrom.resize(device.bufferSize, inParams.nChannels);
				mShared.bufferFrom.sampleRate = device.sampleRate;
			}

			if (0 < outParams.nChannels) {
				mShared.bufferTo.resize(device.bufferSize, outParams.nChannels);
				mShared.bufferTo.sampleRate = device.sampleRate;
			}

			result = mAudio.startStream();
			if (result != RTAUDIO_NO_ERROR) {
				cleanUp();
				return false;
			}

			return true;
		}

		bool openDevice(bool input = false) override {
			DeviceInfo device =
				input ? mBackend.getDefaultInputDevice() : mBackend.getDefaultOutputDevice();
			if (input) {
				device.channelsOut = 0;
			} else {
				device.channelsIn = 0;
			}
			return openDevice(device);
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
			info.sampleRate = uint(rtInfo.preferredSampleRate);
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
