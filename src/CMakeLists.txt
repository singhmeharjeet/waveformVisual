project(waveform)
cmake_minimum_required(VERSION 3.13)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} 
	PRIVATE 
		${CMAKE_HOME_DIRECTORY}/include
		${CMAKE_HOME_DIRECTORY}/vendor/SDL/include
		"${CMAKE_HOME_DIRECTORY}/vendor/tinyfiledialogs-cmake/src"
		${CMAKE_HOME_DIRECTORY}/vendor/AudioFile
		${CMAKE_HOME_DIRECTORY}/vendor/imgui

)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
		vendor
		tfd
		imgui
)