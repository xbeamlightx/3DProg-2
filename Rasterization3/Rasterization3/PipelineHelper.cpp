#include "PipelineHelper.h"
#include <string>
#include <fstream>
#include <iostream>

#include "stb_image.h"

bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		/*std::cerr <<*/ OutputDebugString(L"\n\nCould not open vertex shader file\n\n!")/* << std::endl*/;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());
	
	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}

	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	reader.open("PixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open pixel shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
		return false;
	}
	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);
	return !FAILED(hr);
	
}

bool CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer*& vertexBuffer)
{
	/*triangle[3] =
	{
		{{0, 0.5f, 0}, {0, 0, 1}},
		{{0.f, -0.5f, 0}, {0, 1, 0}},
		{{-0.5f, -0.5f, 0}, {1, 0, 0}}

	};*/
	Vertex quad[4] =
	{
		{{0.5f, 0.5f, 0}, {1, 0}, {1, 0, 0}},
		{{0.5f, -0.5f, 0}, {1, 1}, {0, 1, 0}},
		{{-0.5f, 0.5f, 0}, {0, 0}, {0, 0, 1}},
		{{-0.5f, -0.5f, 0.f}, {0, 1}, {1, 1, 1} }
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(quad);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = quad;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;


	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	return !FAILED(hr);

}


bool SetupPipline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv, ID3D11SamplerState*& sState)
{
	 
	std::string vShaderByteCode;
	if (!LoadShaders(device, vShader, pShader, vShaderByteCode))
	{
		std::cerr << "Error loading shader!" << std::endl;
		return false;
	}

	if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}

	if (!CreateVertexBuffer(device, vertexBuffer))
	{
		std::cerr << "Error creating vertex buffer!" << std::endl;
		return false;
	}

	if (!CreateTexture(device, texture, srv))
	{
		std::cerr << "Error creating texture!" << std::endl;
	}

	if (!CreateSampler(device, sState))
	{
		std::cerr << "Error creating sampler!" << std::endl;
	}

	
	

	return true;
}

bool CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv)
{
	int comp, width, height;
	std::string fileName = "Nextage.png";

	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &comp, STBI_rgb_alpha);

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MiscFlags = 0;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = image;
	data.SysMemPitch = static_cast<UINT>(width * 4);
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, &data, &texture);
	if (FAILED(hr))
	{
		return false;
	}

	//Adapter

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	

	

	hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);

	if (FAILED(hr))
	{

		return false;
	}

	//delete image;

	return !FAILED(hr);

	return false;
}

bool CreateSampler(ID3D11Device* device, ID3D11SamplerState*& sState)
{
	D3D11_SAMPLER_DESC sDesc = {};
	sDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	//sDesc.MaxAnisotropy = 16;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;
	

	HRESULT hr = device->CreateSamplerState(&sDesc, &sState);

	return !FAILED(hr);
}

