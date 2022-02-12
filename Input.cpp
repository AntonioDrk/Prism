#include "pch.h"
#include "Input.h"

bool Input::keyState[219] = { 0 };

bool Input::KeyDown(Windows::System::VirtualKey key)
{
    return keyState[(int)key];
}

bool Input::KeyUp(Windows::System::VirtualKey key)
{
    return !keyState[(int)key];
}

void Input::SetKeyDown(Windows::System::VirtualKey key)
{
    keyState[(int)key] = true;
}

void Input::SetKeyUp(Windows::System::VirtualKey key)
{
    keyState[(int)key] = false;
}