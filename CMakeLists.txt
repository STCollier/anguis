cmake_minimum_required(VERSION 3.10)
project(anguis VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_CPP
    src/main.cpp
    src/engine/*.cpp
    src/anguis/*.cpp
)

find_package(glm REQUIRED)
find_package(OpenSSL REQUIRED)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/imgui)
add_subdirectory(lib/socketio)
add_subdirectory(lib/openal-soft)

add_executable(anguis ${SRC_CPP})

target_include_directories(anguis PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(anguis PRIVATE
    glad
    glfw
    glm::glm
    sioclient
    imgui
    OpenSSL::SSL
    OpenSSL::Crypto
    OpenAL::OpenAL
)