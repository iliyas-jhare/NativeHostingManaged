
#include <iostream>
#include <Windows.h>

// Objects
WNDCLASSEX host_window;
MSG message;
HINSTANCE h_instance = GetModuleHandle(nullptr);
HWND host_window_handle;
HWND managed_window_handle;

// Configs
const wchar_t host_window_name[] = L"Native Host Window";
bool host_window_running = false;

// Functions
LRESULT CALLBACK HostWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
}

void init_host_window()
{
	const auto host_wnd_icon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPLICATION));

	host_window.cbSize = sizeof(WNDCLASSEX);
	host_window.lpfnWndProc = HostWindowProc;
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

	std::cout << "Native hosting Managed application closed." << std::endl;
	getchar();
	return 0;
}
