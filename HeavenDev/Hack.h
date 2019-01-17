#ifndef FUNC_H
#define FUNC_H

#include "Includes.h"
struct LocalPlayer {
	DWORD base;
	int isAlive;
	int team;
	int crosshair_id;
	int target;
	int shots_fired;
	int flags;
	QAngle punch;
	Vector position;
	Vector view;
	DWORD client_state;
	int local_index;
	int index;
	QAngle view_angles;
};

struct EntityCrossHair {
	DWORD base;
	int team;
	int health;
};

struct MapInfo {
	char name[32];
};

typedef struct PlayerInfo
{
	__int64         unknown;            //0x0000 
	union
	{
		__int64       steamID64;          //0x0008 - SteamID64
		struct
		{
			__int32     xuid_low;
			__int32     xuid_high;
		};
	};
	char            szName[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            szFriendsName[128];
	bool            fakeplayer;
	bool            ishltv;
	unsigned int    customfiles[4];
	unsigned char   filesdownloaded;
} player_info_t;

struct EntityList {
	DWORD base;
	int team;
	int health;	
	int spotted_mask;
	bool dormant;
	/*
	LIFE_ALIVE = 0, // alive
	LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
	LIFE_DEAD = 2;
	*/
	int index;
	int isAlive;
	int glow_index;
	Vector bone;
	Vector position;
};

struct m_GlowObject
{
	int EntityPointer;
	float r;
	float g;
	float b;
	float a;
	int offset1;
	int offset2;
	float BloomAmount;
	int offset3;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
};

struct PlayerObject
{
public:
	Vector position;
	float size;
	QAngle angle;
	char *name;
	float status;
};

struct SModule
{
public:
	DWORD dwBase;
	DWORD dwSize;
};
#endif

