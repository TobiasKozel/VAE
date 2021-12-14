cmake_minimum_required(VERSION 3.10)

project(vae_example LANGUAGES CXX)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory("../../" "./build-vae")

add_executable(vae_example
	main.cpp
)

target_include_directories(vae_example
	PRIVATE
		vae
)

target_link_libraries(vae_example
	PRIVATE
		vae
)
