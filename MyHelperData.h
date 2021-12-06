#pragma once
#ifndef MYHELPER_GUARD
#define MYHELPER_GUARD
struct VERTEX
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};

//struct MyMeshEqual : public std::binary_function<const Mesh, const Mesh, bool>
//{
//	bool operator()(const Mesh _Left, const Mesh _Right) const
//	{
//		return (_Left.GetVertexBuffer() == _Right.GetVertexBuffer() && _Left.GetIndexBuffer() == _Right.GetIndexBuffer());
//	};
//};
#endif
