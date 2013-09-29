#ifndef _SPACE_SHIP_BEAM_
#define _SPACE_SHIP_BEAM_

#include "QuadTree.h"
#include "../../IDestroyable.h"
#include <glm/glm.hpp>

class SpaceShipBeam : public ISpatialObject, public IDestroyable
{
public:

	static const unsigned int INTERFACE_BEAM = 538356732;

	SpaceShipBeam(const glm::vec3& pos, float dir, float speed);

	// ISpatialObject
	virtual const glm::vec3& GetPos() const;
	virtual glm::vec2 GetDir() const;
	virtual float GetSpeed() const;

	virtual void* QueryInterface(unsigned int) const;

	virtual const Math::ICollisionPolygon& GetCollisionPolygon() const;

	virtual bool Update(float dt, QuadTree& tree);
	virtual void Render(class IRenderer& renderer);

	// IDestroyable
	virtual void Destroy();

private:

	glm::vec3 m_pos;
	Math::CCircle m_collisionPoly;
	float m_speed;
	float m_dir;
	float m_age;
	bool m_bDestroyed;

};

#endif // _SPACE_SHIP_BEAM_
