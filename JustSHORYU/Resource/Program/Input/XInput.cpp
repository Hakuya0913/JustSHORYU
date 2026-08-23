#include"XInput.h"

#pragma comment(lib, "XInput.lib")

void XInput::Update() {

	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);

}
