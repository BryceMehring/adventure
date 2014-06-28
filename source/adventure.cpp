#include "adventure.h"
#include "IRenderer.h"
#include "IInput.h"
#include "Game.h"
#include "Camera.h"
#include "Log.h"
#include "Slider.h"

#include "ships/AISpaceShip.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>
#include <functional>
#include <sstream>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT IPlugin* CreatePlugin()
{
	return new adventure();
}

adventure::adventure() : m_quadTree(Math::FRECT(glm::vec2(-8000,8000),glm::vec2(8000,-8000)),64), m_bRenderQuadTree(false)
{
}
adventure::~adventure()
{
	//m_pProgressBar->Release();
}

// Called only once when the plugin is created
void adventure::Init(Game& game)
{
	IRenderer& renderer = game.GetRenderer();

	int width;
	int height;
	renderer.GetDisplayMode(&width, &height);
	m_camera.SetLens(90.0f, (float)width, (float)height, 0.1f, 7000.0f);
	m_camera.LookAt(glm::vec3(0.0f,0.0f,99.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	m_camera.Update();

	renderer.SetCamera(&m_camera);

	m_enemies.reserve(1000);

	// ships
	for(unsigned int i = 0; i < 1000; ++i)
	{
		glm::vec3 pos = glm::vec3(glm::linearRand(glm::vec2(-4000),glm::vec2(4000)),-100.0f);
		unsigned int shipTile = rand() % 5;

		SpaceShip* pShip = new AISpaceShip("ship", shipTile, 30 + rand() % 50, pos);

		m_enemies.emplace_back(pShip);
		m_quadTree.Insert(*pShip);
	}

	renderer.SetClearColor(glm::vec3(0.01f, 0.01f, 0.1f));
	renderer.EnableVSync(false);

	BuildGUI(game);
}

// Called only once when the plugin is destroyed
void adventure::Destroy(Game& game)
{
	IRenderer& renderer = game.GetRenderer();
	renderer.SetClearColor(glm::vec3(0.0f));
	renderer.SetCamera(nullptr);
}

// Called every frame to update the state of the game
void adventure::Update(Game& game)
{
	if(!m_bEnableOptionsMenu)
	{
		UpdateUserInput(game);
	}

	// Update all of the enemy ships
	for(auto iter = m_enemies.begin(); iter != m_enemies.end(); )
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

	m_gui.Update(game.GetInput(),game.GetDt());
}

// Called every frame to render the game
void adventure::Draw(Game& game)
{
	IRenderer& renderer = game.GetRenderer();
	renderer.SetRenderSpace(RenderSpace::World);

	int zPos = -400;

	const Math::FRECT& gridRect = m_quadTree.GetRect();

	//Draws layers of stars
	for(int i = 0; i < 6; ++i)
	{
		glm::mat4 T = glm::translate(glm::vec3(0.0f,0.0f,(float)zPos));
		T = glm::scale(T,glm::vec3(gridRect.Width(), gridRect.Height(),1.0f));
		renderer.DrawSprite("stars",T,glm::vec4(1.0f),glm::vec2(40.0f / (i + 1),40.0f / (i + 1))); // 15

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

	static float angle = 0.0f;
	angle += 10.0f * game.GetDt();

	glm::mat4 rotMatrix = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

	for(auto& iter : m_selectedObjects)
	{
		SpaceShip* pShip = static_cast<SpaceShip*>(iter->QueryInterface(SpaceShip::INTERFACE_SPACESHIP));
		if(pShip != nullptr)
		{
			float R = pShip->GetRadius() * 3;

			glm::mat4 T = glm::translate(pShip->GetPos());
			T *= rotMatrix;
			T = glm::scale(T, glm::vec3(R, R, 1.0f));

			renderer.DrawSprite("dashed_circle", T, glm::vec4(0.2f, 0.4f, 0.6f, 0.3f), glm::vec2(1.0f), 0, "dashed_circle_shader");
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
	renderer.DrawString(converter.str().c_str(),glm::vec3(width / 2,height,-10.0f), glm::vec4(1.0f), 50.0f, nullptr, FontAlignment::Center);

	if(m_drawSelectionQuad)
	{
		glm::mat4 transformation = glm::translate(glm::vec3((m_Min + m_Max) / 2, 0));
		transformation = glm::scale(transformation, glm::vec3(m_Max.x - m_Min.x, m_Max.y - m_Min.y, 1.0f));
		renderer.DrawSprite(transformation, glm::vec4(0.0f,0.7f,0.0f,0.4f));

		glm::vec3 linePos[5] =
		{
			glm::vec3(m_Min, 0),
			glm::vec3(m_Min.x, m_Max.y, 0),
			glm::vec3(m_Max, 0),
			glm::vec3(m_Max.x, m_Min.y, 0),
			glm::vec3(m_Min, 0)
		};

		renderer.DrawLine(linePos, 5, 4.0f, glm::vec4(0.0f,0.75f,0.0f,0.8f));
	}

	renderer.SetRenderSpace(RenderSpace::Screen);
	m_gui.Render(renderer);
}

void adventure::ToggleCallback(UI::Button&)
{
	m_bEnableOptionsMenu = !m_bEnableOptionsMenu;

	if(m_bEnableOptionsMenu)
	{
		m_gui.SetNode(m_optionsNode);
	}
	else
	{
		m_gui.SetNode(m_rootNode);
	}
}

void adventure::UpdateUserInput(Game& game)
{
	IInput& input = game.GetInput();
	IRenderer& renderer = game.GetRenderer();
	double dt = game.GetDt();

	int width, height;
	renderer.GetDisplayMode(&width, &height);

	const glm::ivec2& cursorPos = input.GetCursorPos();

	// Update the camera with user input
	if(input.KeyPress(KEY_UP, false) || ((float)cursorPos.y > 0.95f*height && cursorPos.x < 0.75*width))
	{
		m_cameraPos.y += 400.0f * dt;
	}
	if(input.KeyPress(KEY_DOWN, false) || (float)cursorPos.y < 0.05f*height)
	{
		m_cameraPos.y += -400.0f * dt;
	}
	if(input.KeyPress(KEY_LEFT, false) || (float)cursorPos.x < 0.05f*width)
	{
		m_cameraPos.x += -400.0f * dt;
	}
	if(input.KeyPress(KEY_RIGHT, false) || ((float)cursorPos.x > 0.95f*width && cursorPos.y < 0.95*height))
	{
		m_cameraPos.x += 400.0f * dt;
	}

	// Update the z level of the camera via the mouse scroll
	m_fCameraZVelocity += 200.0f * -input.MouseZ();
	m_cameraPos.z += m_fCameraZVelocity * dt;

	double fps = 1.0 / dt;
	m_fCameraZVelocity *= (fps / (fps + 2));

	m_camera.LookAt(m_cameraPos, glm::vec3(m_cameraPos.x, m_cameraPos.y, 0.0f));
	m_camera.Update();

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

	// if the user clicks, send all of the ships selected to that target
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

	// If the user presses the delete key, destroy all the destroyable units that the user has selected
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

	// If the user presses the f1 key, toggle rendering the quadtree
	if(input.KeyPress(KEY_F1))
	{
		m_bRenderQuadTree = !m_bRenderQuadTree;
	}
}

void adventure::BuildGUI(Game& game)
{	
	using namespace std::placeholders;

	IRenderer& renderer = game.GetRenderer();

	int width, height;
	renderer.GetDisplayMode(&width, &height);

	m_rootNode = m_gui.CreateNode();
	m_gui.SetNode(m_rootNode);

	Math::FRECT R;
	renderer.GetStringRect("Toggle Options", 50.0f, FontAlignment::Left, R);

	auto pToggleButton = UI::GUIFactory<UI::Button>::CreateElement(game, glm::vec2(width - R.Width() / 2.0f, height), glm::vec4(0.0f,0.0f,0.0f,0.5f), glm::vec4(1.0f, 0.0f, 0.0f,0.5f),
										 50.0f, "Toggle Options", std::bind(&adventure::ToggleCallback, this, _1));

	m_gui.AddElement(m_rootNode, pToggleButton);

	m_optionsNode = m_gui.CreateNode();
	m_gui.LinkNodes(m_optionsNode, m_rootNode);

	auto pCohesionWeightBar = std::make_shared<UI::Slider>(glm::vec2(50.0f, height / 4), glm::vec2(width - 50.0f, height / 4), 0.0f, 10.0f, 2,
														   "blank", "Cohesion Weight", AISpaceShip::SetCohesionWeight);
	pCohesionWeightBar->SetValue(4.0f);
	auto pAlignmentWeightBar = std::make_shared<UI::Slider>(glm::vec2(50.0f, height / 2), glm::vec2(width - 50.0f, height / 2), 0.0f, 10.0f, 2,
															"blank", "Alignment Weight", AISpaceShip::SetAlignmentWeight);
	pAlignmentWeightBar->SetValue(2.0f);
	auto pSeperationWeightBar = std::make_shared<UI::Slider>(glm::vec2(50.0f, 3 * height / 4), glm::vec2(width - 50.0f, 3 * height / 4), 0.0f, 10000.0f, 2,
															 "blank", "Seperation Weight", AISpaceShip::SetSeperationWeight);
	pSeperationWeightBar->SetValue(10000.0f);

	m_gui.AddElement(m_optionsNode, pCohesionWeightBar);
	m_gui.AddElement(m_optionsNode, pAlignmentWeightBar);
	m_gui.AddElement(m_optionsNode, pSeperationWeightBar);
	m_gui.AddElement(m_optionsNode, pToggleButton);
}

