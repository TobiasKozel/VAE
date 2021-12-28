#ifndef _VAE_EVENT
#define _VAE_EVENT

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "./vae_mixer.hpp"



namespace vae { namespace core {
	/**
	 * @brief An Event is used to control most of the eingines behavior.
	 */
	struct Event {
		enum class Action {
			start = 0,		///< Starts a source
			stop,			///< Stops a source
			emit,			///< Emits an event to the EventCallback defined in the engine config
			random			///< triggers one random on_start event
		} action : 3 ;
		bool force_mixer : 1;	///< Prevents overriding the mixer from chained events or fireEvent
		bool loop : 1;			///< gapless looping
		bool HRTF : 1;			///< Listener and event has to have hrtf set
		bool spatial : 1;		///< no spatial rendering at all
		bool attenuate : 1;		///< whether distance is taken into consideration
		bool critical : 1;		///< wheather the voice can be killer

		MixerHandle mixer = Mixer::MasterMixerHandle;			///< Mixer the source gets written to
		EventHandle id = InvalidEventHandle;					///< Own id
		SourceHandle source = InvalidSourceHandle;				///< Handle to a source
		Sample gain;											///< Volume applied to triggered voice
		EventHandle on_start[StaticConfig::MaxChainedEvents];	///< Events called when the source starts playing
		EventHandle on_end[StaticConfig::MaxChainedEvents];		///< Events fired once the source is finished, not called when there's no source
		NameString name;										///< Name for debugging

		/**
		 * TODO this isn't exactly pod style but the arrays need to be
		 * initialized properly since 0 is a valid event handle
		 */
		Event() {
			force_mixer = false;
			loop = false;
			HRTF = false;
			spatial = true;
			attenuate = true;
			critical = false;
			gain = 1.0;
			for (size_t i = 0; i < StaticConfig::MaxChainedEvents; i++) {
				on_start[i] = InvalidEventHandle;
				on_end[i] = InvalidEventHandle;
			}
		}
	};

	constexpr int _VAE_EVENT_SIZE = sizeof(Event);
} } // namespace vae::vore

#endif // _VAE_EVENT
