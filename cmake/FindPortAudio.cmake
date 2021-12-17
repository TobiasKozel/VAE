enable_language(C)

if (BUILD_SHARED_LIBS)
	set(LINK_PRIVATE_SYMBOLS ON)
endif()

set(PA_USE_ASIO OFF)
add_subdirectory(external/portaudio)

set(PortAudio_FOUND true)
