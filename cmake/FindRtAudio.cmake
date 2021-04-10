# set(RTAUDIO_API_ASIO true)
set(RTAUDIO_BUILD_TESTING false)
set(RTAUDIO_BUILD_SHARED_LIBS BUILD_SHARED_LIBS)
set(BUILD_SHARED_LIBS false)
add_subdirectory(external/rtaudio)
set(BUILD_SHARED_LIBS RTAUDIO_BUILD_SHARED_LIBS)

set(RtAudio_FOUND true)

set(RtAudio_INCLUDE_DIRS
	external/rtaudio
	external/rtaudio/include)

	# set(RtAudio_LIBRARIES
	# )
