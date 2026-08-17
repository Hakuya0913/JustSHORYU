#include"RawInput.h"

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

		SetKeyboard(static_cast<USHORT>(wParam), true);

		break;
	case WM_LBUTTONDOWN:

		SetMouseButton(KeyMouseConst::MouseButtonL, true);

		break;
	case WM_LBUTTONUP:

		SetMouseButton(KeyMouseConst::MouseButtonL, false);

		break;
	case WM_RBUTTONDOWN:

		SetMouseButton(KeyMouseConst::MouseButtonR, false);

		break;
	case WM_RBUTTONUP:

		SetMouseButton(KeyMouseConst::MouseButtonR, false);

		break;
	case WM_MBUTTONDOWN:

		SetMouseButton(KeyMouseConst::MouseButtonM, false);

		break;
	case WM_MBUTTONUP:

		SetMouseButton(KeyMouseConst::MouseButtonM, false);

		break;
	case WM_XBUTTONDOWN:

		

		break;
	case WM_XBUTTONUP:



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

void RawInput::SetMouseButton(int button, bool isPress) {

	if (button >= )
	{

	}

}

void RawInput::SetMouseMove(float dx, float dy) {

	mouseDelta = { dx,dy };

}