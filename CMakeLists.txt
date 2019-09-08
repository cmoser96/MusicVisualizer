cmake_minimum_required(VERSION 3.2)
project(musicvisualizer)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(musicvisualizer.run ${SOURCES} ${HEADERS})
target_link_libraries(musicvisualizer.run pulse-simple pulse fftw3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
