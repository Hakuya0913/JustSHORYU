#include"RawInput.h"

RawInput::RawInput() {

	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(mouseButton, sizeof(mouseButton));

}

void RawInput::Update() {

	//WndProcで値の更新
	//マウスの値を舞フレーム０にする設計もある

}

bool RawInput::IsPress(PhysicalKey keyCode) const {

	switch (keyCode) {

		case PhysicalKey::W :			return keyState['W'];
		case PhysicalKey::A :			return keyState['A'];
		case PhysicalKey::S :			return keyState['S'];
		case PhysicalKey::D :			return keyState['D'];
		case PhysicalKey::Space :		return keyState[VK_SPACE];
		case PhysicalKey::LeftMouse :	return keyState[0];
		case PhysicalKey::RightMouse :	return keyState[1];
		default :						return false;

	}

}

float RawInput::GetAxis(PhysicalAxis keyCode) const {

	switch (keyCode){

		case PhysicalAxis::MouseX:	return mouseX;
		case PhysicalAxis::MouseY:	return mouseY;
		default:					return	0.0f;

	}

}

void RawInput::SetKeyboard(USHORT vk, bool isPress) {

	if (vk < KeyMouseConst::KeyMax) {

		keyState[vk] = isPress;

	}

}

void RawInput::SetMouseButton(int button, bool isPress) {

	if (button >= 0 && button < KeyMouseConst::MouseButtonMax) {

		mouseButton[button] = isPress;

	}

}

void RawInput::SetMouseMove(float dx, float dy) {

	mouseX = dx;
	mouseY = dy;

}