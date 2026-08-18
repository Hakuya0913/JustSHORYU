#include"RawInput.h"
#include<windowsx.h>

RawInput::RawInput() {

	keyState.resize(KeyMouseConst::KeyMax, InputState::None);
	mouseButtonState.resize(KeyMouseConst::MouseButtonMax, InputState::None);

}

void RawInput::Update() {

	keyStatePrev = keyState;
	mouseButtonStatePrev = mouseButtonState;
	mousePosPrev = mousePos;
	wheelDeltaPrev = wheelDelta;

	SetInput();

	//RAWINPUTŽÀŒ±
	RAWINPUT rawinput;

}

void RawInput::SetMessage(UINT message, WPARAM wParam, LPARAM lParam) {

	this->message = message;
	this->wParam  = wParam;
	this->lParam  = lParam;

}

void RawInput::SetInput() {

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

		SetMouseMove(static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam)));

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

}

void RawInput::SetMouseButton(int number, bool isPress) {

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

}

void RawInput::SetMouseMove(float dx, float dy) {

	mouseDelta = { dx,dy };

}

void RawInput::SetMouseWheel(int delta) {

	wheelDelta = delta;

}