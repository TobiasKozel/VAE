cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0077 NEW) # overrides cmake options with predefined variables
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(
	vae
	VERSION 0.0.1
	DESCRIPTION "Virtual Audio Engine"
	LANGUAGES CXX
)

option(VAE_BUILD_STATIC_LIBS "Build static libs" ON)
option(VAE_PROFILER "Use profiler" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # needed for clang

# find_package will look for custom scripts in the cmake folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Need to define these before loading the packages
if (VAE_BUILD_STATIC_LIBS)
	message("Build static vae")
	set(TKLB_BUILD_STATIC_LIBS ON)
	set(BUILD_SHARED_LIBS OFF)
else()
	message("Build shared vae")
	set(TKLB_BUILD_STATIC_LIBS OFF)
	set(BUILD_SHARED_LIBS ON)
endif()

find_package(PortAudio REQUIRED)
# find_package(RtAudio REQUIRED)
find_package(tklb REQUIRED)

file(GLOB PUBLIC_API "src/api/*.cpp")
file(GLOB SRC_CORE "src/core/*.hpp")
file(GLOB SRC_WRAPPED "src/wrapped/*.cpp")
file(GLOB SRC_DEVICE "src/core/device/*.hpp")

if (VAE_BUILD_STATIC_LIBS)
	add_library(vae
		STATIC
			${SRC_CORE}
			${SRC_WRAPPED}
			${SRC_DEVICE}
			${PUBLIC_API}
	)
else()
	add_library(vae
		SHARED
			${SRC_CORE}
			${SRC_WRAPPED}
			${SRC_DEVICE}
			${PUBLIC_API}
	)
endif()
#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(VAE::vae ALIAS vae)

#Set target properties
target_include_directories(vae
	PUBLIC
		$<INSTALL_INTERFACE:include> # not relevant since I want to avoid the whole install thing?
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # the pimpl API
		${CMAKE_CURRENT_SOURCE_DIR}/src # temporarily
		tklb
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
		PortAudio
		# rtaudio
		tklb
)

source_group("api" FILES ${PUBLIC_API})
source_group("core" FILES ${SRC_CORE})

target_link_libraries(vae
# Goal is to have no public dependencies
	PRIVATE
		tklb
		PortAudio
		# rtaudio
)

# optional profiler
if(VAE_PROFILER)
	target_compile_definitions(
		vae
		PUBLIC
			VAE_USE_PROFILER
	)
	# add_compile_definitions(VAE_USE_PROFILER)
	message("Build with profiler")
endif()




# OPTIONAL
# create a single lib file from all the objects for prebuild dist
# ${lib_tool}
# libtool -static -o
if(MSVC)
	add_custom_target(
		vae_pack
		DEPENDS vae
		COMMAND
			${CMAKE_AR}
			/NOLOGO
			/Name:vae
			/LIBPATH:${CMAKE_CURRENT_BINARY_DIR}
			/OUT:${CMAKE_STATIC_LIBRARY_PREFIX}vae_pack${CMAKE_STATIC_LIBRARY_SUFFIX}
			$<TARGET_OBJECTS:vae>
			$<TARGET_OBJECTS:tklb>
			# $<TARGET_OBJECTS:PortAudio>
			$<TARGET_OBJECTS:PortAudio>
		COMMAND_EXPAND_LISTS
	)
else()
	add_custom_target(
		vae_pack
		DEPENDS vae
		COMMAND
			# r Don't warn when creating libfile
			# s Act like ranlib/create index
			# c replace
			${CMAKE_AR} rsc
			${CMAKE_STATIC_LIBRARY_PREFIX}vae_pack${CMAKE_STATIC_LIBRARY_SUFFIX}
			$<TARGET_OBJECTS:vae>
			$<TARGET_OBJECTS:tklb>
			# $<TARGET_OBJECTS:rtaudio>
			# ${ALSA_LIBRARY}
			# ${PULSE_LIB} ${PULSESIMPLE_LIB}
			$<TARGET_OBJECTS:PortAudio>
		COMMAND_EXPAND_LISTS # this is important to expand cmake lists properly
	)
endif()

add_subdirectory(dev)
