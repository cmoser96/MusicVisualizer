cmake_minimum_required(VERSION 3.2)
project(tests)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(include)

set(PULSE_INTERFACE_TEST pulse_interface_test.cpp)
set(PULSE_INTERFACE ../src/pulse_interface.cpp)
set(PULSE_INTERFACE_H ../include/pulse_interface.h)


add_executable(pulse_interface_test.run ${PULSE_INTERFACE_TEST} ${PULSE_INTERFACE} ${PULSE_INTERFACE_H})
target_link_libraries(pulse_interface_test.run pulse)
