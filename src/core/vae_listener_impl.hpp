#pragma once

#include "./config.hpp"
#include "../../external/tklb/util/TMemory.hpp"
#include "../../external/tklb/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/types/audio/TResampler.hpp"
#include "../../include/VAE/vae_device_info.hpp"
#include "../../external/rtaudio/RtAudio.h"

namespace VAE::Impl {

	struct ListnerShared {
		tklb::ResamplerTpl<Config::Sample> resampler;
		tklb::AudioBufferTpl<Config::Sample> buffer;
	};

	int AudioCallback(
		void* out, void* input,
		unsigned int frames, double time,
		RtAudioStreamStatus status, void *data
	) {
		ListnerShared& shared = *reinterpret_cast<ListnerShared*>(data);
		float* outBuffer = reinterpret_cast<float*>(out);
		return 0;
	}

	class ListenerImpl {
		RtAudio* mAudio = nullptr;

		ListnerShared mShared; // Data shared with the audio thread

		void cleanUp() {
			if (mAudio == nullptr) { return; }
			if (mAudio->isStreamRunning()) {
				mAudio->stopStream();
			}
			if (mAudio->isStreamOpen()) {
				mAudio->stopStream();
			}
			TKLB_DELETE(mAudio);
		}

		void setUp() {
			if (mAudio == nullptr) {
				mAudio = TKLB_NEW(RtAudio);
			}
		}
	public:
		bool openDevice() {
			setUp();
			DeviceInfo device;
			device.id = mAudio->getDefaultOutputDevice();
			return openDevice(device);
		}

		bool openDevice(const DeviceInfo& device) {
			setUp();
			unsigned int bufferFrames = device.bufferSize;
			RtAudio::StreamParameters parameters;
			parameters.deviceId = device.id;
			parameters.nChannels = 2; // We only do 2 channels
			parameters.firstChannel = 0;

			try {
				mAudio->openStream(
					&parameters,
					nullptr, // no input
					RTAUDIO_FLOAT32,
					Config::SampleRate, // We might not get the desired rate
					&bufferFrames, // We might not get this either
					&AudioCallback,
					&mShared // Pass shared data to audio thread
				);

				unsigned int sampleRate = mAudio->getStreamSampleRate();
				if (sampleRate != Config::SampleRate) {
					mShared.resampler.init(Config::SampleRate, sampleRate, Config::MaxBlock);
				}
				mShared.buffer.resize(Config::MaxBlock, 2);
				mAudio->startStream();
			} catch ( RtAudioError& e ) {
				cleanUp();
				return false;
			}
			return true;
		}

		unsigned int getDeviceCount() {
			setUp();
			return mAudio->getDeviceCount();
		}


	};
}
