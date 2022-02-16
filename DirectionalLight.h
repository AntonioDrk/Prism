#pragma once
#include "pch.h"
struct DirectionalLight
{
	DirectX::XMFLOAT3 dir;
	float pad;	// HLSL packs everything into FLOAT4, so structure padding is required
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;

	DirectionalLight() { ZeroMemory(this, sizeof(DirectionalLight)); };
};

DirectionalLight directionalLight;

struct cbPerFrame
{
	DirectionalLight directionalLight;
};

cbPerFrame constbuffPerFrame;
