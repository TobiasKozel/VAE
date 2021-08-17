#ifndef VAEX_EMITTER
#define VAEX_EMITTER

#include <cstddef>
#include <bitset>
#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/tklb/src/types/THandleBuffer.hpp"
#include "../../external/glm/glm/glm.hpp"
#include "./vae_config.hpp"


namespace vae { namespace core {
	struct Clip {
		tklb::Handle id;			// simply a unique id to identify this instance
		tklb::AudioBuffer data;

		enum Property {
			streaming = 0,
			ambisonix,
			binaural,
			PROPERTY_COUNT
		};
		std::bitset<PROPERTY_COUNT> property;
	};

	struct Listener {
		size_t time = 0; // time in sample
		glm::vec3 position;
		glm::vec3 direction = { 1, 0, 0 }; // vector or angle?
	};

	struct Emitter {
		tklb::Handle clip;			// References a Clip
		Config::Time time = 0;		// time in samples
		glm::vec3 position;
		glm::vec3 velocity = { 0, 0, 0 };
		glm::vec3 direction = { 0, 0, 0 };
		float speed = 1.0;			// playback speed
		float gain = 1.0;			// volume
		enum State {
			playing = 0,
			loop,
			virt,					// whether voice is virtual
			canvirt,				// whether voice can be turned virtual
			killable,				// whether voice will be killed instead of virtual, if both a re false the voice will keep playing
			binaural,
			doppler,
			delay,
			STATE_COUNT
		};
		std::bitset<STATE_COUNT> state;
		// float size = 0;
		// float radius = 1.0;
		// float falloff;
	};
} } // vae::core
#endif // VAEX_EMITTER
