#ifndef _VAE_PROFILER
#define _VAE_PROFILER

const char* const _VAE_PROFILER_AUDIO = "Audio Frame";
const char* const _VAE_PROFILER_UPDATE = "Update Frame";


#ifdef VAE_USE_PROFILER
	#define TRACY_ENABLE
	#include "../../external/tracy/Tracy.hpp"
	#define VAE_PROFILER_FRAME_MARK_START(name)	FrameMarkStart(name)
	#define VAE_PROFILER_FRAME_MARK_END(name)	FrameMarkEnd(name)
	#define VAE_PROFILER_SCOPE		ZoneScoped
#else
	#define VAE_PROFILER_FRAME_MARK_START(name)		///< Starts a named frame, uses const defined above to maintain the same pointer
	#define VAE_PROFILER_FRAME_MARK_END(name)		///< Stops a named frame
	#define VAE_PROFILER_SCOPE						///< Profiles a scope
#endif // VAE_USE_PROFILER

#endif // _VAE_PROFILER
