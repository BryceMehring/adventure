#ifndef _SPACESHIP_
#define _SPACESHIP_

#include <glm/glm.hpp>
#include "QuadTree.h"
#include "../IDestroyable.h"
#include "../SpriteAnimation.h"
#include "GUI.h"
#include "ProgressBar.h"
#include "Camera.h"

#include <string>


class SpaceShip : public ISpatialObject, public IDestroyable
{
public:

	SpaceShip(const std::string& str, unsigned int tile, float s, const glm::vec3& pos = glm::vec3(0.0f,0.0f,-100.0f));
	virtual ~SpaceShip();

	const glm::vec3& GetPos() const override;
	glm::vec3 GetDir() const;
	float GetRadius() const;

	void* QueryInterface(unsigned int) const override;

	const Math::ICollisionPolygon& GetCollisionPolygon() const override;

	virtual bool Update(float dt, Camera&, QuadTree& tree);
	virtual void Render(class IRenderer&);

	void MoveTo(const glm::vec3& target);

	void Destroy() override;

	static const unsigned int INTERFACE_SPACESHIP = 4272475246;

protected:

	std::string m_sprite;

	glm::vec3 m_pos;
	glm::vec3 m_target;

	Math::CCircle m_collisonPolygon;
	unsigned int m_tile;
	float m_fAngle;
	float m_fSpeed;
	bool m_bVisable;
	bool m_bCollison;
	int m_iHealth;
	UI::GUI m_gui;
	UI::ProgressBar* m_pProgressBar;


	void PrepareToDie();

};




#endif // _SPACESHIP_
