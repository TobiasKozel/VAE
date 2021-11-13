#ifndef VAEZ_RTAUDIO
#define VAEZ_RTAUDIO

#include "./vae_device.hpp"
#include "../vae_config.hpp"

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
			Device::AudioBuffer bufferTo;
			Device::AudioBuffer bufferFrom;
			size_t underruns = 0;
			AudioThreadShared(Device& d) : device(d) { }
		};

		RtAudio mAudio; // Rt Device instance
		AudioThreadShared mShared; // Data shared with the audio thread

		bool cleanUp() {
			try {
				if (mAudio.isStreamRunning()) {
					mAudio.stopStream();
				}
				if (mAudio.isStreamOpen()) {
					mAudio.closeStream();
				}
			} catch (RtAudioError& e) {
				e.printMessage();
				TKLB_ASSERT(false)
				return false;
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
		DeviceRtaudio(Backend& backend) : mShared(*this), Device(backend) { };

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

			try {
				mAudio.openStream(
					outParams.nChannels ? &outParams : nullptr,
					inParams.nChannels  ? &inParams  : nullptr,
					RTAUDIO_FLOAT32, device.sampleRate,
					&device.bufferSize, &AudioCallback, &mShared
				);
			} catch (RtAudioError& e) {
				e.printMessage();
				TKLB_ASSERT(false)
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

			try {
				mAudio.startStream();
			} catch (RtAudioError& e) {
				e.printMessage();
				cleanUp();
				TKLB_ASSERT(false)
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

		Device* createDevice() override {
			return TKLB_NEW(DeviceRtaudio, *this);
		}
	};
} } // namespace vae::core

#endif // VAEZ_RTAUDIO
