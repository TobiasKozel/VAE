#pragma once

namespace VAE {
	class AudioBuffer {
	protected:
		size_t mLength = 0; // Length in frames
		unsigned char mChannels = 0;

	public:
		unsigned char channels() const { return mChannels; }
		size_t length() const { return mLength; }
	};
}
