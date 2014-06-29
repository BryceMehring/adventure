cmake_minimum_required(VERSION 2.8)
project(adventure CXX)

option(JENKINS_BUILD "Jenkins CI Build" OFF)

set(ENGINE_VERSION_MAJOR 1)
set(ENGINE_VERSION_MINOR 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules")

find_package(GAME_ENGINE)

list(APPEND CMAKE_MODULE_PATH "${GAME_ENGINE_INCLUDE_DIR}/CMake/Modules")

include(${GAME_ENGINE_INCLUDE_DIR}/CMake/VisualStudioVersionCheck.cmake)
include(${GAME_ENGINE_INCLUDE_DIR}/CMake/CompileFlags.cmake)
include(${GAME_ENGINE_INCLUDE_DIR}/CMake/RedirectOutputTo.cmake)

include_directories(${GAME_ENGINE_INCLUDE_DIR}/source/GameEngine)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/source/GameEngine/interfaces)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/source/common)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/source/QuadTree)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/source/GUI)

find_package(GLM)
include_directories(${GLM_INCLUDE_DIR})
add_definitions(-DGLM_FORCE_RADIANS)

RedirectOutputTo(plugin/adventure)
add_subdirectory(${CMAKE_SOURCE_DIR}/source)


