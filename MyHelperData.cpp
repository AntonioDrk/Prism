#include "pch.h"
#include "MyHelperData.h"

bool MyLib::IsXMFloat3Equal(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right)
{
	return left.x == right.x && left.y == right.y && left.z == right.z;
}
