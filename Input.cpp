#include "pch.h"
#include "Input.h"


bool Input::keyState[219] = { 0 };
XMFLOAT2 Input::mousePos = XMFLOAT2(0.0f, 0.0f);
XMFLOAT2 Input::prevMousePos = XMFLOAT2(0.0f, 0.0f);
PointerPointProperties^ Input::mouseState = nullptr;

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
    if (!mouseState) return false;
    switch (mButt)
    {
    case MouseButton::LeftMouseButton:
        return mouseState->IsLeftButtonPressed;
        break;
    case MouseButton::RightMouseButton:
        return mouseState->IsRightButtonPressed;
        break;
    case MouseButton::MiddleMouseButton:
        return mouseState->IsMiddleButtonPressed;
        break;
    }
}

XMFLOAT2 Input::GetMousePos()
{
    return mousePos;
}

XMFLOAT2 Input::GetMouseDelta()
{
    return XMFLOAT2(mousePos.x - prevMousePos.x, mousePos.y - prevMousePos.y);
}

void Input::SetKeyDown(Windows::System::VirtualKey key)
{
    keyState[(int)key] = true;
}

void Input::SetKeyUp(Windows::System::VirtualKey key)
{
    keyState[(int)key] = false;
}

void Input::SetMousePos(float x, float y)
{
    prevMousePos = mousePos;
    mousePos.x = x;
    mousePos.y = y;
}

void Input::SaveMouseState(PointerPointProperties^ Proprieties)
{
    mouseState = Proprieties;
}
