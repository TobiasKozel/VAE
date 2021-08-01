#ifndef VAEZ_DEVICE
#define VAEZ_DEVICE

#include "../vae_config.hpp"
#include "../../../include/vae/vae_device_info.hpp"
#include "../../../external/tklb/src/memory/TMemory.hpp"
#include "../../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"
#include "../../../external/tklb/src/types/TSpinLock.hpp"
#include "../../../external/tklb/src/types/TLockGuard.hpp"

#include <functional>


namespace vae { namespace core {

	/**
	 * @brief Backend interface used to query devices before
	 * creating a actual device object
	 */
	class Backend {
	public:
		using uint = unsigned int;

		/**
		 * Returns name of the api
		 */
		virtual const char* getName() = 0;

		/**
		 * @brief Gets number of devices, needed to iterate them.
		 * Device index != does not have to be the device index!
		 */
		virtual uint getDeviceCount() = 0;

		/**
		 * @brief Returns a spefic device info for index.
		 */
		virtual DeviceInfo getDevice(uint index) = 0;

		virtual DeviceInfo getDefaultInputDevice() = 0;

		virtual DeviceInfo getDefaultOutputDevice() = 0;
	};

	/**
	 * @brief Interface for audio devices.
	 * Default implementation for resampling already provided.
	 */
	class Device {
	public:
		using uint = unsigned int;
		using uchar = unsigned char;
		using AudioBuffer = tklb::AudioBufferTpl<Config::Sample>;
		using Resampler = tklb::ResamplerTpl<Config::Sample>;
		using RingBuffer = tklb::AudioRingBufferTpl<Config::Sample>;
		using Mutex = tklb::SpinLock;
		using Lock = tklb::LockGuard<Mutex>;
		using SyncCallback = std::function<void(const AudioBuffer& fromDevice, AudioBuffer& toDevice)>;

	protected:
		Backend& mBackend;
		RingBuffer mQueueToDevice;   // Queue needed for async mode
		RingBuffer mQueueFromDevice; // Queue needed for async mode

		Resampler mResamplerToDevice;
		Resampler mResamplerFromDevice;

		AudioBuffer mBufferToDevice;   // Engine samplerate only needed for resampling
		AudioBuffer mBufferFromDevice; // Engine samplerate only needed for resampling

		uint mChannelsOut = 0;
		uint mChannelsIn = 0;

		size_t mStreamTime = 0;

		uint mUnderruns = 0;

		/**
		 * @brief If set, the backend will operate in sync/pull mode and drive
		 * the engine mainloop. Only one backend can be used in sync mode.
		 */
		SyncCallback mSyncCallback = nullptr;

		/**
		 * @brief Needed in async mode to pretect the queues from clashes.
		 * Maybe a lock free queue at some point.
		 */
		Mutex mMutex;

		/**
		 * @brief initializes buffers, queues and resamplers if needed
		 * Has to be called in openDevice once the samplerate
		 * and channel config is known
		 */
		void init(uint sampleRate, uint channelsIn, uint channelsOut) {
			mChannelsIn = channelsIn;
			mChannelsOut = channelsOut;

			if (sampleRate != Config::SampleRate) {
				const uint size = Resampler::calculateBufferSize(sampleRate, Config::SampleRate, Config::MaxBlock);
				if (0 < mChannelsIn) {
					mResamplerFromDevice.init(sampleRate, Config::SampleRate, Config::MaxBlock);
					mBufferFromDevice.resize(size, channelsIn);
					// Since it will be resampled to that
					mBufferFromDevice.sampleRate = Config::SampleRate;
				}

				if (0 < mChannelsOut) {
					mResamplerToDevice.init(Config::SampleRate, sampleRate, Config::MaxBlock);
					mBufferToDevice.resize(size, channelsOut);
					// Same rate as above, since it's the source buffer for the resampler
					mBufferToDevice.sampleRate = Config::SampleRate;
				}
			}

			if (mSyncCallback == nullptr) { // need queues for async mode
				if (0 < mChannelsIn) {
					mQueueFromDevice.resize(Config::MaxBlock * Config::DeviceMaxPeriods, channelsIn);
				}
				if (0 < mChannelsOut) {
					mQueueToDevice.resize(Config::MaxBlock * Config::DeviceMaxPeriods, channelsIn);
				}
			}
		}

