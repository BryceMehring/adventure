#ifndef _USERCONTROLLEDSPACESHIP_
#define _USERCONTROLLEDSPACESHIP_

#include "SpaceShip.h"
#include "weapons/spaceShipBeam.h"

class UserControlledSpaceShip : public SpaceShip
{
public:

	UserControlledSpaceShip();
	virtual ~UserControlledSpaceShip();

	virtual bool Update(float dt, Camera& cam, class IInput&, QuadTree& tree);
	virtual void Render(class IRenderer&);

private:

	std::list<SpaceShipBeam> m_beams;

};

#endif // _USERCONTROLLEDSPACESHIP_
