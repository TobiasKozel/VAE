#ifndef _VAE_SPCAP
#define _VAE_SPCAP

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "./vae_vec.hpp"
#include <array>			// used for constructor parameter, easily replaceable

namespace vae { namespace core {
	/**
	 * @brief Struct containing SPCAP and preconfigured speaker setups
	 */
	struct SPCAP {
		/**
		 * @brief Class handling SPCAP for arbitrary speaker setups
		 * @tparam N Amount of speakers. Does not include LFE.
		 */
		template <Size N>
		class SPCAPConfig {
			struct Speaker {
				vector::Vec3 dir;	///< Position of the speaker on a unit sphere
				Sample effective;	//
			};
			Speaker mSpeakers[N];
		public:
			/**
			 * @brief Construct a new SPCAPConfig object
			 * Implemented according to
			 * https://www.researchgate.net/publication/235080603_A_Novel_Multichannel_Panning_Method_for_Standard_and_Arbitrary_Loudspeaker_Configurations
			 * @param positions Speaker positions, will be normalized.
			 */
			constexpr SPCAPConfig(const std::array<Vector3, N>&& positions) {
				Size i = 0;
				for (const auto& speaker : positions) {
					mSpeakers[i] = { vector::normalize(speaker), Sample(0) };
					i++;
				}
				for (i = 0; i < N; i++) {
					for (Size j = 0; j < N; j++) {
						// Calculate the effective number of speakers according to (2)
						// Can be done in advance and ideally at compiletime
						// since the positions of the speakers will not change
						mSpeakers[i].effective +=
							Sample(0.5) * (Sample(1.0) + vector::dot(mSpeakers[i].dir, mSpeakers[j].dir));
					}
				}
			}

			static constexpr Size speakers = N;


			/**
			 * @brief Calculate per channel volumes for a given direction
			 *
			 * @param direction The relative and normalized direction
			 * @param result Result array of channel volumes
			 * @param attenuation Distance attenuation multiplied on the result
			 * @param spread Value from 0-1 controlling "wideness" of the sound
			 */
			inline void pan(const vector::Vec3& direction, Sample result[N], Sample attenuation, Sample spread) const {
				TKLB_PROFILER_SCOPE_NAMED("SPCAP Pan")
				// TODO make spread change based on distance and use something like radius instead
				Sample sumGains = 0.0;
				// const Sample tightness = (Sample(1) - spread) * Sample(10) + Sample(0.05);
				const Sample tightness = 1.0;
				for (Size i = 0; i < N; i++) {
					result[i] = Sample(0);
				}
				for (Size i = 0; i < N; i++) {
					Sample gain = vector::dot(mSpeakers[i].dir, direction) + Sample(1.0); // (1)
					gain  = powf(gain, tightness);	// (9)
					gain *= Sample(0.5);			// (1)
					gain /= mSpeakers[i].effective;	// (3)
					gain  = gain * gain;			// (4)
					result[i] = gain;
					sumGains += gain;				// (4)
				}

				for (Size i = 0; i < N; i++) {
					result[i] = tklb::sqrt(result[i] / sumGains) * attenuation; // (8)
				}
			}
		};

		// Initialized below because c++ 14
		static const SPCAPConfig<1> MonoSPCAP;
		static const SPCAPConfig<2> HeadphoneSPCAP;
		static const SPCAPConfig<2> StereroSPCAP;
		static const SPCAPConfig<4> QuadSPCAP;
		static const SPCAPConfig<5> SuroundSPCAP;
	}; // SPCAP

	/**
	 * TODO there's probably a smart  way to make this all constexpr
	 */

	#ifdef VAE_IMPL
		const SPCAP::SPCAPConfig<1> SPCAP::MonoSPCAP = {{
							StaticConfig::Speakers::center
		}};
		const SPCAP::SPCAPConfig<2> SPCAP::HeadphoneSPCAP = {{
			StaticConfig::Speakers::left, StaticConfig::Speakers::right
		}};
		const SPCAP::SPCAPConfig<2> SPCAP::StereroSPCAP = {{
			StaticConfig::Speakers::frontLeft, StaticConfig::Speakers::frontRight
		}};
		const SPCAP::SPCAPConfig<4> SPCAP::QuadSPCAP = {{
			StaticConfig::Speakers::frontLeft, StaticConfig::Speakers::frontRight,
			StaticConfig::Speakers::rearLeft,  StaticConfig::Speakers::rearRight
		}};
		const SPCAP::SPCAPConfig<5> SPCAP::SuroundSPCAP = {{
			StaticConfig::Speakers::frontLeft, StaticConfig::Speakers::frontRight,
			StaticConfig::Speakers::rearLeft,  StaticConfig::Speakers::rearRight,
							StaticConfig::Speakers::center
		}};
	#endif // VAE_IMPL
} } // core::vae

#endif // _VAE_SPCAP
