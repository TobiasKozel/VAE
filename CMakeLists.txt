cmake_minimum_required(VERSION 3.10)

project(vae VERSION 0.0.1 DESCRIPTION "An Audio Engine" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find_package will look for custom scripts in the cmake folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(portaudio REQUIRED)
find_package(tklb REQUIRED)

file(GLOB PUBLIC_API "src/api/*.cpp")
file(GLOB SRC_CORE "src/core/*.hpp" )

# should be add_library later on
add_executable(vae
	${SRC_CORE}
	${PUBLIC_API}
	src/test/main.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
# add_library(VAE::vae ALIAS vae)

#Set target properties
target_include_directories(vae
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
		portaudio
		tklb
)

source_group("api" FILES ${PUBLIC_API})
source_group("core" FILES ${SRC_CORE})

# if(MSVC)
# 	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build/$<0:>)
# 	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build/$<0:>)
# 	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/$<0:>)
# 	set(CMAKE_SUPPRESS_REGENERATION true) # gets rid of some of the cmake junk
# 	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT dof_example)
# 	# set visual studio debugger working directory
# 	# set_target_properties(dof_example PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "build/")

# else()
# 	# add_definitions(-DASSET_PATH="${CMAKE_CURRENT_SOURCE_DIR}/build/")
# endif()

target_link_libraries(vae
	# PUBLIC
	# 	Boost::headers RapidJSON::RapidJSON
	PRIVATE
		portaudio
		tklb
)
