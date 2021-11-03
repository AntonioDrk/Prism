#include "pch.h"
#include "MyEngine.h"
#include "MyHelperData.h"

#include <fstream>

// Prepares Direct3D for use
void BasicGameEngine::Initialize()
{
	// Define the temporary pointers to device and device context
	ComPtr<ID3D11Device> dev11;
	ComPtr<ID3D11DeviceContext> devcon11;

	// Create device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&dev11,
		nullptr,
		&devcon11
	);

	// Convert the pointers from DX11 to DX11.1 version
	dev11.As(&dev);
	devcon11.As(&devcon);

	// Convert our ID3D11Device1 into an IDXGIDevice1
	ComPtr<IDXGIDevice1> dxgiDevice;
	dev.As(&dxgiDevice);

	// Use the IDXGIDevice1 interface to get access to the adapter
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	// Use the IDXGIAdapter interface to get access to the factory
	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how the swap chain should be used
	scd.BufferCount = 2;                                  // a front buffer and a back buffer
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;    // the recommended flip mode
	scd.SampleDesc.Count = 1;                             // disable anti-aliasing

	CoreWindow^ CurrWindow = CoreWindow::GetForCurrentThread(); 

	dxgiFactory->CreateSwapChainForCoreWindow(
		dev.Get(),									// address of the device
		reinterpret_cast<IUnknown*>(CurrWindow),    // address of the window
		&scd,										// address of the swap chain description
		nullptr,									// advanced
		&swapchain);								// address of the new swap chain pointer

	// get a pointer directly to the back buffer
	ComPtr<ID3D11Texture2D> backbuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);

	// Create a render target pointing to the back buffer
	dev->CreateRenderTargetView(backbuffer.Get(), nullptr, &renderTarget);

	// Set the viewport normalized coordinate system
	D3D11_VIEWPORT viewport = { 0 };

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = CurrWindow->Bounds.Width;
	viewport.Height = CurrWindow->Bounds.Height;

	devcon->RSSetViewports(1, &viewport);

	Meshes->Append(ref new Mesh());

	// initialize graphics and the pipeline
	InitGraphics();
	InitPipeline();
}

// Loads and initializes all graphics data
void BasicGameEngine::InitGraphics()
{
	Meshes->GetAt(0)->Initialize((int)dev.Get());
}

void BasicGameEngine::Update()
{

}

void BasicGameEngine::Render()
{
	// Set our new render target object as the active render target
	devcon->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);

	// clear the back buffer to a solid color
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	devcon->ClearRenderTargetView(renderTarget.Get(), color);

	//// Set the vertex buffer
	//UINT stride = sizeof(VERTEX);
	//UINT offset = 0;
	//devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	//// Set the primitive type of topology
	//devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Meshes->GetAt(0)->Render((int)devcon.Get());

	// draw 3 verticies (in our case), starting from vertex 0
	devcon->Draw(Meshes->GetAt(0)->GetIndexCount(), 0);

	// Switch the back buffer and the front buffer
	swapchain->Present(1, 0);
}

// Initializes the GPU settings and prepares it for rendering
void BasicGameEngine::InitPipeline()
{
	// load the shader files
	// .hlsl files become .cso files after compilation
	Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	// Creates the shader objects
	dev->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexShader);
	dev->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelShader);

	// Set the shader objects as the active shaders
	devcon->VSSetShader(vertexShader.Get(), nullptr, 0);
	devcon->PSSetShader(pixelShader.Get(), nullptr, 0);

	// Initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
	dev->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputLayout);
	devcon->IASetInputLayout(inputLayout.Get());
}

// Loads a file into an array
Array<byte>^ BasicGameEngine::LoadShaderFile(std::string File)
{
	Array<byte>^ FileData = nullptr;
	
	// Open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// If we opened it successfully
	if (VertexFile.is_open())
	{
		// Get length of file
		int Length = (int)VertexFile.tellg();

		// Collect the data of the file
		FileData = ref new Array<byte>(Length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), Length);
		VertexFile.close();
	}

	return FileData;
}
