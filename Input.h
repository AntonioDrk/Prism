#pragma once
using namespace DirectX;
class Input
{
public:
	enum MouseButton
	{
		LeftMouseButton,
		RightMouseButton,
		MiddleMouseButton
	};

	static bool KeyDown(Windows::System::VirtualKey key);
	static bool KeyUp(Windows::System::VirtualKey key);
	static bool GetMouseKeyDown(MouseButton mButt);
	static bool GetMouseKeyUp(MouseButton mButt);
	static XMFLOAT2 GetMousePos();

	static void SetKeyDown(Windows::System::VirtualKey key);
	static void SetKeyUp(Windows::System::VirtualKey key);
	static void SetMouseKeyDown(MouseButton mButt);
	static void SetMouseKeyUp(MouseButton mButt);
	static void SetMousePos(float x, float y);
private:
	static bool keyState[219];
	static bool mouseKeyState[3];
	static XMFLOAT2 mousePos;
	static XMFLOAT2 prevMousePos;
};