#include "spaceShipBeam.h"
#include "IRenderer.h"
#include "../SpaceShip.h"
#include <glm/gtx/transform.hpp>

SpaceShipBeam::SpaceShipBeam(const glm::vec3& pos, float dir, float speed) :
m_pos(pos), m_dir(dir), m_speed(speed), m_age(0), m_collisionPoly(Math::Circle(glm::vec2(pos.x,pos.y),15.0f)), m_bDestroyed(false)
{
}

// ISpatialObject
const glm::vec3& SpaceShipBeam::GetPos() const
{
	return m_pos;
}
glm::vec2 SpaceShipBeam::GetDir() const
{
	return glm::vec2(0.0f);
}
float SpaceShipBeam::GetSpeed() const
{
	return m_speed;
}

void* SpaceShipBeam::QueryInterface(unsigned int i) const
{
	if(i == INTERFACE_BEAM)
		return (void*)this;

	if(i == IDestroyable::INTERFACE_DESTROY)
		return (void*)((IDestroyable*)this);

	return nullptr;
}

const Math::ICollisionPolygon& SpaceShipBeam::GetCollisionPolygon() const
{
	return m_collisionPoly;
}

// IDestroyable
void SpaceShipBeam::Destroy()
{
	m_bDestroyed = true;
}

bool SpaceShipBeam::Update(float dt, QuadTree& tree)
{
	glm::vec3 dir(-sin(m_dir * 3.14f / 180.0f),cos(m_dir * 3.14f / 180.0f),0.0f);
	m_pos += dir * m_speed * dt;
	m_age += dt;

	/*tree.Erase(*this);
	m_collisionPoly.GetCircle().center = glm::vec2(m_pos.x,m_pos.y);
	tree.Insert(*this);

	std::vector<ISpatialObject*> nearObj;
	tree.QueryNearObjects(this,nearObj);

	for(unsigned int i = 0; i < nearObj.size(); ++i)
	{
		void* pInterface = nearObj[i]->QueryInterface(IDestroyable::INTERFACE_DESTROY);
		void* pShipInterface = nearObj[i]->QueryInterface(SpaceShip::INTERFACE_SPACESHIP);

		if(pInterface != nullptr && pShipInterface == nullptr)
		{
			IDestroyable* pDestroyable = static_cast<IDestroyable*>(pInterface);
			pDestroyable->Destroy();
		}
	}*/

	return m_bDestroyed || m_age > 2.7f;
}

void SpaceShipBeam::Render(IRenderer& renderer)
{
	float r = m_collisionPoly.GetCircle().r;

	glm::mat4 T = glm::translate(m_pos.x,m_pos.y,-100.0f);

	renderer.DrawSprite("beam",glm::scale(T,r,r,1.0f));
}
