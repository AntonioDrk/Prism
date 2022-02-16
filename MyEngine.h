#pragma once

#include "D3dclass.h"
#include "Mesh.h"
#include "CameraClass.h"
#include "Colorshader.h"

/// <summary>
/// The root class for the GameEngine
/// </summary>
class BasicGameEngine
{
public:
	
	BasicGameEngine();
	bool Initialize();
	void InitGraphics();
	void Update(float TotalTime, float DeltaTime);
	bool Render();
	//Platform::Array<byte>^ LoadShaderFile(std::string);
	void InitPipeline();

private:
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

	CameraClass* m_Camera;
	ColorShaderClass* m_ColorShader;
	ID3D11Buffer* m_cbPerFrameBuffer;

	unsigned int vertexCount;
	__int64 devContextPtrAdress;
	__int64 devPtrAdress;

};
