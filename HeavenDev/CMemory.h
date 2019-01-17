#ifndef _C_MEMORY_
#define _C_MEMORY_

#include "Includes.h"

class CMemory
{
	HANDLE _process;
	DWORD  _processID;
	DWORD  _processSize;

public:
	CMemory();
	~CMemory();

	bool Attach(const std::string pName);
	void DeAttach();

	HANDLE getProcess();

	SModule GetModule(const std::string sModule) const;
	DWORD   PatternScan(SModule sModule, const char* sig, const char* mask) const;
	DWORD	GetOffset(SModule sModule, const char* sig, const char* mask, int dwAdd, bool bRead, bool bSub);

	bool Read(DWORD dwAddress, LPVOID lpBuffer, DWORD dwSize) const;
	bool Write(DWORD dwAddress, LPCVOID lpBuffer, DWORD dwSize) const;
	static bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask);

	template<typename T>
	T Read(DWORD dwAddress, const T& tDefault = T())
	{
		T tRet;
		if (!Read(dwAddress, &tRet, sizeof(T)))
			return tDefault;

		return tRet;
	}

	template<typename T>
	bool Write(DWORD dwAddress, const T& tValue)
	{
		return Write(dwAddress, &tValue, sizeof(T));
	}

	template<typename T>
	static T FromRawData(uint8_t* Data, DWORD dwAddress)
	{
		return *reinterpret_cast< T* >(reinterpret_cast< DWORD >(Data) + dwAddress);
	}
};

extern CMemory* g_pMemory;
#endif