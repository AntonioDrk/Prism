#pragma once
#include <string>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

/// <summary>
/// The root class for the GameEngine
/// </summary>
class BasicGameEngine
{
public:
	ComPtr<ID3D11Device1> dev;				// The device interface to the GPU
	ComPtr<ID3D11DeviceContext1> devcon;	// The device context;
	ComPtr<IDXGISwapChain1> swapchain;		// Swap chain interface
	ComPtr<ID3D11RenderTargetView> renderTarget;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11InputLayout> inputLayout;

	// Shader stuff
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11VertexShader> vertexShader;

	unsigned int vertexCount;

	void Initialize();
	void InitGraphics();
	void Update();
	void Render();
	Array<byte>^ LoadShaderFile(std::string);
	void InitPipeline();
};
