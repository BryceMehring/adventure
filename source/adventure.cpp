#define PLUGIN_EXPORTS

#include "adventure.h"
#include "IRenderer.h"
#include "IKMInput.h"

RTTI_IMPL(adventure)

extern "C" IPlugin* CreatePlugin()
{
	return new adventure();
}

// Called only once when the plugin is created
void adventure::Init(IGame& game)
{
}

// Called only once when the plugin is destroyed
void adventure::Destroy(IGame& game)
{

}

// Called every frame to update the date of the game
void adventure::Update(IGame& game)
{

}

// Called every frame to render the game
void adventure::Draw(IGame& game)
{
	game.GetRenderer().DrawString("Adventure",glm::vec3(0.0f,0.0f,-20.0f),glm::vec2(10.0f),glm::vec3(0.0f,0.0f,0.0f),0,FontAlignment::Center);
}

