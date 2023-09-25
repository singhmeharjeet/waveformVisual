cmake_minimum_required(VERSION 3.10)
project (waveform)

# SDL3 Flags
set(SDL_TESTS OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

add_subdirectory(SDL)


# TinyFileDialogs Flags
set(TFD_BUILD_STATIC ON)
add_subdirectory(tinyfiledialogs-cmake)

#Audio File Flags
set(BUILD_TESTS OFF)
set(BUILD_EXAMPLES OFF)
add_subdirectory(AudioFile)

#IMGUI
add_subdirectory(imgui)

add_library(vendor INTERFACE)
target_sources(vendor 
	INTERFACE 
		${CMAKE_CURRENT_SOURCE_DIR}/AudioFile/AudioFile.h
	)

target_include_directories(vendor 
	INTERFACE 
		${CMAKE_CURRENT_SOURCE_DIR}/tinyfiledialogs-cmake/src
	)
target_link_libraries(vendor 
	INTERFACE 
		SDL3::SDL3
	)