#pragma once

class Engine : public Singleton<Engine>
{

public:

	EntityList getEntityByIndex(int index);
	void GetEntityListInfo();
	void GetPlayerInfo();
	char* GetMapName();
	void SetPacketSending(bool active);
	int GetCommandNumver();
	int GetChokedCommandNumber();
	void updateBSP();
	void Wall();
	void WallLegit();
	void NoFlash();
	void Trigger();
	void RCS();
	void Start();
	void debug();
	void bhop();
	void fakelag();
	//bool IsConnected();
	bool IsInGame();

	enum SIGNONSTATE
	{
		SIGNONSTATE_NONE = 0,
		SIGNONSTATE_CHALLENGE = 1,
		SIGNONSTATE_CONNECTED = 2,
		SIGNONSTATE_NEW = 3,
		SIGNONSTATE_PRESPAWN = 4,
		SIGNONSTATE_SPAWN = 5,
		SIGNONSTATE_FULL = 6,
		SIGNONSTATE_CHANGELEVEL = 7
	};

};