cmake_minimum_required(VERSION 2.8)
project(adventure CXX)

set(ENGINE_VERSION_MAJOR 1)
set(ENGINE_VERSION_MINOR 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../GameEngine/CMake/Modules")
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


