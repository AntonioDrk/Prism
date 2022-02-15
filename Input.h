#pragma once
using namespace DirectX;
using namespace Windows::UI::Input;
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
	static XMFLOAT2 GetMousePos();
	static XMFLOAT2 GetMouseDelta();

	static void SetKeyDown(Windows::System::VirtualKey key);
	static void SetKeyUp(Windows::System::VirtualKey key);
	static void SetMousePos(float x, float y);
	static void SaveMouseState(PointerPointProperties^ Proprieties);
private:
	static bool keyState[219];
	static XMFLOAT2 mousePos;
	static XMFLOAT2 prevMousePos;
	static PointerPointProperties^ mouseState;
};