#include "Window.h"
#include <iostream>
#include <chrono>//
#pragma once

class App
{
	public:
		App();
		int Go();
		float Peek() const { return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count(); };//
	private:
		void DoFrame();
	private:
		Window wnd;
		std::chrono::steady_clock::time_point last;//
};
