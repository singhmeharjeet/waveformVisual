cmake_minimum_required(VERSION 3.10)
project (waveform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(vendor)
add_subdirectory(include)
add_subdirectory(src)