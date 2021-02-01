#define STB_IMAGE_IMPLEMENTATION

#include <Windows.h>
#include "WindowHeader.h"
#include <iostream>
#include <d3d11.h>
#include "D3D11Helper.h"
#include "PipelineHelper.h"
#include "stb_image.h"


void Render(ID3D11DeviceContext* immediateContext, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsView,
	D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout,
	ID3D11Buffer* vertexBuffer, ID3D11SamplerState* mySampler, ID3D11ShaderResourceView* srv)
{
	float clearColour[4] = {0, 0, 0, 0};
	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	
	//Förberedd pipeline
	immediateContext->PSSetShaderResources(0, 1, &srv);
	immediateContext->PSSetSamplers(0, 1, &mySampler);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);
	

	immediateContext->Draw(4, 0);
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;
	HWND window;

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Failed to setup window!" << std::endl;
		return -1;
	}

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewport;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader; 
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;

	ID3D11RasterizerState* rState;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	ID3D11SamplerState* sState;

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to setup d3d11" << std::endl;
 		return -1;
	}

	if (!SetupPipline(device, vertexBuffer, vShader, pShader, inputLayout, texture, srv, sState))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	
	immediateContext->RSGetState(&rState);
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.CullMode = D3D11_CULL_FRONT;
	//Solid ändring
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = true;
	device->CreateRasterizerState(&rsDesc, &rState);
	immediateContext->RSSetState(rState);

	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, sState, srv);
		swapChain->Present(0, 0);
	}
	
	vertexBuffer->Release();
	inputLayout->Release();
	pShader->Release();
	vShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();
	rState->Release();
	
	

	return 0;
}