cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0077 NEW) # overrides cmake options with predefined variables
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(
	vae
	VERSION 0.0.1
	DESCRIPTION "Virtual Audio Engine"
	LANGUAGES CXX
)

option(BUILD_SHARED_LIBS "Build shared lib" OFF) # TODO always build this and make it work in dev
option(VAE_BUILD_PIMPL "Build PIMPL API" OFF)
option(VAE_PROFILER "Use profiler" OFF)
option(VAE_BACKEND_PORTAUDIO "Compile with port audio" OFF)
option(VAE_BACKEND_RTAUDIO "Compile with RtAudio" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # needed for clang

# find_package will look for custom scripts in the cmake folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

#################            add main lib             #################

if (VAE_BUILD_PIMPL)
	file(GLOB PIMPL_API "src/api/*.cpp")
endif()
file(GLOB SRC_CORE "src/core/*.hpp")
file(GLOB SRC_WRAPPED "src/wrapped/*.cpp")
file(GLOB SRC_DEVICE "src/core/device/*.hpp")

if (BUILD_SHARED_LIBS)
	message("VAE: shared build")
	add_library(vae
		SHARED
			${SRC_CORE}
			${SRC_WRAPPED}
			${SRC_DEVICE}
			${PIMPL_API}
	)
	target_compile_definitions(
		vae
		PUBLIC
			VAE_DLL_EXPORT
	)
else()
	message("VAE: static build")
	add_library(vae
		STATIC
			${SRC_CORE}
			${SRC_WRAPPED}
			${SRC_DEVICE}
			${PIMPL_API}
	)
endif()

source_group("core" FILES ${SRC_CORE})

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(VAE::vae ALIAS vae)

#################            find third party libs             #################

find_package(tklb REQUIRED)

set(VAE_DEPS tklb)

if(VAE_BACKEND_RTAUDIO)
	find_package(RtAudio REQUIRED)
	list(APPEND VAE_DEPS rtaudio) # lower case smh
endif()
if(VAE_BACKEND_PORTAUDIO)
	find_package(PortAudio REQUIRED)
	list(APPEND VAE_DEPS PortAudio)
endif()

message(${CMAKE_CURRENT_LIST_DIR}/external/rtaudio/)

# Goal is to have no public dependencies
target_include_directories(vae
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # the pimpl API
	PRIVATERANDOM
		${VAE_DEPS}
)

target_link_libraries(vae PRIVATE ${VAE_DEPS})

# optional profiler
if(VAE_PROFILER)
	target_compile_definitions(
		vae
		PUBLIC
			VAE_USE_PROFILER
	)
	message("VAE: Build with profiler")
endif()

# for deveoloping
add_subdirectory(dev)
