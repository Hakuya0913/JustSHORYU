#include"XInput.h"

#pragma comment(lib, "XInput.lib")

XInput::XInput(DWORD index)
	: padIndex(index), isConnect(false) 
{

	ZeroMemory(&stateCurrent, sizeof(XINPUT_STATE));
	ZeroMemory(&statePrev, sizeof(XINPUT_STATE));

}

void XInput::Update() {

	DWORD result;

	statePrev = stateCurrent;

	ZeroMemory(&stateCurrent, sizeof(XINPUT_STATE));
	result = XInputGetState(padIndex, &stateCurrent);

	if (result == ERROR_SUCCESS) {

		isConnect = true;

	}
	else {

		isConnect = false;

	}

}

InputState XInput::GetDegitalState(PadInput inputType) const {

	

}