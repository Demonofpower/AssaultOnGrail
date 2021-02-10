#include "pch.h"

#include <cstdio>
#include <iostream>

#include "../GrailLib/Ex.h"
#include "../GrailLib/In.h"
#include "../GrailLib/Util.h"
#include "../GrailLib/GLDraw.h"
#include "Reversed.h"
#include "ESP.h"

typedef BOOL (__stdcall* twglSwapBuffers)(HDC hDc);
twglSwapBuffers owglSwapBuffers;
BOOL __stdcall hkwglSwapBuffers(HDC hDc);

void init();
void deinit(FILE* f, HMODULE hModule);
void ChangeSwapBuffers(bool hook);
void print();
void getKeyStates();
void godmode(bool god);
void unlimitedAmmo(bool infiniteAmmo);
void noRecoil(bool no_recoil_enabled);
void trigger(bool trigger);
void aimbot(bool aim);
void esp(bool esp);
void ninja(bool ninja);
void shootWalls(bool shootWalls);

PlayerEnt* getBestPlayerToAimOn(PlayerEnt* ownPlayer);
bool sameTeam(const PlayerEnt* p1, const PlayerEnt* p2);
bool isTeamMode();

bool end = false;
bool debugEnabled = false;
bool godEnabled = false;
bool unlimitedAmmoEnabled = false;
bool noRecoilEnabled = false;
bool triggerEnabled = false;
bool aimbotEnabled = false;
bool espEnabled = false;
bool ninjaEnabled = false;
bool shootWallEnabled = false;

uintptr_t moduleBase;
PlayerEnt* ownPlayer;
PlayerList* playerList;

DWORD WINAPI HackThread(HMODULE hModule)
{
	FILE* f = 0;
	if(debugEnabled)
	{
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);		
	}

	init();
	print();

	while (!end)
	{
		Sleep(100);
	}

	deinit(f, hModule);
	return 0;
}

BOOL __stdcall hkwglSwapBuffers(HDC hDc) //Main Exec
{
	print();
	getKeyStates();

	godmode(godEnabled);
	unlimitedAmmo(unlimitedAmmoEnabled);
	trigger(triggerEnabled);
	aimbot(aimbotEnabled);
	esp(espEnabled);
	ninja(ninjaEnabled);
	shootWalls(shootWallEnabled);
	
	return owglSwapBuffers(hDc);
}

GL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

const char* watermarkVersion = "AssaultOnGrail v0.8a - Cleaning";
const char* watermarkName = "By Paranoia with <3";
void print()
{
	HDC currentHDC = wglGetCurrentDC();

	HWND hWnd = WindowFromDC(currentHDC);

	RECT window_rect = {};
	GetClientRect(hWnd, &window_rect);
	LONG window_w = window_rect.right - window_rect.left;
	LONG window_h = window_rect.bottom - window_rect.top;

	if (!glFont.bBuilt || currentHDC != glFont.hdc)
	{
		glFont.Build(FONT_HEIGHT);
	}

	GL::SetupOrtho();
	
	glFont.Print(0, window_h / 2 - 8 * 9, RGB::RED, "%s", watermarkVersion);
	glFont.Print(0, window_h / 2 - 8 * 7, RGB::GREEN, "%s", Util::stringToCharPtr("F1 Godmode SP:" + Util::boolToString(godEnabled)));
	glFont.Print(0, window_h / 2 - 8 * 5, RGB::GREEN, "%s", Util::stringToCharPtr("F2 UnlimitedAmmo SP:" + Util::boolToString(unlimitedAmmoEnabled)));
	glFont.Print(0, window_h / 2 - 8 * 3, RGB::GREEN, "%s", Util::stringToCharPtr("F3 NoRecoil:" + Util::boolToString(noRecoilEnabled)));
	glFont.Print(0, window_h / 2 - 8 * 1, RGB::GREEN, "%s", Util::stringToCharPtr("F4 Trigger:" + Util::boolToString(triggerEnabled)));
	glFont.Print(0, window_h / 2 + 8 * 1, RGB::GREEN, "%s", Util::stringToCharPtr("F5 Aimbot:" + Util::boolToString(aimbotEnabled)));
	glFont.Print(0, window_h / 2 + 8 * 3, RGB::GREEN, "%s", Util::stringToCharPtr("F6 ESP:" + Util::boolToString(espEnabled)));
	glFont.Print(0, window_h / 2 + 8 * 5, RGB::GREEN, "%s", Util::stringToCharPtr("F7 Ninja:" + Util::boolToString(ninjaEnabled)));
	glFont.Print(0, window_h / 2 + 8 * 7, RGB::GREEN, "%s", Util::stringToCharPtr("F8 ShootWalls:" + Util::boolToString(shootWallEnabled)));
	glFont.Print(0, window_h / 2 + 8 * 9, RGB::RED, "%s", watermarkName);
	
	GL::RestoreGL();
}

