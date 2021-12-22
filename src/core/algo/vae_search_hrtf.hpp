#ifndef _VAE_SEARCH_HRTF
#define _VAE_SEARCH_HRTF

#include "../vae_types.hpp"
#include "../pod/vae_hrtf.hpp"
#include <limits>

namespace vae { namespace core {
	struct SearchHRTF {
		/**
		 * @brief Looks for the best matching IR in a HRTF
		 * Does bad brute force search, should use kd tree or the mysodalib
		 * @param hrtf
		 * @param direction
		 * @return Size
		 */
		static Size closest(const HRTF& hrtf, const Vec3& direction) {
			// TODO this is obviously bad
			Sample closest = std::numeric_limits<Sample>::max();
			Size closestIndex = ~0;
			for (Size i = 0; i < hrtf.positions.size(); i++) {
				const auto& pos = hrtf.positions[i];
				const Sample dist = glm::distance(pos.pos, direction);
				if (dist < closest) {
					closestIndex = i;
					closest = dist;
					if (closest < 0.05) {
						break; // * good enough
					}
				}
			}
			return closestIndex;
		};
	};
} } // core::vae

#endif // _VAE_SEARCH_HRTF
