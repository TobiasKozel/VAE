#ifndef _VAE_HRTF_LOADER
#define _VAE_HRTF_LOADER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../pod/vae_hrtf.hpp"

#include <fstream>
#include "../../../external/json.hpp"
#include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"

namespace vae { namespace core {
	struct HRTFLoader {
		static Result load(const char* path, const char* rootPath, const Size sampleRate, HRTF& hrtf) {
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

			for (Size i = 0; i < positionCount; i++) {
				HRTF::Position& p = hrtf.positions[i];
				auto& pi = positions[i];
				p.pos.x = pi["x"];
				p.pos.y = pi["y"];
				p.pos.z = pi["z"];
				auto& left = pi["left"];
				auto& right = pi["right"];
				const Size irLength = left.size();
				longestIr = std::max(longestIr, irLength);

				if (needsResample) {
					// make sure the resampled ir fits later
					p.ir.resize(double(sampleRate) / double(hrtf.originalRate) * irLength + 10, 2);
				} else {
					p.ir.resize(longestIr, 2);
				}

				for (Size j = 0; j < irLength; j++) {
					p.ir[0][j] = left[j] / Sample(1 << 23);
					p.ir[1][j] = right[j] / Sample(1 << 23);
				}
				p.ir.setValidSize(irLength);
			}

			if (needsResample) {
				tklb::ResamplerTpl<Sample> resampler;
				AudioBuffer temp;
				temp.resize(longestIr, 2);
				for(auto& i : hrtf.positions) {
					resampler.init(hrtf.originalRate, sampleRate, longestIr);
					temp.set(i.ir);
					temp.sampleRate = hrtf.originalRate;
					i.ir.sampleRate = sampleRate;
					resampler.process(temp, i.ir);
				}
			}

			VAE_DEBUG("Finished loading HRTF %s", path)

			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_HRTF_LOADER
