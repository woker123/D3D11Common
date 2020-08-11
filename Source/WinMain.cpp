#include <Windows.h>
#include <memory>
#include "GraphicWindow.h"
#include "D3D11System.h"
#include "D3D11Shader.h"
#include "D3D11CBuffer.h"
#include "VertexBuffer.h"

using namespace std;
using namespace D3DShader;

unsigned int stride = 3 * sizeof(float), offset = 0;

shared_ptr<GraphicWindow> g_window;
shared_ptr<VertexBuffer> g_triBuffer;
shared_ptr<VertexShader> g_vs;
shared_ptr<PixelShader> g_ps;
ComPtr<ID3D11InputLayout> g_vertexLayout;

void initVertexBuffer();
void initShaders();
void initInputLayout();
void setContext();
void draw();

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR lpCmdLine, INT nCmdShow)
{	
	g_window.reset(new GraphicWindow(hInstance, "Test", 0, 0, 800, 800));
	g_window->showWindow();
	D3D11System::initD3D11System(g_window->getHWND());

	initVertexBuffer();
	initShaders();
	initInputLayout();
	setContext();
	while (g_window->doMessage())
	{
		D3D11System::getInstance()->setViewport(0, 0, 784, 761);
		D3D11System::Context()->OMSetRenderTargets(1, D3D11System::getInstance()->getBackBufferRTV().GetAddressOf(), 
			D3D11System::getInstance()->getBaseDSV().Get());

		draw();

		D3D11System::getInstance()->getSwapChain()->Present(0, 0);
	}

	return 0;
}

void initVertexBuffer()
{
	float triangle[] = {0, 1, 0, 1, -1, 0, -1, -1, 0};
	g_triBuffer.reset(new VertexBuffer(triangle, sizeof(triangle)));
}

void initShaders()
{
	g_vs.reset(new VertexShader(L"./Shader/vs.hlsl", true));
	g_ps.reset(new PixelShader(L"./Shader/ps.hlsl", true));
}

void initInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC ied =
		{"V_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
	HRESULT create_inputLayout = 
	D3D11System::Device()->CreateInputLayout(&ied, 1, g_vs->getShaderBlob()->GetBufferPointer(),
		g_vs->getShaderBlob()->GetBufferSize(), &g_vertexLayout);
	assert(SUCCEEDED(create_inputLayout));
}

void setContext()
{
	D3D11System::Context()->IASetInputLayout(g_vertexLayout.Get());
	D3D11System::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11System::Context()->IASetVertexBuffers(0, 1, g_triBuffer->getD3D11Buffer().GetAddressOf(), &stride, &offset);
	D3D11System::Context()->VSSetShader(g_vs->getD3DShader().Get(), NULL, 0);
	D3D11System::Context()->PSSetShader(g_ps->getD3DShader().Get(), NULL, 0);
}

void draw()
{
	D3D11System::getInstance()->clearRTVAndDSV(0, 1, 0.4, 1, 1.0, 0xff);
	D3D11System::Context()->Draw(3, 0);
}

