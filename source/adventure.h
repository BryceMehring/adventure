#ifndef _ADVENTURE_
#define _ADVENTURE_

#include "IGameState.h"
#include "RTTI.h"

class adventure : public IGameState
{
public:

	RTTI_DECL

	// returns the type of the plug-in
	virtual DLLType GetPluginType() const { return DLLType::Game; }

	// returns the name of the plug-in
	virtual const char* GetName() const { return "adventure"; }

	// returns the version number of the plugin
	virtual int GetVersion() const { return 0; }

	virtual void Init(class asIScriptEngine*) {}
	virtual void Destroy(class asIScriptEngine*) {}


	// Called only once when the plugin is created
	virtual void Init(IGame& game);

	// Called only once when the plugin is destroyed
	virtual void Destroy(IGame& game);

	// Called every frame to update the date of the game
	virtual void Update(IGame& game);

	// Called every frame to render the game
	virtual void Draw(IGame& game);
};

#endif // _ADVENTURE_
