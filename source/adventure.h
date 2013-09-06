#ifndef _ADVENTURE_
#define _ADVENTURE_

#include "IGameState.h"
#include "space_ship.h"
#include "Camera.h"

class adventure : public IGameState
{
public:

	// returns the type of the plug-in
	virtual DLLType GetPluginType() const { return DLLType::Game; }

	// returns the name of the plug-in
	virtual const char* GetName() const { return "adventure"; }

	// returns the version number of the plugin
	virtual int GetVersion() const { return 0; }

	virtual void Init(class asIScriptEngine*) {}
	virtual void Destroy(class asIScriptEngine*) {}

	// Called only once when the plugin is created
	virtual void Init(Game& game);

	// Called only once when the plugin is destroyed
	virtual void Destroy(Game& game);

	// Called every frame to update the date of the game
	virtual void Update(Game& game);

	// Called every frame to render the game
	virtual void Draw(Game& game);

private:

	UserControlledSpaceShip m_spaceShip;
	Camera* m_pCamera;
};

#endif // _ADVENTURE_
