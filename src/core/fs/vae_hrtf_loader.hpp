#ifndef _VAE_HRTF_LOADER
#define _VAE_HRTF_LOADER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../pod/vae_hrtf.hpp"

#include <fstream>
#include "../../../external/headeronly/json.hpp"
#include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"
#include "../../../external/tklb/src/types/audio/fft/TOouraFFT.hpp"

namespace vae { namespace core {
	struct HRTFLoader {
		static Result load(const char* path, const char* rootPath, const Size sampleRate, HRTF& hrtf) {
			VAE_PROFILER_SCOPE
			std::string json = path;
			json = rootPath + json;
			std::ifstream file(json);

			if (!file.is_open()) {
				VAE_ERROR("Failed to load HRTF %s%s", rootPath, path)
				return Result::FileOpenError;
			}

			VAE_DEBUG("Started loading HRTF %s", path)
			auto data = nlohmann::json::parse(file);

			hrtf.originalRate = data["samplerate"];
			hrtf.rate = sampleRate;

			auto& positions = data["positions"];
			const Size positionCount = positions.size();
			hrtf.positions.resize(positionCount);

			Size longestIr = 0;
			const bool needsResample = hrtf.originalRate != sampleRate;

			AudioBuffer timeDomain;				// temporary time domain signal
			timeDomain.sampleRate = hrtf.originalRate;
			AudioBuffer timeDomainResampled;	// temporary time domain signal
			timeDomainResampled.sampleRate = sampleRate;
			tklb::ResamplerTpl<Sample> resampler;
			tklb::FFTOoura fft;

			for (Size i = 0; i < positionCount; i++) {
				HRTF::Position& p = hrtf.positions[i];
				auto& pi = positions[i];
				p.pos.x = pi["x"];
				p.pos.y = pi["y"];
				p.pos.z = pi["z"];
				nlohmann::json irSamples[2] = { pi["left"], pi["right"]};
				const Size irLength = irSamples[0].size();
				longestIr = std::max(longestIr, irLength);

				if (needsResample) {
					Size needed = double(sampleRate) / double(hrtf.originalRate) * irLength + 10;
					if (timeDomainResampled.size() < needed) {
						timeDomainResampled.resize(needed);
					}
				}
				timeDomain.resize(irLength);

				for (int c = 0; c < 2; c++) {
					timeDomain.set(0);
					timeDomain[0][30] = 1.0;
					// for (Size j = 0; j < irLength; j++) {
					// 	timeDomain[0][j] = int(irSamples[c][j]) / Sample(1 << 23);
					// }
					timeDomain.setValidSize(irLength);

					if (needsResample) {
						resampler.init(hrtf.originalRate, sampleRate, longestIr);
						resampler.process(timeDomain, timeDomainResampled);
						fft.resize(timeDomainResampled.size());
						p.ir[c].resize(timeDomainResampled.size() / 2, 2);
						fft.forward(timeDomainResampled, p.ir[c]);
					} else {
						fft.resize(timeDomain.size());
						p.ir[c].resize(timeDomain.size() / 2, 2);
						fft.forward(timeDomain, p.ir[c]);
					}
				}

			}

			VAE_DEBUG("Finished loading HRTF %s", path)

			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_HRTF_LOADER
