#include "space_ship.h"
#include "IRenderer.h"
#include "IKMInput.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <sstream>

SpaceShip::SpaceShip() : m_pos(0.0f,0.0f,-100.0f), m_fAngle(0.0f), m_fSpeed(50.0f)
{
}

void SpaceShip::Update(float dt, Camera* pCam)
{
	m_pos += m_fSpeed * glm::vec3(sin(m_fAngle * 3.14f / 180.0f),-cos(m_fAngle * 3.14f / 180.0f),0.0f) * dt;

	if(m_pos.x > 4000.0f)
	{
		m_pos.x = -4000.0f;
	}

	m_fSpeed -= 10.0f * dt;
	m_fSpeed = glm::clamp(m_fSpeed,0.0f,1000.0f);

	pCam->lookAt(glm::vec3(m_pos.x,m_pos.y,-m_pos.z),glm::vec3(m_pos.x,m_pos.y,0.0f),glm::vec3(0,1,0));
	pCam->update();
}

void SpaceShip::Render(IRenderer& renderer)
{
	glm::mat4 T = glm::translate(m_pos.x,m_pos.y,m_pos.z);
	T = glm::scale(T,100.0f,100.0f,1.0f);
	T = glm::rotate(T,m_fAngle,glm::vec3(0,0,1));

	renderer.DrawSprite("ship",T);


	std::stringstream stream;
	stream << "("<<int(m_pos.x) << ","<<int(m_pos.y)<<")";
	renderer.DrawString(stream.str().c_str(),m_pos,glm::vec2(20.0f,25.0f),glm::vec3(1),0,::FontAlignment::Center);
}

UserControlledSpaceShip::UserControlledSpaceShip()
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
		//m_pos.x -= scale * dt;
		m_fAngle += 100.0f * dt;
	}
	if(input.KeyDown('S',false))
	{
		m_fSpeed -= 100.0f * dt;
	}
	if(input.KeyDown('D',false))
	{
		//m_pos.x += scale * dt;
		m_fAngle -= 100.0f * dt;
	}

	m_pos.z += 20.0f*input.MouseZ();

	SpaceShip::Update(dt,pCam);
}

/*void UserControlledSpaceShip::Render(class IRenderer&)
{

}*/
