#include"KeyMouseInput.h"
#include<windowsx.h>
#include<hidusage.h>

KeyMouseInput::KeyMouseInput() {

	isKeyPress.resize(		KeyMouseConst::KeyMax, false);
	keyStateCurrent.resize(	KeyMouseConst::KeyMax, InputState::None);
	keyStatePrev.resize(	KeyMouseConst::KeyMax, InputState::None);

	isMouseButtonPress.resize(		KeyMouseConst::MouseButtonNum, false);
	mouseButtonStateCurrent.resize(	KeyMouseConst::MouseButtonNum, InputState::None);
	mouseButtonStatePrev.resize(	KeyMouseConst::MouseButtonNum, InputState::None);

	mouseDeltaCurrent	= Vector2_LONG::Zero();
	mouseDeltaPrev		= Vector2_LONG::Zero();

	wheelDeltaCurrent	= Vector2_SHORT::Zero();
	wheelDeltaPrev		= Vector2_SHORT::Zero();

	mousePosCurrent = DirectX::SimpleMath::Vector2::Zero;
	mousePosPrev	= DirectX::SimpleMath::Vector2::Zero;

	wheelDeltaCurrent	= Vector2_SHORT::Zero();
	wheelDeltaPrev		= Vector2_SHORT::Zero();

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

		const RAWKEYBOARD& keyboard = raw->data.keyboard;

		USHORT vk = keyboard.VKey;
		bool isPress = !(keyboard.Flags & RI_KEY_BREAK);

		SetKeyboard(vk, isPress);

	}
	else if (raw->header.dwType == RIM_TYPEMOUSE) {

		const RAWMOUSE& mouse = raw->data.mouse;

		// マウス移動
		if (mouse.usFlags & MOUSE_MOVE_RELATIVE) {

			SetMouseMove(static_cast<LONG>(mouse.lLastX), static_cast<LONG>(mouse.lLastY));

		}

		// マウスボタン
		//左クリック
		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)	SetMouseButton(KeyMouseConst::MouseButtonL, true);
		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)		SetMouseButton(KeyMouseConst::MouseButtonL, false);
		//右クリック
		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)	SetMouseButton(KeyMouseConst::MouseButtonR, true);
		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)		SetMouseButton(KeyMouseConst::MouseButtonR, false);
		//ホイールクリック
		if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)	SetMouseButton(KeyMouseConst::MouseButtonM, true);
		if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)	SetMouseButton(KeyMouseConst::MouseButtonM, false);
		//X1クリック(戻るボタン)
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)		SetMouseButton(KeyMouseConst::MouseButtonX1, true);
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)			SetMouseButton(KeyMouseConst::MouseButtonX1, false);
		//X2クリック(進むボタン)
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)		SetMouseButton(KeyMouseConst::MouseButtonX2, true);
		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)			SetMouseButton(KeyMouseConst::MouseButtonX2, false);

		// ホイール移動
		if (mouse.usButtonFlags & RI_MOUSE_WHEEL) {

			SHORT delta = static_cast<SHORT>(mouse.usButtonData);
			SetMouseWheel(true, delta);

		}
		if (mouse.usButtonFlags & RI_MOUSE_HWHEEL) {

			SHORT delta = static_cast<SHORT>(mouse.usButtonData);
			SetMouseWheel(false, delta);

		}

	}
}

//各入力状況の更新
void KeyMouseInput::Update() {

	//キーボード
	keyStatePrev = keyStateCurrent;
	UpdateKeyboard();

	//マウス
	mouseButtonStatePrev = mouseButtonStateCurrent;
	mousePosPrev		 = mousePosCurrent;
	mouseDeltaPrev		 = mouseDeltaCurrent;
	wheelDeltaPrev		 = wheelDeltaCurrent;
	UpdateMouseButton();

	//マウス移動
	mouseDeltaCurrent.x = static_cast<LONG>(mousePosCurrent.x - mousePosPrev.x);
	mouseDeltaCurrent.y = static_cast<LONG>(mousePosCurrent.y - mousePosPrev.y);

	//マウスホイールのリセット
	wheelDeltaCurrent = Vector2_SHORT::Zero();
}

void KeyMouseInput::UpdateKeyboard() {

	for (UINT i = 0; i < isKeyPress.size(); ++i) {

		if (isKeyPress[i]) {

			if (keyStatePrev[i] == InputState::None ||
				keyStatePrev[i] == InputState::DeActive) {

				keyStateCurrent[i] = InputState::Active;
				continue;

			}

			if (keyStatePrev[i] == InputState::Active) {

				keyStateCurrent[i] = InputState::Hold;
				continue;

			}

		}
		else {

			if (keyStatePrev[i] == InputState::Active ||
				keyStatePrev[i] == InputState::Hold) {

				keyStateCurrent[i] = InputState::DeActive;
				continue;

			}

			if (keyStatePrev[i] == InputState::DeActive) {

				keyStateCurrent[i] = InputState::None;
				continue;

			}

		}

	}

}

void KeyMouseInput::UpdateMouseButton() {

	for (UINT mouseNum = 0; mouseNum < KeyMouseConst::MouseButtonNum; ++mouseNum) {

		if (isMouseButtonPress[mouseNum]) {

			if (mouseButtonStatePrev[mouseNum] == InputState::None ||
				mouseButtonStatePrev[mouseNum] == InputState::DeActive) {

				mouseButtonStateCurrent[mouseNum] = InputState::Active;
				continue;

			}

			if (mouseButtonStatePrev[mouseNum] == InputState::Active) {

				mouseButtonStateCurrent[mouseNum] = InputState::Hold;
				continue;

			}

		}
		else {

			if (mouseButtonStatePrev[mouseNum] == InputState::Active ||
				mouseButtonStatePrev[mouseNum] == InputState::Hold) {

				mouseButtonStateCurrent[mouseNum] = InputState::DeActive;
				continue;

			}

			if (mouseButtonStatePrev[mouseNum] == InputState::DeActive) {

				mouseButtonStateCurrent[mouseNum] = InputState::None;
				continue;

			}

		}

	}

}

void KeyMouseInput::SetKeyboard(USHORT vk, bool isPress) {

	if (vk >= KeyMouseConst::KeyMax) return;

	isKeyPress[vk] = isPress;

}

void KeyMouseInput::SetMouseButton(USHORT number, bool isPress) {

	if (number >= KeyMouseConst::MouseButtonNum)
	{

		return;

	}

	isMouseButtonPress[number] = isPress;

}

void KeyMouseInput::SetMouseMove(LONG dx, LONG dy) {

	mousePosCurrent.x += static_cast<float>(dx);
	mousePosCurrent.y += static_cast<float>(dy);

}

void KeyMouseInput::SetMouseWheel(bool isVertical, SHORT delta) {

	if (isVertical) {

		wheelDeltaCurrent.y += delta;

	}
	else {

		wheelDeltaCurrent.x += delta;

	}

}