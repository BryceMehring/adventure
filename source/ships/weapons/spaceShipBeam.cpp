#include "spaceShipBeam.h"
#include "IRenderer.h"
#include "../SpaceShip.h"
#include <glm/gtx/transform.hpp>

SpaceShipBeam::SpaceShipBeam(const glm::vec3& pos, float dir, float speed) :
	m_pos(pos), m_dir(dir), m_speed(speed), m_age(0), m_collisionPoly(Math::Circle(glm::vec2(pos.x,pos.y),7.0f)), m_bDestroyed(false), m_animation(2,40.0)
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

	tree.Erase(*this);
	m_collisionPoly.GetCircle().center = glm::vec2(m_pos.x,m_pos.y);
	tree.Insert(*this);

	std::vector<ISpatialObject*> nearObj;
	tree.QueryNearObjects(this,nearObj);

	for(unsigned int i = 0; i < nearObj.size(); ++i)
	{
		void* pInterface = nearObj[i]->QueryInterface(IDestroyable::INTERFACE_DESTROY);
		void* pBeam = nearObj[i]->QueryInterface(SpaceShipBeam::INTERFACE_BEAM);

		if(pInterface != nullptr && pBeam == nullptr)
		{
			IDestroyable* pDestroyable = static_cast<IDestroyable*>(pInterface);
			pDestroyable->Destroy();
			m_bDestroyed = true;
		}
	}

	m_animation.Update(dt);

	return m_bDestroyed || m_age > 1.5f;
}

void SpaceShipBeam::Render(IRenderer& renderer)
{
	float r = m_collisionPoly.GetCircle().r * 2.0f;

	glm::mat4 T = glm::translate(glm::vec3(m_pos.x,m_pos.y,-100.0f));

	unsigned int tile = m_animation.GetTile();

	renderer.DrawSprite("weapon",glm::scale(T,glm::vec3(r,r,1.0f)),glm::vec4(1.0f),glm::vec2(1.0f),tile);
}
