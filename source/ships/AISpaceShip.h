#ifndef _AISPACESHIP_
#define _AISPACESHIP_

#include "SpaceShip.h"

class AISpaceShip : public SpaceShip
{
public:

	AISpaceShip(unsigned int tile,const glm::vec3& pos = glm::vec3(0.0f,0.0f,-100.0f));

	virtual bool Update(float dt, class Camera*, QuadTree& tree);

private:

	float m_fTime;
};

#endif // _AISPACESHIP_
