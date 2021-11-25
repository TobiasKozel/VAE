# set(RTAUDIO_API_ASIO true)
set(RTAUDIO_BUILD_TESTING off)
set(RTAUDIO_API_JACK off)
set(RTAUDIO_API_ALSA off)

if (VAE_BUILD_STATIC_LIBS)
	set(RTAUDIO_BUILD_STATIC_LIBS ON)
	set(RTAUDIO_STATIC_MSVCRT OFF)
else()
	set(RTAUDIO_BUILD_STATIC_LIBS OFF)
	set(RTAUDIO_STATIC_MSVCRT OFF)
endif()



add_subdirectory(external/rtaudio)

set(RtAudio_FOUND true)

set(RtAudio_INCLUDE_DIRS
	external/rtaudio
	external/rtaudio/include
)
