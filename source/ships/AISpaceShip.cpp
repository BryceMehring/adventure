#include "AISpaceShip.h"
#include <glm/gtx/transform.hpp>
#include "Random.h"

#include "IRenderer.h"

float AISpaceShip::COHESION_WEIGHT = 0.0f;
float AISpaceShip::ALIGNMENT_WEIGHT = 0.0f;
float AISpaceShip::SEPARATION_WEIGHT = 0.0f;
float AISpaceShip::USER_WEIGHT = 0.0f;
float AISpaceShip::MAX_FORCE = 4.0f;

AISpaceShip::AISpaceShip(const std::string& str, unsigned int tile, float s, const glm::vec3& pos) : SpaceShip(str,tile,s,pos)
{
	Random& rnd = Random::Instance();
	m_fSpeed = rnd.Generate(500.0f,800.0f);
}

bool AISpaceShip::Update(float dt, Camera& cam, QuadTree& tree)
{
	Math::CCircle tempCircle = m_collisonPolygon;
	tempCircle.GetCircle().r *= 5;

	std::vector<ISpatialObject*> nearObjects;
	tree.QueryNearObjects(tempCircle, nearObjects);

	glm::vec3 targetVector;
	if(m_bMoveToTarget)
	{
		glm::vec3 diff = m_target - m_pos;
		float length = glm::length(diff);
		diff /= length;
		targetVector = diff;

		if(length < 100.0f)
		{
			m_bMoveToTarget = false;
		}
	}

	glm::vec3 acceleration = Cohere(nearObjects) * COHESION_WEIGHT +
							 Align(nearObjects) * ALIGNMENT_WEIGHT +
							 Separation(nearObjects) * SEPARATION_WEIGHT +
							 targetVector * USER_WEIGHT;

	m_velocity += acceleration;
	LimitVector(m_velocity, m_fSpeed);

	m_pos.x += m_velocity.x * dt;
	m_pos.y += m_velocity.y * dt;

	return SpaceShip::Update(dt,cam,tree);
}

void AISpaceShip::SetCohesionWeight(float w)
{
	COHESION_WEIGHT = w;
}

void AISpaceShip::SetAlignmentWeight(float w)
{
	ALIGNMENT_WEIGHT = w;
}

void AISpaceShip::SetSeparationWeight(float w)
{
	SEPARATION_WEIGHT = w;
}

void AISpaceShip::SetUserWeight(float w)
{
	USER_WEIGHT = w;
}

void AISpaceShip::SetMaxForce(float w)
{
	MAX_FORCE = w;
}

glm::vec3 AISpaceShip::Cohere(const std::vector<ISpatialObject *> &nearObjects)
{
	glm::vec3 sum;
	unsigned int count = 0;
	for(auto iter : nearObjects)
	{
		SpaceShip* pShip = static_cast<SpaceShip*>(iter->QueryInterface(INTERFACE_SPACESHIP));
		if(pShip != nullptr)
		{
			sum += pShip->GetPos();
			count++;
		}
	}

	if(count > 0)
	{
		sum = SteerTo(sum / (float)count);
	}

	return sum;
}

glm::vec3 AISpaceShip::Align(const std::vector<ISpatialObject *> &nearObjects)
{
	glm::vec3 mean;
	unsigned int count = 0;
	for(auto iter : nearObjects)
	{
		SpaceShip* pShip = static_cast<SpaceShip*>(iter->QueryInterface(INTERFACE_SPACESHIP));
		if(pShip != nullptr)
		{
			mean += pShip->GetDir();
			count++;
		}
	}

	if(count > 0)
	{
		mean /= (float)count;
	}

	LimitVector(mean, MAX_FORCE);

	return mean;
}

glm::vec3 AISpaceShip::Separation(const std::vector<ISpatialObject *> &nearObjects)
{
	glm::vec3 mean;
	unsigned int count = 0;
	for(auto iter : nearObjects)
	{
		SpaceShip* pShip = static_cast<SpaceShip*>(iter->QueryInterface(INTERFACE_SPACESHIP));
		if(pShip != nullptr)
		{
			float d = glm::distance(m_pos, pShip->GetPos());

			if(d > 0 && d < m_collisonPolygon.GetCircle().r * 3)
			{
				mean += glm::normalize(m_pos - pShip->GetPos()) / d;
				count++;
			}
		}
	}

	if(count > 0)
	{
		mean /= count;
	}

	return mean;
}

glm::vec3 AISpaceShip::SteerTo(const glm::vec3 &target)
{
	glm::vec3 desired = target - m_pos;
	glm::vec3 steer;
	float d = glm::length(desired);

	if(d > 0)
	{
		desired /= d;

		if(d < 100.0f)
		{
			desired *= m_fSpeed * d / 100.0f;
		}
		else
		{
			desired *= m_fSpeed;
		}

		steer = desired - m_velocity;
		LimitVector(steer, MAX_FORCE);
	}

	return steer;
}

void AISpaceShip::LimitVector(glm::vec3 &vec, float length) const
{
	float currentLength = glm::length(vec);
	if(currentLength > length)
	{
		vec /= currentLength;
		vec *= length;
	}
}
