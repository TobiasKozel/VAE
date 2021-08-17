# set(RTAUDIO_API_ASIO true)
set(RTAUDIO_BUILD_TESTING off)
set(RTAUDIO_API_JACK off)
set(RTAUDIO_API_ALSA off)

add_subdirectory(external/rtaudio)

set(RtAudio_FOUND true)

set(RtAudio_INCLUDE_DIRS
	external/rtaudio
	external/rtaudio/include
)
