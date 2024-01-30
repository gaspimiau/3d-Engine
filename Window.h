#pragma once

#include "miWin.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>
#include <optional>


//Debug macros, hay mas en Graphics.cpp
#define MIEXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define LAST_MIEXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define CHWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )

class Window
{
public:
	// MANEJO DE EXCEPCIONES
	class Exception : public MiExcepcion
	{
		using MiExcepcion::MiExcepcion;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
private:
	class WindowClass
	{
	public:
		static const char *GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass &) = delete;
		WindowClass &operator=(const WindowClass &) = delete;
		static constexpr const char *wndClassName = "3D Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, const char *name); // noexcept;
	~Window();
	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;
	static std::optional<int> ProcessMessages();
	Graphics &Gfx();

private:
	// GESTOR DE MENSAJES (API A CLASE)
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard kbd;
	Mouse mouse;

private:
	// PARAMETROS DE INSTANCIACION DE CLASE
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};

