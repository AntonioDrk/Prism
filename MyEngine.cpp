#include "pch.h"
#include <string>
#include <collection.h>
#include <fstream>
#include "spdlog/spdlog.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

#include "FileHandling.cpp"
#include "MyEngine.h"

BasicGameEngine::BasicGameEngine()
{
	m_Camera = 0;
	m_ColorShader = 0;
}

void InitializeLogger(std::wstring path)
{
	try
	{
		std::string path(path.begin(), path.end());
		auto logger = spdlog::basic_logger_mt("main_file_logger", path);
		//logger->info("Created log file.");
		//logger->flush();
		//spdlog::register_logger(logger);
		spdlog::set_default_logger(logger);
		spdlog::flush_on(spdlog::level::info);

		spdlog::info("Log File Generated with success.");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::string err(ex.what());
		std::wstring w_str = std::wstring(err.begin(), err.end());
		const wchar_t* wchars = w_str.c_str();
		Platform::String^ errStr = ref new Platform::String(wchars);
		MessageDialog Dialog(errStr, "ERROR");
		Dialog.ShowAsync();
	}
}

// Prepares Direct3D for use
bool BasicGameEngine::Initialize()
{
	LogHelper logHelper;
	logHelper.CreateFile().then([this](StorageFile^ file) {
		InitializeLogger(file->Path->Data());
		
		spdlog::get("main_file_logger")->info("Initializing Game Engine");
		
		Meshes->Append(ref new Mesh());
		//Meshes->Append(ref new Mesh());

		// initialize graphics and the pipeline
		InitGraphics();
		InitPipeline();
		spdlog::get("main_file_logger")->info("Game Engine initialized with success");
	});
	return true;
}

// Loads and initializes all graphics data
void BasicGameEngine::InitGraphics()
{
	spdlog::get("main_file_logger")->info("Initializing Basic Graphics");
	CoreWindow^ currWindow = CoreWindow::GetForCurrentThread();
	int screenWidth = currWindow->Bounds.Width;
	int screenHeight = currWindow->Bounds.Height;
	if (d3dClass.Initialize(currWindow, screenWidth, screenHeight, false, true, 1000.0f, 0.001f))
	{
		bool rez;
		devContextPtrAdress = reinterpret_cast<__int64>(d3dClass.GetDeviceContext());
		devPtrAdress = reinterpret_cast<__int64>(d3dClass.GetDevice());

		spdlog::get("main_file_logger")->info("Loading mesh data");
		Meshes->GetAt(0)->LoadSimpleTriangleData();
		rez = Meshes->GetAt(0)->Initialize(devPtrAdress);
		if (!rez)
		{
			spdlog::get("main_file_logger")->error("Could not initialize mesh 0 with success");
		}
		/*Meshes->GetAt(1)->LoadSimpleTriangleData();
		rez = Meshes->GetAt(1)->Initialize(devPtrAdress);
		if (!rez)
		{
			spdlog::get("main_file_logger")->error("Could not initialize mesh 1 with success");
		}*/
	}
	else
	{
		spdlog::get("main_file_logger")->error("Could not initialize d3dClass");
	}
	spdlog::get("main_file_logger")->info("End of Basic Graphics Init");
}

void BasicGameEngine::Update(float TotalTime, float DeltaTime)
{
	if (m_Camera)
	{
		float speed = 1.0f;
		XMFLOAT3 camPos = m_Camera->GetRotation();
		float amount = speed * DeltaTime;
		DX::OutputDebug(camPos.x + "," + camPos.y + "," + camPos.z + "\n");
		m_Camera->SetRotation(camPos.x + amount, camPos.y , camPos.z );
	}
}

bool BasicGameEngine::Render()
{
	if (devContextPtrAdress == 0 || devPtrAdress == 0) return false;

	XMMATRIX worldMatrix = *new XMMATRIX();
	XMMATRIX viewMatrix = *new XMMATRIX();
	XMMATRIX projectionMatrix = *new XMMATRIX();
	bool result;

	// Clear the buffers to begin the scene.
	d3dClass.BeginScene(0.0f, 0.2f, 0.4f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	d3dClass.GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	d3dClass.GetProjectionMatrix(projectionMatrix);

	// clear the back buffer to a solid color
	//float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	//devcon->ClearRenderTargetView(renderTarget.Get(), color);

	//// Set the vertex buffer
	//UINT stride = sizeof(VERTEX);
	//UINT offset = 0;
	//devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	//// Set the primitive type of topology
	//devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Render all the stored objects
	for (unsigned int i = 0; i < Meshes->Size; i++)
	{
		Meshes->GetAt(i)->Render(devContextPtrAdress);
		// Render the model using the color shader.
		result = m_ColorShader->Render(d3dClass.GetDeviceContext(), Meshes->GetAt(i)->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
		// draw verticies, starting from vertex 0
		//d3dClass.GetDeviceContext()->Draw(Meshes->GetAt(i)->GetIndexCount(), 0);
	}

	d3dClass.EndScene();
	return true;
}

// Initializes the GPU settings and prepares it for rendering
void BasicGameEngine::InitPipeline()
{
	bool result;
	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		spdlog::get("main_file_logger")->error("Could not create camera object");
		throw new std::exception("Could not create camera object");
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, 5.0f);
	m_Camera->SetRotation(10, 18, 0);

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		spdlog::get("main_file_logger")->error("Could not create color shader object");
		throw new std::exception("Could not create color shader object");
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(d3dClass.GetDevice());
	if (!result)
	{
		MessageDialog Dialog("Could not initialize the color shader object.");
		Dialog.ShowAsync();
	}
	//// load the shader files
	//// .hlsl files become .cso files after compilation
	//Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	//Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	//// Creates the shader objects
	//d3dClass.GetDevice()->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexShader);
	//d3dClass.GetDevice()->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelShader);

	//// Set the shader objects as the active shaders
	//d3dClass.GetDeviceContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
	//d3dClass.GetDeviceContext()->PSSetShader(pixelShader.Get(), nullptr, 0);

	//// Initialize input layout
	//D3D11_INPUT_ELEMENT_DESC ied[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//};

	//// Create the input layout
	//HRESULT result = d3dClass.GetDevice()->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputLayout);
	//if (FAILED(result))
	//{
	//	return;
	//}
	//d3dClass.GetDeviceContext()->IASetInputLayout(inputLayout.Get());
}

//// Loads a file into an array
//Array<byte>^ BasicGameEngine::LoadShaderFile(std::string File)
//{
//	Array<byte>^ FileData = nullptr;
//	
//	// Open the file
//	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);
//
//	// If we opened it successfully
//	if (VertexFile.is_open())
//	{
//		// Get length of file
//		int Length = (int)VertexFile.tellg();
//
//		// Collect the data of the file
//		FileData = ref new Array<byte>(Length);
//		VertexFile.seekg(0, std::ios::beg);
//		VertexFile.read(reinterpret_cast<char*>(FileData->Data), Length);
//		VertexFile.close();
//	}
//
//	return FileData;
//}
