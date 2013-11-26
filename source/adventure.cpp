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

adventure::adventure() : m_quadTree(Math::FRECT(glm::vec2(-10000,10000),glm::vec2(10000,-10000)),32), m_bRenderQuadTree(false)
{
}
adventure::~adventure()
{
	//m_pProgressBar->Release();
}


void adventure::BuildGUI()
{
	/*UI::Menu* pMenu = new UI::Menu();

	//std::bind()
	UI::ProgressBar* pProgressBar = new UI::ProgressBar(glm::vec2(-500,500),glm::vec2(500,500),callback);
	pMenu->AddElement(pProgressBar);

	pProgressBar->AddRef();
	m_pProgressBar = pProgressBar;

	m_gui.SetMenu(pMenu);*/

}

// Called only once when the plugin is created
void adventure::Init(Game& game)
{
	int width;
	int height;
	game.GetRenderer().GetDisplayMode(width,height);
	m_camera.setLens(90.0f,1920,1080,1.0f,5000.0f);
	m_camera.lookAt(glm::vec3(0.0f,0.0f,100.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	m_camera.update();

	game.GetRenderer().SetCamera(&m_camera);

	m_enemies.reserve(500);

	// ships
	for(unsigned int i = 0; i < 2000; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = rand() % 5;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new AISpaceShip("ship",shipTile,20.0f,pos)));

		this->m_quadTree.Insert(*m_enemies.back());
	}

	//Squid
	for(unsigned int i = 0; i < 500; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::diskRand(7000.0f),-100.0f);
		unsigned int shipTile = 3;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new SquidSpaceShip("squid",shipTile,20.0f,pos)));

		this->m_quadTree.Insert(*m_enemies.back());
	}

	game.GetRenderer().SetClearColor(glm::vec3(0.01,0.01,0.1));

	game.GetRenderer().EnableVSync(false);

	BuildGUI();
}

// Called only once when the plugin is destroyed
void adventure::Destroy(Game& game)
{
	game.GetRenderer().SetCamera(nullptr);
}

// Called every frame to update the state of the game
void adventure::Update(Game& game)
{
	m_spaceShip.Update(game.GetDt(),m_camera,game.GetInput(),m_quadTree);

	for(auto iter = this->m_enemies.begin(); iter != m_enemies.end(); )
	{
		if((*iter)->Update(game.GetDt(),m_camera,m_quadTree))
		{
			m_deathAnimation.push_back(std::make_pair((*iter)->GetPos(),SpriteAnimation(90,30)));

			m_quadTree.Erase(**iter);
			iter = m_enemies.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for(auto iter = m_deathAnimation.begin(); iter != m_deathAnimation.end(); )
	{
		if(iter->second.Update(game.GetDt()))
		{
			iter = m_deathAnimation.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if(game.GetInput().KeyPress(GLFW_KEY_F1))
	{
		m_bRenderQuadTree = !m_bRenderQuadTree;
	}

	m_gui.Update(game.GetInput(),game.GetDt());
}

// Called every frame to render the game
void adventure::Draw(Game& game)
{
	IRenderer& renderer = game.GetRenderer();
	renderer.SetRenderSpace(RenderSpace::World);

	int zPos = -400;

	//Draws layers of stars
	for(int i = 0; i < 4; ++i)
	{
		glm::mat4 T = glm::translate(0.0f,0.0f,(float)zPos);
		T = glm::scale(T,8000.0f,8000.0f,1.0f);
		renderer.DrawSprite("stars",T,glm::vec3(1.0f),glm::vec2(40.0f /(i + 1),40.0f / (i + 1))); // 15

		zPos += 15;
	}

	//Loops over all enemies
	for(auto& iter : m_enemies)
	{
		iter->Render(renderer);
	}

	m_spaceShip.Render(renderer);

	for(auto& iter : m_deathAnimation)
	{
		glm::vec3 pos = iter.first;
		glm::mat4 T = glm::translate(pos.x,pos.y,pos.z);
		T = glm::scale(T,glm::vec3(200.0f,200.0f,1.0f));

		renderer.DrawSprite("explosion",T,glm::vec3(1),glm::vec2(1),iter.second.GetTile());
	}

	if(m_bRenderQuadTree)
	{
		m_quadTree.Render(renderer);
	}
}

