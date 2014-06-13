#ifndef _ADVENTURE_
#define _ADVENTURE_

#include "IGameState.h"
#include "Camera.h"
#include "QuadTree.h"
#include "ships/UserControlledSpaceShip.h"
#include "GUI.h"
#include "ProgressBar.h"

#include <vector>
#include <deque>
#include <memory>

class adventure : public IGameState
{
public:

	adventure();
	~adventure();

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
	std::vector<std::auto_ptr<SpaceShip>> m_enemies;
	std::deque<std::pair<glm::vec3,SpriteAnimation>> m_deathAnimation;
	PerspectiveCamera m_camera;
	QuadTree m_quadTree;
	UI::GUI m_gui;
	UI::ProgressBar* m_pProgressBar;
	bool m_bRenderQuadTree;

	unsigned int creatureCount;

	std::vector<ISpatialObject*> m_selectedObjects;

	glm::ivec2 m_Min;
	glm::ivec2 m_Max;
	bool m_drawSelectionQuad = false;

	glm::vec3 m_unprojectedMin;
	glm::vec3 m_unprojectedMax;

	void BuildGUI();

};

#endif // _ADVENTURE_
