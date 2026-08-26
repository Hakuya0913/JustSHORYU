#include"XInput.h"

#pragma comment(lib, "XInput.lib")

XInput::XInput(DWORD index)
	: padIndex(index), isConnect(false) 
{

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));

	buttonStateCurrent.fill(InputState::None);
	buttonStatePrev.fill(	InputState::None);

	analogValueCurrent.fill(0.0f);
	analogValuePrev.fill(	0.0f);

}

void XInput::Update() {

	DWORD result;

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
	result = XInputGetState(padIndex, &xInputState);

	isConnect = (result == ERROR_SUCCESS);

	UpdateDegitalInput();
	UpdateAnalogInput();

}

void XInput::UpdateDegitalInput() {

	buttonStatePrev = buttonStateCurrent;

	auto button = xInputState.Gamepad.wButtons;

	for (size_t i = 0; i < DigitalList.size(); ++i) {

		bool	isPress	= button & ConvertWORD(DigitalList[i]);
		size_t	index	= PadInputIndexTableDigital[static_cast<int>(DigitalList[i])];

		if (isPress) {

			if (buttonStatePrev[index] == InputState::Release ||
				buttonStatePrev[index] == InputState::None) {

				buttonStateCurrent[index] = InputState::Trigger;
				continue;

			}

			if (buttonStatePrev[index] == InputState::Trigger) {

				buttonStateCurrent[index] = InputState::Hold;
				continue;

			}

		}
		else {

			if (buttonStatePrev[index] == InputState::Trigger ||
				buttonStatePrev[index] == InputState::Hold) {

				buttonStateCurrent[index] = InputState::Release;
				continue;

			}

			if (buttonStatePrev[index] == InputState::Release) {

				buttonStateCurrent[index] = InputState::None;
				continue;

			}

		}

	}

}

void XInput::UpdateAnalogInput() {

	analogValuePrev = analogValueCurrent;

	for (int i = 0; i < analogValueCurrent.size(); ++i) {

		analogValueCurrent[i] = ConvertAnalogValue(AnalogList[i]);

	}
}

InputState XInput::GetDegitalState(PadInputDigital inputType) const {

	return buttonStateCurrent[PadInputIndexTableDigital[static_cast<int>(inputType)]];

}

AnalogStrength XInput::GetAnalogStrength(PadInputAnalog inputType) const {

	if (analogValueCurrent[static_cast<int>(inputType)] > PadConst::ThresholdHigh)		return AnalogStrength::High;
	if (analogValueCurrent[static_cast<int>(inputType)] > PadConst::ThresholdMiddle)	return AnalogStrength::Middle;
	if (analogValueCurrent[static_cast<int>(inputType)] > PadConst::ThresholdLow)		return AnalogStrength::Low;

	return AnalogStrength::Zero;
}

float XInput::GetAnalogValue(PadInputAnalog inputType) const {

	return analogValueCurrent[static_cast<int>(inputType)];

}

WORD XInput::ConvertWORD(PadInputDigital padInput) const {

	switch (padInput) {

	case PadInputDigital::Up:		return XINPUT_GAMEPAD_DPAD_UP;
	case PadInputDigital::Down:		return XINPUT_GAMEPAD_DPAD_DOWN;
	case PadInputDigital::Left:		return XINPUT_GAMEPAD_DPAD_LEFT;
	case PadInputDigital::Right:	return XINPUT_GAMEPAD_DPAD_RIGHT;

	case PadInputDigital::A:		return XINPUT_GAMEPAD_A;
	case PadInputDigital::B:		return XINPUT_GAMEPAD_B;
	case PadInputDigital::X:		return XINPUT_GAMEPAD_X;
	case PadInputDigital::Y:		return XINPUT_GAMEPAD_Y;

	case PadInputDigital::LB:		return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case PadInputDigital::RB:		return XINPUT_GAMEPAD_RIGHT_SHOULDER;

	case PadInputDigital::LThumb:	return XINPUT_GAMEPAD_LEFT_THUMB;
	case PadInputDigital::RThumb:	return XINPUT_GAMEPAD_RIGHT_THUMB;

	case PadInputDigital::Start:	return XINPUT_GAMEPAD_START;
	case PadInputDigital::View:		return XINPUT_GAMEPAD_BACK;

	default:						return 0;

	}

}
