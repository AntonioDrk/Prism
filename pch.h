#pragma once
#include <winrt/Windows.Foundation.h>
#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>

namespace DX
{
	inline void OutputDebug(Platform::String^ Msg)
	{
		auto data = std::wstring(Msg->Data());
		OutputDebugString(data.c_str());
	}
}