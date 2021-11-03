#pragma once
#include "MyHelperData.h"
#include <iostream>
#include <d3d11.h>
/// <summary>
/// Component that holds the information of the 3D Object to be rendered.
/// It also can also render it.
/// </summary>
ref class Mesh
{

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	VERTEX *vertices;
	unsigned long* indices;
	uint32 m_vertexCount, m_indexCount;
	
	bool LoadVertexData();
	void UnloadVertexData();

public:	
	//constexpr bool operator() (const Mesh& lhs, const Mesh& rhs)
	//{
	//	return (lhs.m_indexBuffer == rhs.m_indexBuffer && lhs.m_vertexBuffer == rhs.m_vertexBuffer);
	//}

	bool Initialize(int ID3D11Device1);
	void LoadByFile(Platform::String^);
	void Shutdown();
	void Render(int ID3D11DeviceContext1);

	int GetIndexCount();
};