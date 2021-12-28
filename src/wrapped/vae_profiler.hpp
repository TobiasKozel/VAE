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
	const char* const tklbAllocator			= "tklb Allocator";		///< Old tklb stuff, shouldn't be anything important in here
	const char* const objAllocator			= "new Allocator";		///< Generic object heap like vae::core::Device
	const char* const fsAllocator			= "FS Allocator";		///< Deals with everything  deserialization
	const char* const mainAllocator			= "Main Allocator";		///< Allocates all the bank, event emitter arrays
	const char* const dspAllocator			= "DSP allocator";		///< Tracks DSP memory usage
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
	#define TKLB_TRACK_ALLOCATE(ptr, size)		VAE_PROFILER_MALLOC_L(ptr, size, vae::core::profiler::tklbAllocator)
	#define TKLB_TRACK_FREE(ptr, size)			VAE_PROFILER_FREE_L(ptr, vae::core::profiler::tklbAllocator)

	#define VAE_PROFILER_OVERLOAD_NEW() 		\
	void* operator new(std::size_t count) {		\
		auto ptr = std::malloc(count);			\
		VAE_PROFILER_MALLOC_L(ptr, count,		\
			vae::core::profiler::objAllocator	\
		);										\
		return ptr;								\
	}											\
	void operator delete (void* ptr) noexcept {	\
		VAE_PROFILER_FREE_L(ptr,				\
			vae::core::profiler::objAllocator	\
		);										\
		std::free(ptr);							\
	}											\

	namespace vae { namespace core {
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
	#define VAE_PROFILER_FREE_L(ptr, name)			///< Track named allocaions
	#define VAE_PROFILER_MALLOC_L(ptr, size, name)	///< Track named allocaions
#endif // VAE_USE_PROFILER

#endif // _VAE_PROFILER
