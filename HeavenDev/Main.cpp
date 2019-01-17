#include "Includes.h"

void menu() {
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F1 NOFLASH", Store::Get().noFlashOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F2 WALLHACK", Store::Get().wallOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F3 TRIGGER", Store::Get().triggerOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F4 RCS", Store::Get().rcsOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F5 BHOP", Store::Get().bhopOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F6 FAKELAG", Store::Get().fakelagOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F7 AIMBOT", Store::Get().aimbotOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F8 Wall Legit", Store::Get().wallLegitOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s  %s \n", "F9 DEBUG", Store::Get().debugOn ? "[Ativado]" : "[Desativado]");
	Log::Get().ConsolePrint("[Heaven]: %s \n", "F10 EXIT");
}

void BSPHandler()
{
	for (;; )
	{
		if (Engine::Get().IsInGame())
		{
				Engine::Get().updateBSP();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}

void AimbotThread()
{
	for (;; )
	{
		if (Store::Get().aimbotOn) {
			Aimbot::Get().tick();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Better sleep method for C++11 multi-threading
	}
}

void Wallhack()
{
	while (true) {
		if (Engine::Get().IsInGame())
		{
			Engine::Get().Wall();
			Engine::Get().WallLegit();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void FakeLag()
{
	for (;;)
	{
		if (Engine::Get().IsInGame())
		{
			Engine::Get().fakelag();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Better sleep method for C++11 multi-threading
	}
}

void Misc() {
	for (;;)
	{
		if (Engine::Get().IsInGame())
		{
			Engine::Get().NoFlash();
			Engine::Get().Trigger();
			Engine::Get().RCS();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Better sleep method for C++11 multi-threading
	}
}

void info() {
	for (;; )
	{
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			Store::Get().noFlashOn = !Store::Get().noFlashOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			Store::Get().wallOn = !Store::Get().wallOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			Store::Get().triggerOn = !Store::Get().triggerOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F4) & 1)
		{
			Store::Get().rcsOn = !Store::Get().rcsOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			Store::Get().bhopOn = !Store::Get().bhopOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			Store::Get().fakelagOn = !Store::Get().fakelagOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			Store::Get().aimbotOn = !Store::Get().aimbotOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F8) & 1)
		{
			Store::Get().wallLegitOn = !Store::Get().wallLegitOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F9) & 1)
		{
			Store::Get().debugOn = !Store::Get().debugOn;
			g_pConFuncs->ClearScreen();
			menu();
		}
		if (GetAsyncKeyState(VK_F10) & 1)
			exit(0);
		Engine::Get().debug();
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Better sleep method for C++11 multi-threading
	}
}


int main()
{
	g_pConFuncs->SetConsoleCursor(false);
	g_pConFuncs->SetConsoleColor(aqua, black);

	Log::Get().ConsolePrint("[Heaven]: %s \n", "Looking for CSGO");
	if (!g_pMemory->Attach("csgo.exe")) {
		Log::Get().ConsolePrint("[Heaven]: %s \n", "CSGO not found");
		exit(1);
	}

	g_pConFuncs->ClearScreen();

	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	Store::Get().Start();
	Engine::Get().Start();

	menu();

	std::thread tFakeLag(FakeLag);
	tFakeLag.detach();

	std::thread tAimbot(AimbotThread);
	tAimbot.detach();

	std::thread tWallhack(Wallhack);
	tWallhack.detach();

	std::thread tMisc(Misc);
	tMisc.detach();

	std::thread tBSPHandler(BSPHandler);
	tBSPHandler.detach();

	std::thread tinfor(info);
	tinfor.detach();

	while (true)
	{
		Engine::Get().GetPlayerInfo();
		Engine::Get().GetEntityListInfo();
	}

	/*system(0);
	return 0;*/
}