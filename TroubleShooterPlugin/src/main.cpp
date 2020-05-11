// Include SuperBLT
#include <superblt_flat.h>

#include <windows.h>
#include <fstream>
#include <ctime>

using namespace std;

// See superblt_flat.h for a description of what these functions do

#define localtime_r(time, out) localtime_s(out, time)

//Taken from SBLT's code.
std::string GetDateString() {
	std::time_t currentTime = time(0);
	std::tm now;
	localtime_r(&currentTime, &now);

	char datestring[100];
	std::strftime(datestring, sizeof(datestring), "%Y_%m_%d", &now);
	return datestring;
}

bool FileExists(const string& dir) {
	DWORD attr = GetFileAttributes(dir.c_str());
	return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}


void MoveLogs() {
	string env = getenv("LOCALAPPDATA");
	string crashPath = env+"\\PAYDAY 2\\crash.txt";

	PD2HOOK_LOG_LOG("Copying crashlog!");
	if (FileExists(crashPath) && !CopyFile(crashPath.c_str(), "mods\\TroubleShooter\\EnterHere\\crash.txt", 0))
		PD2HOOK_LOG_ERROR("Something went wrong while trying to copy the crashlog");

	string logPath = "mods\\logs\\"+GetDateString()+"_log.txt";

	//Force BLT to save the log so important information isn't cut :P
	PD2HOOK_LOG_LOG("Copying log!");
	for (int i = 0; i < 20; i++) {
		PD2HOOK_LOG_LOG("..............................................................");
	}

	if (!FileExists(logPath)) {
		PD2HOOK_LOG_ERROR(logPath.c_str());
		PD2HOOK_LOG_ERROR("Couldn't copy the BLT log: File doesn't exist.");
	}
	else if (!CopyFile(logPath.c_str(), "mods\\TroubleShooter\\EnterHere\\log.txt", 0))
		PD2HOOK_LOG_ERROR("Couldn't copy the BLT log.");
}

void Plugin_Init() {
	// Put your one-time initialization code here
	std::atexit(MoveLogs);
}

void Plugin_Update() {
	// Put your per-frame update code here
}

void Plugin_Setup_Lua(lua_State *L) {
	// Deprecated, see this function's documentation (in superblt_flat.h) for more detail
}

int Plugin_PushLua(lua_State *L) {
	// Put your Lua setup code here
	// Please note that this will likely be called many times, so
	//  don't do actual important setup here - use Plugin_Init for that.
	// More specifically, this is called every time blt.load_native is called
	//  for this DLL, even if it has already been loaded.

	// And read this until you understand what's happening: https://www.lua.org/pil/24.html
	// For a comprehensive reference of what all the functions do, please
	// read the Lua 5.1 manual:	http://www.lua.org/manual/5.1/manual.html
	// (note that on Windows, only some of the functions in the manual are avaiable).

	// Now return the table to Lua
	return 0;

	// Now, this Lua:

	/*
	-- Note you can call blt.load_native as many times as you like for the
	--  same DLL, and the DLL will only be loaded once.
	local result, myclibrary = blt.load_native(ModPath .. "my_library.dll")
	log("Hi: " .. tostring(myclibrary))
	log("Hi: " .. myclibrary.mystring)
	log("Hi: " .. myclibrary.myfunction(64))
	*/

	// Will get this result:

	/*
	02:52:55 PM Lua: Hi: table: 0x25baeac0
	02:52:55 PM Lua: Hi: Hello, World!
	02:52:55 PM Log:  (ExtModDLL c:\users\znix\source\repos\superbltplugins\pluginbase\src\myfuncs.cpp:11) Hello world!
	02:52:55 PM Lua: Hi: 256
	*/
}
