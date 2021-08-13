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

	class Device;

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

		/**
		 * Creates a device instance for this backend
		 */
		virtual Device* createDevice() = 0;
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
				if (0 < mChannelsIn) {
					mResamplerFromDevice.init(sampleRate, Config::SampleRate, Config::MaxBlock);
					mBufferFromDevice.resize(mResamplerFromDevice.calculateBufferSize(Config::MaxBlock), channelsIn);
					// Since it will be resampled to that
					mBufferFromDevice.sampleRate = Config::SampleRate;
				}

				if (0 < mChannelsOut) {
					mResamplerToDevice.init(Config::SampleRate, sampleRate, Config::MaxBlock);
					mBufferToDevice.resize(mResamplerFromDevice.calculateBufferSize(Config::MaxBlock), channelsIn);
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
		Device(Backend& backend) : mBackend(backend) { }

		virtual ~Device() { }

		virtual void setSync(SyncCallback& callback) {
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
		}
	};
} } // namespace vae::core

#endif // VAEZ_DEVICE

