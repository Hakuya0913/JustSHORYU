#include"XInput.h"

#pragma comment(lib, "XInput.lib")

XInput::XInput(DWORD index)
	: padIndex(index), isConnect(false) 
{

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));

	buttonStateCurrent.resize(	static_cast<int>(PadInputIndexDegital::Count), InputState::None);
	buttonStatePrev.resize(		static_cast<int>(PadInputIndexDegital::Count), InputState::None);

	analogValueCurrent.resize(	static_cast<int>(PadInputIndexAnalog::Count), 0.0f);
	analogValuePrev.resize(		static_cast<int>(PadInputIndexAnalog::Count), 0.0f);

}

void XInput::Update() {

	DWORD result;

	//前フレーム情報の更新
	buttonStatePrev = buttonStateCurrent;

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
	result = XInputGetState(padIndex, &xInputState);

	if (result == ERROR_SUCCESS) {

		isConnect = true;

	}
	else {

		isConnect = false;

	}


}

void XInput::UpdateDegitalInput() {

	auto button = xInputState.Gamepad.wButtons;

	for (auto list : DegitalList) {

		int index = ConvertToPadInputIndexAnalog(list);

	}

}

InputState XInput::GetDegitalState(PadInput inputType) const {

	PadInputIndexDegital index = ConvertToPadInputIndexDegital(inputType);

	//無効な値、アナログ入力ならはじく
	if (index == PadInputIndexDegital::None ||
		inputType > PadInput::ThresholdAnalog) {

		return InputState::None;

	}

	return buttonStateCurrent[static_cast<UINT>(index)];

}

AnalogStrength XInput::GetAnalogStrength(PadInput inputType) const {

	PadInputIndexAnalog index = ConvertToPadInputIndexAnalog(inputType);

	//無効な値、デジタル入力ならはじく
	if (index == PadInputIndexAnalog::None ||
		inputType < PadInput::ThresholdAnalog) {

		return AnalogStrength::Zero;

	}

	if (analogValueCurrent[static_cast<UINT>(index)] > PadConst::ThresholdLow)		return AnalogStrength::Low;
	if (analogValueCurrent[static_cast<UINT>(index)] > PadConst::ThresholdMiddle)	return AnalogStrength::Middle;
	if (analogValueCurrent[static_cast<UINT>(index)] > PadConst::ThresholdHigh)	return AnalogStrength::High;

	return AnalogStrength::Zero;

}

float XInput::GetAnalogValue(PadInput inputType) const {

	PadInputIndexAnalog index = ConvertToPadInputIndexAnalog(inputType);

	//無効な値、デジタル入力ならはじく
	if (index == PadInputIndexAnalog::None ||
		inputType < PadInput::ThresholdAnalog) {

		return 0.0f;

	}

	return analogValueCurrent[static_cast<UINT>(index)];

}

/*

PadInputIndexDegital XInput::ConvertToPadInputIndexDegital(PadInput inputType) const {

	switch (inputType) {

	//デジタル用

		case PadInput::Up:		return PadInputIndexDegital::Up;
		case PadInput::Down:	return PadInputIndexDegital::Down;
		case PadInput::Left:	return PadInputIndexDegital::Left;
		case PadInput::Right:	return PadInputIndexDegital::Right;

		case PadInput::A:		return PadInputIndexDegital::A;
		case PadInput::B:		return PadInputIndexDegital::B;
		case PadInput::X:		return PadInputIndexDegital::X;
		case PadInput::Y:		return PadInputIndexDegital::Y;

		case PadInput::LB:		return PadInputIndexDegital::LB;
		case PadInput::RB:		return PadInputIndexDegital::RB;

		case PadInput::LThumb:	return PadInputIndexDegital::LThumb;
		case PadInput::RThumb:	return PadInputIndexDegital::RThumb;

		case PadInput::Start:	return PadInputIndexDegital::Start;
		case PadInput::View:	return PadInputIndexDegital::View;
		case PadInput::XBox:	return PadInputIndexDegital::XBox;

		default:				return PadInputIndexDegital::None;
	}

}

PadInputIndexAnalog XInput::ConvertToPadInputIndexAnalog(PadInput inputType) const {

	switch (inputType) {

		case PadInput::LStickX:	return PadInputIndexAnalog::LStickX;
		case PadInput::LStickY:	return PadInputIndexAnalog::LStickY;

		case PadInput::RStickX:	return PadInputIndexAnalog::RStickX;
		case PadInput::RStickY:	return PadInputIndexAnalog::RStickY;

		case PadInput::LT:		return PadInputIndexAnalog::LT;
		case PadInput::RT:		return PadInputIndexAnalog::RT;

		default:				return PadInputIndexAnalog::None;

	}

}

PadInput XInput::ConvertToPadInput(PadInputIndexDegital index) const {

	switch (index) {

		case PadInputIndexDegital::Up:		return PadInput::Up;
		case PadInputIndexDegital::Down:	return PadInput::Down;
		case PadInputIndexDegital::Left:	return PadInput::Left;
		case PadInputIndexDegital::Right:	return PadInput::Right;

		case PadInputIndexDegital::A:		return PadInput::A;
		case PadInputIndexDegital::B:		return PadInput::B;
		case PadInputIndexDegital::X:		return PadInput::X;
		case PadInputIndexDegital::Y:		return PadInput::Y;

		case PadInputIndexDegital::LB:		return PadInput::LB;
		case PadInputIndexDegital::RB:		return PadInput::RB;

		case PadInputIndexDegital::LThumb:	return PadInput::LThumb;
		case PadInputIndexDegital::RThumb:	return PadInput::RThumb;

		case PadInputIndexDegital::Start:	return PadInput::Start;
		case PadInputIndexDegital::View:	return PadInput::View;
		case PadInputIndexDegital::XBox:	return PadInput::XBox;

		default:							return PadInput::None;

	}

}

PadInput XInput::ConvertToPadInput(PadInputIndexAnalog index) const {

	switch (index)
	{

		case PadInputIndexAnalog::LStickX:	return PadInput::LStickX;
		case PadInputIndexAnalog::LStickY:	return PadInput::LStickY;

		case PadInputIndexAnalog::RStickX:	return PadInput::RStickX;
		case PadInputIndexAnalog::RStickY:	return PadInput::RStickY;

		case PadInputIndexAnalog::LT:		return PadInput::LT;
		case PadInputIndexAnalog::RT:		return PadInput::RT;

		default:							return PadInput::None;

	}

}

*/