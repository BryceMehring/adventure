#include "SpaceShip.h"
#include "IRenderer.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <sstream>

static void callback()
{
}


SpaceShip::SpaceShip(const std::string& str, unsigned int tile, const glm::vec3& pos) : m_sprite(str), m_pos(pos), m_collisonPolygon(Math::Circle(glm::vec2(pos.x,pos.y),20.0f)),
	m_tile(tile), m_fAngle(0.0f), m_fSpeed(50.0f), m_bVisable(false), m_bDrawLazers(true), m_bCollison(false), m_iHealth(3)
{
	UI::Menu* pMenu = new UI::Menu();

	//std::bind()
	UI::ProgressBar* pProgressBar = new UI::ProgressBar(glm::vec2(pos.x - 10.0f,pos.y + 10.0f),glm::vec2(pos.x + 10.0f,pos.y + 10.0f),callback);
	pMenu->AddElement(pProgressBar);

	pProgressBar->AddRef();
	m_pProgressBar = pProgressBar;

	m_pProgressBar->SetProgress(0.5f);

	m_gui.SetMenu(pMenu);
}

SpaceShip::~SpaceShip()
{
	m_pProgressBar->Release();
}

void* SpaceShip::QueryInterface(unsigned int i) const
{
	if(i == IDestroyable::INTERFACE_DESTROY)
		return (void*)((IDestroyable*)this);

	if(i == SpaceShip::INTERFACE_SPACESHIP)
		return (void*)(this);

	return nullptr;
}

const Math::ICollisionPolygon& SpaceShip::GetCollisionPolygon() const
{
	return m_collisonPolygon;
}

void SpaceShip::Destroy()
{
	m_iHealth -= 1;
}

bool SpaceShip::Update(float dt, Camera* pCam, QuadTree& tree)
{
	m_pos += m_fSpeed * glm::vec3(-sin(m_fAngle * 3.14f / 180.0f),cos(m_fAngle * 3.14f / 180.0f),0.0f) * dt;

	m_bVisable = pCam->IsVisible(glm::vec3(m_pos.x - 50.0f,m_pos.y - 50.0f,-100.0f),glm::vec3(m_pos.x + 50.0f,m_pos.y + 50.0f,-100.0f));

	m_pProgressBar->SetProgress(m_iHealth / 3.0f);
	m_pProgressBar->SetPos(glm::vec2(m_pos));

	m_fSpeed -= 10.0f * dt;
	m_fSpeed = glm::clamp(m_fSpeed,0.0f,1000.0f);

	tree.Erase(*this);
	m_collisonPolygon.GetCircle().center = glm::vec2(m_pos.x,m_pos.y);
	tree.Insert(*this);

	return m_iHealth <= 0;
}

void SpaceShip::Render(IRenderer& renderer)
{
	if(m_bVisable)
	{
		glm::mat4 T = glm::translate(m_pos.x,m_pos.y,-100.0f);
		T = glm::rotate(T,m_fAngle,glm::vec3(0,0,1));

		glm::vec3 color = m_bCollison ? glm::vec3(0.9f,0.3f,0.3f) : glm::vec3(1.0f);

		renderer.DrawSprite(m_sprite,glm::scale(T,50.0f,50.0f,1.0f),color,glm::vec2(1.0f),m_tile);

		if(m_bDrawLazers)
		{

			glm::vec3 lineArray[2] =
			{
				glm::vec3(0.0f,0.0f,0.0f),
				glm::vec3(0.0f,100.0f,0.0f)
			};

			//renderer.DrawLine(lineArray,2,2.0f,glm::vec4(0.0f,1.0f,0.0f,1.0f),T);
		}

		m_gui.Render(renderer);
	}
}













