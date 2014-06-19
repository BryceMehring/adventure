#ifndef _AISPACESHIP_
#define _AISPACESHIP_

#include "SpaceShip.h"

class AISpaceShip : public SpaceShip
{
public:

	AISpaceShip(const std::string& str, unsigned int tile, float s,const glm::vec3& pos = glm::vec3(0.0f,0.0f,-100.0f));

	virtual bool Update(float dt, Camera& cam, QuadTree& tree);

private:

	glm::vec3 Cohere(const std::vector<ISpatialObject*>& nearObjects);
	glm::vec3 Align(const std::vector<ISpatialObject*>& nearObjects);
	glm::vec3 Seperation(const std::vector<ISpatialObject*>& nearObjects);
	glm::vec3 SteerTo(const glm::vec3& target);

	void LimitVector(glm::vec3 &vec, float length) const;
};

#endif // _AISPACESHIP_
