#include "Includes.h"

namespace {
	static float aim_angles[3] = { 0.0f, 0.0f, 0.0f };
	//DWORD dwPBase;
}

void Engine::Start() {
    //dwPBase = g_pMemory->Read<DWORD>(client->GetImage() + offsets::dwLocalPlayer);
}

void Store::Start() {
	triggerOn = false;
	wallOn = false;
	bhopOn = false;
	rcsOn = false;
	noFlashOn = false;
	debugOn = false;
	Store::Get().Client_DLL = g_pMemory->GetModule("client_panorama.dll");
	Store::Get().Engine_DLL = g_pMemory->GetModule("engine.dll");
}

std::unique_ptr< Valve::BSPParser > g_pBSPParser = std::make_unique< Valve::BSPParser >();

void Engine::updateBSP()
{
	if (strcmp(GetMapName(), "") == 1)
	{
		if (Engine::Get().IsInGame()) {
			char filename[MAX_PATH];
			if (GetModuleFileNameEx(g_pMemory->getProcess(), NULL, filename, MAX_PATH) == 0) {
				return;
			}

			std::string tmp = filename;
			int pos = tmp.find("csgo");
			tmp = tmp.substr(0, pos);
			tmp = tmp + "csgo\\maps\\";

			std::string file = GetMapName();
			file = file + ".bsp";

			if (g_pBSPParser->parse_map(tmp, file)) {
				static std::string last_map;
				auto bsp_file = g_pBSPParser->get_bsp();
				if (last_map != bsp_file.m_FileName) {
					last_map = bsp_file.m_FileName;
					std::cout << bsp_file << std::endl;
				}
			}

			//Log::Get().ConsolePrint("Local %s", tmp);


			/*Sleep(1000 * 60);
			Log::Get().ConsolePrint("Carregou a bsp");*/
		}
	}
}

EntityList Engine::getEntityByIndex(int index) {

	for (int i = 1; i < 64; i++) {
		if (Store::Get().Store::Get().entity_list[i].index == index)
			return  Store::Get().Store::Get().entity_list[i];
	}


}

void Engine::GetPlayerInfo() {
	DWORD dwClientState = g_pMemory->Read<DWORD>(Store::Get().Engine_DLL.dwBase + offsets::dwClientState);
	Store::Get().player_info = g_pMemory->Read<PlayerInfo>(dwClientState + offsets::dwClientState_PlayerInfo);
	Store::Get().player.base = g_pMemory->Read<DWORD>(Store::Get().Client_DLL.dwBase + offsets::dwLocalPlayer);
	Store::Get().player.team = g_pMemory->Read<int>(Store::Get().player.base + offsets::m_iTeamNum);
	Store::Get().player.isAlive = g_pMemory->Read<int>(Store::Get().player.base + offsets::m_lifeState);
	Store::Get().player.crosshair_id = g_pMemory->Read<int>(Store::Get().player.base + offsets::m_iCrosshairId);
	Store::Get().player.shots_fired = g_pMemory->Read<int>(Store::Get().player.base + offsets::m_iShotsFired);
	Store::Get().player.punch = g_pMemory->Read<QAngle>(Store::Get().player.base + 0x301C);
	Store::Get().player.flags = g_pMemory->Read<int>(Store::Get().player.base + offsets::m_fFlags);;
	Store::Get().player.position = g_pMemory->Read<Vector>(Store::Get().player.base + offsets::m_vecOrigin);
	Store::Get().player.client_state = g_pMemory->Read<DWORD>(Store::Get().Engine_DLL.dwBase + offsets::dwClientState);
	Store::Get().player.local_index = g_pMemory->Read<int>(Store::Get().player.client_state + offsets::dwClientState_GetLocalPlayer);
	Store::Get().player.index = g_pMemory->Read<int>(Store::Get().player.base + 0x64);
	Store::Get().player.view_angles = g_pMemory->Read<QAngle>(Store::Get().player.client_state + offsets::dwClientState_ViewAngles);
	Store::Get().player.view = g_pMemory->Read<Vector>(Store::Get().player.base + offsets::m_vecViewOffset);
}

