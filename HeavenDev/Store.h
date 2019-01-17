#pragma once
#include "Includes.h"

class Store : public Singleton<Store>
{

public:
	void Start();
	bool triggerOn, wallOn, wallLegitOn, bhopOn, rcsOn, noFlashOn, fakelagOn, debugOn, aimbotOn;
	SModule Client_DLL;
	SModule Engine_DLL;
	LocalPlayer player;
	EntityList entity_list[64];
	EntityCrossHair enemy;
	MapInfo map;
	PlayerInfo player_info;
};