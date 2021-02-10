#pragma once
#include "Reversed.h"
#include "../GrailLib/GLDraw.h"

//Scaling
const int VIRTUAL_SCREEN_WIDTH = 800;
const int GAME_UNIT_MAGIC = 400;

const float PLAYER_HEIGHT = 5.25f;
const float PLAYER_WIDTH = 2.0f;
const float EYE_HEIGHT = 4.5f;
const float PLAYER_ASPECT_RATIO = PLAYER_HEIGHT / PLAYER_WIDTH;

const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;

class ESP
{
public:
	int viewport[4];

	bool IsTeamGame();
	bool IsEnemy(PlayerEnt* e);
	bool IsValidEnt(PlayerEnt* ent);

	void DrawESPBox(PlayerEnt* p, Vector3 screen, GL::Font& font);
	void Draw(GL::Font& font);
};