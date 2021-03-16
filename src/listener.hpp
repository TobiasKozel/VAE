#pragma once

#include "../external/tklb/types/audio/TAudioBuffer.h"
#include "./config.hpp"
namespace VAE {
	class Listener {
	protected:
		tklb::AudioBufferTpl<Config::Sample> mBuffer;
	};
}