	public:
		Device(Backend& backend) : mBackend(backend) {
			mSyncCallback = nullptr;
		}

		Device(SyncCallback& callback, Backend& backend) : mBackend(backend) {
			mSyncCallback = callback;
		}

		/**
		 * @brief Tries to open the default audio device whith desired in out channels
		 * TODO check why implementation can't be moved up here
		 */
		virtual bool openDevice(bool input = false) = 0;

		/**
		 * @brief Opens a specific audio device.
		 * The device struct may be altered to match the actual hardware.
		 * (sampleRate, bufferSize and channel count)
		 */
		virtual bool openDevice(DeviceInfo& device) = 0;

		/**
		 * @brief Closes the currently open device.
		 * Otherwise does nothing.
		 */
		virtual bool closeDevice() = 0;

		/**
		 * @brief Push samples to the audio device
		 * @param buffer Always a stereo buffer, pushes the amount of valid samples
		 */
		void push(const AudioBuffer& buffer) {
			TKLB_ASSERT(0 < mChannelsOut)
			auto frames = buffer.validSize();
			TKLB_ASSERT(frames != 0) // need to have valid frames
			TKLB_ASSERT(mSyncCallback == nullptr) // can't be called in sync mode
			Lock lock(mMutex);
			mQueueToDevice.push(buffer);
		}

		/**
		 * @brief Get samples form audio device
		 */
		void pop(AudioBuffer& buffer) {
			TKLB_ASSERT(0 < mChannelsIn)
			auto frames = buffer.validSize();
			TKLB_ASSERT(frames != 0) // need to have valid frames
			TKLB_ASSERT(mSyncCallback == nullptr) // can't be called in sync mode
			Lock lock(mMutex);
			mQueueFromDevice.pop(buffer, frames);
		}

		uint getChannelsOut() const { return mChannelsOut; }

		uint getChannelsIn() const { return mChannelsIn; }

		size_t getStreamTime() const { return mStreamTime; }

