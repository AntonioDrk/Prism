#pragma once

class Input
{
public:
	static bool KeyDown(Windows::System::VirtualKey key);
	static bool KeyUp(Windows::System::VirtualKey key);
	static void SetKeyDown(Windows::System::VirtualKey key);
	static void SetKeyUp(Windows::System::VirtualKey key);
private:
	static bool keyState[219];

};