#include"RawInput.h"
#include<windowsx.h>

RawInput::RawInput() {

	keyState.resize(KeyMouseConst::KeyMax, InputState::None);
	mouseButtonState.resize(KeyMouseConst::MouseButtonMax, InputState::None);

}

void RawInput::RegisterRID(HWND hwnd) {

	//キーボード
	

}

void RawInput::Update() {

	keyStatePrev = keyState;
	mouseButtonStatePrev = mouseButtonState;
	mousePosPrev = mousePos;
	wheelDeltaPrev = wheelDelta;

	SetInput();

	//RAWINPUT実験
	RAWINPUT rawinput;

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