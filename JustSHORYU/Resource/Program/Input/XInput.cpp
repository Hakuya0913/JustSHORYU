#include"XInput.h"

void XInput::Update() {

	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);

}

bool XInput::IsPress(PhysicalKey keyCode) const {

	switch (keyCode) {
	case PhysicalKey::PadA:
		break;
	case PhysicalKey::PadB:
		break;
	case PhysicalKey::PadX:
		break;
	case PhysicalKey::PadY:
		break;
	case PhysicalKey::PadUp:
		break;
	case PhysicalKey::PadDown:
		break;
	case PhysicalKey::PadLeft:
		break;
	case PhysicalKey::PadRight:
		break;
	}

}