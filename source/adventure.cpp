#include "adventure.h"
#include "IRenderer.h"
#include "IKMInput.h"
#include "Game.h"
#include "Camera.h"

#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

extern "C" IPlugin* CreatePlugin()
{
	return new adventure();
}

// Called only once when the plugin is created
void adventure::Init(Game& game)
{
	m_pCamera = CreateCamera();

	int width, height;
	game.GetRenderer().GetDisplayMode(game.GetRenderer().GetCurrentDisplayMode(),width,height);
	m_pCamera->setLens(90.0f,width,height,1.0f,5000.0f);
	m_pCamera->lookAt(glm::vec3(0.0f,0.0f,100.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	m_pCamera->update();

	m_pCamera->AddRef();
	game.GetRenderer().SetCamera(m_pCamera);
}

// Called only once when the plugin is destroyed
void adventure::Destroy(Game& game)
{
	ReleaseCamera(m_pCamera);
}

// Called every frame to update the date of the game
void adventure::Update(Game& game)
{
	m_spaceShip.Update(game.GetDt(),m_pCamera,game.GetInput());
}

// Called every frame to render the game
void adventure::Draw(Game& game)
{
	IRenderer& renderer = game.GetRenderer();

	int zPos = -800;
	for(int i = 0; i < 10; ++i)
	{
		glm::mat4 T = glm::translate(0.0f,0.0f,(float)zPos);
		T = glm::scale(T,8000.0f,8000.0f,1.0f);
		renderer.DrawSprite("stars",T,glm::vec2(30.0f /(i + 1),30.0f / (i + 1))); // 15

		zPos += 50;
	}


	m_spaceShip.Render(renderer);
}