void Engine::GetEntityListInfo() {
	for (int i = 0; i < 64; i++) {
		Store::Get().entity_list[i].base = g_pMemory->Read<DWORD>(Store::Get().Client_DLL.dwBase + offsets::dwEntityList +(i * 0x10));
		Store::Get().entity_list[i].team = g_pMemory->Read<int>(Store::Get().entity_list[i].base + offsets::m_iTeamNum);
		Store::Get().entity_list[i].health = g_pMemory->Read<int>(Store::Get().entity_list[i].base + offsets::m_iHealth);
		Store::Get().entity_list[i].spotted_mask = g_pMemory->Read<int>(Store::Get().entity_list[i].base + offsets::m_bSpottedByMask);
		Store::Get().entity_list[i].isAlive = g_pMemory->Read<int>(Store::Get().entity_list[i].base + offsets::m_lifeState);
		Store::Get().entity_list[i].dormant = g_pMemory->Read<bool>(Store::Get().entity_list[i].base + 0xE9);//m_bDormant
		Store::Get().entity_list[i].glow_index = g_pMemory->Read<int>(Store::Get().entity_list[i].base + offsets::m_iGlowIndex);
		Store::Get().entity_list[i].position = g_pMemory->Read<Vector>(Store::Get().entity_list[i].base + offsets::m_vecOrigin);
		Store::Get().entity_list[i].index =  g_pMemory->Read<int>(Store::Get().entity_list[i].base + 0x64);
		DWORD bonematrix = g_pMemory->Read<DWORD>(Store::Get().entity_list[i].base + offsets::m_dwBoneMatrix);
		int x = g_pMemory->Read<float>(bonematrix + 8 * 0x30 + 0x0C);
		int y = g_pMemory->Read<float>(bonematrix + 8 * 0x30 + 0x1C);
		int z = g_pMemory->Read<float>(bonematrix + 8 * 0x30 + 0x2C);
		Store::Get().entity_list[i].bone = Vector(x,y,z);
	}
}

char* Engine::GetMapName() {
	if (!Store::Get().Engine_DLL.dwBase)
		return nullptr;
	DWORD dwClientState = g_pMemory->Read<DWORD>(Store::Get().Engine_DLL.dwBase + offsets::dwClientState);
	Store::Get().map = g_pMemory->Read<MapInfo>(dwClientState + offsets::dwClientState_Map);
	return Store::Get().map.name;
}

void Engine::SetPacketSending(bool active)
{
	if (active)
		g_pMemory->Write< BYTE >((Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets), 1);
	else g_pMemory->Write< BYTE >((Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets), 0);
}

int Engine::GetCommandNumver(void)
{
	return g_pMemory->Read< int >(g_pMemory->Read< DWORD >(Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets) + 0x4C24);
}

int Engine::GetChokedCommandNumber(void)
{
	return g_pMemory->Read< int >(g_pMemory->Read< DWORD >(Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets) + 0x4C28);
}

void clampAngles(QAngle &angle) {
	if (angle.yaw > 180.0f)
		angle.yaw = 179.0f;
	if (angle.yaw < -180.0f)
		angle.yaw = -179.0f;

	if (angle.pitch > 89.0f)
		angle.pitch = 88.0f;
	if (angle.pitch < -89.0f)
		angle.pitch = -88.0f;

	angle.roll = 0;
}

void normalizeAngles(QAngle &angle) {
	while (angle.pitch > 180.0F)
		angle.pitch -= 360.0F;
	while (angle.pitch < -180.0F)
		angle.pitch += 360.0F;
	while (angle.yaw > 180.0F)
		angle.yaw -= 360.0F;
	while (angle.yaw < -180.0F)
		angle.yaw += 360.0F;
	while (angle.roll > 180.0F)
		angle.roll -= 360.0F;
	while (angle.roll < -180.0F)
		angle.roll += 360.0F;
}
#define M_PI       3.14159265358979323846   // pi
void calcAngle(Vector src, Vector dst, QAngle aim) {
	Vector delta = src - dst;
	double hyp = sqrt((delta.x * delta.x) + (delta.y * delta.y));
	aim.pitch = atan(delta.z / hyp) * 180 / M_PI;
	aim.yaw = atan(delta.y / delta.x) * 180 / M_PI;
	if (delta.x >= 0.0)
		aim.yaw += 180.0f;
}

static QAngle oldPunch = QAngle();

