add_executable(vae_dev main.cpp)

target_include_directories(vae_dev PRIVATE vae)

target_link_libraries(vae_dev PRIVATE vae)
