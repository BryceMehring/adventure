#ifndef _ADVENTURE_
#define _ADVENTURE_

#include "IGameState.h"
#include "Camera.h"
#include "QuadTree.h"
#include "GUI.h"
#include "ProgressBar.h"
#include "GUIFactory.h"
#include "ships/SpaceShip.h"

#include <vector>
#include <deque>
#include <memory>
#include <set>

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

	// Called only once when the plugin is created
	virtual void Init(Game& game);

	// Called only once when the plugin is destroyed
	virtual void Destroy(Game& game);

	// Called every frame to update the date of the game
	virtual void Update(Game& game);

	// Called every frame to render the game
	virtual void Draw(Game& game);

	void ToggleCallback(UI::Button& button);

private:

	void UpdateUserInput(Game&);

	void BuildGUI(Game& game);

	std::vector<std::unique_ptr<SpaceShip>> m_ships;
	std::deque<std::pair<glm::vec3,SpriteAnimation>> m_deathAnimation;

	PerspectiveCamera m_camera;
	glm::vec3 m_cameraPos = glm::vec3(0, 0, 1000);
	float m_fCameraZVelocity = 0.0f;

	QuadTree m_quadTree;
	bool m_bRenderQuadTree;
	bool m_bDisableColorClearing = true;

	std::set<ISpatialObject*> m_selectedObjects;

	glm::ivec2 m_Min;
	glm::ivec2 m_Max;
	bool m_drawSelectionQuad = false;

	glm::vec3 m_unprojectedMin;
	glm::vec3 m_unprojectedMax;

	struct StarData
	{
		glm::vec2 offset;
		float color;
	};

	std::vector<StarData> m_starOffsets;

	// GUI members
	UI::GUI m_gui;
	UI::GUI::HANDLE m_rootNode;
	UI::GUI::HANDLE m_optionsNode;
	bool m_bEnableOptionsMenu = false;

};

#endif // _ADVENTURE_
