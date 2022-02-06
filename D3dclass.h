#pragma once
class D3DClass
{
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;	// Video card memory in megabytes
	char m_videoCardDescription[128];
	Microsoft::WRL::ComPtr<ID3D11Device1> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	//Microsoft::WRL::ComPtr<ID3D11Texture2D*> m_depthStencilBuffer;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	//Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_orthoMatrix;


public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	/// <summary>
	/// The Initialize function is what does the entire setup of Direct3D for DirectX 11
	/// </summary>
	/// <param name="renderWindow">If or not to sync the refreshrate to the monitor</param>
	/// <param name="screenWidth">Rendering width</param>
	/// <param name="screenHeight">Rendering height</param>
	/// <param name="vsync">If or not to sync the refreshrate to the monitor</param>
	/// <param name="fullscreen">If the render is fullscreen or not</param>
	/// <param name="screenDepth">The depth of our scene</param>
	/// <param name="screenNear">The near plane of our projection</param>
	/// <returns>(boolean) representing the success</returns>
	bool Initialize(Windows::UI::Core::CoreWindow^ renderWindow, int screenWidth, int screenHeight, 
					bool vsync, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float,float,float,float);
	void EndScene();

	ID3D11Device1* GetDevice();
	ID3D11DeviceContext1* GetDeviceContext();

	void RenderTriangleTest();
	
	void GetProjectionMatrix(DirectX::XMMATRIX&);
	void GetWorldMatrix(DirectX::XMMATRIX&);
	void GetOrthoMatrix(DirectX::XMMATRIX&);
	
	void GetVideoCardInfo(char*, int&);
};