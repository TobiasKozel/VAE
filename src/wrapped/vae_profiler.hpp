#ifndef _VAE_PROFILER
#define _VAE_PROFILER

namespace vae { namespace core { namespace profiler {
	const char* const audioFrame = "Audio Frame";
	const char* const updateFrame = "Update Frame";
	const char* const voiceCount = "Voices";
	const char* const voiceFinishedCount = "Voices Finished Queue";
	const char* const voiceVirtualCount = "Virtual voices";
	const char* const voiceHRTFCount = "HRTF voices";
	const char* const emitters = "Emitters";
	const char* const deviceUnderruns = "Device Underruns";
	const char* const deviceOverruns = "Device Overruns";
	const char* const engineUnderruns = "Engine Underruns";
	const char* const engineOverruns = "Engine Overruns";
} } } // vae::core::profiler


#ifdef VAE_USE_PROFILER
	#define TRACY_ENABLE
	#include "../../external/tracy/Tracy.hpp"
	#define VAE_PROFILER_FRAME_MARK				FrameMark
	#define VAE_PROFILER_FRAME_MARK_START(name)	FrameMarkStart(name)
	#define VAE_PROFILER_FRAME_MARK_END(name)	FrameMarkEnd(name)
	#define VAE_PROFILER_SCOPE					ZoneScoped
	#define VAE_PROFILER_SCOPE_NAMED(name)		ZoneScopedN(name)
	#define VAE_PROFILER_PLOT(name, value)		TracyPlot(name, value)
	#define VEA_PROFILER_THREAD_NAME(name)		tracy::SetThreadName(name);
	#define VAE_PROFILER_MESSAGE_L(msg)			TracyMessageL(msg)
	#define VAE_PROFILER_MALLOC(ptr, size)		TracyAlloc(ptr, size);
	#define VAE_PROFILER_FREE(ptr)				TracyFree(ptr);

	// Add tracking to tklb
	// #define TKLB_TRACK_ALLOCATE(ptr, size)		VAE_PROFILER_MALLOC(ptr, size)
	// #define TKLB_TRACK_FREE(ptr, size)			VAE_PROFILER_FREE(ptr)

	#include <cstddef>
	#include <memory>
	#include <cstdlib>
	#include <new>
	#include <limits>


	namespace vae { namespace core { namespace profiler {


		template <class T>
		struct Allocator {
			typedef T value_type;

			Allocator () = default;
			template <class U> constexpr Allocator (const Allocator <U>&) noexcept { }

			T* allocate(std::size_t n) {
				if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
					throw std::bad_array_new_length();
				}

				if (auto ptr = static_cast<T*>(std::malloc(n * sizeof(T)))) {
					VAE_PROFILER_MALLOC(ptr, n * sizeof(T))
					return ptr;
				}

				throw std::bad_alloc();
			}

			void deallocate(T* ptr, std::size_t n) noexcept {
				VAE_PROFILER_FREE(ptr)
				std::free(ptr);
			}

		};
	} } } // vae::core::profiler
#else
	#define VAE_FRAME_MARK
	#define VAE_PROFILER_FRAME_MARK_START(name)		///< Starts a named frame, uses const defined above to maintain the same pointer
	#define VAE_PROFILER_FRAME_MARK_END(name)		///< Stops a named frame
	#define VAE_PROFILER_SCOPE						///< Profiles a scope
	#define VAE_PROFILER_SCOPE_NAMED(name)			///< Profiles a scope and names it
	#define VAE_PROFILER_PLOT(name, value)			///< Records a value
	#define VEA_PROFILER_THREAD_NAME(name)			///< Sets name for current thread
	#define VAE_PROFILER_MESSAGE_L					///< Send message to profiler
	#define VAE_PROFILER_MALLOC(ptr, size)			///< Track allocation
	#define VAE_PROFILER_FREE(ptr)					///< Track free
#endif // VAE_USE_PROFILER

#endif // _VAE_PROFILER
