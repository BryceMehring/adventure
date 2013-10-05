cmake_minimum_required(VERSION 2.8)
project(adventure CXX)

# The version number.
set(ENGINE_VERSION_MAJOR 1)
set(ENGINE_VERSION_MINOR 0)

if( MSVC )
	if(NOT ${CMAKE_GENERATOR} STREQUAL "Visual Studio 11")
		message( SEND_ERROR "Visual Studio 11 is needed to compile the code" )
	endif()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../GameEngine/CMake/Modules")
include("${CMAKE_SOURCE_DIR}/../GameEngine/CMake/CompileFlags.cmake")
include("${CMAKE_SOURCE_DIR}/../GameEngine/CMake/RedirectOutputTo.cmake")

RedirectOutputTo("plugin/adventure")

find_package(GAME_ENGINE)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/GameEngine)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/GameEngine/interfaces)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/common)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/QuadTree)
include_directories(${GAME_ENGINE_INCLUDE_DIR}/GUI)

find_package(GLM)
include_directories(${GLM_INCLUDE_DIR})

find_package(ANGELSCRIPT)
include_directories(${ANGELSCRIPT_INCLUDE_DIR})
include_directories(${ANGELSCRIPT_ADDON_INCLUDE_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/source)


