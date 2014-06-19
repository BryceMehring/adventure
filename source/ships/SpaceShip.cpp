#include "SpaceShip.h"
#include "IRenderer.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>
#include <sstream>

SpaceShip::SpaceShip(const std::string& str, unsigned int tile, float s, const glm::vec3& pos) :
	m_sprite(str), m_pos(pos), m_target(pos), m_bMoveToTarget(false), m_collisonPolygon(Math::Circle(glm::vec2(pos.x,pos.y),s)),
	m_tile(tile), m_fAngle(0.0f), m_fSpeed(50.0f), m_bVisable(false), m_bCollison(false), m_iHealth(3)
{
	//UI::Menu* pMenu = new UI::Menu();

	/*UI::ProgressBar* pProgressBar = new UI::ProgressBar(glm::vec2(pos.x - 10.0f,pos.y + 10.0f),glm::vec2(pos.x + 10.0f,pos.y + 10.0f),callback);
	pMenu->AddElement(pProgressBar);

	m_pProgressBar = pProgressBar;

	m_pProgressBar->SetProgress(0.5f);*/

	//m_gui.SetMenu(pMenu);

	m_velocity = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
	m_target = m_pos;
}

SpaceShip::~SpaceShip()
{
	//m_pProgressBar->Release();
}

const glm::vec3& SpaceShip::GetPos() const
{
	return m_pos;
}

const glm::vec3& SpaceShip::GetDir() const
{
	return m_velocity;
}

float SpaceShip::GetRadius() const
{
	return m_collisonPolygon.GetCircle().r;
}

float SpaceShip::GetSpeed() const
{
	return m_fSpeed;
}
void SpaceShip::SetSpeed(float speed)
{
	m_fSpeed = speed;
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
bool SpaceShip::Update(float dt, Camera& cam, QuadTree& tree)
{
	WrapIfNeeded(tree.GetRect());

	tree.Erase(*this);
	m_collisonPolygon.GetCircle().center = glm::vec2(m_pos.x,m_pos.y);
	tree.Insert(*this);

	m_bVisable = cam.IsVisible(glm::vec3(m_pos.x - 50.0f,m_pos.y - 50.0f,-100.0f),glm::vec3(m_pos.x + 50.0f,m_pos.y + 50.0f,-100.0f));
	m_fAngle = atan2(m_velocity.y, m_velocity.x) + glm::radians(-90.0f);

	return m_iHealth <= 0;
}

void SpaceShip::Render(IRenderer& renderer)
{
	if(m_bVisable)
	{
		float w = m_collisonPolygon.GetCircle().r * 2;
		glm::mat4 T = glm::translate(m_pos);
		T = glm::rotate(T,m_fAngle,glm::vec3(0,0,1));
		T = glm::scale(T,glm::vec3(w,w,1.0f));

		glm::vec4 color = m_bCollison ? glm::vec4(0.9f,0.3f,0.3f,1.0f) : glm::vec4(1.0f);

		renderer.DrawSprite(m_sprite,T,color,glm::vec2(1.0f),m_tile);

		m_gui.Render(renderer);
	}
}

void SpaceShip::MoveTo(const glm::vec3 &target)
{
	m_target = target;
	m_target.z = m_pos.z;

	m_bMoveToTarget = true;
}

void SpaceShip::WrapIfNeeded(const Math::FRECT& R)
{
	if(m_pos.x > R.bottomRight.x)
	{
		m_pos.x = R.topLeft.x;
	}
	else if(m_pos.x < R.topLeft.x)
	{
		m_pos.x = R.bottomRight.x;
	}

	if(m_pos.y > R.topLeft.y)
	{
		m_pos.y = R.bottomRight.y;
	}
	else if(m_pos.y < R.bottomRight.y)
	{
		m_pos.y = R.topLeft.y;
	}
}

void SpaceShip::PrepareToDie()
{
	//m_bDrawDeath = true;
}

