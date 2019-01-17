#include "Includes.h"
class CL_Move : public Singleton<Engine>
{
public:
	CL_Move(void)
	{

	}
	~CL_Move(void)
	{

	}

	void SetPacketSending(bool active)
	{
		if (active)
			g_pMemory->Write< BYTE >(Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets, 1);
		else g_pMemory->Write< BYTE >(Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets, 0);
	}

	int GetCommandNumver(void)
	{
		return g_pMemory->Read< int >(g_pMemory->Read< DWORD >(Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets) + 0x4C24);
	}
	int GetChokedCommandNumber(void)
	{
		return g_pMemory->Read< int >(g_pMemory->Read< DWORD >(Store::Get().Engine_DLL.dwBase + offsets::dwbSendPackets) + 0x4C28);
	}

}gCLMove;