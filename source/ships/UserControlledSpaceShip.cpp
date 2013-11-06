#include "UserControlledSpaceShip.h"
#include "IKMInput.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

UserControlledSpaceShip::UserControlledSpaceShip() : SpaceShip("ship",3, 30.0f,glm::vec3(0.0f,0.0f,-99.0f))
{
}

UserControlledSpaceShip::~UserControlledSpaceShip()
{

}

bool UserControlledSpaceShip::Update(float dt, Camera& cam, IKMInput& input, QuadTree& tree)
{
	if(input.KeyPress('W',false))
	{
		m_fSpeed += 100.0f * dt;
	}
	if(input.KeyPress('A',false))
	{
		m_fAngle += 200.0f * dt;
	}
	if(input.KeyPress('S',false))
	{
		m_fSpeed -= 100.0f * dt;
	}
	if(input.KeyPress('D',false))
	{
		m_fAngle -= 200.0f * dt;
	}

	if(input.KeyPress(265))
	{
		m_tile = (m_tile + 1) % 5;
	}

	if(input.KeyPress(GLFW_KEY_SPACE,false))
	{
		//m_bDrawLazers = !m_bDrawLazers;

		m_beams.push_back(SpaceShipBeam(m_pos + 1.2f*m_collisonPolygon.GetCircle().r*glm::vec3(-sin(m_fAngle * 3.14f / 180.0f),cos(m_fAngle * 3.14f / 180.0f),0.0f),m_fAngle,290.0f));
		tree.Insert(m_beams.back());
	}

	m_pos.z += 100.0f*dt*input.MouseY();

	/*std::vector<ISpatialObject*> nearObjects;
	tree.QueryNearObjects(this,nearObjects);

	m_bCollison = !nearObjects.empty();*/

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

	bool success = SpaceShip::Update(dt,cam,tree);

	cam.lookAt(glm::vec3(m_pos.x,m_pos.y,-m_pos.z),glm::vec3(m_pos.x,m_pos.y,0.0f),glm::vec3(0,1,0));
	cam.update();

	return success;

}

void UserControlledSpaceShip::Render(IRenderer& renderer)
{
	for(auto& iter : m_beams)
	{
		iter.Render(renderer);
	}

	SpaceShip::Render(renderer);
}
