cmake_minimum_required(VERSION 3.8)

project(
	vae
	VERSION 0.0.1
	DESCRIPTION "An Audio Engine"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find_package will look for custom scripts in the cmake folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(portaudio REQUIRED)
find_package(tklb REQUIRED)

file(GLOB PUBLIC_API "src/api/*.cpp")
file(GLOB SRC_CORE "src/core/*.hpp" )

add_library(vae
	${SRC_CORE}
	${PUBLIC_API}
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(VAE::vae ALIAS vae)

#Set target properties
target_include_directories(vae
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
		portaudio_static
		tklb
)

source_group("api" FILES ${PUBLIC_API})
source_group("core" FILES ${SRC_CORE})

target_link_libraries(vae
# Goal is to have no public dependencies
	PRIVATE
		tklb
		portaudio_static
)

# OPTIONAL
# create a single lib file from all the objects
# ${lib_tool}
# libtool -static -o

if(MSVC)
	set(VAE_ARCHIVE_COMMAND )
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
			$<TARGET_OBJECTS:portaudio_static>
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
			$<TARGET_OBJECTS:portaudio_static>
		COMMAND_EXPAND_LISTS # this is important to expand cmake lists properly
	)
endif()
