#include "AISpaceShip.h"
#include <glm/gtc/random.hpp>

AISpaceShip::AISpaceShip(unsigned int tile, const glm::vec3& pos) : SpaceShip(tile,pos), m_fTime(0.0f)
{
	m_fSpeed = glm::linearRand(10.0f,100.0f);
	m_fAngle = glm::linearRand(0.0f,360.0f);
}

bool AISpaceShip::Update(float dt, Camera* pCam, QuadTree& tree)
{
	//if(m_bVisable)
	{
		//m_fAngle += 100.0f * dt;
		//m_fSpeed += 10.0f * dt;
	}

	std::vector<ISpatialObject*> nearObjects;
	tree.QueryNearObjects(this,nearObjects);

	m_bCollison = !nearObjects.empty();

	return SpaceShip::Update(dt,pCam,tree);
}
