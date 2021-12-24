#ifndef _VAE_PROFILER
#define _VAE_PROFILER

namespace vae { namespace core { namespace profiler {
	const char* const audioFrame			= "Audio Frame";
	const char* const updateFrame			= "Update Frame";
	const char* const voiceCount			= "Voices";
	const char* const voiceFinishedCount	= "Voices Finished Queue";
	const char* const voiceVirtualCount		= "Virtual voices";
	const char* const voiceHRTFCount		= "HRTF voices";
	const char* const emitters				= "Emitters";
	const char* const deviceUnderruns		= "Device Underruns";
	const char* const deviceOverruns		= "Device Overruns";
	const char* const engineUnderruns		= "Engine Underruns";
	const char* const engineOverruns		= "Engine Overruns";
	const char* const tklbHeap				= "tklb heap";			///< Contains all tklb heap like tklb::AudioBuffer or tkbl::Resampler
	const char* const vaeHeap				= "vae heapbuffers";	///< All heapbuffers like voice pools and so on
	const char* const objHeap				= "global new heap";	///< Generic object heap like vae::core::Device
} } } // vae::core::profiler


#ifdef VAE_USE_PROFILER
	#define TRACY_ENABLE
	#include "../../external/tracy/Tracy.hpp"
	#define VAE_PROFILER_FRAME_MARK()				FrameMark
	#define VAE_PROFILER_FRAME_MARK_START(name)		FrameMarkStart(name)
	#define VAE_PROFILER_FRAME_MARK_END(name)		FrameMarkEnd(name)
	#define VAE_PROFILER_SCOPE()					ZoneScoped
	#define VAE_PROFILER_SCOPE_NAMED(name)			ZoneScopedN(name)
	#define VAE_PROFILER_PLOT(name, value)			TracyPlot(name, value)
	#define VEA_PROFILER_THREAD_NAME(name)			tracy::SetThreadName(name);
	#define VAE_PROFILER_MESSAGE_L(msg)				TracyMessageL(msg)
	#define VAE_PROFILER_MALLOC(ptr, size)			TracyAlloc(ptr, size);
	#define VAE_PROFILER_FREE(ptr)					TracyFree(ptr);
	#define VAE_PROFILER_MALLOC_L(ptr, size, name)	TracyAllocN(ptr, size, name);
	#define VAE_PROFILER_FREE_L(ptr, name)			TracyFreeN(ptr, name);
	#define VAE_PROFILER_MUTEX(type, name, desc)	TracyLockableN(type, name, desc)

	// Add tracking to tklb which is mostly audiobuffers
	#define TKLB_TRACK_ALLOCATE(ptr, size)		VAE_PROFILER_MALLOC_L(ptr, size, vae::core::profiler::tklbHeap)
	#define TKLB_TRACK_FREE(ptr, size)			VAE_PROFILER_FREE_L(ptr, vae::core::profiler::tklbHeap)

	#define VAE_PROFILER_OVERLOAD_NEW() 		\
	void* operator new(std::size_t count) {		\
		auto ptr = std::malloc(count);			\
		VAE_PROFILER_MALLOC_L(ptr, count,		\
			vae::core::profiler::objHeap		\
		);										\
		return ptr;								\
	}											\
	void operator delete (void* ptr) noexcept {	\
		VAE_PROFILER_FREE_L(ptr,				\
			vae::core::profiler::objHeap		\
		);										\
		std::free(ptr);							\
	}											\

	#include <cstdlib>
	#include <cstddef>
	#include <new>
	#include <cstddef>
	#include <memory>
	#include <limits>

	// #include "../../external/tklb/src/types/TSpinLock.hpp"
	#include <mutex>

	namespace vae { namespace core { namespace profiler {
		/**
		 * @brief Allocator used for all heapbuffers in VAE
		 * @tparam T
		 */
		template <class T, class NAME>
		struct Allocator {
			typedef T value_type;

			Allocator () = default;
			// template <class U, const char* N> constexpr Allocator (const Allocator<U><N>&) noexcept { }

			T* allocate(std::size_t n) {
				if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
					throw std::bad_array_new_length();
				}

				if (auto ptr = static_cast<T*>(std::malloc(n * sizeof(T)))) {
					VAE_PROFILER_MALLOC_L(ptr, n * sizeof(T), NAME::name)
					return ptr;
				}

				throw std::bad_alloc();
			}

			void deallocate(T* ptr, std::size_t n) noexcept {
				VAE_PROFILER_FREE_L(ptr, NAME::name)
				std::free(ptr);
			}
		};
		struct FsAllocator   { static constexpr const char* name = "FS Allocator";   };
		struct MainAllocator { static constexpr const char* name = "Main Allocator"; };
	} // profiler
		template <class T> using AllocatorFS   = profiler::Allocator<T, profiler::FsAllocator>;
		template <class T> using AllocatorMain = profiler::Allocator<T, profiler::MainAllocator>;
		using Mutex = std::mutex;
		using Lock = std::lock_guard<LockableBase(Mutex)>;
	} } // vae::core

#else
	#define VAE_PROFILER_FRAME_MARK()				///< Marks a frame to calculate FPS, not really needed for audio
	#define VAE_PROFILER_FRAME_MARK_START(name)		///< Starts a named frame, uses const defined above to maintain the same pointer
	#define VAE_PROFILER_FRAME_MARK_END(name)		///< Stops a named frame
	#define VAE_PROFILER_SCOPE()					///< Profiles a scope
	#define VAE_PROFILER_SCOPE_NAMED(name)			///< Profiles a scope and names it
	#define VAE_PROFILER_PLOT(name, value)			///< Records a value
	#define VEA_PROFILER_THREAD_NAME(name)			///< Sets name for current thread
	#define VAE_PROFILER_MESSAGE_L(msg)				///< Send message to profiler
	#define VAE_PROFILER_MALLOC(ptr, size)			///< Track allocation
	#define VAE_PROFILER_FREE(ptr)					///< Track free
	#define VAE_PROFILER_OVERLOAD_NEW()				///< Overloads new and delete of class to be tracked
	#define VAE_PROFILER_MUTEX(type, name, desc) type name;
#endif // VAE_USE_PROFILER

#endif // _VAE_PROFILER
