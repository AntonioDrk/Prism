#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d10shader.h>
#include <fstream>
#include <spdlog/spdlog.h>

using namespace DirectX;
using namespace Windows::UI::Popups;

class ColorShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX MVP;
	};

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialize(ID3D11Device1*);
	void Shutdown();
	bool Render(ID3D11DeviceContext1*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device1*, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameteres(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext1*, int);
	void OutputShaderErrorMessage(ID3D10Blob*);
	std::vector<uint8_t> LoadShaderFile(std::string);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

