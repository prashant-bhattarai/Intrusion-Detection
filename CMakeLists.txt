cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(WorkspaceIntrusion)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(IntrusionDetector src/intrusion_detector.cpp)

target_link_libraries(IntrusionDetector ${OpenCV_LIBS})