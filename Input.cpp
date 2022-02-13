#include "pch.h"
#include "Input.h"


bool Input::keyState[219] = { 0 };
bool Input::mouseKeyState[3] = { 0 };
XMFLOAT2 Input::mousePos = XMFLOAT2(0.0f, 0.0f);
XMFLOAT2 Input::prevMousePos = XMFLOAT2(0.0f, 0.0f);

bool Input::KeyDown(Windows::System::VirtualKey key)
{
    return keyState[(int)key];
}

bool Input::KeyUp(Windows::System::VirtualKey key)
{
    return !keyState[(int)key];
}

bool Input::GetMouseKeyDown(MouseButton mButt)
{
    return mouseKeyState[(int)mButt];
}

bool Input::GetMouseKeyUp(MouseButton mButt)
{
    return !mouseKeyState[(int)mButt];
}

XMFLOAT2 Input::GetMousePos()
{
    return mousePos;
}

void Input::SetKeyDown(Windows::System::VirtualKey key)
{
    keyState[(int)key] = true;
}

void Input::SetKeyUp(Windows::System::VirtualKey key)
{
    keyState[(int)key] = false;
}

void Input::SetMouseKeyDown(MouseButton mButt)
{
    mouseKeyState[(int)mButt] = true;
}

void Input::SetMouseKeyUp(MouseButton mButt)
{
    mouseKeyState[(int)mButt] = false;
}

void Input::SetMousePos(float x, float y)
{
    prevMousePos = mousePos;
    mousePos.x = x;
    mousePos.y = y;
}
