// Include SuperBLT
#include <superblt_flat.h>

#include <Windows.h>
#include <fstream>
#include "myfuncs.h"

// See superblt_flat.h for a description of what these functions do


//Returns the last Win32 error, in string format. Returns an empty string if there is no error.

void MoveCrashlog() {
	std::string s;
	s = getenv("LOCALAPPDATA");
	s += "\\PAYDAY 2\\crash.txt";
	auto cs = s.c_str();
	bool b = CopyFile(cs, "mods\\TroubleShooter\\EnterHere\\crash.txt", 0);
	if (!b) {
		void* lpBuffer;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpBuffer,
			0,
			NULL);

		MessageBox(NULL, (LPCTSTR)lpBuffer, "Error", MB_OK);
		LocalFree(lpBuffer);
	}
	else {
		PD2HOOK_LOG_LOG("Okay");
	}
}

void Plugin_Init() {
	// Put your one-time initialization code here
	std::atexit(MoveCrashlog);
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

	// Create a Lua table
	lua_newtable(L);

	// Add a hello-world string to it
	lua_pushstring(L, "Hello, World!");
	lua_setfield(L, -2, "mystring");

	// Add a C function to it
	lua_pushcfunction(L, say_hello);
	lua_setfield(L, -2, "myfunction");

	// Now return the table to Lua
	return 1;

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
