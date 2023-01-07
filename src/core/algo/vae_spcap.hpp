#ifndef _VAE_SPCAP
#define _VAE_SPCAP

#include "../vae_util.hpp"
#include "./vae_vec.hpp"
#include "../../wrapped/vae_tklb.hpp"

namespace vae { namespace core {
	/**
	 * @brief Struct containing SPCAP and preconfigured speaker setups
	 */
	namespace SPCAP {
		/**
		 * @brief Panning algorithm Implemented according to
		 * https://www.researchgate.net/publication/235080603_A_Novel_Multichannel_Panning_Method_for_Standard_and_Arbitrary_Loudspeaker_Configurations
		 * @tparam N SPeaker count
		 */
		template <Size N>
		class SPCAPConfig {
			struct Speaker {
				Vector3 dir;			///< Position of the speaker on a unit sphere
				Sample effectiveInv;	///< Inversed effective speaker count
			};
			Speaker mSpeakers[N];
		public:
			static constexpr Size speakers = N;

			/**
			 * @brief Calculate per channel volumes for a given direction
			 *
			 * @param direction The relative and normalized direction
			 * @param result Result array of channel volumes
			 * @param attenuation Distance attenuation multiplied on the result
			 * @param seperation 0 means no panning at all, while higher values increase the seperation of speakers. 1 is usually a good value
			 */
			constexpr void pan(const Vector3& direction, Sample result[N], Sample attenuation, Sample seperation) const {
				TKLB_PROFILER_SCOPE_NAMED("SPCAP Pan")
				// TODO make spread change based on distance and use something like radius instead
				Sample sumGains = 0.0;
				for (Size i = 0; i < N; i++) { result[i] = Sample(0); }
				for (Size i = 0; i < N; i++) {
					Sample gain = (Sample(1) + vector::dot(mSpeakers[i].dir, direction)); // (1)
					gain = tklb::pow(gain, seperation);		// (9)
					gain *= Sample(0.5);				// (1)
					gain *= mSpeakers[i].effectiveInv;	// (3)
					gain *= gain;						// (4)
					result[i] = gain;
					sumGains += gain;					// (4)
				}

				for (Size i = 0; i < N; i++) {
					result[i] = tklb::sqrt(result[i] / sumGains) * attenuation; // (8)
				}
			}

			/**
			 * @brief Adds a speaker to the configuration at a specific index
			 *
			 * @param speaker
			 * @param index
			 */
			constexpr SPCAPConfig& add(const Vector3& speaker, Size index) {
				TKLB_ASSERT(index < N)
				mSpeakers[index] = { vector::normalize(speaker), Sample(0) };
				return (*this);
			}

			/**
			 * @brief Normalizes the speaker positions and calculates the effective speaker value
			 */
			constexpr SPCAPConfig& prepare() {
				for (Size i = 0; i < N; i++) {
					for (Size j = 0; j < N; j++) {
						// Calculate the effective number of speakers according to (2)
						// Can be done in advance and ideally at compiletime
						// since the positions of the speakers will not change
						mSpeakers[i].effectiveInv +=
							Sample(0.5) * (Sample(1) + vector::dot(mSpeakers[i].dir, mSpeakers[j].dir));
					}
					mSpeakers[i].effectiveInv = Sample(1) / mSpeakers[i].effectiveInv;
				}
				return (*this);
			}
		};

		const auto MonoSPCAP = SPCAPConfig<1>()
			.add(StaticConfig::Speakers::center, 0)
			.prepare();

		const auto HeadphoneSPCAP = SPCAPConfig<2>()
			.add(StaticConfig::Speakers::left, 0).add(StaticConfig::Speakers::right, 1)
			.prepare();

		const auto StereroSPCAP = SPCAPConfig<2>()
			.add(StaticConfig::Speakers::frontLeft, 0).add(StaticConfig::Speakers::frontRight, 1)
			.prepare();

		const auto QuadSPCAP = SPCAPConfig<4>()
			.add(StaticConfig::Speakers::frontLeft, 0).add(StaticConfig::Speakers::frontRight, 1)
			.add(StaticConfig::Speakers::rearLeft, 2).add(StaticConfig::Speakers::rearRight, 3)
			.prepare();

		const auto SuroundSPCAP = SPCAPConfig<5>()
									.add(StaticConfig::Speakers::center, 4)
			.add(StaticConfig::Speakers::frontLeft, 0).add(StaticConfig::Speakers::frontRight, 1)
			.add(StaticConfig::Speakers::rearLeft, 2).add(StaticConfig::Speakers::rearRight, 3)
			.prepare();
	}; // SPCAP

	// #ifdef VAE_IMPL
	// #endif // VAE_IMPL
} } // core::vae

#endif // _VAE_SPCAP