void init()
{
	moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

	ownPlayer = *(PlayerEnt**)(moduleBase + 0x0010F4F4);
	playerList = *(PlayerList**)(moduleBase + 0x0010F4F8);

	ChangeSwapBuffers(true);
}

void deinit(FILE* f, HMODULE hModule)
{
	ChangeSwapBuffers(false);

	if(debugEnabled)
	{
		fclose(f);
		FreeConsole();
	}
	
	FreeLibraryAndExitThread(hModule, 0);
}

void ChangeSwapBuffers(bool hook)
{
	if (hook)
	{
		owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
		owglSwapBuffers = (twglSwapBuffers)In::TrampHook(owglSwapBuffers, hkwglSwapBuffers, 5);
	}
	else
	{
		owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
		In::Patch((BYTE*)owglSwapBuffers, (BYTE*)"\x8B\xFF\x55\x8B\xEC", 5);
	}
}


void getKeyStates()
{
	if (GetAsyncKeyState(VK_END) & 1)
	{
		end = true;
	}
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		godEnabled = !godEnabled;
		print();
	}
	if (GetAsyncKeyState(VK_F2) & 1)
	{
		unlimitedAmmoEnabled = !unlimitedAmmoEnabled;
		print();
	}
	if (GetAsyncKeyState(VK_F3) & 1)
	{
		noRecoilEnabled = !noRecoilEnabled;
		noRecoil(noRecoilEnabled);
		print();
	}
	if (GetAsyncKeyState(VK_F4) & 1)
	{
		triggerEnabled = !triggerEnabled;
		print();
	}
	if (GetAsyncKeyState(VK_F5) & 1)
	{
		aimbotEnabled = !aimbotEnabled;
		print();
	}
	if (GetAsyncKeyState(VK_F6) & 1)
	{
		espEnabled = !espEnabled;
		print();
	}
	if (GetAsyncKeyState(VK_F7) & 1)
	{
		ninjaEnabled = !ninjaEnabled;

		if (!ninjaEnabled)
		{
			ownPlayer->PlayerState = PlayerState::Normal;
		}
		
		print();
	}
	if (GetAsyncKeyState(VK_F8) & 1)
	{
		shootWallEnabled = !shootWallEnabled;
		print();
	}
}

void godmode(bool god)
{
	if (!god)
	{
		return;
	}

	int health = 6969;
	ownPlayer->Health = health;
}

void unlimitedAmmo(bool infiniteAmmo)
{
	if (!infiniteAmmo)
	{
		return;
	}

	int ammo = 6969;
	ownPlayer->CurrWeapon->CurrAmmo->currAmmo = ammo;
}

void noRecoil(bool enable)
{
	if (enable)
	{
		In::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\xEB\x08", 2);
	}
	else
	{
		In::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D", 2);
	}
}

bool alreadyStopped = true;

