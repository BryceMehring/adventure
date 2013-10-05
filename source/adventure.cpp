#include "adventure.h"
#include "IRenderer.h"
#include "IKMInput.h"
#include "Game.h"
#include "Camera.h"
#include "Log.h"
#include "ProgressBar.h"

#include "ships/AISpaceShip.h"

#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>

#include <functional>

#include <GLFW/glfw3.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT IPlugin* CreatePlugin()
{
	return new adventure();
}

adventure::adventure() : m_quadTree(Math::FRECT(glm::vec2(-8000,8000),glm::vec2(8000,-8000)),64)
{

}

static void callback()
{
}


void adventure::BuildGUI()
{
	UI::Menu* pMenu = new UI::Menu();

	//std::bind()
	UI::ProgressBar* pProgressBar = new UI::ProgressBar(glm::vec2(-500,500),glm::vec2(500,500),callback);
	pMenu->AddElement(pProgressBar);

	pProgressBar->AddRef();
	m_pProgressBar = pProgressBar;

	m_gui.SetMenu(pMenu);

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
	for(unsigned int i = 0; i < 200; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = rand() % 5;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new AISpaceShip("ship",shipTile,pos)));

		this->m_quadTree.Insert(*m_enemies.back());
	}

	//Squid
	for(unsigned int i = 0; i < 200; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = 3;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new SquidSpaceShip("squid",shipTile,pos)));

		this->m_quadTree.Insert(*m_enemies.back());
	}

	game.GetRenderer().SetClearColor(glm::vec3(0.01,0.01,0.1));

	game.GetRenderer().EnableVSync(false);

	BuildGUI();
}

// Called only once when the plugin is destroyed
void adventure::Destroy(Game& game)
{
	m_pProgressBar->Release();
	ReleaseCamera(m_pCamera);
}

// Called every frame to update the state of the game
void adventure::Update(Game& game)
{
	m_spaceShip.Update(game.GetDt(),m_pCamera,game.GetInput(),m_quadTree);

	for(auto iter = this->m_enemies.begin(); iter != m_enemies.end(); )
	{
		if((*iter)->Update(game.GetDt(),m_pCamera,m_quadTree))
		{
			m_quadTree.Erase(**iter);
			m_enemies.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	m_gui.Update(game.GetInput(),game.GetDt());

	static float progress = 0.0f;

	if(game.GetInput().KeyPress(GLFW_KEY_RIGHT,false))
	{
		progress += game.GetDt() * 0.1f;

		m_pProgressBar->SetProgress(progress);
	}
	else if(game.GetInput().KeyPress(GLFW_KEY_LEFT,false))
	{
		progress -= game.GetDt() * 0.1f;

		m_pProgressBar->SetProgress(progress);
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

	this->m_quadTree.Render(renderer);

	std::stringstream stream;
	stream << game.GetFps();

	renderer.SetRenderSpace(RenderSpace::Screen);
	renderer.DrawString(stream.str().c_str(),glm::vec3(-850.0f,500.0f,-5.0f),glm::vec2(40.0f));

	m_gui.Render(renderer);



}

