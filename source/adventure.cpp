#include "adventure.h"
#include "IRenderer.h"
#include "IKMInput.h"
#include "Game.h"
#include "Camera.h"
#include "Log.h"

#include "ships/AISpaceShip.h"

#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT IPlugin* CreatePlugin()
{
	return new adventure();
}

adventure::adventure() : m_quadTree(Math::FRECT(glm::vec2(-8000,8000),glm::vec2(8000,-8000)))
{

}

// Called only once when the plugin is created
void adventure::Init(Game& game)
{
	m_pCamera = CreateCamera();

	int m, i, width, height;
	game.GetRenderer().GetCurrentDisplayMode(m, i);
	game.GetRenderer().GetDisplayMode(m,i,width,height);
	m_pCamera->setLens(90.0f,width,height,1.0f,5000.0f);
	m_pCamera->lookAt(glm::vec3(0.0f,0.0f,100.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	m_pCamera->update();

	m_pCamera->AddRef();
	game.GetRenderer().SetCamera(m_pCamera);

	m_enemies.reserve(500);
	for(unsigned int i = 0; i < 10; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = rand() % 5;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new AISpaceShip("ship",shipTile,pos)));

		this->m_quadTree.Insert(*m_enemies.back());
	}

	//Squid
	for(unsigned int i = 0; i < 300; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = 3;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new SquidSpaceShip("squid",shipTile,pos)));

		this->m_quadTree.Insert(*m_enemies.back());
	}

	game.GetRenderer().SetClearColor(glm::vec3(0.01,0.01,0.1));

	game.GetRenderer().EnableVSync(false);
}

// Called only once when the plugin is destroyed
void adventure::Destroy(Game& game)
{
	ReleaseCamera(m_pCamera);
}

// Called every frame to update the state of the game
void adventure::Update(Game& game)
{
	m_spaceShip.Update(game.GetDt(),m_pCamera,game.GetInput(),m_quadTree);

	for(auto iter = this->m_enemies.begin(); iter != m_enemies.end(); ++iter)
	{
		(*iter)->Update(game.GetDt(),m_pCamera,m_quadTree);
	}
}

// Called every frame to render the game
void adventure::Draw(Game& game)
{
	IRenderer& renderer = game.GetRenderer();
	renderer.SetRenderSpace(RenderSpace::World);

	int zPos = -800;

	//Draws X (10) layers of stars
	for(int i = 0; i < 10; ++i)
	{
		glm::mat4 T = glm::translate(0.0f,0.0f,(float)zPos);
		T = glm::scale(T,8000.0f,8000.0f,1.0f);
		renderer.DrawSprite("stars",T,glm::vec3(1.0f),glm::vec2(30.0f /(i + 1),30.0f / (i + 1))); // 15

		zPos += 50;
	}

	//Loops over all enemies
	for(auto iter = this->m_enemies.begin(); iter != m_enemies.end(); ++iter)
	{
		(*iter)->Render(renderer);
	}

	m_spaceShip.Render(renderer);

	std::stringstream stream;
	stream << game.GetFps();

	renderer.SetRenderSpace(RenderSpace::Screen);
	renderer.DrawString(stream.str().c_str(),glm::vec3(50,200,-5.0f),glm::vec2(40.0f));

	//this->m_quadTree.Render(renderer);

}

