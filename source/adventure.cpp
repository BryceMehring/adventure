#include "adventure.h"
#include "IRenderer.h"
#include "IInput.h"
#include "Game.h"
#include "Camera.h"
#include "Log.h"
#include "ProgressBar.h"

#include "ships/AISpaceShip.h"

#include <sstream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>

#include <functional>
#include <algorithm>

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
	game.GetRenderer().GetDisplayMode(&width,&height);
	m_camera.SetLens(90.0f, (float)width, (float)height, 0.1f, 7000.0f);
	m_camera.LookAt(glm::vec3(0.0f,0.0f,1000.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	m_camera.Update();

	game.GetRenderer().SetCamera(&m_camera);

	m_enemies.reserve(500);

	// ships
	for(unsigned int i = 0; i < 2000; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = rand() % 5;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new AISpaceShip("ship",shipTile,30 + rand() % 50,pos)));

		m_quadTree.Insert(*m_enemies.back());
	}

	//Squid
	for (unsigned int i = 0; i < 500; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::diskRand(6000.0f),-100.0f);
		unsigned int shipTile = 3;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new SquidSpaceShip("squid",shipTile,20.0f,pos)));

		m_quadTree.Insert(*m_enemies.back());
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
	m_spaceShip.Update((float)game.GetDt(),m_camera,game.GetInput(),m_quadTree);

	for(auto iter = this->m_enemies.begin(); iter != m_enemies.end(); )
	{
		bool bDead = (*iter)->Update((float)game.GetDt(),m_camera,m_quadTree);

		if(bDead || std::find(m_selectedObjects.begin(), m_selectedObjects.end(), &(**iter)) != m_selectedObjects.end())
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

	if(game.GetInput().KeyPress(KEY_F1))
	{
		m_bRenderQuadTree = !m_bRenderQuadTree;
	}

	IInput& input = game.GetInput();

	if((m_drawSelectionQuad = input.GetSelectedRect(m_Min, m_Max)))
	{
		IRenderer& renderer = game.GetRenderer();

		int width, height;
		renderer.GetDisplayMode(&width, &height);

		glm::vec3 unprojectedMin = m_camera.UnProjectWS(glm::vec3(m_Min, -99), glm::vec4(0.0f, 0.0f, width, height));
		glm::vec3 unprojectedMax = m_camera.UnProjectWS(glm::vec3(m_Max, -99), glm::vec4(0.0f, 0.0f, width, height));

		Math::CRectangle collisionRect(Math::FRECT(glm::vec2(unprojectedMin.x, unprojectedMax.y), glm::vec2(unprojectedMax.x, unprojectedMin.y)));

		m_selectedObjects.clear();
		m_quadTree.QueryNearObjects(collisionRect, m_selectedObjects);

		m_unprojectedMin = unprojectedMin;
		m_unprojectedMax = unprojectedMax;
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
	for(int i = 0; i < 5; ++i)
	{
		glm::mat4 T = glm::translate(glm::vec3(0.0f,0.0f,(float)zPos));
		T = glm::scale(T,glm::vec3(8000.0f,8000.0f,1.0f));
		renderer.DrawSprite("stars",T,glm::vec4(1.0f),glm::vec2(40.0f /(i + 1),40.0f / (i + 1))); // 15

		zPos += 30;
	}

	//Loops over all enemies
	for(auto& iter : m_enemies)
	{
		iter->Render(renderer);
	}

	m_spaceShip.Render(renderer);

	for(auto& iter : m_deathAnimation)
	{
		glm::mat4 T = glm::translate(iter.first);
		T = glm::scale(T,glm::vec3(200.0f,200.0f,1.0f));

		renderer.DrawSprite("explosion",T,glm::vec4(1.0f,1.0f,1.0f,0.9f),glm::vec2(1),iter.second.GetTile());
	}

	if(m_bRenderQuadTree)
	{
		m_quadTree.Render(renderer);
	}

	std::ostringstream converter;
	converter << "Number of enemies: " <<this->m_enemies.size();
	renderer.SetRenderSpace(RenderSpace::Screen);
	renderer.DrawString(converter.str().c_str(),glm::vec3(0.0f,100.0f,-10.0f));

	converter.str("");
	converter << "Selected objects: " << this->m_selectedObjects.size() << std::endl;
	converter << "Selected RECT in world space: (" << m_unprojectedMin.x << "," << m_unprojectedMin.y << "), " << std::endl <<
												"("<< m_unprojectedMax.x << "," << m_unprojectedMax.y << ")";
	renderer.DrawString(converter.str().c_str(), glm::vec3(600.0f, 200.0f, -10.0f));

	if(m_drawSelectionQuad)
	{
		glm::mat4 transformation = glm::translate(glm::vec3((m_Min + m_Max) / 2, 0));
		transformation = glm::scale(transformation, glm::vec3(m_Max.x - m_Min.x, m_Max.y - m_Min.y, 1.0f));
		renderer.DrawSprite(transformation, glm::vec4(1.0f,0.0f,0.0f,0.5f));
	}

}

