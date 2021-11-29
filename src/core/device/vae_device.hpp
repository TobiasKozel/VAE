#ifndef _VAE_DEVICE
#define _VAE_DEVICE

#include "../../../external/tklb/src/memory/TMemory.hpp"
#include "../../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../../external/tklb/src/types/audio/TAudioRingBuffer.hpp"
#include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"
#include "../../../external/tklb/src/types/TSpinLock.hpp"
#include "../../../external/tklb/src/types/TLockGuard.hpp"
#include "../../../external/tklb/src/types/TDelegate.hpp"

#include "../../../include/vae/vae.hpp"
#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"
#include "./vae_backend.hpp"

namespace vae { namespace core {
	/**
	 * @brief Interface for audio devices.
	 * Default implementation for resampling already provided.
	 */
	class Device {
	public:
		using Resampler = tklb::ResamplerTpl<Sample>;
		using Mutex = tklb::SpinLock;
		using Lock = tklb::LockGuard<Mutex>;

	protected:
		Backend& mBackend;
		EngineConfig& mConfig;
		using Callback = tklb::Delegate<void(Device*)>;
		Size mSampleRate = 0;
		Size mRealSampleRate = 0;

		Resampler mResamplerToDevice;
		AudioBuffer mResamplerBufferToDevice;

		/**
		 * @brief Data shared with audio thread
		 */
		struct AudioThreadWorker {
			Callback callback;				// Optional callback to process synchronously in the audio callback
			Device* device;					// Device passed to callback function
			RingBuffer queueToDevice;		// Deinterleaved data in device samplerate
			RingBuffer queueFromDevice;		// Deinterleaved data in device samplerate
			AudioBuffer convertBuffer;		// convert from interleaved, and Sample type

			Resampler resamplerFromDevice;	// Resample audio before putting it in queueFromDevice
			AudioBuffer resampleBufferFromdevice;

			size_t streamTime = 0;			// device time without regard of underruns and overruns
			Size underruns = 0;				// Happens when the sound card needs more data than queueToDevice provides
			Size overruns = 0;				// Happens when the sound card emites more data than queueFromDevice fits
			Mutex mutex;					// Lock the queues
			Uchar channelsOut = 0;
			Uchar channelsIn = 0;

			/**
			 * @brief Called from audio backend to push in interleaved audio data
			 *
			 * @tparam T Sample type
			 * @tparam INTERLEAVED Whether to treat data as interleaved
			 * @param from
			 * @param to
			 * @param frames
			 */
			template <typename T>
			void swapBufferInterleaved(const T* from, T* to, Size frames) {
				if (from != nullptr) {
					convertBuffer.setFromInterleaved(from, frames, channelsIn);
					if (resamplerFromDevice.isInitialized()) {
						resamplerFromDevice.process(convertBuffer, resampleBufferFromdevice);
						Lock l(mutex);
						auto pushed = queueFromDevice.push(resampleBufferFromdevice);
						overruns += (frames - pushed);
					} else {
						Lock l(mutex);
						auto pushed = queueFromDevice.push(convertBuffer);
						overruns += (frames - pushed);
					}
				}

				if (callback.valid()) {
					callback(device);
				}

				if (to != nullptr) {
					int popped;
					{
						Lock l(mutex);
						popped = queueToDevice.pop(convertBuffer, frames);
						underruns += (frames - popped);
					}
					convertBuffer.putInterleaved(to, popped);
				}
				streamTime += frames;
			}
		};

		static constexpr int _VAE_WORKER_SIZE = sizeof(AudioThreadWorker);

		AudioThreadWorker mWorker;

		/**
		 * @brief initializes buffers, queues and resamplers if needed
		 * Has to be called in openDevice once the samplerate
		 * and channel config is known
		 *
		 * @param sampleRate The actual samplerate the device has, might not match requested rate
		 * @param channelsIn The real channelcount, might not match requested
		 * @param channelsOut The real channelcount, might not match requested
		 * @param bufferSize The amount of frames a callback will provide/request
		 */
		void init(Size sampleRate, Uchar channelsIn, Uchar channelsOut, Size bufferSize) {
			mWorker.channelsIn  = channelsIn;
			mWorker.channelsOut = channelsOut;
			mRealSampleRate  = sampleRate;

			if (sampleRate != mConfig.preferredSampleRate) {
				if (0 < channelsIn) {
					// we get full device buffersize for these buffers
					mWorker.resamplerFromDevice.init(
						sampleRate, mConfig.preferredSampleRate, bufferSize
					);
					mWorker.resampleBufferFromdevice.resize(
						mWorker.resamplerFromDevice.calculateBufferSize(bufferSize),
						channelsIn
					);
					mWorker.resampleBufferFromdevice.sampleRate =
						mConfig.preferredSampleRate;
					mSampleRate = mConfig.preferredSampleRate;
				}
				if (0 < channelsOut) {
					// But only DSP max block size in this direction
					mResamplerToDevice.init(
						mConfig.preferredSampleRate, sampleRate, Config::MaxBlock
					);
					mResamplerBufferToDevice.resize(
						mResamplerToDevice.calculateBufferSize(Config::MaxBlock),
						channelsOut
					);
					mResamplerBufferToDevice.sampleRate = sampleRate;
					mSampleRate = mConfig.preferredSampleRate;
				}
			} else {
				mSampleRate = mConfig.preferredSampleRate;
			}

			if (0 < channelsIn) {
				mWorker.queueFromDevice.resize(
					bufferSize * mConfig.bufferPeriods,
					channelsIn
				);
			}
			if (0 < channelsOut) {
				mWorker.queueToDevice.resize(
					bufferSize * mConfig.bufferPeriods,
					channelsOut
				);
			}

			mWorker.convertBuffer.sampleRate = sampleRate;
			mWorker.convertBuffer.resize(
				bufferSize,
				std::max(channelsOut, channelsIn) // buffer is sharedfor in and out, so make sure it has space for both
			);
		}

