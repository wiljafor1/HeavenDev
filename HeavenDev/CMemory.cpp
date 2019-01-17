#include "Includes.h"

CMemory* g_pMemory = new CMemory();

CMemory::CMemory() : _process(nullptr), _processID(NULL), _processSize(NULL)
{
}

CMemory::~CMemory()
{
	if (_process)
		CloseHandle(_process);

	_process = nullptr;
	_processID = NULL;
	_processSize = NULL;
}

HANDLE CMemory::getProcess() {
	return _process;
}

bool CMemory::Attach(const std::string pName)
{
	DeAttach();
	if (pName.empty())
		return false;

	auto Handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 mEntry; mEntry.dwSize = sizeof(mEntry);

	do
	{
		if (!strcmp(mEntry.szExeFile, pName.c_str()))
		{
			_processID = mEntry.th32ProcessID;
			_processSize = mEntry.dwSize;
			_process = OpenProcess(PROCESS_ALL_ACCESS, false, _processID);

			CloseHandle(Handle);
			return true;
		}
	} while (Process32Next(Handle, &mEntry));

	return false;
}

void CMemory::DeAttach()
{
	if (_process)
		CloseHandle(_process);

	_process = nullptr;
	_processID = NULL;
	_processSize = NULL;
}

SModule CMemory::GetModule(const std::string sModule) const
{
	if (sModule.empty())
		return{ NULL, NULL };

	auto Handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _processID);
	MODULEENTRY32 mEntry; mEntry.dwSize = sizeof(mEntry);

	do
	{
		if (!strcmp(mEntry.szModule, sModule.c_str()))
		{
			CloseHandle(Handle);
			return{ reinterpret_cast< DWORD >(mEntry.hModule), mEntry.modBaseSize };
		}
	} while (Module32Next(Handle, &mEntry));

	return{ NULL, NULL };
}

bool CMemory::Read(DWORD dwAddress, LPVOID lpBuffer, DWORD dwSize) const
{
	SIZE_T Out = NULL;
	return (ReadProcessMemory(_process, reinterpret_cast< LPVOID >(dwAddress), lpBuffer, dwSize, &Out) == TRUE);
}

bool CMemory::Write(DWORD dwAddress, LPCVOID lpBuffer, DWORD dwSize) const
{
	SIZE_T Out = NULL;
	if (dwAddress == NULL)
		return false;

	return (WriteProcessMemory(_process, reinterpret_cast< LPVOID >(dwAddress), lpBuffer, dwSize, &Out) == TRUE);
}

bool CMemory::DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
{
	for (; *pszMask; ++pszMask, ++pData, ++pMask)
		if (*pszMask == 'x' && *pData != *pMask)
			return false;
	return (*pszMask == NULL);
}

DWORD CMemory::PatternScan(SModule sModule, const char* sig, const char* mask) const
{
	auto data = new BYTE[sModule.dwSize];
	DWORD bytesRead;

	if (!ReadProcessMemory(_process, reinterpret_cast< LPVOID >(sModule.dwBase), data, sModule.dwSize, &bytesRead))
	{
		delete[] data;
		return NULL;
	}

	for (auto i = 0; i < sModule.dwSize; i++)
	{
		if (DataCompare(reinterpret_cast< const BYTE* >(data + i), reinterpret_cast< const BYTE* >(sig), mask))
		{
			delete[] data;
			return sModule.dwBase + i;
		}
	}

	delete[] data;
	return NULL;
}

DWORD CMemory::GetOffset(SModule sModule, const char* sig, const char* mask, int dwAdd, bool bRead, bool bSub)
{
	auto dwData = PatternScan(sModule, sig, mask);
	if (dwAdd != 0)
		dwData += dwAdd;

	if (bRead)
		dwData = Read< DWORD >(dwData);

	if (bSub)
		dwData -= sModule.dwBase;

	return dwData;
}