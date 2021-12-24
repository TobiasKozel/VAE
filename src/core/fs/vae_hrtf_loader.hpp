#ifndef _VAE_HRTF_LOADER
#define _VAE_HRTF_LOADER

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../vae_config.hpp"
#include "../pod/vae_hrtf.hpp"

#include <fstream>
#include "../../../external/headeronly/json.hpp"
#include "../../../external/glm/glm/gtc/matrix_transform.hpp"
// #include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"
// #include "../../../external/tklb/src/types/audio/fft/TOouraFFT.hpp"

namespace vae { namespace core {
	struct HRTFLoader {
		Result load(const char* path, const char* rootPath, const Size sampleRate, HRTF& hrtf) {
			VAE_PROFILER_SCOPE()
			PathString json = path;
			json = rootPath + json;
			std::ifstream file(json);

			if (!file.is_open()) {
				VAE_ERROR("Failed to load HRTF %s%s", rootPath, path)
				return Result::FileOpenError;
			}

			VAE_DEBUG("Started loading HRTF %s", path)
			auto data = nlohmann::basic_json<
				std::map, std::vector, std::string, bool, int, Size, Sample, AllocatorFS
			>::parse(file);
			hrtf.originalRate = data["samplerate"];
			hrtf.rate = sampleRate;

			auto& positions = data["positions"];
			const Size positionCount = positions.size();
			hrtf.positions.resize(positionCount);

			Vec3 up = {
				float(data["up"][0]),
				float(data["up"][1]),
				float(data["up"][2]),
			};

			Vec3 front = {
				float(data["front"][0]),
				float(data["front"][1]),
				float(data["front"][2]),
			};

			LocationOrientation ref;
			Vec3 frontNeed = Vec3(ref.front.x, ref.front.y, ref.front.z);
			Vec3 upNeed = Vec3(ref.up.x, ref.up.y, ref.up.z);

			glm::mat4x4 matchCoord = glm::lookAt(
				Vec3(0.f, 0.f, 0.f),
				front,
				up
			);

			Vec3 up1 = (matchCoord * glm::vec4(up, 1.f));
			Vec3 front1 = (matchCoord * glm::vec4(front, 1.f));
			// These should match upNeed

			const bool needsResample = hrtf.originalRate != sampleRate;
			if (needsResample) {
				// TODO
				VAE_ERROR("Can't open HRTF, resampling needed!")
				return Result::GenericFailure;
			}

			Size maxIrLength = 0;

			for (Size i = 0; i < positionCount; i++) {
				HRTF::Position& p = hrtf.positions[i];
				auto& pi = positions[i];
				p.pos = (matchCoord * glm::vec4(pi["x"], pi["y"], pi["z"], 1.f));
				nlohmann::json irSamples[2] = { pi["left"], pi["right"]};
				const Size irLength = irSamples[0].size();
				maxIrLength = std::max(maxIrLength, irLength);
				for (int c = 0; c < 2; c++) {
					p.ir[c].resize(irLength, 1);
					for (Size j = 0; j < irLength; j++) {
						p.ir[c][0][j] = irSamples[c][j];
					}
				}
			}

			hrtf.irLength = maxIrLength;

			VAE_DEBUG("Finished loading HRTF %s", path)

			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_HRTF_LOADER