void trigger(bool trigger)
{
	if (!trigger)
	{
		return;
	}

	const PlayerEnt* playerInCrosshair = (PlayerEnt*)((unsigned int(__cdecl*)(void))0x004607c0)();

	if (playerInCrosshair == 0 || (isTeamMode() && sameTeam(ownPlayer, playerInCrosshair)))
	{
		if (!alreadyStopped)
		{
			ownPlayer->CurrShooting = 0;
			alreadyStopped = true;
		}
	}
	else
	{
		ownPlayer->CurrShooting = 1;
		alreadyStopped = false;
	}
}

void aimbot(bool aim)
{
	if (!aim)
	{
		return;
	}

	PlayerEnt* otherPlayer = getBestPlayerToAimOn(ownPlayer);

	if (otherPlayer == 0)
	{
		return;
	}

	Vector3 ownXYZ = ownPlayer->XYZ;
	Vector3 ownYawPitchRoll = ownPlayer->YawPitchRoll;

	Vector3 otherPlayerXYZ = otherPlayer->XYZ;

	Vector3 aimAngel = Util::CalcAngle(ownXYZ, otherPlayerXYZ);

	ownPlayer->YawPitchRoll.x = aimAngel.x;
	ownPlayer->YawPitchRoll.y = aimAngel.y;
}


PlayerEnt* getBestPlayerToAimOn(PlayerEnt* ownPlayer)
{
	playerList = *(PlayerList**)(moduleBase + 0x0010F4F8);
	int playerCount = *(int*)(moduleBase + 0x0010f500);
	if (playerList == 0 || playerCount == 0)
	{
		return nullptr;
	}

	PlayerEnt* nearestValidPlayer = 0;
	float nearestDist = 999999.0f;
	int currPlayer = 0;
	for (PlayerEnt* otherPlayer : playerList->players)
	{
		currPlayer += 1;
		if (otherPlayer == 0)
		{
			continue;
		}

		if (currPlayer > playerCount)
		{
			break;
		}

		if (isTeamMode() && sameTeam(ownPlayer, otherPlayer))
		{
			continue;
		}

		if (otherPlayer->IsDead)
		{
			continue;
		}

		float currDist = Util::Distance(ownPlayer->XYZ, otherPlayer->XYZ);
		if (nearestDist > currDist)
		{
			nearestDist = currDist;
			nearestValidPlayer = otherPlayer;
		}
	}

	return nearestValidPlayer;
}

bool sameTeam(const PlayerEnt* p1, const PlayerEnt* p2)
{
	return p1->Team == p2->Team;
}

bool isTeamMode()
{
	CurrentGamemode gameMode = ((Gamemode*)(moduleBase + 0x0010F49C))->CurrGamemode;
	return (gameMode == CurrentGamemode::TEAMDEATHMATCH || gameMode == CurrentGamemode::TEAMSURVIVOR || gameMode ==
		CurrentGamemode::CTF || gameMode == CurrentGamemode::BOTTEAMDEATHMATCH || gameMode ==
		CurrentGamemode::HUNTTHEFLAG || gameMode == CurrentGamemode::TEAMONESHOTONEKILL || gameMode ==
		CurrentGamemode::TEAMKEEPTHEFLAG || gameMode == CurrentGamemode::TEAMLASTSWISSSTANDING || gameMode ==
		CurrentGamemode::TEAMPISTOLFFRENZY || gameMode == CurrentGamemode::BOTTEAMSURVIVOR || gameMode ==
		CurrentGamemode::BOTTEAMONESHOTONKILL);
}

ESP espIns;
void esp(bool esp)
{
	if(esp)
	{
		espIns.Draw(glFont);
	}
}

void ninja(bool ninja)
{
	if (ninja)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0 || (GetKeyState(VK_RBUTTON) & 0x8000) != 0)
		{
			ownPlayer->PlayerState = PlayerState::Normal;
		}				
		else
		{
			ownPlayer->PlayerState = PlayerState::Invisible;
		}
	}
}

void shootWalls(bool shootWalls)
{
	if(shootWalls)
	{
		glDisable(GL_DEPTH_TEST);
	}
}


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}