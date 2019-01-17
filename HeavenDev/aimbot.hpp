#pragma once
#include "Includes.h"


class Aimbot : public Singleton<Aimbot>
{
public:
	int index = -1;
	void tick();
	int getTarget();
	bool validateTarget(EntityList entity);
	Vector getBonePos(DWORD boneAddress, int bone);
	QAngle getViewAngles(LocalPlayer player);



};