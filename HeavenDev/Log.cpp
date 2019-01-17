#include "Includes.h"



bool Log::ConsolePrint(const char* fmt, ...)
{


	HANDLE _out = GetStdHandle(STD_OUTPUT_HANDLE);

	char buf[1024];
	va_list va;
	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

	return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
}