#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;


Window::WindowClass::WindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	// debug icono, para ver si esta en la misma carpeta, cambiar por direccion dinamica


	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	// cambiar por direccion dinamica
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, "C:\\Users\\Chulu\\OneDrive\\Escritorio\\projectos\\3D_Renderer_Final\\Icon_32.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, "C:\\Users\\Chulu\\OneDrive\\Escritorio\\projectos\\3D_Renderer_FinalIcon_32.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char *Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int width, int height, const char *name)
	: width(width),
	  height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	if ((AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)) == 0)
	{
		throw LAST_MIEXCEPT();
	}
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);
	if (hWnd == nullptr)
	{
		throw LAST_MIEXCEPT();
	}
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

Graphics &Window::Gfx()
{
	if (!pGfx)
	{
		throw;
	}
	return *pGfx;
}

////////////////////////////////////////////////////////////
//		GESTION DE MENSAJES
////////////////////////////////////////////////////////////

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
		Window *const pWnd = static_cast<Window *>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window *const pWnd = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return (0);
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

	// Mensajes del teclado
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;

	// Mensajes del mouse
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move/ mantain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRigthPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRigthReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// chech for quit message processed by peekmessage
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// return empty optional when not quitting app
	return {};
}

////////////////////////////////////////////////////////////
//		MANEJO DE EXCEPCIONES
////////////////////////////////////////////////////////////


std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char *pMsgBuf = nullptr;
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0)
	{
		return "Codigo de Error no Identificado";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Mi Excepcion";
}


HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}
std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(hr);
}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "Mi Excepcion [No Graphics]";
}
