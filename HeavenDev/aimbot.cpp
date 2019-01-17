#pragma once
#include "aimbot.hpp"



void Aimbot::tick() {

	if (GetAsyncKeyState(VK_LBUTTON)) {
		EntityList target = Engine::Get().getEntityByIndex(getTarget());
		if (!validateTarget(target))
			return;
		Vector pos = target.bone;
		QAngle view = getViewAngles(Store::Get().player);
		Vector src = Store::Get().player.position + Store::Get().player.view;
		QAngle punch = Store::Get().player.punch;

		/*Log::Get().ConsolePrint("pos : %f x : %f y \n", pos.x, pos.y);
		Log::Get().ConsolePrint("src : %f x : %f y \n", src.x, src.y);*/

		QAngle dst = g_pMath->CalcAngle(src, pos);
		g_pMath->normalizeAngles(dst);

		if (Store::Get().player.shots_fired > 1) {
			dst.pitch -= punch.pitch * 2.f;
			dst.yaw -= punch.yaw * 2.f;
			g_pMath->normalizeAngles(dst);
		}

		g_pMath->normalizeAngles(dst);
		g_pMath->clampAngles(dst);

		//Log::Get().ConsolePrint("pitch : %f yaw : %f \n", dst.pitch, dst.yaw);
		g_pMemory->Write<QAngle>(Store::Get().player.client_state + offsets::dwClientState_ViewAngles, dst);

	}

}


QAngle Aimbot::getViewAngles(LocalPlayer player) {

	return g_pMemory->Read<QAngle>(player.client_state + offsets::dwClientState_ViewAngles);

}


Vector Aimbot::getBonePos(DWORD boneAddress, int bone) {

	return g_pMemory->Read<Vector>((Store::Get().Engine_DLL.dwBase + boneAddress) + 0x30 * bone);

}


int Aimbot::getTarget() {

	auto maxFov = 20;
	auto bestTarget = -1;

	QAngle view = getViewAngles(Store::Get().player);
	Vector src = Store::Get().player.position + Store::Get().player.view;

	for (int i = 1; i < 64; i++) {

		if (!validateTarget(Store::Get().entity_list[i]))
			continue;

		Vector bone = Store::Get().entity_list[i].bone;
		float fov = g_pMath->getRealFOV(view, src, bone);
		if (fov > maxFov)
			continue;

		maxFov = fov;
		bestTarget = Store::Get().entity_list[i].index;

	}


	return bestTarget;
}


bool Aimbot::validateTarget(EntityList entity) {

	/*if (entity.base <= 0) {
		Log::Get().ConsolePrint("base!\n");
		return false;
	}*/

	if (entity.index == Store::Get().player.index) {
		return false;
	}

	if (entity.team == Store::Get().player.team) {
		return false;
	}

	if (entity.dormant == 1) {
		return false;
	}

	if (entity.isAlive != 0){
		return false;
	}
		

	return true;


}