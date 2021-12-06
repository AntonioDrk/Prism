#include "pch.h"
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;

#include "D3dclass.h"

D3DClass::D3DClass()
{
}

D3DClass::~D3DClass()
{
}


bool D3DClass::Initialize(CoreWindow^ renderWindow, int screenWidth, int screenHeight, 
						  bool vsync, bool fullscreen, float screenDepth, float screenNear)
{	

	// Create DirectX graphics interface factory
	// Define the temporary pointers to device and device context
	HRESULT result;
	ComPtr<ID3D11Device> dev11;
	ComPtr<ID3D11DeviceContext> devcon11;
	ComPtr<IDXGIOutput> adapterOutput;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_MODE_DESC* displayModeList;
	float fieldOfView, screenAspect;
	int error;
	size_t stringLength;
	unsigned int numModes, numerator, denominator;

	m_vsync_enabled = vsync;

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
	dev11.As(&m_device);
	devcon11.As(&m_deviceContext);

	// Convert our ID3D11Device1 into an IDXGIDevice1
	ComPtr<IDXGIDevice1> dxgiDevice;
	m_device.As(&dxgiDevice);

	// Use the IDXGIDevice1 interface to get access to the adapter
	ComPtr<IDXGIAdapter> dxgiAdapter;
	result = dxgiDevice->GetAdapter(&dxgiAdapter);
	if (FAILED(result))
	{
		return false;
	}
	// Use the IDXGIAdapter interface to get access to the factory
	ComPtr<IDXGIFactory2> dxgiFactory;
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);
	if (FAILED(result))
	{
		return false;
	}
	// Enumerate the primary adapter output (monitor).
	result = dxgiAdapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}
	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}
	// Now we fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = dxgiAdapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedSystemMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();

	// Release the adapter.
	dxgiAdapter->Release();

	// Release the factory.
	dxgiFactory->Release();

	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how the swap chain should be used
	scd.BufferCount = 2;                                  // a front buffer and a back buffer
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;    // the recommended flip mode
	scd.SampleDesc.Count = 1;                             // disable anti-aliasing
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	//scd.Width = screenWidth;	// You can use this to render higher/lower resolutions on the same screen
	//scd.Height = screenHeight; 

	result = dxgiFactory->CreateSwapChainForCoreWindow(
		m_device.Get(),									// address of the device
		reinterpret_cast<IUnknown*>(renderWindow),    // address of the window
		&scd,										// address of the swap chain description
		nullptr,									// advanced
		&m_swapChain);								// address of the new swap chain pointer
	if (FAILED(result))
	{
		return false;
	}

	// Credits to
	// https://stackoverflow.com/questions/59263246/changing-refresh-rate-under-full-screen-mode-in-directx11-code-does-not-work
	DXGI_MODE_DESC modeDesc = { 0 };
	modeDesc.Width = screenWidth;
	modeDesc.Height = screenHeight;
	if (m_vsync_enabled)
	{
		modeDesc.RefreshRate.Numerator = numerator;
		modeDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		modeDesc.RefreshRate.Numerator = 0;
		modeDesc.RefreshRate.Denominator = 1;
	}

	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	m_swapChain->SetFullscreenState(fullscreen, nullptr);
	m_swapChain->ResizeTarget(&modeDesc);

	// get a pointer directly to the back buffer
	ComPtr<ID3D11Texture2D> backbuffer;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create a render target pointing to the back buffer
	result = m_device->CreateRenderTargetView(backbuffer.Get(), nullptr, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Set the viewport normalized coordinate system
	D3D11_VIEWPORT viewport = { 0 };

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Width = renderWindow->Bounds.Width;
	viewport.Height = renderWindow->Bounds.Height;

	m_deviceContext->RSSetViewports(1, &viewport);

	// TODO: Add the depth buffer stencil
	// Set our new render target object as the active render target
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	// Setup the projection matrix
	fieldOfView = DirectX::XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
	}

	if (m_device)
	{
		m_device->Release();
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
	}
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

	// Clear the depth buffer.
	//m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device1* D3DClass::GetDevice()
{
	return m_device.Get();
}

ID3D11DeviceContext1* D3DClass::GetDeviceContext()
{
	return m_deviceContext.Get();
}

void D3DClass::RenderTriangleTest()
{

}

void D3DClass::GetPorjectionMatrix(DirectX::XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}

void D3DClass::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
}

void D3DClass::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}
