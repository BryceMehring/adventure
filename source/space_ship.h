#ifndef _SPACE_SHIP_
#define _SPACE_SHIP_

#include <glm/glm.hpp>

/*class IRenderable
{

};*/

class SpaceShip
{
public:

	SpaceShip();

	virtual void Update(float dt, class Camera*);
	virtual void Render(class IRenderer&);


protected:

	glm::vec3 m_pos;
	float m_fAngle;
	float m_fSpeed;
};

class UserControlledSpaceShip : public SpaceShip
{
public:

	UserControlledSpaceShip();

	virtual void Update(float dt, class Camera*, class IKMInput&);

private:

	//virtual void Render(class IRenderer&);

};


#endif // _SPACE_SHIP_
