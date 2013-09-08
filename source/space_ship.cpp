#include "space_ship.h"
#include "IRenderer.h"
#include "IKMInput.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>
#include <sstream>

SpaceShip::SpaceShip(unsigned int tile, const glm::vec3& pos) : m_pos(pos), m_tile(tile), m_fAngle(0.0f), m_fSpeed(50.0f), m_bVisable(false)
{
}

void SpaceShip::Update(float dt, Camera* pCam)
{
	m_pos += m_fSpeed * glm::vec3(-sin(m_fAngle * 3.14f / 180.0f),cos(m_fAngle * 3.14f / 180.0f),0.0f) * dt;

	m_bVisable = pCam->IsVisible(glm::vec3(m_pos.x - 50.0f,m_pos.y - 50.0f,-100.0f),glm::vec3(m_pos.x + 50.0f,m_pos.y + 50.0f,-100.0f));

	m_fSpeed -= 10.0f * dt;
	m_fSpeed = glm::clamp(m_fSpeed,0.0f,1000.0f);
}

void SpaceShip::Render(IRenderer& renderer)
{
	if(m_bVisable)
	{
		glm::mat4 T = glm::translate(m_pos.x,m_pos.y,m_pos.z);
		T = glm::scale(T,50.0f,50.0f,1.0f);
		T = glm::rotate(T,m_fAngle,glm::vec3(0,0,1));

		renderer.DrawSprite("ship",T,glm::vec2(1.0f),m_tile);
	}
}

AISpaceShip::AISpaceShip(unsigned int tile, const glm::vec3& pos) : SpaceShip(tile,pos), m_fTime(0.0f)
{
	m_fSpeed = glm::linearRand(10.0f,100.0f);
	m_fAngle = glm::linearRand(0.0f,360.0f);
}

void AISpaceShip::Update(float dt, Camera* pCam)
{
	m_fAngle += 100.0f * dt;
	m_fSpeed += 10.0f * dt;

	SpaceShip::Update(dt,pCam);
}

UserControlledSpaceShip::UserControlledSpaceShip() : SpaceShip(3)
{
}

void UserControlledSpaceShip::Update(float dt, Camera* pCam, IKMInput& input)
{
	if(input.KeyDown('W',false))
	{
		m_fSpeed += 100.0f * dt;
	}
	if(input.KeyDown('A',false))
	{
		m_fAngle += 100.0f * dt;
	}
	if(input.KeyDown('S',false))
	{
		m_fSpeed -= 100.0f * dt;
	}
	if(input.KeyDown('D',false))
	{
		m_fAngle -= 100.0f * dt;
	}

	if(input.KeyDown(265))
	{
		m_tile = (m_tile + 1) % 5;
	}

	m_pos.z += 20.0f*input.MouseZ();

	SpaceShip::Update(dt,pCam);

	pCam->lookAt(glm::vec3(m_pos.x,m_pos.y,-m_pos.z),glm::vec3(m_pos.x,m_pos.y,0.0f),glm::vec3(0,1,0));
	pCam->update();

}