void Engine::RCS() {
	if (Store::Get().rcsOn) {
		if (Store::Get().player.shots_fired <= 1) {
			oldPunch.pitch = Store::Get().player.punch.pitch;
			oldPunch.yaw = Store::Get().player.punch.yaw;
			return;
		}
		if (Store::Get().player.punch.pitch != 0 || Store::Get().player.punch.yaw != 0) {
			Store::Get().player.view_angles.pitch -= Store::Get().player.punch.pitch  * 2.0f;
			Store::Get().player.view_angles.yaw -= Store::Get().player.punch.yaw * 2.0f;
			Store::Get().player.view_angles.pitch += oldPunch.pitch  * 2.0f;
			Store::Get().player.view_angles.yaw += oldPunch.yaw * 2.0f;
			if (Store::Get().player.view_angles.pitch >= 89) Store::Get().player.view_angles.pitch = 89;
			if (Store::Get().player.view_angles.pitch <= -89) Store::Get().player.view_angles.pitch = -89;
			if (Store::Get().player.view_angles.yaw <= -180) Store::Get().player.view_angles.yaw += 360;
			if (Store::Get().player.view_angles.yaw >= 180) Store::Get().player.view_angles.yaw -= 360;
			//normalizeAngles(Store::Get().player.view_angles);
			//clampAngles(Store::Get().player.view_angles);
			g_pMemory->Write<QAngle>(Store::Get().player.client_state + offsets::dwClientState_ViewAngles, Store::Get().player.view_angles);
		}
		oldPunch.pitch = Store::Get().player.punch.pitch;
		oldPunch.yaw = Store::Get().player.punch.yaw;
	}
}

void Engine::NoFlash() {
	if (Store::Get().noFlashOn) {
		if (g_pMemory->Read<float>(Store::Get().player.base + offsets::m_flFlashMaxAlpha) > 0.0f) {
			g_pMemory->Write<float>(Store::Get().player.base + offsets::m_flFlashMaxAlpha, 0.0f);
		}
	}
}

void Engine::Trigger() {
	if (Store::Get().triggerOn) {
		EntityCrossHair enemy = Store::Get().enemy;
		enemy.base = g_pMemory->Read<DWORD>((Store::Get().Client_DLL.dwBase + offsets::dwEntityList) + ((Store::Get().player.crosshair_id - 1) * 0x10));
		enemy.health = g_pMemory->Read<int>(enemy.base + 0xFC);
		enemy.team = g_pMemory->Read<int>(enemy.base + offsets::m_iTeamNum);
		if (Store::Get().player.team != enemy.team && enemy.health > 0) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
		}
	}
}

void Engine::WallLegit() {
	if (Store::Get().wallLegitOn) {
		for (int i = 0; i < 64; i++)
		{
			g_pMemory->Write<int>(Store::Get().entity_list[i].base + offsets::m_clrRender, 150);//r
			g_pMemory->Write<int>(Store::Get().entity_list[i].base + offsets::m_clrRender + 0x1, 255);//g
			g_pMemory->Write<int>(Store::Get().entity_list[i].base + offsets::m_clrRender + 0x2, 1);//b
			g_pMemory->Write<int>(Store::Get().entity_list[i].base + offsets::m_clrRender + 0x3, 255);
		}
		float brightness = 1.0;
		//Model Brightness
		DWORD thisPtr = (int)(Store::Get().Engine_DLL.dwBase + offsets::dwModelAmbientMin - 0x2c);
		DWORD xored = *(DWORD*)&brightness ^ thisPtr;
		g_pMemory->Write<int>(Store::Get().Engine_DLL.dwBase + offsets::dwModelAmbientMin, xored);
	}
}

void Engine::Wall() {
	if (Store::Get().wallOn) {
		for (int i = 1; i < 64; i++)
		{
			const auto b_Dormant = Store::Get().entity_list[i].dormant;
			if (b_Dormant == 1)
				continue;

			const auto m_lifeState = Store::Get().entity_list[i].isAlive;
			if (m_lifeState != 0)
				continue;

			const auto m_iHealth = Store::Get().entity_list[i].health;
			if (m_iHealth < 1)
				continue;

			if (Store::Get().entity_list[i].team == Store::Get().Store::Get().player.team)
				continue;

			const  auto dwGlowObjectManager = g_pMemory->Read<DWORD>(Store::Get().Client_DLL.dwBase + offsets::dwGlowObjectManager);

			if (!dwGlowObjectManager)
				continue;

			const auto m_iGlowIndex = g_pMemory->Read<int>(Store::Get().entity_list[i].base + offsets::m_iGlowIndex);
			const auto glowAddress = dwGlowObjectManager + (0x38 * m_iGlowIndex);

			if (!glowAddress)
				continue;

			const auto health = (m_iHealth / 100.0F);

			auto entity_glow = g_pMemory->Read<m_GlowObject>(glowAddress);
			entity_glow.r = 1.0F - health;
			entity_glow.g = health * 0.5F;
			if (g_pBSPParser->is_visible(Valve::Vector3(Store::Get().player.position.x, Store::Get().player.position.y, Store::Get().player.position.z), Valve::Vector3(Store::Get().entity_list[i].position.x, Store::Get().entity_list[i].position.y, Store::Get().entity_list[i].position.z))) {
				entity_glow.b = 1.0F;
			}
			else {
				entity_glow.b = 0.0F;
			}
			entity_glow.a = 1;
			entity_glow.m_bRenderWhenOccluded = true;
			entity_glow.m_bRenderWhenUnoccluded = false;
			entity_glow.m_bFullBloom = false;
			g_pMemory->Write<m_GlowObject>(glowAddress, entity_glow);
		}
	}
}

