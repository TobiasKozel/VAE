cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0077 NEW) # overrides cmake options with predefined variables
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(
	vae
	VERSION 0.0.1
	DESCRIPTION "Virtual Audio Engine"
	LANGUAGES CXX
)

option(VAE_BUILD_SHARED_LIBS "Build shared lib" OFF)	# if this is on vae_dev doesn't compile :/
option(VAE_PROFILER "Use profiler" OFF)
option(VAE_BACKEND_PORTAUDIO "Compile with port audio" OFF)
option(VAE_BACKEND_RTAUDIO "Compile with RtAudio" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # needed for clang

# find_package will look for custom scripts in the cmake folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

#################            add main lib             #################

file(GLOB PIMPL_API "src/api/vae_pimpl.cpp")
file(GLOB C_API "src/api/vae_c_api.cpp")
file(GLOB SRC_CORE "src/core/*.hpp")
file(GLOB SRC_WRAPPED "src/wrapped/*.cpp")
file(GLOB SRC_DEVICE "src/core/device/*.hpp")

set(VAE_DEPS tklb)	# Things to link against
set(VAE_DEFINES)	# Preprocessor info about stuff being built


if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	list(APPEND VAE_DEFINES "VAE_RELEASE")
	if (NOT MSVC)
		# add_compile_options(-march=native -mtune=native -Ofast)
	endif()
endif()

function(VAE_ADD_SHARED)
	message("VAE: shared build")
	add_library(vae
		SHARED
			${SRC_CORE}
			${SRC_WRAPPED}
			${SRC_DEVICE}
	)
	list(APPEND VAE_DEFINES "VAE_DLL_EXPORT")
endfunction()

function(VAE_ADD_STATIC)
	message("VAE: static build")
	add_library(vae
		STATIC
			${SRC_CORE}
			${SRC_WRAPPED}
			${SRC_DEVICE}
	)
endfunction()

if(DEFINED VAE_BUILD_SHARED_LIBS)
	if (VAE_BUILD_SHARED_LIBS)
		VAE_ADD_SHARED()
	else()
		VAE_ADD_STATIC()
	endif()
else()
	# standart cmake behavior
	if (BUILD_SHARED_LIBS)
		VAE_ADD_SHARED()
	else()
		VAE_ADD_STATIC()
	endif()
endif()

source_group("core" FILES ${SRC_CORE})

# TODO there are a lot of warnings to take care of
# if (MSVC)
# 	# warning level 4 and all warnings as errors
# 	add_compile_options(/W4 /WX)
# else()
# 	# lots of warnings and all warnings as errors
# 	add_compile_options(-Wall -Wextra -pedantic -Werror)
# endif()



#Add an alias so that library can be used inside the build tree, e.g. when testing
# add_library(VAE::vae ALIAS vae)

#################            find third party libs             #################

find_package(tklb REQUIRED)



set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
list(APPEND VAE_DEPS Threads::Threads)

if(VAE_BACKEND_RTAUDIO)
	find_package(RtAudio REQUIRED)

	list(APPEND VAE_DEFINES "_VAE_HAS_RT_AUDIO")
	message("VAE: Build with RtAudio")
	list(APPEND VAE_DEPS rtaudio) # lower case smh
endif()
if(VAE_BACKEND_PORTAUDIO)
	list(APPEND VAE_DEFINES "_VAE_HAS_PORT_AUDIO")
	message("VAE: Build with portaudio")
	find_package(PortAudio REQUIRED)
	list(APPEND VAE_DEPS PortAudio)
endif()

# Goal is to have no public dependencies
target_include_directories(vae
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # the pimpl API
	PRIVATE
		${VAE_DEPS}
)

# optional profiler
if(VAE_PROFILER)
	list(APPEND VAE_DEPS ${CMAKE_DL_LIBS})
	list(APPEND VAE_DEFINES "VAE_USE_PROFILER")
	message("VAE: Build with profiler")
endif()

# if (NOT MSVC)
# 	if (VAE_PROFILER)
# 		add_compile_options(-finstrument-functions)
# 		message("VAE: instrument functions")
# 	endif()
# endif()

target_compile_definitions(
	vae
	PUBLIC
		${VAE_DEFINES}
)
target_link_libraries(vae PRIVATE ${VAE_DEPS})


# for developing
add_subdirectory(dev)

add_library(vae_pimpl
	SHARED
		${SRC_CORE}
		${SRC_WRAPPED}
		${SRC_DEVICE}
		${PIMPL_API}
)
target_include_directories(vae_pimpl PRIVATE vae)
target_link_libraries(vae_pimpl PRIVATE vae)

add_library(vae_c_api
	SHARED
		${SRC_CORE}
		${SRC_WRAPPED}
		${SRC_DEVICE}
		${C_API}
)
target_include_directories(vae_c_api PRIVATE vae)
target_link_libraries(vae_c_api PRIVATE vae)


