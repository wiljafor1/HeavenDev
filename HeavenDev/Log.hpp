#pragma once

#include "Includes.h"

class Log : public Singleton<Log>
{
public:

	bool ConsolePrint(const char* fmt, ...);


};