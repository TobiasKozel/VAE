#ifndef VAEX_EMITTER
#define VAEX_EMITTER

#include <cstddef>
#include <bitset>
#include "../../external/tklb/src/types/THeapBuffer.hpp"
#include "../../external/tklb/src/types/audio/TAudioBuffer.hpp"
#include "../../external/glm/glm/glm.hpp"
#include "./vae_config.hpp"
#include "./vae_handle_pool.hpp"


namespace vae { namespace core {
	struct Clip {
		Handle id;
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
		Handle clip;
		Config::Time time = 0; // time in samples
		glm::vec3 position;
		glm::vec3 velocity = { 0, 0, 0 };
		glm::vec3 direction = { 0, 0, 0 };
		float speed = 1.0; // playback speed
		float gain = 1.0; // volume
		enum State {
			playing = 0,
			loop,
			virt, // whether voice is virtual
			canvirt, // wheth	er voice can be turned virtual
			killable, // whether voice will be killed instead of virtual
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
