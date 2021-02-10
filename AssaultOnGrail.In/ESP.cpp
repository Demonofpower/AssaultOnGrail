#include "pch.h"
#include "ESP.h"

bool ESP::IsTeamGame()
{
	int* gameMode = (int*)(0x50F49C);
	if ((*gameMode == 0 || *gameMode == 4 || *gameMode == 5 || *gameMode == 7 || *gameMode == 11 || *gameMode == 13 ||
		*gameMode == 14 || *gameMode == 16 || *gameMode == 17 || *gameMode == 20 || *gameMode == 21))
		return true;
	else return false;
}

bool ESP::IsEnemy(PlayerEnt* e)
{
	PlayerEnt* localPlayer = *(PlayerEnt**)0x50F4F4;
	if (localPlayer->Team == e->Team)
		return false;
	else return true;
}

bool ESP::IsValidEnt(PlayerEnt* ent)
{
	if (ent)
	{
		if (ent->Vtable == 0x4E4A98 || ent->Vtable == 0x4E4AC0)
		{
			return true;
		}
	}

	return false;
}

void ESP::DrawESPBox(PlayerEnt* e, Vector3 screen, GL::Font& font)
{
	PlayerEnt* localPlayer = *(PlayerEnt**)0x50F4F4;
	const GLubyte* color = nullptr;

	if (IsTeamGame() && !IsEnemy(e))
		color = RGB::GREEN;
	else color = RGB::RED;

	float dist = Util::Distance(localPlayer->XYZ, e->XYZ);

	float scale = (GAME_UNIT_MAGIC / dist) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);
	float x = screen.x - scale;
	float y = screen.y - scale * PLAYER_ASPECT_RATIO;
	float width = scale * 2;
	float height = scale * PLAYER_ASPECT_RATIO * 2;

	GL::DrawOutline(x, y, width, height, 2.0f, color);

	GL::DrawFilledRect(x - 10, y + height / 100.0f * (100 - e->Health), 3.0f, height / 100.0f * e->Health, RGB::BLUE);

	float textX = font.CenterText(x, width, strlen(e->Name) * ESP_FONT_WIDTH);
	float textY = y - ESP_FONT_HEIGHT / 2;
	std::string playerTxt = e->Name;
	/*playerTxt += " Health: " + std::to_string(e->Health);
	playerTxt += e->IsDead ? " Dead" : " Alive";*/
	font.Print(textX, textY, color, "%s", Util::stringToCharPtr(playerTxt));
}

void ESP::Draw(GL::Font& font)
{
	PlayerList* entlist = *(PlayerList**)0x50F4F8;
	int* numOfPlayers = (int*)(0x50f500);
	float* matrix = (float*)(0x501AE8);
	
	glGetIntegerv(GL_VIEWPORT, viewport);

	for (int i = 0; i < (*numOfPlayers); i++)
	{
		if (entlist && entlist->players && IsValidEnt(entlist->players[i]))
		{
			PlayerEnt* e = entlist->players[i];

			if(e->IsDead)
			{
				continue;
			}
			
			Vector3 center = e->ViewXYZ;
			center.z = center.z - EYE_HEIGHT + PLAYER_HEIGHT / 2;

			Vector3 screenCoords;

			if (Util::WorldToScreen(center, screenCoords, matrix, viewport[2], viewport[3]))
			{
				DrawESPBox(e, screenCoords, font);
			}
		}
	}
}
