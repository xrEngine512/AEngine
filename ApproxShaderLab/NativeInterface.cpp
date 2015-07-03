#include "NativeInterface.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // Required for the ::SendInput function

namespace NativeInterface
{
	void SendInput(int key)
	{
#ifdef WIN32
		//...
		// Toggle Caps Lock key:
		INPUT input[2];
		ZeroMemory(input, sizeof(input));
		input[0].type = input[1].type = INPUT_KEYBOARD;
		input[0].ki.wVk = input[1].ki.wVk = key;
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;  // THIS IS IMPORTANT
		SendInput(2, input, sizeof(INPUT));
#endif
	}
}