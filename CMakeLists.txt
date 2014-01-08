cmake_minimum_required(VERSION 2.8)
project(adventure CXX)

set(ENGINE_VERSION_MAJOR 1)
set(ENGINE_VERSION_MINOR 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../GameEngine/CMake/Modules")
include(${GAME_ENGINE_INCLUDE_DIR}/CMake/VisualStudioVersionCheck.cmake)
include(${GAME_ENGINE_INCLUDE_DIR}/CMake/CompileFlags.cmake)
include(${GAME_ENGINE_INCLUDE_DIR}/CMake/RedirectOutputTo.cmake)

RedirectOutputTo(plugin/adventure)

find_package(GAME_ENGINE)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/GameEngine)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/GameEngine/interfaces)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/common)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/QuadTree)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/GUI)

find_package(GLFW)
include_directories(${GLFW_INCLUDE_DIR})

find_package(GLM)
include_directories(${GLM_INCLUDE_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/source)