/*char* GetPlayernameByIndex(int index)
{
	DWORD radarBase = g_pMemory->Read<DWORD>(offsets::dwRadarBase);

	if (radarBase == IntPtr.Zero) return string.Empty;

	DWORD radarPtr = g_pMemory->Read<DWORD>(radarBase + 0x6C);

	if (radarPtr == IntPtr.Zero) return string.Empty;

	index++;

	const auto nameAddr = radarPtr + index * 0x168 + 0x14;

	return name;
}*/

void Engine::bhop() {
	if (Store::Get().Store::Get().player.flags & 0x1 == true) {
		Log::Get().ConsolePrint("Pulo");
		g_pMemory->Write<int>(Store::Get().Client_DLL.dwBase + offsets::dwForceJump, 5);
		Sleep(50);
		g_pMemory->Write<int>(Store::Get().Client_DLL.dwBase + offsets::dwForceJump, 4);
	}
}

void Engine::debug() {
	if (Store::Get().debugOn) {

		std::cout << "debug " << GetMapName() << " \n";

		/*const auto map = Valve::Get().WorldToMap(Store::Get().Store::Get().player.position, Vector(-3230, 1713, 0), 5);
		Log::Get().ConsolePrint("Loc x %f y %f \n", map.x, map.y);*/

		/*
		RadarBase = ReadInt32((IntPtr)(Offsets.CLIENT + Offsets.dwRadarBase));
		Radar = ReadInt32((IntPtr)(RadarBase + 0x54));
		name = ReadStringUnicode((IntPtr)(Radar + ((0x1E0 * (id+1)) + 0x24)), 40);
		*/
		/*if (Checks.IsPanorama)
		{
			Offsets.m_dwRadarBasePointer = 0x6C;
			Offsets.m_dwRadarStructSize = 0x168;
			Offsets.m_dwRadarStructPos = 0x18;
		}
		else
		{
			Offsets.m_dwRadarBasePointer = 0x54;
			Offsets.m_dwRadarStructSize = 0x1E0;
			Offsets.m_dwRadarStructPos = 0x24;
		}*/

		/*for (int i = 0; i < 64; i++) {
			DWORD RadarBase = g_pMemory->Read<DWORD>(Store::Get().Client_DLL.dwBase + offsets::dwRadarBase);
			Log::Get().ConsolePrint("RadarBase %d \n", RadarBase);
			DWORD Radarpointer = g_pMemory->Read<DWORD>(RadarBase + 0x54);
			Log::Get().ConsolePrint("Radarpointer %d \n", Radarpointer);
			DWORD numBytes = NULL;
			wchar_t pName[32] = { NULL };
			g_pMemory->Read<LPVOID>(((LPVOID)(Radarpointer + (0x1E0 * i + 0x24)), &pName, sizeof(wchar_t) * 32, &numBytes) && numBytes == sizeof(wchar_t) * 32);
			Log::Get().ConsolePrint("Name %s \n", &pName);
			g_pConFuncs->ClearScreen();
		}*/

		/*const auto radarBase = g_pMemory->Read<PlayerObject>(Store::Get().Client_DLL.dwBase + offsets::dwRadarBase);
		Log::Get().ConsolePrint("Teste %s \n", radarBase);*/
	}
}

void Engine::fakelag() {
	if (Store::Get().fakelagOn) {
		SetPacketSending(false);
		Sleep(2);
		SetPacketSending(true);
	}else{
		SetPacketSending(true);
	}
}

bool Engine::IsInGame() {
	if (strcmp(GetMapName(), "") == 1)
	{
		return true;
	}
	else {
		return false;
	}
}

/*bool Engine::IsConnected() {
	if (!g_pMemory->Read<DWORD_PTR>(Store::Get().Engine_DLL.dwBase))
		return false;
	return (g_pMemory->Read<int>(Store::Get().Engine_DLL.dwBase + 0xE8) == (int)SIGNONSTATE_CONNECTED);
}*/