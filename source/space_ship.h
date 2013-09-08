#ifndef _SPACE_SHIP_
#define _SPACE_SHIP_

#include <glm/glm.hpp>

class SpaceShip
{
public:

	SpaceShip(unsigned int tile, const glm::vec3& pos = glm::vec3(0.0f,0.0f,-100.0f));

	virtual void Update(float dt, class Camera*);
	virtual void Render(class IRenderer&);

protected:

	glm::vec3 m_pos;
	unsigned int m_tile;
	float m_fAngle;
	float m_fSpeed;
	bool m_bVisable;
};

class AISpaceShip : public SpaceShip
{
public:

	AISpaceShip(unsigned int tile,const glm::vec3& pos = glm::vec3(0.0f,0.0f,-100.0f));

	virtual void Update(float dt, class Camera*);

private:

	float m_fTime;

};

class UserControlledSpaceShip : public SpaceShip
{
public:

	UserControlledSpaceShip();

	virtual void Update(float dt, class Camera*, class IKMInput&);

private:

};


#endif // _SPACE_SHIP_
