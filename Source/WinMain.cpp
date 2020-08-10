#include <Windows.h>
#include <memory>
#include "GraphicWindow.h"
#include "D3D11System.h"
#include "D3D11Shader.h"

using namespace std;
using namespace D3DShader;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR lpCmdLine, INT nCmdShow)
{	
	shared_ptr<GraphicWindow> window(new GraphicWindow(hInstance, "Test", 50, 50, 800, 800));
	window->showWindow();
	D3D11System::initD3D11System(window->getHWND());

	shared_ptr<VertexShader> vs(new VertexShader(L"./Shader/vs.hlsl", true));
	shared_ptr<PixelShader> ps(new PixelShader(L"./Shader/ps.hlsl", true));
	D3D11System::Context()->VSSetShader(vs->getD3DShader().Get(), NULL, 0);
	D3D11System::Context()->PSSetShader(ps->getD3DShader().Get(), NULL, 0);

	
	while (window->doMessage())
	{
		D3D11System::getInstance()->setViewport(0, 0, window->getWidth(), window->getHeight());
		D3D11System::getInstance()->clearRTVAndDSV(0, 1, 0.4, 1, 1.0, 0xff);

		D3D11System::getInstance()->getSwapChain()->Present(0, 0);
	}






	return 0;
}
