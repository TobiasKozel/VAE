# set(RTAUDIO_API_ASIO true)

set(RTAUDIO_BUILD_TESTING OFF)
set(RTAUDIO_API_JACK OFF)
set(RTAUDIO_API_ALSA OFF)
add_subdirectory(external/rtaudio)
set(RtAudio_FOUND true)

set(RtAudio_INCLUDE_DIRS
	external/rtaudio
	external/rtaudio/include
)
