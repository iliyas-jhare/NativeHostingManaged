
#include <iostream>
#include <Windows.h>

WNDCLASSEX host_window;
MSG message;
HINSTANCE h_instance = GetModuleHandle(nullptr);
HWND host_window_handle;
HWND managed_window_handle;

const wchar_t host_window_name[] = L"Native Host Window";
bool host_window_running = false;

typedef void (*have_you_reached_ptr) ();

namespace
{
	LRESULT CALLBACK host_window_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			host_window_running = false;
			break;
		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}

		return 0;
	}

	void init_host_window()
	{
		const auto host_wnd_icon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPLICATION));

		host_window.cbSize = sizeof(WNDCLASSEX);
		host_window.lpfnWndProc = host_window_proc;
		host_window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		host_window.cbClsExtra = 0;
		host_window.style = 0;
		host_window.cbWndExtra = 0;
		host_window.hInstance = h_instance;
		host_window.hIcon = host_wnd_icon;
		host_window.hIconSm = host_wnd_icon;
		host_window.lpszClassName = host_window_name;
		host_window.lpszMenuName = nullptr;

		// DO NOT set host_window.hbrBackground field otherwise it will flicker WPF rendering. 
	}

	void init_host_window_handle()
	{
		host_window_handle = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			host_window_name,
			host_window_name,
			WS_THICKFRAME | WS_OVERLAPPEDWINDOW,
			CW_DEFAULT,
			CW_USEDEFAULT,
			800,
			500,
			nullptr,
			nullptr,
			h_instance,
			nullptr);
	}

	void show_host_window()
	{
		RECT window_rect;
		GetWindowRect(host_window_handle, &window_rect);

		RECT desktop_rect;
		GetWindowRect(GetDesktopWindow(), &desktop_rect);

		const int x_pos = (desktop_rect.right - (window_rect.right - window_rect.left)) / 2;
		const int y_pos = (desktop_rect.bottom - (window_rect.bottom - window_rect.top)) / 2;

		::SetWindowPos(host_window_handle, 0, x_pos, y_pos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		ShowWindow(host_window_handle, SW_SHOW);
		UpdateWindow(host_window_handle);
		BringWindowToTop(host_window_handle);
		host_window_running = true;

		while (GetMessage(&message, nullptr, 0, 0) > 0 && host_window_running)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

int main()
{
	std::cout << "Native hosting Managed application started." << std::endl;

	init_host_window();

	if (!RegisterClassEx(&host_window))
	{
		std::cout << "Error code: " << GetLastError() << std::endl;
		getchar();
		return 0;
	}

	init_host_window_handle();

	if (host_window_handle == nullptr)
	{
		std::cout << "Error code: " << GetLastError() << std::endl;
		getchar();
		return 0;
	}

	show_host_window();

	std::cout << "Native hosting Managed application closed." << std::endl;
	getchar();
	return 0;
}

BOOL have_you_reached(const std::string* input)
{
	if (input->empty())
		return FALSE;

	if (!std::strcmp(input->c_str(), "OKAY"))
		return FALSE;

	return TRUE;
}
