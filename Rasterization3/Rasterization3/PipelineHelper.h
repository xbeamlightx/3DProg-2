#pragma once

#include <array>
#include <d3d11.h>
struct SimpleVertex
{
	float pos[3];
	float color[3];

	SimpleVertex(const std::array<float, 3>& position, const std::array<float, 3> color)
	{
		for (int i = 0; i < 3; i++)
		{
			pos[i] = position[i];
			this->color[i] = color[i];
		}
	}

};
struct Vertex
{
	float pos[4];
	float uv[2];
	float color[4];


	Vertex(const std::array<float, 4>& position, const std::array<float, 2> UV, const std::array<float, 4> color)
	{
		uv[0] = UV[0];
		uv[1] = UV[1];

		for (int i = 0; i < 4; i++)
		{
			pos[i] = position[i];
			this->color[i] = color[i];
		}
	}
};



bool SetupPipline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader,
	ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& rsv,
	ID3D11SamplerState*& sState);

bool CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& rsv);

bool CreateSampler(ID3D11Device* device, ID3D11SamplerState*& sState);
	 
bool Bind(ID3D11Device* device, ID3D11RenderTargetView*& rtv, ID3D11ShaderResourceView*& rsv);