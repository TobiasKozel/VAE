#ifndef VAEX_PROFILER
#define VAEX_PROFILER

#ifdef VAE_USE_PROFILER
	#define TRACY_ENABLE
	#include "../../external/tracy/Tracy.hpp"
	#define VAE_PROFILER_FRAME_MARK	FrameMark
	#define VAE_PROFILER_SCOPE		ZoneScoped
#else
	#define VAE_PROFILER_FRAME_MARK
	#define VAE_PROFILER_SCOPE
#endif // VAE_USE_PROFILER

#endif // VAEX_PROFILER
