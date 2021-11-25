enable_language(C)

# if (VAE_BUILD_STATIC_LIBS)
# 	set(PA_BUILD_SHARED OFF)
# 	set(PA_BUILD_STATIC ON)
# else()
# 	set(PA_BUILD_SHARED ON)
# 	set(PA_BUILD_STATIC OFF)
# endif()

set(PA_USE_ASIO OFF)
add_subdirectory(external/portaudio)

set(PortAudio_FOUND true)
