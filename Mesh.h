#pragma once
#include "MyHelperData.h"
#include <iostream>
/// <summary>
/// Component that holds the information of the 3D Object to be rendered.
/// It also can also render it.
/// </summary>
ref class Mesh
{

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	MyLib::VERTEX *vertices;
	unsigned short* indices;
	uint32 m_vertexCount, m_indexCount;
	
	void CalcNormals();

public:	
	bool Initialize(__int64 ID3D11Device1);
	void LoadByFile(Platform::String^);
	void Shutdown();
	void Render(__int64 ID3D11DeviceContext1);
	bool LoadVertexData();
	bool LoadSimpleTriangleData();
	bool LoadSimplePlaneData();
	bool LoadSimpleCubeData();
	bool LoadShape();

	int GetIndexCount();
};
