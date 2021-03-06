#ifndef _AISPACESHIP_
#define _AISPACESHIP_

#include "SpaceShip.h"

class AISpaceShip : public SpaceShip
{
public:

	AISpaceShip(const std::string& sprite,
				unsigned int tile,
				float size,
				float repulsiveness,
				const glm::vec3& pos = glm::vec3(0.0f,0.0f,-100.0f));

	virtual bool Update(float dt, Camera& cam, QuadTree& tree);

	static void SetCohesionWeight(float w);
	static void SetAlignmentWeight(float w);
	static void SetSeparationWeight(float w);
	static void SetUserWeight(float w);
	static void SetMaxForce(float w);

private:

	glm::vec3 Cohere(const std::vector<ISpatialObject*>& nearObjects);
	glm::vec3 Align(const std::vector<ISpatialObject*>& nearObjects);
	glm::vec3 Separation(const std::vector<ISpatialObject*>& nearObjects);
	glm::vec3 SteerTo(const glm::vec3& target);

	void LimitVector(glm::vec3 &vec, float length) const;

	static float COHESION_WEIGHT;
	static float ALIGNMENT_WEIGHT;
	static float SEPARATION_WEIGHT;
	static float USER_WEIGHT;
	static float MAX_FORCE;
};

#endif // _AISPACESHIP_
