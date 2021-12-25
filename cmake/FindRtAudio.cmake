# set(RTAUDIO_API_ASIO true)
set(RTAUDIO_BUILD_TESTING OFF)
set(RTAUDIO_API_JACK OFF)
set(RTAUDIO_API_ALSA OFF)

# we want to static link rt audio so there aren't any dll flying around
set(RTAUDIO_BUILD_STATIC_LIBS ON)
set(RTAUDIO_BUILD_SHARED_LIBS OFF)
set(RTAUDIO_STATIC_MSVCRT OFF) # TODO to make this static we need to set the main cmake build to be static as well

add_subdirectory(external/rtaudio)

set(RtAudio_INCLUDE_DIRS
	external/rtaudio/
	external/rtaudio/include/
)
