#ifndef _VAE_SPCAP
#define _VAE_SPCAP

#include "../vae_types.hpp"
#include "../vae_util.hpp"

namespace vae { namespace core {
	/**
	 * @brief Struct providing SPCAP configurations hard coded
	 */
	struct SPCAP {
		template <int N>
		class SPCAPConfig {
			struct Speaker {
				Vec3 dir;
				Sample effective;
			};
			Speaker mSpeakers[N];
		public:
			SPCAPConfig(const std::initializer_list<Vec3>& directions) {
				int i = 0;
				for (const auto& speaker : directions) {
					mSpeakers[i] = { speaker, Sample(0) };
					i++;
				}
				for (i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						mSpeakers[i].effective +=
							0.5 * (1.0 + glm::dot(mSpeakers[i].dir, mSpeakers[j].dir));
					}
				}
			}

			constexpr int speakers() const { return N; }

			void pan(const Vec3& source, Sample* result) const {
				Sample sumSquaredGains = 0.0;
				const Sample tightness = 4.0;
				std::fill_n(result, N, 0);
				for (int i = 0; i < N; i++) {
					Sample gain = 0.5 * powf(1.0 + glm::dot(mSpeakers[i].dir, source), tightness);
					gain /= mSpeakers[i].effective;
					gain = gain * gain;
					result[i] = gain;
					sumSquaredGains += gain;
				}

				for (int i = 0; i < N; i++) {
					result[i] = sqrtf(result[i] / sumSquaredGains);
				}
			}
		};
		// Initialized below because c++
		static const SPCAPConfig<2> HeadphoneSPCAP;
		static const SPCAPConfig<2> StereroSPCAP;
		static const SPCAPConfig<4> QuadSPCAP;
		static const SPCAPConfig<5> SuroundSPCAP;
	}; // SPCAP

	const SPCAP::SPCAPConfig<2> SPCAP::HeadphoneSPCAP = {
		{ -1, 0, 0}, { +1, 0, 0}		// LR Side
	};
	const SPCAP::SPCAPConfig<2> SPCAP::StereroSPCAP = {
		{ -1, +1, 0}, { +1, +1, 0}		// LR Front
	};
	const SPCAP::SPCAPConfig<4> SPCAP::QuadSPCAP = {
		{ -1, +1, 0}, { +1, +1, 0},		// LR Front
		{ -1, -1, 0}, { +1, -1, 0}		// LR Back
	};
	const SPCAP::SPCAPConfig<5> SPCAP::SuroundSPCAP = {
		{ -1, +1, 0}, { +1, +1, 0},		// LR Front
		{ -1, -1, 0}, { +1, -1, 0},		// LR Back
		{  0, +1, 0}					// Center
	};
} } // core::vae

#endif // _VAE_SPCAP