		/**
		 * @brief Called from the backend implementation.
		 * Don't call this otherwise.
		 * Derived class is responsible for the Buffer supplied to the function.
		 * TODO tklb streamtime could be used if device is temporarly disabled
		 * @param in input frames in device samplerate
		 * @param out output frames in device samplerate
		 */
		void callback(const AudioBuffer& fromDevice, AudioBuffer& toDevice) {
			const uint frames = std::max(fromDevice.validSize(), toDevice.validSize());
			const bool resampleFrom = mResamplerFromDevice.isInitialized();
			const bool resampleTo = mResamplerToDevice.isInitialized();
			const bool needResample = resampleFrom | resampleTo;

			mStreamTime += frames;

			if (!needResample) {
				if (mSyncCallback == nullptr) { // async mode
					Lock lock(mMutex);
					if (0 < mChannelsIn) {
						mQueueFromDevice.push(fromDevice);
					}
					if (0 < mChannelsOut) {
						// Push the same amout to the device
						mQueueToDevice.pop(toDevice, frames);
					}
				} else { // sync mode
					mSyncCallback(fromDevice, toDevice);
				}
				return;
			}

			// float target_rate = AudioServer::get_singleton()->get_mix_rate();
			// float global_rate_scale = AudioServer::get_singleton()->get_global_rate_scale();

			// uint64_t mix_increment = uint64_t(((get_stream_sampling_rate() * p_rate_scale) / double(target_rate * global_rate_scale)) * double(FP_LEN));

			// for (int i = 0; i < p_frames; i++) {
			// 	uint32_t idx = CUBIC_INTERP_HISTORY + uint32_t(mix_offset >> FP_BITS);
			// 	// 4 point, 4th order optimal resampling algorithm from: http://yehar.com/blog/wp-content/uploads/2009/08/deip.pdf
			// 	float mu = (mix_offset & FP_MASK) / float(FP_LEN);
			// 	AudioFrame y0 = internal_buffer[idx - 3];
			// 	AudioFrame y1 = internal_buffer[idx - 2];
			// 	AudioFrame y2 = internal_buffer[idx - 1];
			// 	AudioFrame y3 = internal_buffer[idx - 0];

			// 	AudioFrame even1 = y2 + y1, odd1 = y2 - y1;
			// 	AudioFrame even2 = y3 + y0, odd2 = y3 - y0;
			// 	AudioFrame c0 = even1 * 0.46835497211269561 + even2 * 0.03164502784253309;
			// 	AudioFrame c1 = odd1 * 0.56001293337091440 + odd2 * 0.14666238593949288;
			// 	AudioFrame c2 = even1 * -0.250038759826233691 + even2 * 0.25003876124297131;
			// 	AudioFrame c3 = odd1 * -0.49949850957839148 + odd2 * 0.16649935475113800;
			// 	AudioFrame c4 = even1 * 0.00016095224137360 + even2 * -0.00016095810460478;
			// 	p_buffer[i] = (((c4 * mu + c3) * mu + c2) * mu + c1) * mu + c0;

			// 	mix_offset += mix_increment;

			// 	while ((mix_offset >> FP_BITS) >= INTERNAL_BUFFER_LEN) {
			// 		internal_buffer[0] = internal_buffer[INTERNAL_BUFFER_LEN + 0];
			// 		internal_buffer[1] = internal_buffer[INTERNAL_BUFFER_LEN + 1];
			// 		internal_buffer[2] = internal_buffer[INTERNAL_BUFFER_LEN + 2];
			// 		internal_buffer[3] = internal_buffer[INTERNAL_BUFFER_LEN + 3];
			// 		if (is_playing()) {
			// 			_mix_internal(internal_buffer + 4, INTERNAL_BUFFER_LEN);
			// 		} else {
			// 			//fill with silence, not playing
			// 			for (int j = 0; j < INTERNAL_BUFFER_LEN; ++j) {
			// 				internal_buffer[j + 4] = AudioFrame(0, 0);
			// 			}
			// 		}
			// 		mix_offset -= (INTERNAL_BUFFER_LEN << FP_BITS);
			// 	}
			// }

			if (resampleFrom) {
				// audio from device can be resampled and put in the buffer
				mResamplerFromDevice.process(fromDevice, mBufferFromDevice);
			}

			if (resampleTo) {
				// however, we don't know how many samples we need to process
				// since this is determined by the resampler
				mBufferToDevice.setValidSize(
					mResamplerToDevice.estimateNeed(toDevice.validSize())
				);
			}

			if (mSyncCallback == nullptr) { // async mode
				Lock lock(mMutex);
				if (resampleFrom) {
					mQueueFromDevice.push(mBufferFromDevice); // store away the resampled frames
				}

				if (resampleTo) {
					mQueueToDevice.pop(mBufferToDevice, mBufferToDevice.validSize());
				}
			} else { // sync mode
				mSyncCallback(mBufferFromDevice, mBufferToDevice);
			}

			if (resampleTo) {
				mResamplerToDevice.process(mBufferToDevice, toDevice);
			}
			int out = toDevice.validSize();

			static int miss = 0;
			static int hit = 0;
			miss += frames != out;
			hit += frames == out;
			if (miss % 100 == 0) {
				printf("Hit: %i Miss: %i\n", hit, miss);
			}


		}
	};
} } // namespace vae::core

#endif // VAEZ_DEVICE

