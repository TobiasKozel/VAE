# set(RTAUDIO_API_ASIO true)
# set(RTAUDIO_BUILD_SHARED_LIBS BUILD_SHARED_LIBS)
# set(BUILD_SHARED_LIBS false)
# set(BUILD_SHARED_LIBS RTAUDIO_BUILD_SHARED_LIBS)

set(RTAUDIO_BUILD_TESTING false)
set(RTAUDIO_BUILD_STATIC_LIBS)
set(RTAUDIO_API_JACK false)
set(RTAUDIO_API_ALSA false)
add_subdirectory(external/rtaudio)
set(RtAudio_FOUND true)

set(RtAudio_INCLUDE_DIRS
	external/rtaudio
	external/rtaudio/include)

	# set(RtAudio_LIBRARIES
	# )
