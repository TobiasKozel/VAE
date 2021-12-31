#ifndef _VAE_HRTF_LOADER
#define _VAE_HRTF_LOADER

#include "../vae_types.hpp"
#include "../vae_util.hpp"
#include "../vae_config.hpp"
#include "../pod/vae_hrtf.hpp"

#include "../../wrapped/vae_fs.hpp"

#ifndef _JSON_H
	#include "../../../external/headeronly/json.h"
	#include "../../../external/headeronly/json.c"
#endif

#include "../../../external/glm/glm/gtc/matrix_transform.hpp"
// #include "../../../external/tklb/src/types/audio/resampler/TResampler.hpp"
// #include "../../../external/tklb/src/types/audio/fft/TOouraFFT.hpp"

namespace vae { namespace core {
	class HRTFLoader {
		static void* allocate(size_t size, int zero, void* context) {
			memory::AllocatorFS<char> allocator;
			void* ptr = reinterpret_cast<void*>(allocator.allocate(size));
			if (zero) {
				memset(ptr, 0, size);
			}
			return ptr;
		}

		static void deallocate(void* ptr, void* context) {
			memory::AllocatorFS<char> allocator;
			allocator.deallocate(reinterpret_cast<char*>(ptr), 0);
		}
	public:
		Result load(const char* path, Size length, const char* rootPath, const Size sampleRate, HRTF& hrtf) {
			VAE_PROFILER_SCOPE()

			/**
			 *					Open file and decode json
			 */
			const char* encoded = path;	// The plain json text

			String jsonText;
			if (length == 0) { // length 0 indicates the file is on disk
			#ifndef VAE_NO_STDIO
				VAE_DEBUG("Started loading HRTF %s", path)
				VAE_PROFILER_SCOPE_NAMED("HRTF IO")
				PathString folder;
				folder = rootPath;
				folder.append(path);

				fs::File file(folder.c_str());
				jsonText.resize(file.size());
				if (!file.readAll(jsonText.data())) {
					return Result::FileOpenError;
				}
				length = jsonText.size();
				encoded = jsonText.c_str();
			#else
				return Result::FileOpenError;
			#endif
			}




			json_settings settings = { };
			settings.mem_alloc = allocate;
			settings.mem_free = deallocate;

			json_value* json;
			{
				VAE_PROFILER_SCOPE_NAMED("HRTF Parse")
				json = json_parse_ex(&settings, encoded, length, 0);
			}
			if (json == nullptr) {
				VAE_ERROR("Failed to parse HRTF")
				return Result::BankFormatError;
			}
			json_value& data = (*json);

			hrtf.rate = sampleRate;
			hrtf.originalRate = (json_int_t) data["samplerate"];

			Vec3 up = {
				float((double) data["up"][0]),
				float((double) data["up"][1]),
				float((double) data["up"][2]),
			};

			Vec3 front = {
				float((double) data["front"][0]),
				float((double) data["front"][1]),
				float((double) data["front"][2]),
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

			auto& positions = data["positions"].u.array;
			const Size positionCount = positions.length;
			hrtf.positions.resize(positionCount);

			const bool needsResample = hrtf.originalRate != sampleRate;
			if (needsResample) {
				// TODO
				VAE_ERROR("Can't open HRTF, resampling needed!")
				return Result::GenericFailure;
			}

			Size maxIrLength = 0;
			{
				VAE_PROFILER_SCOPE_NAMED("HRTF Convert")
				for (Size i = 0; i < positionCount; i++) {
					HRTF::Position& p = hrtf.positions[i];
					auto& pi = *positions.values[i];
					glm::vec4 pos((double) pi["x"], (double)pi["y"], (double)pi["z"], 1.0);
					p.pos = matchCoord * pos;
					json_value irSamples[2] = { pi["left"], pi["right"]};
					const Size irLength = irSamples[0].u.array.length;
					maxIrLength = std::max(maxIrLength, irLength);
					for (int c = 0; c < 2; c++) {
						p.ir[c].resize(irLength, 1);
						for (Size j = 0; j < irLength; j++) {
							p.ir[c][0][j] = (double) *(irSamples[c].u.array.values[j]);
						}
					}
				}
			}

			hrtf.irLength = maxIrLength;

			json_value_free_ex(&settings, json);

			VAE_DEBUG("Finished loading HRTF %s", path)

			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_HRTF_LOADER
