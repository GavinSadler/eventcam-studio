
cmake_minimum_required(VERSION 3.5)

set(executable_name "eventcamstudio")

project(${executable_name} C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(MetavisionSDK COMPONENTS core driver REQUIRED)
find_package(pylon REQUIRED)
find_package(Threads REQUIRED)

file(GLOB sources *.cpp)

add_executable(${executable_name} ${sources})
target_link_libraries(${executable_name}
    MetavisionSDK::core
    MetavisionSDK::driver
    pylon::pylon
    Threads::Threads
)

