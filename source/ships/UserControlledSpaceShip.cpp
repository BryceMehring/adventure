#include "UserControlledSpaceShip.h"
#include "IKMInput.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

UserControlledSpaceShip::UserControlledSpaceShip() : SpaceShip(3,glm::vec3(0.0f,0.0f,-50.0f))
{
}

UserControlledSpaceShip::~UserControlledSpaceShip()
{

}

bool UserControlledSpaceShip::Update(float dt, Camera* pCam, IKMInput& input, QuadTree& tree)
{
	if(input.KeyDown('W',false))
	{
		m_fSpeed += 100.0f * dt;
	}
	if(input.KeyDown('A',false))
	{
		m_fAngle += 200.0f * dt;
	}
	if(input.KeyDown('S',false))
	{
		m_fSpeed -= 100.0f * dt;
	}
	if(input.KeyDown('D',false))
	{
		m_fAngle -= 200.0f * dt;
	}

	if(input.KeyDown(265))
	{
		m_tile = (m_tile + 1) % 5;
	}

	if(input.KeyDown(GLFW_KEY_SPACE,false))
	{
		//m_bDrawLazers = !m_bDrawLazers;

		m_beams.push_back(SpaceShipBeam(m_pos,m_fAngle,300.0f));
		tree.Insert(m_beams.back());
	}

	m_pos.z += 50.0f*input.MouseZ();

	pCam->lookAt(glm::vec3(m_pos.x,m_pos.y,-m_pos.z),glm::vec3(m_pos.x,m_pos.y,0.0f),glm::vec3(0,1,0));
	pCam->update();

	std::vector<ISpatialObject*> nearObjects;
	tree.QueryNearObjects(this,nearObjects);

	m_bCollison = !nearObjects.empty();

	for(unsigned int i = 0; i < nearObjects.size(); ++i)
	{
		//nearObjects[i]->
	}

	for(auto iter = m_beams.begin(); iter != m_beams.end(); )
	{
		bool dead = iter->Update(dt,tree);
		if(dead)
		{
			tree.Erase(*iter);
			iter = m_beams.erase(iter);

		}
		else
		{
			++iter;
		}
	}

	return SpaceShip::Update(dt,pCam,tree);

}

void UserControlledSpaceShip::Render(IRenderer& renderer)
{
	for(auto& iter : m_beams)
	{
		iter.Render(renderer);
	}
	/*for(unsigned int i = 0;	i < m_beams.size(); ++i)
	{
		m_beams[i].Render(renderer);
	}*/

	SpaceShip::Render(renderer);
}
