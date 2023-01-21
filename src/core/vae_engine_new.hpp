#pragma once


#include "./vae_types.hpp"
#include "./device/vae_device.hpp"
#include "vae_allocators.hpp"


namespace vae { namespace core {
	struct BaseVoice {
		BankHandle bank;							///< Which bank it belongs to
		MixerHandle mixer;							///< Where the voice should mix to
		EventHandle event;							///< Which event triggered the voice to be played
		EmitterHandle emitter;						///< Emitter used to control voice properties
		Sample gain = 1.0;							///< Volume of the voice
		SampleIndex time = 0;						///< Current time in samples
		bool loop VAE_SMALL(1);			///< Voice will loop until killed
		bool filtered VAE_SMALL(1);		///< This will enable high/lowpass filters and variable speed playback. Gets turned on when signal does not match EngineStaticConfig::internalSampleRate
		bool critical VAE_SMALL(1);		///< Voice can't be killed in favor of new voice
		bool started VAE_SMALL(1);		///< Whether the voice has started playing
		bool audible VAE_SMALL(1);		///< Whether the voice was heard by any listener
	};

	struct StreamingVoice {
	};

	struct HRTFVoice {
		ScratchBuffer convolutionBuffer;	///< Temporary buffer for the convolution
		Size convolutionIndex;				///< position in convolution buffer
	};

	struct SPCAPVoice {
		Sample volumes[StaticConfig::MaxChannels];	//< Volume for each channel containing the SPCAP Result to allow interpolation between audio blocks.
	};

	struct FilteredVoice {
		Real lowpass = 0.0;		///< Lowpasses the signal as the value approaches 1
		Real highpass = 0.0;	///< Highpasses the signal as the value approaches 1
		Real speed = 1.0;		///< Playback speed, will alter pitch
		Real timeFract = 0.0;	///< Fractional time component for interpolation
		Sample lowpassScratch[StaticConfig::MaxChannels];	///< Last sample per channel for IIR filter
		Sample highpassScratch[StaticConfig::MaxChannels];	///< Last sample per channel for IIR filter
	};

	class EngineNew {
		HeapBuffer<Device> Devices;								///< All opened devices

		// BaseVoice Voices[512];
		// SPCAPVoice SPCAPVoices[256];
		// FilteredVoice FilteredVoices[256];
		// StreamingVoice StreamingVoices[16];
		// HRTFVoice HRTFVoices[16];

		memory::HandleBuffer<BaseVoice, Size, memory::AllocatorVoice<>> Voices = { 512 };
		memory::HandleBuffer<SPCAPVoice, Size, memory::AllocatorVoice<>> SPCAPVoices = { 256 };
		memory::HandleBuffer<FilteredVoice, Size, memory::AllocatorVoice<>> FilteredVoices = { 256 };
		memory::HandleBuffer<StreamingVoice, Size, memory::AllocatorVoice<>> StreamingVoices = { 16 };
		memory::HandleBuffer<HRTFVoice, Size, memory::AllocatorVoice<>> HRTFVoices = { 16 };

		void process() {

		}
	};

} } // vae::core
