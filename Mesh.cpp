#include <iostream>
#include "pch.h"
#include "spdlog/spdlog.h"

using namespace Microsoft::WRL;
using namespace Platform;

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

	vertices = new VERTEX[m_vertexCount];
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

	vertices = new VERTEX[m_vertexCount];
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

	vertices = new VERTEX[m_vertexCount];
	indices = new unsigned short[m_indexCount];
	float halfSize = 0.5;
	VERTEX cubeData[] =
	{
		// Front face
		{
			DirectX::XMFLOAT3(-halfSize,-halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
		},
		{
			DirectX::XMFLOAT3(halfSize,-halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
		},
		{
			DirectX::XMFLOAT3(-halfSize,halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
		},
		{
			DirectX::XMFLOAT3(halfSize,halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
		},
		// Back Face
		{
			DirectX::XMFLOAT3(-halfSize,-halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
		},
		{
			DirectX::XMFLOAT3(halfSize,-halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
		},
		{
			DirectX::XMFLOAT3(-halfSize,halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
		},
		{
			DirectX::XMFLOAT3(halfSize,halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
		},
		// Left
		{ 
			DirectX::XMFLOAT3(-halfSize,-halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
		},
		{ 
			DirectX::XMFLOAT3(-halfSize,halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
		},
		{ 
			DirectX::XMFLOAT3(-halfSize,-halfSize,halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
		},
		{ 
			DirectX::XMFLOAT3(-halfSize,halfSize,halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
		},
		// Right

		{ 
			DirectX::XMFLOAT3(halfSize,-halfSize,-halfSize),
			DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)
		},
		{ 
			DirectX::XMFLOAT3(halfSize,halfSize,-halfSize),
			DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)
		},
		{ 
			DirectX::XMFLOAT3(halfSize,-halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)
		},
		{ 
			DirectX::XMFLOAT3(halfSize,halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)
		},
		// Bottom
		{ 
			DirectX::XMFLOAT3(-halfSize,-halfSize,-halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f)
		},
		{ 
			DirectX::XMFLOAT3(halfSize,-halfSize,-halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f)
		},
		{ 
			DirectX::XMFLOAT3(-halfSize,-halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f)
		},
		{ 
			DirectX::XMFLOAT3(halfSize,-halfSize,halfSize),
			DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f)
		},
		// Top
		{ 
			DirectX::XMFLOAT3(-halfSize,halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f)
		},
		{ 
			DirectX::XMFLOAT3(halfSize,halfSize,-halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f)
		},
		{ 
			DirectX::XMFLOAT3(-halfSize,halfSize,halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f)
		},
		{ 
			DirectX::XMFLOAT3(halfSize,halfSize,halfSize),
			DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f)
		}
	};
	
	std::copy(std::begin(cubeData), std::end(cubeData), vertices);
	//delete[] cubeData;

	unsigned long cubeIndices [] =
	{
		0,2, 1,    2,3,1,
		4,5, 7,    4,7,6,
		8,10, 9,  10,11,9,
		12,13,15, 12,15,14,
		16,17,18, 18,17,19,
		20,23,21, 20,22,23
	};

	std::copy(std::begin(cubeIndices), std::end(cubeIndices), indices);
	//delete[] cubeIndices;

	if (!vertices || !indices)
	{
		return false;
	}

	return true;
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
	
	
	bd_vertices.ByteWidth = sizeof(VERTEX) * m_vertexCount;
	bd_vertices.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_indices.StructureByteStride = sizeof(VERTEX);

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
	
	stride = sizeof(VERTEX);
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
