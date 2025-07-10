cmake_minimum_required(VERSION 3.16)
project(VPE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG master)
FetchContent_MakeAvailable(SFML)

set(SOURCES 
    src/main.cpp
    src/pObj.cpp
    src/renderer.cpp
    src/verletSolver.cpp
    src/particle.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)