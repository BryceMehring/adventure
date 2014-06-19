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

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT IPlugin* CreatePlugin()
{
	return new adventure();
}

adventure::adventure() : m_quadTree(Math::FRECT(glm::vec2(-8000,8000),glm::vec2(8000,-8000)),32), m_bRenderQuadTree(false)
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
	m_camera.LookAt(glm::vec3(0.0f,0.0f,99.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	m_camera.Update();

	game.GetRenderer().SetCamera(&m_camera);

	m_enemies.reserve(500);

	// ships
	for(unsigned int i = 0; i < 1000; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = rand() % 5;
		m_enemies.push_back(std::auto_ptr<SpaceShip>(new AISpaceShip("ship",shipTile,30 + rand() % 50,pos)));

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
	IRenderer& renderer = game.GetRenderer();
	IInput& input = game.GetInput();

	int width, height;
	renderer.GetDisplayMode(&width, &height);

	const glm::ivec2& cursorPos = input.GetCursorPos();

	// Update the camera with user input
	if(input.KeyPress(KEY_UP, false) || (float)cursorPos.y > 0.95f*height)
	{
		m_cameraPos.y += 400.0f * game.GetDt();
	}
	if(input.KeyPress(KEY_DOWN, false) || (float)cursorPos.y < 0.05f*height)
	{
		m_cameraPos.y += -400.0f * game.GetDt();
	}
	if(input.KeyPress(KEY_LEFT, false) || (float)cursorPos.x < 0.05f*width)
	{
		m_cameraPos.x += -400.0f * game.GetDt();
	}
	if(input.KeyPress(KEY_RIGHT, false) || (float)cursorPos.x > 0.95f*width)
	{
		m_cameraPos.x += 400.0f * game.GetDt();
	}

	m_cameraPos.z += -30*input.MouseZ();

	m_camera.LookAt(m_cameraPos, glm::vec3(m_cameraPos.x, m_cameraPos.y, 0.0f));
	m_camera.Update();

	// Update all of the enemy ships
	for(auto iter = this->m_enemies.begin(); iter != m_enemies.end(); )
	{
		bool bDead = (*iter)->Update((float)game.GetDt(),m_camera,m_quadTree);

		if(bDead)
		{
			m_deathAnimation.push_back(std::make_pair((*iter)->GetPos(),SpriteAnimation(90,30)));
			m_selectedObjects.erase(&(**iter));

			m_quadTree.Erase(**iter);
			iter = m_enemies.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// Update all the death animations
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

	// Check if the user made a selection with the mouse
	if((m_drawSelectionQuad = input.GetSelectedRect(m_Min, m_Max)))
	{

		glm::vec3 unprojectedMin = m_camera.UnProjectWS(glm::vec3(m_Min, -99), glm::vec4(0.0f, 0.0f, width, height));
		glm::vec3 unprojectedMax = m_camera.UnProjectWS(glm::vec3(m_Max, -99), glm::vec4(0.0f, 0.0f, width, height));

		Math::CRectangle collisionRect(Math::FRECT(glm::vec2(unprojectedMin.x, unprojectedMax.y), glm::vec2(unprojectedMax.x, unprojectedMin.y)));

		if(!input.KeyPress(KEY_LEFT_CONTROL, false) && !input.KeyPress(KEY_RIGHT_CONTROL, false))
		{
			m_selectedObjects.clear();
		}

		std::vector<ISpatialObject*> newSelectedObjects;
		m_quadTree.QueryNearObjects(collisionRect, newSelectedObjects);

		for(auto iter : newSelectedObjects)
		{
			m_selectedObjects.insert(iter);
		}

		m_unprojectedMin = unprojectedMin;
		m_unprojectedMax = unprojectedMax;
	}

	if(input.MouseClick(1))
	{
		glm::vec3 worldPos = m_camera.UnProjectWS(glm::vec3(cursorPos, -100), glm::vec4(0,0, width, height));

		for(auto unit : m_selectedObjects)
		{
			SpaceShip* pShip = static_cast<SpaceShip*>(unit->QueryInterface(SpaceShip::INTERFACE_SPACESHIP));
			if(pShip != nullptr)
			{
				pShip->MoveTo(worldPos);
			}
		}
	}

	if(input.KeyPress(KEY_DELETE, false))
	{
		for(auto unit : m_selectedObjects)
		{
			IDestroyable* pDestroyable = static_cast<IDestroyable*>(unit->QueryInterface(SpaceShip::INTERFACE_DESTROY));
			if(pDestroyable != nullptr)
			{
				pDestroyable->Destroy();
			}
		}
	}

	if(input.KeyPress(KEY_F1))
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
	for(int i = 0; i < 6; ++i)
	{
		glm::mat4 T = glm::translate(glm::vec3(0.0f,0.0f,(float)zPos));
		T = glm::scale(T,glm::vec3(10000.0f,10000.0f,1.0f));
		renderer.DrawSprite("stars",T,glm::vec4(1.0f),glm::vec2(40.0f /(i + 1),40.0f / (i + 1))); // 15

		zPos += 30;
	}

	//Loops over all enemies
	for(auto& iter : m_enemies)
	{
		iter->Render(renderer);
	}

	for(auto& iter : m_deathAnimation)
	{
		glm::mat4 T = glm::translate(glm::vec3(iter.first.x, iter.first.y, -90));
		T = glm::scale(T,glm::vec3(200.0f,200.0f,1.0f));

		renderer.DrawSprite("explosion",T,glm::vec4(1.0f,1.0f,1.0f,0.9f),glm::vec2(1),iter.second.GetTile());
	}

	for(auto& iter : m_selectedObjects)
	{
		SpaceShip* pShip = static_cast<SpaceShip*>(iter->QueryInterface(SpaceShip::INTERFACE_SPACESHIP));
		if(pShip != nullptr)
		{
			renderer.DrawCircle(pShip->GetPos(), pShip->GetRadius(), 1.0f, 72, glm::vec4(0.2f,0.4f,0.6f, 0.7f));
		}
	}

	if(m_bRenderQuadTree)
	{
		m_quadTree.Render(renderer);
	}

	std::ostringstream converter;
	converter << "Number of enemies: " <<this->m_enemies.size() << std::endl;
	converter << "Selected objects: " << this->m_selectedObjects.size();
	renderer.SetRenderSpace(RenderSpace::Screen);

	int width, height;
	renderer.GetDisplayMode(&width, &height);
	renderer.DrawString(converter.str().c_str(),glm::vec3(width / 2,height,-10.0f), 50.0f, glm::vec4(1.0f), nullptr, FontAlignment::Center);

	if(m_drawSelectionQuad)
	{
		glm::mat4 transformation = glm::translate(glm::vec3((m_Min + m_Max) / 2, 0));
		transformation = glm::scale(transformation, glm::vec3(m_Max.x - m_Min.x, m_Max.y - m_Min.y, 1.0f));
		renderer.DrawSprite(transformation, glm::vec4(1.0f,0.0f,0.0f,0.5f));
	}

}

