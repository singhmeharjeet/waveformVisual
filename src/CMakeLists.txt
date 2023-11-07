project(waveform)
cmake_minimum_required(VERSION 3.13)

add_executable(${PROJECT_NAME})


target_include_directories(${PROJECT_NAME}
	PUBLIC
	"${CMAKE_HOME_DIRECTORY}/include"
	"${CMAKE_HOME_DIRECTORY}/vendor/SDL/include"
	"${CMAKE_HOME_DIRECTORY}/vendor/tinyfiledialogs-cmake/src"
	"${CMAKE_HOME_DIRECTORY}/vendor/imgui"
	"${CMAKE_HOME_DIRECTORY}/vendor/imgui/backends"
	"${CMAKE_HOME_DIRECTORY}/vendor/tinyfiledialogs-cmake/include"
)

target_sources(${PROJECT_NAME} 
	PRIVATE 
		main.cpp 
		myfont.cpp
)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
		vendor
)