#include"XInput.h"

#pragma comment(lib, "XInput.lib")

XInput::XInput(DWORD index)
	: padIndex(index), isConnect(false) 
{

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));

	buttonStateCurrent.resize(PadConst::ButtonNum, InputState::None);
	analogValue.resize(PadConst::AnalogNum, 0.0f);

}

void XInput::Update() {

	DWORD result;

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
	result = XInputGetState(padIndex, &xInputState);

	if (result == ERROR_SUCCESS) {

		isConnect = true;

	}
	else {

		isConnect = false;

	}

	UpdateButton();

}

void XInput::UpdateButton() {

	buttonStatePrev = buttonStateCurrent;

	for (UINT i = 0; i < buttonStateCurrent.size(); ++i) {



	}

}

InputState XInput::GetDegitalState(PadInput inputType) const {

	UINT index;
	index = GetIndex(inputType);

	//無効な値、アナログ入力ならはじく
	if (index == -1 ||
		inputType > PadInput::ThresholdAnalog) {

		return InputState::None;

	}

	return buttonStateCurrent[index];

}

AnalogStrength XInput::GetAnalogStrength(PadInput inputType) const {

	UINT index;
	index = GetIndex(inputType);

	//無効な値、デジタル入力ならはじく
	if (index == -1 ||
		inputType < PadInput::ThresholdAnalog) {

		return AnalogStrength::Zero;

	}

	if (analogValue[index] > PadConst::ThresholdLow)	return AnalogStrength::Low;
	if (analogValue[index] > PadConst::ThresholdMiddle) return AnalogStrength::Middle;
	if (analogValue[index] > PadConst::ThresholdHigh)	return AnalogStrength::High;

	return AnalogStrength::Zero;

}

float XInput::GetAnalogValue(PadInput inputType) const {

	UINT index;
	index = GetIndex(inputType);

	//無効な値、デジタル入力ならはじく
	if (index == -1 ||
		inputType < PadInput::ThresholdAnalog) {

		return 0.0f;

	}

	return analogValue[index];

}

PadInputIndex XInput::GetIndex(PadInput inputType) const {

	switch (inputType) {

	//デジタル用

	case PadInput::Up:		return PadInputIndex::Up;	break;
	case PadInput::Down:	return PadInputIndex::Down;	break;
	case PadInput::Left:	return PadInputIndex::Left;	break;
	case PadInput::Right:	return PadInputIndex::Right;	break;

	case PadInput::A:		return PadInputIndex::A;	break;
	case PadInput::B:		return PadInputIndex::B;	break;
	case PadInput::X:		return PadInputIndex::X;	break;
	case PadInput::Y:		return PadInputIndex::Y;	break;

	case PadInput::LB:		return PadInputIndex::LB;	break;
	case PadInput::RB:		return PadInputIndex::RB;	break;

	case PadInput::LThumb:	return PadInputIndex::LThumb;	break;
	case PadInput::RThumb:	return PadInputIndex::RThumb;	break;

	case PadInput::Start:	return PadInputIndex::Start;	break;
	case PadInput::View:	return PadInputIndex::View;	break;
	case PadInput::XBox:	return PadInputIndex::XBox;	break;
	
	//アナログ用

	case PadInput::LStickX:	return PadInputIndex::LStickX;	break;
	case PadInput::LStickY:	return PadInputIndex::LStickY;	break;

	case PadInput::RStickX:	return PadInputIndex::RStickX;	break;
	case PadInput::RStickY:	return PadInputIndex::RStickY;	break;

	case PadInput::LT:		return PadInputIndex::LT;	break;
	case PadInput::RT:		return PadInputIndex::RT;	break;

	default:

		return PadInputIndex::None;

		break;
	}

}

PadInput XInput::GetPadInput(UINT index) const {

	switch (index) {



	}

}