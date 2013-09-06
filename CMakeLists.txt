cmake_minimum_required (VERSION 2.8)
project (adventure CXX)

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

find_package(GAME_ENGINE)
if(GAME_ENGINE_FOUND)
  include_directories(${GAME_ENGINE_INCLUDE_DIR})
  include_directories(${GAME_ENGINE_INCLUDE_DIR}/interfaces)
  include_directories(${GAME_ENGINE_INCLUDE_DIR}/../common)
endif (GAME_ENGINE_FOUND)

find_package(GLM)
if(GLM_FOUND)
  include_directories(${GLM_INCLUDE_DIR})
endif (GLM_FOUND)

find_package(ANGELSCRIPT)
if(ANGELSCRIPT_FOUND)
  include_directories(${ANGELSCRIPT_INCLUDE_DIR})
  include_directories(${ANGELSCRIPT_ADDON_INCLUDE_DIR})
endif(ANGELSCRIPT_FOUND)

add_library(adventure MODULE source/space_ship.h
							 source/space_ship.cpp
							 source/adventure.h
							 source/adventure.cpp)

target_link_libraries(adventure ${CMAKE_SOURCE_DIR}/../GameEngine/bin/libcommon.so ${GAME_ENGINE_LIBRARY_DEBUG})
