#include "pch.h"
#include "Colorshader.h"

ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other){}

ColorShaderClass::~ColorShaderClass(){}

bool ColorShaderClass::Initialize(ID3D11Device1* device)
{
	bool result;

	// Init vertex and pixel shader
	result = InitializeShader(device, L"VertexShader.cso", L"PixelShader.cso");
	if (!result)
	{
		return false;
	}

	return true;
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext1* devContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameteres(devContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	RenderShader(devContext, indexCount);

	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device1* device, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	auto VSFile = LoadShaderFile("VertexShader.cso");
	auto PSFile = LoadShaderFile("PixelShader.cso");

	// Compile the vertex shader code.
	result = device->CreateVertexShader(&VSFile[0], VSFile.size(), nullptr, &m_vertexShader);
	if(FAILED(result))
	{
		spdlog::get("main_file_logger")->error("Can't create vertex shader!");
		return false;
	}
	spdlog::get("main_file_logger")->info("Vertex shader created with success");

	// Compile the pixel shader code.
	result = device->CreatePixelShader(&PSFile[0], PSFile.size(), nullptr, &m_pixelShader);
	if (FAILED(result))
	{
		spdlog::get("main_file_logger")->error("Can't create pixel shader!");
		return false;
	}
	spdlog::get("main_file_logger")->info("Pixel shader created with success");

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, &VSFile[0],
									   VSFile.size(), &m_layout);
	if (FAILED(result))
	{
		spdlog::get("main_file_logger")->error("Error creating input layout!");
		return false;
	}
	spdlog::get("main_file_logger")->info("Input Layout created with success");

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		spdlog::get("main_file_logger")->error("Error creating constant buffer!");
		return false;
	}
	spdlog::get("main_file_logger")->info("Constant Buffer created with success");

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

bool ColorShaderClass::SetShaderParameteres(ID3D11DeviceContext1* devContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	/*worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);*/

	// Lock the constant buffer so it can be written to.
	result = devContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		spdlog::get("main_file_logger")->error("Could not map to the matrix buffer!");
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	devContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	devContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext1* devContext, int indexCount)
{
	// Set the vertex input layout.
	devContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devContext->VSSetShader(m_vertexShader, NULL, 0);
	devContext->PSSetShader(m_pixelShader, NULL, 0);

	// Render the triangle.
	devContext->DrawIndexed(indexCount, 0, 0);

	return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage)
{
	char* compileErrors;
	unsigned long long bufferSize;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Write out the error message.
	
	spdlog::get("main_file_logger")->error(compileErrors);

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	return;
}

//// Loads a file into an array
std::vector<uint8_t> ColorShaderClass::LoadShaderFile(std::string File)
{
	std::vector<uint8_t> FileData;
	
	// Open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// If we opened it successfully
	if (VertexFile.is_open())
	{
		// Get length of file
		int Length = (int)VertexFile.tellg();

		// Collect the data of the file
		FileData.resize(static_cast<size_t>(Length));
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(&FileData[0]), Length);
		VertexFile.close();
	}

	return FileData;
}