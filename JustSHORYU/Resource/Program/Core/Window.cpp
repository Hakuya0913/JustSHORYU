#include"Window.h"
#include<vector>
#include"ConstantCore.h"
#include"../Input/InputManager.h"

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;

		break;
	case WM_INPUT:

		//キーマウ入力処理

	{

		UINT dwSize = 0;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

		std::vector<BYTE> buffer(dwSize);
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {

			break;

		}

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());

		auto rawInput = InputManager::GetInstance().GetRawInput();

		if (raw->header.dwType == RIM_TYPEKEYBOARD) {

			USHORT vk = raw->data.keyboard.VKey;
			bool isPress = !(raw->data.keyboard.Flags & RI_KEY_BREAK);
			rawInput.SetKeyboard(vk, isPress);

		}
		else if (raw->header.dwType == RIM_TYPEMOUSE) {

			RAWMOUSE& mouse = raw->data.mouse;

			if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {

				rawInput.SetMouseButton(0, true);

			}
			if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {

				rawInput.SetMouseButton(0, false);

			}

			if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {

				rawInput.SetMouseButton(1, true);

			}
			if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {

				rawInput.SetMouseButton(1, false);

			}

			rawInput.SetMouseMove(static_cast<float>(mouse.lLastX), static_cast<float>(mouse.lLastY));

		}

	}

		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}

void Window::Init() {

	hInstance = GetModuleHandle(nullptr);

	if (hInstance == nullptr) {

		return;

	}

	//ウィンドウ設定
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = ConstVal::Window::ClassName;
	wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

	//ウィンドウクラス登録
	RegisterClassEx(&wc);

	//ウィンドウサイズ設定
	RECT rect{};
	rect.right = static_cast<LONG>(ConstVal::Window::ScreenW);
	rect.bottom = static_cast<LONG>(ConstVal::Window::ScreenH);

	//ウィンドウサイズを調整
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	//ウィンドウを生成
	hwnd = CreateWindowEx(
		0,
		ConstVal::Window::ClassName,
		ConstVal::Window::WindowName,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	//ウィンドウを表示
	ShowWindow(hwnd, SW_SHOWNORMAL);

	//ウィンドウにフォーカス
	SetFocus(hwnd);

}