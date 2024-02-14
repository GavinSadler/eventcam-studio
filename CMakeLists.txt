
cmake_minimum_required(VERSION 3.7)

set(TARGET_NAME "eventcamstudio")
project(${TARGET_NAME} C CXX)

set(CMAKE_CXX_STANDARD 17)

# GLFW
add_subdirectory("./glfw")

# Dear ImGui
set(IMGUI_DIR "./imgui")
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

find_package(MetavisionSDK COMPONENTS core driver REQUIRED)
find_package(MetavisionHAL REQUIRED)
find_package(pylon REQUIRED)
find_package(Threads REQUIRED)

#file(GLOB sources *.cpp *h)

add_executable(${TARGET_NAME} ${sources} main.cpp ImageGrabHandler.cpp)
target_link_libraries(${TARGET_NAME}
    glfw
    MetavisionSDK::core
    MetavisionSDK::driver
    Metavision::HAL_discovery
    pylon::pylon
    Threads::Threads
)