		void postInit() {
			VAE_DEBUG("Opened Audio Device with samplerate %i", mRealSampleRate)
			if (mRealSampleRate != mSampleRate) {
				VAE_DEBUG("Audio Device resamples to %i", mSampleRate)
			}
		}

	public:
		Device(
			Backend& backend, EngineConfig& config
		) : mBackend(backend), mConfig(config) { }

		virtual ~Device() {
			VAE_DEBUG("Device destructed. Underruns: %i Overruns:%i", mWorker.underruns, mWorker.overruns)
		}

		void setCallback(Callback callback) {
			mWorker.device = this;
			mWorker.callback = callback;
			VAE_DEBUG("Device uses sync callback")
		}

		/**
		 * @brief Opens a specific audio device.
		 * The device struct may be altered to match the actual hardware.
		 * (sampleRate, bufferSize and channel count)
		 */
		virtual bool openDevice(DeviceInfo& device) = 0;

		/**
		 * @brief Tries to open the default audio device whith desired in out channels
		 */
		virtual bool openDevice(bool input = false) {
			DeviceInfo device =
				input ? mBackend.getDefaultInputDevice() : mBackend.getDefaultOutputDevice();
			return openDevice(device);
		};

		/**
		 * @brief Closes the currently open device.
		 * Otherwise does nothing.
		 */
		virtual bool closeDevice() = 0;

		/**
		 * @brief Push samples to the audio device
		 * @param buffer Pushes the amount of valid samples
		 */
		Size push(const AudioBuffer& buffer) {
			VAE_ASSERT(0 < mWorker.channelsOut)
			auto frames = buffer.validSize();
			VAE_ASSERT(frames != 0) // need to have valid frames
			if (mResamplerToDevice.isInitialized()) {
				mResamplerToDevice.process(buffer, mResamplerBufferToDevice);
				Lock lock(mWorker.mutex);
				return mWorker.queueToDevice.push(mResamplerBufferToDevice);
			} else {
				Lock lock(mWorker.mutex);
				return mWorker.queueToDevice.push(buffer);
			}
		}

		Size canPush() const {
			auto remaining = mWorker.queueToDevice.remaining();
			if (mResamplerToDevice.isInitialized()) {
				return mResamplerToDevice.estimateNeed(remaining);
			}
			return remaining;
		}

		/**
		 * @brief Get samples form audio device
		 * @param buffer Gets the amount of valid samples, might actualy get less
		 */
		void pop(AudioBuffer& buffer) {
			VAE_ASSERT(0 < mWorker.channelsIn)
			auto frames = buffer.validSize();
			VAE_ASSERT(frames != 0) // need to have valid frames
			Lock lock(mWorker.mutex);
			mWorker.queueFromDevice.pop(buffer, frames);
		}

		Size canPop() const {
			return mWorker.queueFromDevice.filled();
		}

		Size getChannelsOut() const { return mWorker.channelsOut; }

		Size getChannelsIn() const { return mWorker.channelsIn; }

		/**
		 * @brief Get the sample rate
		 * @return samplerate after resampling
		 */
		Size getSampleRate() const { return mSampleRate; }

		/**
		 * @brief Get the Real Sample Rate before resampling
		 *
		 * @return Size
		 */
		Size getRealSampleRate() const { return mRealSampleRate; }

		size_t getStreamTime() const { return mWorker.streamTime; }

		Size getOverruns() const { return mWorker.overruns; }
		Size getUnderruns() const { return mWorker.underruns; }
	}; // class Device

	// TODO VAE this seems a bit excessive
	constexpr int _VAE_DEVICE_SIZE = sizeof(Device);
} } // namespace vae::core

#endif // _VAE_DEVICE

