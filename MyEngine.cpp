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
		spdlog::flush_every(std::chrono::seconds(3));

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
void BasicGameEngine::Initialize()
{
	LogHelper logHelper;
	logHelper.CreateFile().then([this](StorageFile^ file) {
		InitializeLogger(file->Path->Data());
		spdlog::get("main_file_logger")->info("Initializing Game Engine");
		Meshes->Append(ref new Mesh());
		Meshes->Append(ref new Mesh());

		// initialize graphics and the pipeline
		InitGraphics();
		InitPipeline();
		spdlog::get("main_file_logger")->info("End of Game Engine initialization");
	});
}

// Loads and initializes all graphics data
void BasicGameEngine::InitGraphics()
{
	spdlog::get("main_file_logger")->info("Initializing Basic Graphics");
	CoreWindow^ currWindow = CoreWindow::GetForCurrentThread();
	if (d3dClass.Initialize(currWindow, currWindow->Bounds.Width, currWindow->Bounds.Height, false, true, 5.0f, 0.1f))
	{
		bool rez;
		devContextPtrAdress = reinterpret_cast<__int64>(d3dClass.GetDeviceContext());
		devPtrAdress = reinterpret_cast<__int64>(d3dClass.GetDevice());

		spdlog::get("main_file_logger")->info("Loading mesh data");
		Meshes->GetAt(0)->LoadSimplePlaneData();
		rez = Meshes->GetAt(0)->Initialize(devPtrAdress);
		if (!rez)
		{
			spdlog::get("main_file_logger")->error("Could not initialize mesh 0 with success");
		}
		Meshes->GetAt(1)->LoadSimpleTriangleData();
		rez = Meshes->GetAt(1)->Initialize(devPtrAdress);
		if (!rez)
		{
			spdlog::get("main_file_logger")->error("Could not initialize mesh 1 with success");
		}
	}
	else
	{
		spdlog::get("main_file_logger")->error("Could not initialize d3dClass");
	}
	spdlog::get("main_file_logger")->info("End of Basic Graphics Init");
}

void BasicGameEngine::Update()
{

}

void BasicGameEngine::Render()
{
	if (devContextPtrAdress == 0 || devPtrAdress == 0) return;

	d3dClass.BeginScene(0.0f, 0.2f, 0.4f, 1.0f);
	// clear the back buffer to a solid color
	// float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	// devcon->ClearRenderTargetView(renderTarget.Get(), color);

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

		// draw verticies, starting from vertex 0
		d3dClass.GetDeviceContext()->Draw(Meshes->GetAt(i)->GetIndexCount(), 0);
	}

	d3dClass.EndScene();
}

// Initializes the GPU settings and prepares it for rendering
void BasicGameEngine::InitPipeline()
{
	// load the shader files
	// .hlsl files become .cso files after compilation
	Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	// Creates the shader objects
	d3dClass.GetDevice()->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexShader);
	d3dClass.GetDevice()->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelShader);

	// Set the shader objects as the active shaders
	d3dClass.GetDeviceContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
	d3dClass.GetDeviceContext()->PSSetShader(pixelShader.Get(), nullptr, 0);

	// Initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
	HRESULT result = d3dClass.GetDevice()->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputLayout);
	if (FAILED(result))
	{
		return;
	}
	d3dClass.GetDeviceContext()->IASetInputLayout(inputLayout.Get());
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
