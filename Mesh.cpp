#include "pch.h"
#include <iostream>
#include "spdlog/spdlog.h"

using namespace Microsoft::WRL;
using namespace Platform;
using namespace DirectX;

#include "Mesh.h"

/// <summary>
/// Loads the vertecies and index data into their arrays to be processed further
/// </summary>
bool Mesh::LoadVertexData()
{
	return false;
}

bool Mesh::LoadSimpleTriangleData()
{
	m_vertexCount = 3;
	m_indexCount = 3;

	vertices = new MyLib::VERTEX[m_vertexCount];
	indices = new unsigned short[m_indexCount];

	if (!vertices || !indices)
	{
		return false;
	}

	//TODO: Remove this brute force code, load the data from a file instead
	vertices[0] = {
			DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
	};

	vertices[1] = {
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	};

	vertices[2] = {
			DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	};

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	return true;
}

bool Mesh::LoadSimplePlaneData()
{
	m_vertexCount = 6;
	m_indexCount = 6;

	vertices = new MyLib::VERTEX[m_vertexCount];
	indices = new unsigned short[m_indexCount];

	if (!vertices || !indices)
	{
		return false;
	}

	//TODO: Remove this brute force code, load the data from a file instead
	vertices[0] = {
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
	};

	vertices[1] = {
			DirectX::XMFLOAT3(0.5f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	};

	vertices[2] = {
			DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	};

	vertices[3] = {
			DirectX::XMFLOAT3(0.5f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	};

	vertices[4] = {
			DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
	};

	vertices[5] = {
			DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	};

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	return true;
}

bool Mesh::LoadSimpleCubeData()
{
	m_vertexCount = 24;
	m_indexCount = 36;

	vertices = new MyLib::VERTEX[m_vertexCount];
	indices = new unsigned short[m_indexCount];
	float halfSize = 0.5;
	// POS, NORMAL, COL
	MyLib::VERTEX cubeData[] =
	{
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
		{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
		{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
		{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f)},
	};
	
	std::copy(std::begin(cubeData), std::end(cubeData), vertices);
	//delete[] cubeData;

	unsigned long cubeIndices [] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	std::copy(std::begin(cubeIndices), std::end(cubeIndices), indices);
	//delete[] cubeIndices;

	if (!vertices || !indices)
	{
		return false;
	}
	CalcNormals();
	return true;
}

bool Mesh::LoadHumanFigure()
{	
	m_vertexCount = 20;
	m_indexCount = 108;

	vertices = new MyLib::VERTEX[m_vertexCount];
	indices = new unsigned short[m_indexCount];

	// POS, NORMAL, COL
	MyLib::VERTEX meshData[] =
	{
		{XMFLOAT3(-0.57735f,  -0.57735f,  0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.934172f,  0.356822f,  0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.934172f,  -0.356822f,  0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(-0.934172f,  0.356822f,  0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(-0.934172f,  -0.356822f,  0.0f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.0f,  0.934172f,  0.356822f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.0f,  0.934172f,  -0.356822f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.356822f,  0.0f,  -0.934172f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(-0.356822f,  0.0f,  -0.934172f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.0f,  -0.934172f,  -0.356822f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.0f,  -0.934172f,  0.356822f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.356822f,  0.0f,  0.934172f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(-0.356822f,  0.0f,  0.934172f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.57735f,  0.57735f,  -0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.57735f,  0.57735f,  0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(-0.57735f,  0.57735f,  -0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(-0.57735f,  0.57735f,  0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.57735f,  -0.57735f,  -0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(0.57735f,  -0.57735f,  0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
		{XMFLOAT3(-0.57735f,  -0.57735f,  -0.57735f),XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.15f, 0.7f, 0.7f)},
	};

	std::copy(std::begin(meshData), std::end(meshData), vertices);
	//delete[] cubeData;

	unsigned long meshIndices[] =
	{
		19,  3,  2,
		12,  19,  2,
		15,  12,  2,
		8,  14,  2,
		18,  8,  2,
		3,  18,  2,
		20,  5,  4,
		9,  20,  4,
		16,  9,  4,
		13,  17,  4,
		1,  13,  4,
		5,  1,  4,
		7,  16,  4,
		6,  7,  4,
		17,  6,  4,
		6,  15,  2,
		7,  6,  2,
		14,  7,  2,
		10,  18,  3,
		11,  10,  3,
		19,  11,  3,
		11,  1,  5,
		10,  11,  5,
		20,  10,  5,
		20,  9,  8,
		10,  20,  8,
		18,  10,  8,
		9,  16,  7,
		8,  9,  7,
		14,  8,  7,
		12,  15,  6,
		13,  12,  6,
		17,  13,  6,
		13,  1,  11,
		12,  13,  11,
		19,  12,  11
	};

	std::copy(std::begin(meshIndices), std::end(meshIndices), indices);
	//delete[] cubeIndices;

	if (!vertices || !indices)
	{
		return false;
	}
	CalcNormals();
	return true;
}

void Mesh::CalcNormals()
{
	// Take each triangle
	for (int i = 0; i < m_indexCount; i += 3)
	{
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices[i]].position);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices[i + 1]].position);
		XMVECTOR p3 = XMLoadFloat3(&vertices[indices[i + 2]].position);
		XMVECTOR U = p2 - p1;
		XMVECTOR V = p3 - p1;

		XMVECTOR N = XMVector3Cross(U, V);
		N = XMVector3Normalize(N);

		// Vert 1
		if (MyLib::IsXMFloat3Equal(vertices[indices[i]].normal,XMFLOAT3(0, 0, 0)))
		{
			XMStoreFloat3(&(vertices[indices[i]].normal), N);
		}
		else
		{
			XMVECTOR NVert = XMLoadFloat3(&vertices[indices[i]].normal);
			NVert = XMVector3Normalize(N + NVert);
			XMStoreFloat3(&(vertices[indices[i]].normal), NVert);
		}

		// Vert 2
		if (MyLib::IsXMFloat3Equal(vertices[indices[i + 1]].normal, XMFLOAT3(0, 0, 0)))
		{
			XMStoreFloat3(&(vertices[indices[i + 1]].normal), N);
		}
		else
		{
			XMVECTOR NVert = XMLoadFloat3(&vertices[indices[i + 1]].normal);
			NVert = XMVector3Normalize(N + NVert);
			XMStoreFloat3(&(vertices[indices[i + 1]].normal), NVert);
		}

		// Vert 3
		if (MyLib::IsXMFloat3Equal(vertices[indices[i + 2]].normal, XMFLOAT3(0, 0, 0)))
		{
			XMStoreFloat3(&(vertices[indices[i + 2]].normal), N);
		}
		else
		{
			XMVECTOR NVert = XMLoadFloat3(&vertices[indices[i + 2]].normal);
			NVert = XMVector3Normalize(N + NVert);
			XMStoreFloat3(&(vertices[indices[i + 2]].normal), NVert);
		}
	}
}



/// <summary>
/// Initializes the d3d vertex and index buffers
/// </summary>
/// <param name="device"></param>
/// <returns></returns>
bool Mesh::Initialize(__int64 devAdress)
{
	spdlog::get("main_file_logger")->info("Initializing mesh");
	ComPtr<ID3D11Device1> dev = reinterpret_cast<ID3D11Device1*>(devAdress);
	if (!vertices || !indices)
	{
		return false;
	}

	HRESULT result;
	D3D11_BUFFER_DESC bd_vertices = { 0 }, bd_indices = { 0 };
	D3D11_SUBRESOURCE_DATA srd_vertices = { vertices, 0, 0 };
	
	
	bd_vertices.ByteWidth = sizeof(MyLib::VERTEX) * m_vertexCount;
	bd_vertices.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertices.StructureByteStride = sizeof(MyLib::VERTEX);

	result = dev->CreateBuffer(&bd_vertices, &srd_vertices, &m_vertexBuffer);
	if (FAILED(result))
	{
		spdlog::get("main_file_logger")->error("Error while trying to create vertex buffer for Mesh Object");
		return false;
	}

	D3D11_SUBRESOURCE_DATA srd_indices = { indices, 0, 0 };
	bd_indices.ByteWidth = sizeof(unsigned short) * m_indexCount;
	bd_indices.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_indices.StructureByteStride = sizeof(unsigned short);

	result = dev->CreateBuffer(&bd_indices, &srd_indices, &m_indexBuffer);

	if (FAILED(result))
	{
		spdlog::get("main_file_logger")->error("Error while trying to create index buffer for Mesh Object");
		return false;
	}

	return true;
}

/// <summary>
/// TODO: Loads data from a given file into the index and vertecies arrays
/// </summary>
/// <param name=""></param>
void Mesh::LoadByFile(String^)
{
}

/// <summary>
/// Clears all data from the object
/// </summary>
void Mesh::Shutdown()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
}

void Mesh::Render(__int64 devConAdress)
{
	ComPtr<ID3D11DeviceContext1> devCon = reinterpret_cast<ID3D11DeviceContext1*>(devConAdress);

	unsigned int stride;
	unsigned int offset;
	
	stride = sizeof(MyLib::VERTEX);
	offset = 0;
	
	// Set the vertex buffer to active in the input assembler so it can be rendered
	devCon->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	
	// Set the index buffer to active in the input assembler so it can be rendered
	devCon->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Mesh::GetIndexCount()
{
	return m_indexCount;
}
