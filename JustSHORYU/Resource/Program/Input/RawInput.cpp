#include"RawInput.h"
#include<windowsx.h>

RawInput::RawInput() {

	keyState.resize(KeyMouseConst::KeyMax, InputState::None);
	mouseButtonState.resize(KeyMouseConst::MouseButtonMax, InputState::None);

}

void RawInput::Update() {

	//WndProcÇ≈ílÇÃçXêV

	keyStatePrev = keyState;
	mouseButtonStatePrev = mouseButtonState;

}

void RawInput::SetMessage(UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_KEYDOWN:

		SetKeyboard(static_cast<USHORT>(wParam), true);

		break;
	case WM_KEYUP:

		SetKeyboard(static_cast<USHORT>(wParam), false);

		break;
	case WM_LBUTTONDOWN:

		SetMouseButton(KeyMouseConst::MouseButtonL, true);

		break;
	case WM_LBUTTONUP:

		SetMouseButton(KeyMouseConst::MouseButtonL, false);

		break;
	case WM_RBUTTONDOWN:

		SetMouseButton(KeyMouseConst::MouseButtonR, true);

		break;
	case WM_RBUTTONUP:

		SetMouseButton(KeyMouseConst::MouseButtonR, false);

		break;
	case WM_MBUTTONDOWN:

		SetMouseButton(KeyMouseConst::MouseButtonM, true);

		break;
	case WM_MBUTTONUP:

		SetMouseButton(KeyMouseConst::MouseButtonM, false);

		break;
	case WM_XBUTTONDOWN:

		if (HIWORD(wParam) == XBUTTON1) {

			SetMouseButton(KeyMouseConst::MouseButtonX1, true);

		}
		else if (HIWORD(wParam) == XBUTTON2) {

			SetMouseButton(KeyMouseConst::MouseButtonX2, true);

		}

		break;
	case WM_XBUTTONUP:

		if (HIWORD(wParam) == XBUTTON1) {

			SetMouseButton(KeyMouseConst::MouseButtonX1, false);

		}
		else if (HIWORD(wParam) == XBUTTON2) {

			SetMouseButton(KeyMouseConst::MouseButtonX2, false);

		}

		break;
	case WM_MOUSEMOVE:

		SetMouseMove(static_cast<float>(GET_X_LPARAM(lParam)),static_cast<float>(GET_Y_LPARAM(lParam)));

		break;
	case WM_MOUSEWHEEL:

		SetMouseWheel(static_cast<int>(GET_WHEEL_DELTA_WPARAM(wParam)));

		break;
	default:
		break;
	}

}

void RawInput::SetKeyboard(USHORT vk, bool isPress) {

	if (vk > KeyMouseConst::KeyMax) return;

	if (isPress) {

		if (keyState[vk] == InputState::None ||
			keyState[vk] == InputState::DeActive) {

			keyState[vk] = InputState::Active;
			return;

		}

		if (keyState[vk] == InputState::Active) {

			keyState[vk] = InputState::Hold;
			return;

		}

	}
	else {

		if (keyState[vk] == InputState::Active ||
			keyState[vk] == InputState::Hold) {

			keyState[vk] = InputState::DeActive;
			return;

		}

		if (keyState[vk] == InputState::DeActive) {

			keyState[vk] = InputState::None;
			return;

		}

	}

}

void RawInput::SetMouseButton(int number, bool isPress) {

	if (number >= KeyMouseConst::MouseButtonL && 
		number < KeyMouseConst::MouseButtonMax)
	{

		if (isPress) {

			if (mouseButtonState[number] == InputState::None ||
				mouseButtonState[number] == InputState::DeActive) {

				mouseButtonState[number] = InputState::Active;
				return;

			}

			if (mouseButtonState[number] == InputState::Active) {

				mouseButtonState[number] = InputState::Hold;
				return;

			}

		}
		else {

			if (mouseButtonState[number] == InputState::Active ||
				mouseButtonState[number] == InputState::Hold) {
				
				mouseButtonState[number] = InputState::DeActive;
				return;

			}

			if (mouseButtonState[number] == InputState::DeActive) {
				
				mouseButtonState[number] = InputState::None;
				return;

			}

		}

	}

}

void RawInput::SetMouseMove(float dx, float dy) {

	mouseDelta = { dx,dy };

}

void RawInput::SetMouseWheel(int delta) {

	wheelDelta = delta;

}