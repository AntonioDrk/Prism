#pragma once

#include "MyHelperData.h"
#include "Mesh.h"
#include "D3dclass.h"

/// <summary>
/// The root class for the GameEngine
/// </summary>
class BasicGameEngine
{
public:
	//ComPtr<ID3D11Device1> dev;				// The device interface to the GPU
	//ComPtr<ID3D11DeviceContext1> devcon;	// The device context;
	//ComPtr<IDXGISwapChain1> swapchain;		// Swap chain interface
	//ComPtr<ID3D11RenderTargetView> renderTarget;
	//ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	D3DClass d3dClass;

	// Shader stuff
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;

	Platform::Collections::Vector<Mesh^>^ Meshes = ref new Platform::Collections::Vector<Mesh^>();

	unsigned int vertexCount;
	__int64 devContextPtrAdress;
	__int64 devPtrAdress;

	void Initialize();
	void InitGraphics();
	void Update();
	void Render();
	Platform::Array<byte>^ LoadShaderFile(std::string);
	void InitPipeline();
};
