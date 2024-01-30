#include "App.h"

App::App()
	:
	wnd( 800,800,"MiApp")
	
{
	last = std::chrono::steady_clock::now();//
}
	
int App::Go()
{
	while( true)
	{
		if( const auto ecode = Window::ProcessMessages() )
		{
			return *ecode;			
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = sin(static_cast<float>(Peek())*5);
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle();
	wnd.Gfx().EndFrame();
	
}
