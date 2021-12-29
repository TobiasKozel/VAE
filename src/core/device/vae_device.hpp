#ifndef _VAE_DEVICE
#define _VAE_DEVICE

#include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"
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
		using Resampler = tklb::ResamplerTpl<Sample, ScratchBuffer>;

	protected:
		Backend& mBackend;			// Can't be const because rt audio isn't doing any const
		const EngineConfig& mConfig;
		using Callback = tklb::Delegate<void(Device*)>;
		Size mSampleRate = 0;		// Samplerate after resampling
		Size mRealSampleRate = 0;	// Device sample rate
		Size mOverruns = 0;			// Happens when too many frames are pushed to the deivce
		Size mUnderruns = 0;		// Happens when too many frames are popper from the device

		Resampler mResamplerToDevice;
		ScratchBuffer mResamplerBufferToDevice;

		/**
		 * @brief Data shared with audio thread
		 */
		struct AudioThreadWorker {
			Callback callback;				// Optional callback to process synchronously in the audio callback
			Device* device;					// Device passed to callback function
			RingBuffer queueToDevice;		// Deinterleaved data in device samplerate
			RingBuffer queueFromDevice;		// Deinterleaved data in device samplerate
			ScratchBuffer convertBuffer;		// convert from interleaved, and Sample type

			Resampler resamplerFromDevice;	// Resample audio before putting it in queueFromDevice
			ScratchBuffer resampleBufferFromdevice;

			size_t streamTime = 0;			// device time without regard of underruns and overruns
			Size underruns = 0;				// Happens when the sound card needs more data than queueToDevice provides
			Size overruns = 0;				// Happens when the sound card emites more data than queueFromDevice fits
			VAE_PROFILER_MUTEX(Mutex, mutex, "Device mutex")	// Lock the queues
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
				VAE_PROFILER_SCOPE()
				VEA_PROFILER_THREAD_NAME("Device Thread")
				if (from != nullptr) {
					convertBuffer.setFromInterleaved(from, frames, channelsIn);
					if (resamplerFromDevice.isInitialized()) {
						resamplerFromDevice.process(convertBuffer, resampleBufferFromdevice);
						Lock l(mutex);
						auto pushed = queueFromDevice.push(resampleBufferFromdevice);
						overruns += (resampleBufferFromdevice.validSize() - pushed);
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
				VAE_PROFILER_PLOT(profiler::deviceUnderruns, int64_t(underruns));
				// VAE_PROFILER_PLOT(profiler::deviceOverruns, int64_t(overruns)); // don't track them since they're not getting drained for now
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
			VAE_PROFILER_SCOPE_NAMED("Init Device")
			mWorker.channelsIn  = channelsIn;
			mWorker.channelsOut = channelsOut;
			mRealSampleRate  = sampleRate;

			if (sampleRate != mConfig.internalSampleRate) {
				if (0 < channelsIn) {
					// we get full device buffersize for these buffers
					mWorker.resamplerFromDevice.init(
						sampleRate, mConfig.internalSampleRate, bufferSize
					);
					mWorker.resampleBufferFromdevice.resize(
						mWorker.resamplerFromDevice.calculateBufferSize(bufferSize),
						channelsIn
					);
					mWorker.resampleBufferFromdevice.sampleRate =
						mConfig.internalSampleRate;
					mSampleRate = mConfig.internalSampleRate;
				}
				if (0 < channelsOut) {
					// But only DSP max block size in this direction
					mResamplerToDevice.init(
						mConfig.internalSampleRate, sampleRate, StaticConfig::MaxBlock
					);
					mResamplerBufferToDevice.resize(
						mResamplerToDevice.calculateBufferSize(StaticConfig::MaxBlock),
						channelsOut
					);
					mResamplerBufferToDevice.sampleRate = sampleRate;
					mSampleRate = mConfig.internalSampleRate;
				}
			} else {
				mSampleRate = mConfig.internalSampleRate;
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
				mWorker.queueToDevice.set(0);
			}

			mWorker.convertBuffer.sampleRate = sampleRate;
			mWorker.convertBuffer.resize(
				bufferSize,
				std::max(channelsOut, channelsIn) // buffer is shared for in and out, so make sure it has space for both
			);
		}

		void postInit() {
			VAE_DEBUG("Opened Audio Device on %s with samplerate %i", mBackend.getName(), mRealSampleRate)
			if (mRealSampleRate != mSampleRate) {
				VAE_DEBUG("Audio Device resamples to %i", mSampleRate)
			}
		}

	public:
		VAE_PROFILER_OVERLOAD_NEW()

		/**
		 * @brief Only a Backend can construct a Device
		 */
		Device(
			Backend& backend, const EngineConfig& config
		) : mBackend(backend), mConfig(config) { }

		virtual ~Device() {
			VAE_DEBUG(
				"Device destructed. Underruns: %i %i Overruns:%i %i",
				mWorker.underruns, mUnderruns,
				mWorker.overruns, mOverruns
			)
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
		Size push(const ScratchBuffer& buffer) {
			VAE_PROFILER_SCOPE()
			VAE_ASSERT(0 < mWorker.channelsOut)
			const auto frames = buffer.validSize();
			VAE_ASSERT(frames != 0) // need to have valid frames
			if (mResamplerToDevice.isInitialized()) {
				mResamplerToDevice.process(buffer, mResamplerBufferToDevice);
				Lock lock(mWorker.mutex);
				const auto pushed = mWorker.queueToDevice.push(mResamplerBufferToDevice);
				mOverruns += (mResamplerBufferToDevice.validSize() - pushed);
				return pushed;
			} else {
				Lock lock(mWorker.mutex);
				const auto pushed = mWorker.queueToDevice.push(buffer);
				mOverruns += (frames - pushed);
				return pushed;
			}
			VAE_PROFILER_PLOT(profiler::engineOverruns, int64_t(mOverruns));
			return 0;
		}

		/**
		 * @brief Return amount of audio frames which can be pushed in buffer
		 * ! this is an estimate when resampling !
		 * @return Size
		 */
		Size canPush() const {
			VAE_PROFILER_SCOPE()
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
		void pop(ScratchBuffer& buffer) {
			VAE_PROFILER_SCOPE()
			VAE_ASSERT(0 < mWorker.channelsIn)
			auto frames = buffer.validSize();
			VAE_ASSERT(frames != 0) // need to have valid frames
			Lock lock(mWorker.mutex);
			const auto popped = mWorker.queueFromDevice.pop(buffer, frames);
			mUnderruns += (frames - popped);
			VAE_PROFILER_PLOT(profiler::engineUnderruns, int64_t(mOverruns));
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

