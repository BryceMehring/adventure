#ifndef _SPACESHIP_
#define _SPACESHIP_

#include <glm/glm.hpp>
#include "QuadTree.h"
#include "../IDestroyable.h"

#include <string>

class SpaceShip : public ISpatialObject, public IDestroyable
{
public:

	SpaceShip(const std::string& str, unsigned int tile, const glm::vec3& pos = glm::vec3(0.0f,0.0f,-100.0f));

	virtual const glm::vec3& GetPos() const { return m_pos; }
	virtual glm::vec2 GetDir() const { return glm::vec2(0.0f); }
	virtual float GetSpeed() const { return m_fSpeed; }

	// todo: implement
	virtual void* QueryInterface(unsigned int) const;

	virtual const Math::ICollisionPolygon& GetCollisionPolygon() const;

	virtual bool Update(float dt, class Camera*, QuadTree& tree);
	virtual void Render(class IRenderer&);

	// todo: implement
	virtual void Destroy() {}

	static const unsigned int INTERFACE_SPACESHIP = 4272475246;

protected:

	std::string m_sprite;
	glm::vec3 m_pos;
	Math::CCircle m_collisonPolygon;
	unsigned int m_tile;
	float m_fAngle;
	float m_fSpeed;
	bool m_bVisable;
	bool m_bDrawLazers;
	bool m_bCollison;
};




#endif // _SPACESHIP_
