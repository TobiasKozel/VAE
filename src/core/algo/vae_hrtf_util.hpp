#ifndef _VAE_HRTF_UTIL
#define _VAE_HRTF_UTIL

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_hrtf.hpp"
#include "../voices/vae_voice_hrtf.hpp"
#include "./vae_vec.hpp"
#include "./vae_vec.hpp"
#include <limits>

namespace vae { namespace core {
	struct HRTFUtil {
		/**
		 * @brief Looks for the best matching IR in a HRTF
		 * @details Does bad brute force search, should use kd tree or the mysofalib
		 * @param hrtf
		 * @param direction
		 * @return Size
		 */
		static inline Size closest(const HRTF& hrtf, const vector::Vec3& direction) {
			// TODO this is obviously bad
			TKLB_PROFILER_SCOPE_NAMED("Search HRTF")
			Position closest = std::numeric_limits<Position>::max();
			Size closestIndex = ~0;
			for (Size i = 0; i < hrtf.positions.size(); i++) {
				const auto& pos = hrtf.positions[i];
				const Position dist = vector::distance(pos.pos, direction);
				if (dist < closest) {
					closestIndex = i;
					closest = dist;
					if (closest < 0.05) {
						break; // * close enough
					}
				}
			}
			return closestIndex;
		};

		/**
		 * @brief Applies simple time domain convolution
		 *
		 * @param hrtf HRTF to use
		 * @param hrtfVoice Convolution data for the voice
		 * @param frames Number of frames
		 * @param target Target buffer to mix into
		 * @param in Signal to convolve
		 * @param distanceAttenuated Volume applied
		 */
		static inline void apply(
			HRTF::Position& hrtf, VoiceHRTF& hrtfVoice, SampleIndex frames,
			ScratchBuffer& target, const Sample* in, Sample distanceAttenuated
		) {
			TKLB_PROFILER_SCOPE_NAMED("Apply HRTF")
			const Sample* irLeft   = hrtf.ir[0][0];
			const Sample* irRight  = hrtf.ir[1][0];
			const Size irLen    = hrtf.ir[0].size();
			Sample* convolutionBuffer = hrtfVoice.convolutionBuffer[0];
			Size convolutionIndex = hrtfVoice.convolutionIndex;
			// TODO bad brute force convolution in time domain
			for (SampleIndex i = 0; i < frames; i++) {
				Sample leftSum = 0;
				Sample rightSum = 0;
				convolutionBuffer[convolutionIndex] = in[i];

				for (SampleIndex n = 0; n < irLen; n++) {
					const Sample conv = convolutionBuffer[(irLen + convolutionIndex - n) % irLen];
					leftSum  += irLeft[n]  * conv;
					rightSum += irRight[n] * conv;
				}
				target[0][i] += leftSum  * distanceAttenuated;
				target[1][i] += rightSum * distanceAttenuated;
				convolutionIndex = (convolutionIndex + 1) % irLen;
			}
			hrtfVoice.convolutionIndex = convolutionIndex;
		}
	}; // HRTFUtil
} } // core::vae

#endif // _VAE_HRTF_UTIL
