
set(adventure_HEADERS
	adventure.h
	IDestroyable.h
    SpriteAnimation.h
	ships/AISpaceShip.h
	ships/SpaceShip.h
	ships/weapons/spaceShipBeam.h
	)

set(adventure_SOURCE
	adventure.cpp
    SpriteAnimation.cpp
	ships/AISpaceShip.cpp
	ships/SpaceShip.cpp
	ships/weapons/spaceShipBeam.cpp
	)

add_library(adventure MODULE ${adventure_HEADERS} ${adventure_SOURCE})
target_link_libraries(adventure ${GAME_ENGINE_QUADTREE_LIB} ${GAME_ENGINE_COMMON_LIB} ${GAME_ENGINE_LIB} ${GAME_ENGINE_GUI_LIB})

include(${GAME_ENGINE_INCLUDE_DIR}/CMake/ConfigurePluginExtension.cmake)
ConfigurePluginExtension(adventure)

