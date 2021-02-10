#pragma once
#include "../GrailLib/Util.h"


enum class CurrentGamemode : int32_t
{
	TEAMDEATHMATCH = 0,
	DEATHMATCH = 2,
	SURVIVOR = 3,
	TEAMSURVIVOR = 4,
	CTF = 5,
	PISTOLFRENZY = 6,
	BOTTEAMDEATHMATCH = 7,
	BOTDEATHMATCH = 8,
	LASTSWISSSTANDING = 9,
	ONESHOTONEKILL = 10,
	TEAMONESHOTONEKILL = 11,
	BOTONESHOTONEKILL = 12,
	HUNTTHEFLAG = 13,
	TEAMKEEPTHEFLAG = 14,
	KEEPTHEFLAG = 15,
	TEAMPISTOLFFRENZY = 16,
	TEAMLASTSWISSSTANDING = 17,
	BOTPISTOLFRENZY = 18,
	BOTLASTSWISSTANDING = 19,
	BOTTEAMSURVIVOR = 20,
	BOTTEAMONESHOTONKILL = 21
};

enum class PlayerState : int8_t
{
	Normal = 0,
	Invisible = 3,
	Spectator = 4
};

class PlayerEnt
{
public:
	uint32_t Vtable; //0x0000
	Vector3 ViewXYZ; //0x0004
	Vector3 VelXYZ; //0x0010
	Vector3 ViewVelXYZ1; //0x001C
	Vector3 ViewVelXYZ2; //0x0028
	Vector3 XYZ; //0x0034
	Vector3 YawPitchRoll; //0x0040
	float VelRecoilPitch; //0x004C
	char pad_0050[4]; //0x0050
	float VelPitch; //0x0054
	char pad_0058[40]; //0x0058
	int8_t N00000072; //0x0080
	int8_t N00000BF5; //0x0081
	PlayerState PlayerState; //0x0082
	int8_t N00000BF6; //0x0083
	char pad_0084[116]; //0x0084
	int32_t Health; //0x00F8
	char pad_00FC[72]; //0x00FC
	int32_t AmmoShotgun; //0x0144
	char pad_0148[8]; //0x0148
	int32_t AmmoAssault; //0x0150
	char pad_0154[16]; //0x0154
	int32_t RecoilIdlePistol; //0x0164
	char pad_0168[4]; //0x0168
	int32_t RecoilIdleShotgun; //0x016C
	char pad_0170[8]; //0x0170
	int32_t RRRecoilIdleAssault; //0x0178
	char pad_017C[168]; //0x017C
	int8_t CurrShooting; //0x0224
	char Name[15]; //0x0225
	char pad_0234[248]; //0x0234
	int8_t Team; //0x032C
	char pad_032D[11]; //0x032D
	int32_t IsDead; //0x0338
	char pad_033C[56]; //0x033C
	class Weapon* CurrWeapon; //0x0374
	char pad_0378[4364]; //0x0378
}; //Size: 0x1484
static_assert(sizeof(PlayerEnt) == 0x1484);

class Weapon
{
public:
	char pad_0000[16]; //0x0000
	class CurrAmmo* CurrAmmo; //0x0010
	char pad_0014[4]; //0x0014
	class WeaponIdle* IdleTime; //0x0018
	char pad_001C[296]; //0x001C
}; //Size: 0x0144
static_assert(sizeof(Weapon) == 0x144);

class CurrAmmo
{
public:
	char pad_0000[40]; //0x0000
	int32_t currAmmo; //0x0028
	char pad_002C[24]; //0x002C
}; //Size: 0x0044
static_assert(sizeof(CurrAmmo) == 0x44);

class WeaponIdle
{
public:
	int32_t weaponIdle; //0x0000
	char pad_0004[64]; //0x0004
}; //Size: 0x0044
static_assert(sizeof(WeaponIdle) == 0x44);

class PlayerList
{
public:
	class PlayerEnt* players[33]; //0x0000
	char pad_0084[188]; //0x0084
}; //Size: 0x0140
static_assert(sizeof(PlayerList) == 0x140);

class Gamemode
{
public:
	CurrentGamemode CurrGamemode; //0x0000
}; //Size: 0x0004
static_assert(sizeof(Gamemode) == 0x4);