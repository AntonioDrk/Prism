#pragma once

namespace MyLib
{
	struct VERTEX
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 color;
	};

	bool IsXMFloat3Equal(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right);
}


//struct MyMeshEqual : public std::binary_function<const Mesh, const Mesh, bool>
//{
//	bool operator()(const Mesh _Left, const Mesh _Right) const
//	{
//		return (_Left.GetVertexBuffer() == _Right.GetVertexBuffer() && _Left.GetIndexBuffer() == _Right.GetIndexBuffer());
//	};
//};
