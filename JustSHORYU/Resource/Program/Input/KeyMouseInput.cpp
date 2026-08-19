#include"KeyMouseInput.h"
#include<windowsx.h>
#include<hidusage.h>

KeyMouseInput::KeyMouseInput() {

	keyState.resize(KeyMouseConst::KeyMax, InputState::None);
	mouseButtonState.resize(KeyMouseConst::MouseButtonMax, InputState::None);

}

void KeyMouseInput::RegisterRID(HWND hwnd) {

	RAWINPUTDEVICE rid[KeyMouseConst::RowInputDeviceCount] = {};

	//キーボード設定
	rid[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage		= HID_USAGE_GENERIC_KEYBOARD;
	rid[0].dwFlags		= RIDEV_INPUTSINK;
	rid[0].hwndTarget	= hwnd;

	//マウス設定
	rid[1].usUsagePage	= HID_USAGE_PAGE_GENERIC;
	rid[1].usUsage		= HID_USAGE_GENERIC_MOUSE;
	rid[1].dwFlags		= RIDEV_INPUTSINK;
	rid[1].hwndTarget	= hwnd;

	RegisterRawInputDevices(rid, KeyMouseConst::RowInputDeviceCount, sizeof(RAWINPUTDEVICE));

}

void KeyMouseInput::SetLParam(LPARAM lParam) {

	UINT dwSize = 0;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

	if (dwSize == 0) {

		return;

	}

	std::vector<BYTE> rawdata(dwSize);
	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawdata.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {

		return;

	}

	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.data());

	if (raw->header.dwType == RIM_TYPEKEYBOARD) {

		const RAWKEYBOARD& kb = raw->data.keyboard;

		USHORT vk = kb.VKey;
		bool isPress = !(kb.Flags & RI_KEY_BREAK);

		SetKeyboard(vk, isPress);
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE) {

		const RAWMOUSE& ms = raw->data.mouse;

		// マウス移動
		if (ms.usFlags == MOUSE_MOVE_RELATIVE) {
			SetMouseMove((float)ms.lLastX, (float)ms.lLastY);
		}

		// ボタン
		if (ms.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)  SetMouseButton(0, true);
		if (ms.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)    SetMouseButton(0, false);

		if (ms.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) SetMouseButton(1, true);
		if (ms.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)   SetMouseButton(1, false);

		if (ms.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) SetMouseButton(2, true);
		if (ms.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)   SetMouseButton(2, false);

		// ホイール
		if (ms.usButtonFlags & RI_MOUSE_WHEEL) {
			SHORT delta = (SHORT)ms.usButtonData;
			SetMouseWheel((int)delta);
		}
	}
}

void KeyMouseInput::Update() {

	keyStatePrev = keyState;
	mouseButtonStatePrev = mouseButtonState;
	mousePosPrev = mousePos;
	wheelDeltaPrev = wheelDelta;


	// キー
	for (int i = 0; i < 256; i++) {
		InputState prev = keyStatePrev[i];
		InputState now = keyState[i];

		if (now == InputState::Press && prev == InputState::Free)
			keyState[i] = InputState::Trigger;
		else if (now == InputState::Free && prev == InputState::Press)
			keyState[i] = InputState::Release;

		keyStatePrev[i] = keyState[i];
	}

	// マウスボタン
	for (int i = 0; i < mouseButtonState.size(); i++) {
		InputState prev = mouseButtonStatePrev[i];
		InputState now = mouseButtonState[i];

		if (now == InputState::Press && prev == InputState::Free)
			mouseButtonState[i] = InputState::Trigger;
		else if (now == InputState::Free && prev == InputState::Press)
			mouseButtonState[i] = InputState::Release;

		mouseButtonStatePrev[i] = mouseButtonState[i];
	}

	// マウス移動
	mouseDeltaPrev = mouseDelta;
	mouseDelta = mousePos - mousePosPrev;
	mousePosPrev = mousePos;

	// ホイール
	wheelDeltaPrev = wheelDelta;

}

void KeyMouseInput::SetKeyboard(USHORT vk, bool isPress) {

	if (vk > KeyMouseConst::KeyMax) return;

	if (isPress) {

		if (keyStatePrev[vk] == InputState::None ||
			keyStatePrev[vk] == InputState::DeActive) {

			keyStatePrev[vk] = InputState::Active;
			return;

		}

		if (keyStatePrev[vk] == InputState::Active) {

			keyState[vk] = InputState::Hold;
			return;

		}

	}
	else {

		if (keyStatePrev[vk] == InputState::Active ||
			keyStatePrev[vk] == InputState::Hold) {

			keyState[vk] = InputState::DeActive;
			return;

		}

		if (keyStatePrev[vk] == InputState::DeActive) {

			keyState[vk] = InputState::None;
			return;

		}

	}

	//AIコピペ分
	if (vk >= 256) return;

	keyState[vk] = isPress ? InputState::Press : InputState::Free;

}

void KeyMouseInput::SetMouseButton(int number, bool isPress) {

	if (number >= KeyMouseConst::MouseButtonL && 
		number < KeyMouseConst::MouseButtonMax)
	{

		if (isPress) {

			if (mouseButtonStatePrev[number] == InputState::None ||
				mouseButtonStatePrev[number] == InputState::DeActive) {

				mouseButtonState[number] = InputState::Active;
				return;

			}

			if (mouseButtonStatePrev[number] == InputState::Active) {

				mouseButtonState[number] = InputState::Hold;
				return;

			}

		}
		else {

			if (mouseButtonStatePrev[number] == InputState::Active ||
				mouseButtonStatePrev[number] == InputState::Hold) {
				
				mouseButtonState[number] = InputState::DeActive;
				return;

			}

			if (mouseButtonStatePrev[number] == InputState::DeActive) {
				
				mouseButtonState[number] = InputState::None;
				return;

			}

		}

	}

	//AIコピペ分
	if (number >= mouseButtonState.size()) return;

	mouseButtonState[number] = isPress ? InputState::Press : InputState::Free;

}

void KeyMouseInput::SetMouseMove(float dx, float dy) {

	mousePos.x += dx;
	mousePos.y += dy;

}

void KeyMouseInput::SetMouseWheel(int delta) {

	wheelDelta = delta;

}