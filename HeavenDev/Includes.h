#ifndef INCLUDES_H //Define Guard
#define INCLUDES_H
#define WIN32_LEAN_AND_MEAN //Excludes Headers We Wont Use (Increase Compile Time)

#include <windows.h> //Standard Windows Functions/Data Types
#include <cstdint>
#include <algorithm>
#include <iostream> //Constains Input/Output Functions (cin/cout etc..)
#include <TlHelp32.h> //Contains Read/Write Functions
#include <string> //Support For Strings
#include <sstream> //Supports Data Conversion
#include "utils\QAngle.hpp"
#include "utils\Vector.hpp"
#include "utils\Vector2D.hpp"
#include "utils\VMatrix.hpp"
#include "csgo.hpp"
#include "Hack.h"
#include "singleton.h"
#include "Engine.h"
#include "Store.h"
#include <vector>
#include "CMemory.h"
#include "Log.hpp"
#include "consolefuncs.h"
#include <thread>
#include "Radar.hpp"
#include "math.h"
#include "BSP.h"
#include "psapi.h"
#include "aimbot.hpp"
#include "BSPParser.hpp"
#endif