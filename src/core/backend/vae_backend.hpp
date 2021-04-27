#ifndef VAEZ_BACKEND
#define VAEZ_BACKEND

#include "../config.hpp"
#include "../../../include/VAE/vae_device_info.hpp"
#include "../../../external/tklb/src/memory/TMemory.hpp"
#include "../../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"
#include "../../../external/tklb/src/types/TSpinLock.hpp"
#include "../../../external/tklb/src/types/TLockGuard.hpp"

#include <functional>


namespace VAE { namespace core {
	/**
	 * @brief Interface for audio backends.
	 * Default implementation for resampling already provided.
	 */
	class Backend {
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
		RingBuffer mQueueToDevice;   // Queue needed for async mode
		RingBuffer mQueueFromDevice; // Queue needed for async mode

		Resampler mResamplerToDevice;
		Resampler mResamplerFromDevice;

		AudioBuffer mBufferToDevice;   // Engine samplerate only needed for resampling
		AudioBuffer mBufferFromDevice; // Engine samplerate only needed for resampling

		uint mChannelsOut = 0;
		uint mChannelsIn = 0;

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
		 */
		void init(uint sampleRate, uint channelsIn, uint channelsOut) {
			mChannelsIn = channelsIn;
			mChannelsOut = channelsOut;
			if (sampleRate != Config::SampleRate) {
				mResamplerFromDevice.init(sampleRate, Config::SampleRate, Config::MaxBlock);
				const uint fromSize = Resampler::calculateBufferSize(sampleRate, Config::SampleRate, Config::MaxBlock);
				mBufferFromDevice.resize(fromSize, channelsIn);

				mResamplerToDevice.init(Config::SampleRate, sampleRate, Config::MaxBlock);
				const uint toSize = Resampler::calculateBufferSize(Config::SampleRate, sampleRate, Config::MaxBlock);
				mBufferToDevice.resize(toSize, channelsOut);
			}

			if (mSyncCallback != nullptr) {
				mQueueFromDevice.resize(Config::MaxBlock * Config::DeviceMaxPeriods, channelsIn);
				mQueueToDevice.resize(Config::MaxBlock * Config::DeviceMaxPeriods, channelsIn);
			}
		}

	public:
		Backend() {
			mSyncCallback = nullptr;
		}

		Backend(SyncCallback& callback) {
			mSyncCallback = callback;
		}

		/**
		 * @brief Opens the default audio device.
		 */
		virtual bool openDevice() = 0;

		/**
		 * @brief Opens a specific audio device.
		 */
		virtual bool openDevice(const DeviceInfo& device) = 0;

		/**
		 * @brief Gets number of devices, needed to iterate them.
		 */
		virtual uint getDeviceCount() = 0;

		/**
		 * @brief Returns a spefic device info for index.
		 */
		virtual DeviceInfo getDevice(uint id) = 0;

		/**
		 * @brief Closes the currently open device.
		 * Otherwise does nothing.
		 */
		virtual bool closeDevice() = 0;

		/**
		 * @brief Push samples to the audio device
		 * @param buffer Always a stereo buffer, pushes the amount of valid samples
		 */
		void pushAsync(const AudioBuffer& buffer) {
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
			auto frames = buffer.validSize();
			TKLB_ASSERT(frames != 0) // need to have valid frames
			TKLB_ASSERT(mSyncCallback == nullptr) // can't be called in sync mode
			Lock lock(mMutex);
			mQueueFromDevice.push(buffer);
		}

		uint getChannelsOut() const { return mChannelsOut; }

		uint getChannelsIn() const { return mChannelsIn; }


		/**
		 * @brief Called from the backend implementation.
		 * Don't call this otherwise.
		 * This only works for duplex callbacks. Getting
		 * and recieving different amounts needs to be implemented
		 * in the derived class.
		 * Derived class is responsible for the Buffer supplied to the function.
		 * TODO tklb streamtime could be used if device is temporarly disabled
		 * @param in input frames in device samplerate
		 * @param out output frames in device samplerate
		 */
		void callback(const AudioBuffer& fromDevice, AudioBuffer& toDevice) {
			// Need to have space in the out buffer
			TKLB_ASSERT(fromDevice.validSize() <= toDevice.size())

			if (mResamplerToDevice.isInitialized()) { // resampling needed
				// Can't resample only one channel, rates need to match as well
				TKLB_ASSERT(mResamplerToDevice.isInitialized() == mResamplerFromDevice.isInitialized())

				mResamplerFromDevice.process(fromDevice, mBufferFromDevice);

				if (mSyncCallback == nullptr) { // async mode
					Lock lock(mMutex);
					mQueueFromDevice.push(mBufferFromDevice); // store away the resampled frames
					// since the two resamplers should be in sync, we can pop the amount of
					// frames the push resampler emitted to figure out how much we need
					mQueueToDevice.pop(mBufferToDevice, mBufferFromDevice.validSize());
					// Only if we got as many samples as we pushed to the device, in out are in sync
					TKLB_ASSERT(mBufferToDevice.validSize() == mBufferFromDevice.validSize())
					/**
					 * No callback here since the consumer is responsible for getting and
					 * providing audio from the queues in time
					 */
				} else { // sync mode
					mSyncCallback(mBufferFromDevice, mBufferToDevice);
				}
				mResamplerToDevice.process(mBufferToDevice, toDevice);
			} else { // No samplerate conversion needed
				if (mSyncCallback == nullptr) { // async mode
					Lock lock(mMutex);
					mQueueFromDevice.push(fromDevice);
					// Push the same amout to the device
					mQueueToDevice.pop(toDevice, fromDevice.validSize());
				} else { // sync mode
					mSyncCallback(fromDevice, toDevice);
				}
			}
		}
	};
} } // namespace VAE::core

#endif // VAEZ_BACKEND

