#include "AISpaceShip.h"
#include <glm/gtc/random.hpp>
#include <glm/gtx/transform.hpp>

#include "IRenderer.h"

void callback() {}

AISpaceShip::AISpaceShip(const std::string& str, unsigned int tile, float s, const glm::vec3& pos) : SpaceShip(str,tile,s,pos)
{
	m_fSpeed = glm::linearRand(50.0f,100.0f);
	m_fAngle = glm::linearRand(0.0f,360.0f);
}

bool AISpaceShip::Update(float dt, Camera& cam, QuadTree& tree)
{
	std::vector<ISpatialObject*> nearObjects;
	tree.QueryNearObjects(this,nearObjects);

	m_bCollison = !nearObjects.empty();

	return SpaceShip::Update(dt,cam,tree);
}

SquidSpaceShip::SquidSpaceShip(const std::string& str, unsigned int tile, float s, const glm::vec3& pos) : AISpaceShip(str,tile, s,pos)
{
	m_dt = 0;
	m_direction = 0;
	m_spriteAnimationTime = 0;
	m_frame = 0;
	m_fAngle = 0;
	m_direction_time = 5 + rand()%10;
}

bool SquidSpaceShip::Update(float dt, Camera& cam, QuadTree& tree)
{
	m_dt += dt;
	if(m_dt >= m_direction_time)
	{
		m_direction = (m_direction + 1) % 4;
		m_dt = 0;

		m_frame = m_direction*3;
	}

	m_spriteAnimationTime += dt;

	if(m_spriteAnimationTime >= 0.3)
	{
		m_tile = m_frame+(m_tile+1)%3;
		m_spriteAnimationTime = 0;
	}

	const glm::vec3 direction_array[4] = {glm::vec3(0,-1,0),glm::vec3(-1,0,0),glm::vec3(1,0,0),glm::vec3(0,1,0)};
	m_pos += 50.0f*direction_array[m_direction] * dt;

	return AISpaceShip::Update(dt,cam,tree);
}
