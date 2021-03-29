#pragma once

#include "./audiobuffer.hpp"

namespace VAE {
	class Voice {
	protected:
		const AudioBuffer* mSample = nullptr;
		size_t mTime = 0;
		float mVolume = 1.0;
		float mPan = 0.0;
		bool mPlaying = false;
		bool mVirtual = false;

	public:
		Voice(const AudioBuffer* buffer) {
			mSample = buffer;
			mPlaying = true;
		}

		bool isVirtual() const { return mVirtual; };

		void process() {
			if (mVirtual) { return; }

		}

		bool tick(size_t frames) {
			if (!mPlaying) { return false; }

			mTime += frames;
			if (mSample->length() <= mTime) {
				mPlaying = false;
				mTime = 0;
			}

			return true;
		}
	};
}
