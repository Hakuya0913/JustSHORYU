#include"XInput.h"
#include<algorithm>

#pragma comment(lib, "XInput.lib")

/*

enum と　index の依存を減らす

デッドゾーン処理の追加
スティックは円形
トリガーは線形

接続状態と非接続状態の処理追加

*/

XInput::XInput(DWORD index)
	: padIndex(index), isConnect(false) 
{

	InitState();

}

//状態の全初期化
void XInput::InitState() {

	ZeroMemory(&xInputState, sizeof(_XINPUT_STATE));

	buttonStateCurrent.fill(InputState::None);
	buttonStatePrev.fill(InputState::None);

	analogValueCurrent.fill(0.0f);
	analogValuePrev.fill(0.0f);
	analogValueDelta.fill(0.0f);

}

void XInput::Update() {

	DWORD result;

	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));
	result = XInputGetState(padIndex, &xInputState);

	isConnect = (result == ERROR_SUCCESS);

	if (isConnect) {

		UpdateDegitalInput();
		UpdateAnalogInput();

	}

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

	for (size_t i = 0; i < AnalogList.size(); ++i) {

		PadInputAnalog padInput = AnalogList[i];

		size_t index = PadInputIndexTableAnalog[static_cast<int>(padInput)];

		analogValueCurrent[index]	= ConvertAnalogValue(padInput);
		analogValueDelta[index]		= analogValueCurrent[index] - analogValuePrev[index];

	}
}

InputState XInput::GetDegitalState(PadInputDigital inputType) const {

	//未接続ならNone
	if (!isConnect) return InputState::None;

	auto index = PadInputIndexTableDigital[static_cast<int>(inputType)];

	return buttonStateCurrent[index];

}

AnalogStrength XInput::GetAnalogStrength(PadInputAnalog padInput) const {

	//未接続なら強さ0
	if (!isConnect) return AnalogStrength::Zero;

	auto index = PadInputIndexTableAnalog[static_cast<int>(padInput)];

	if (analogValueCurrent[static_cast<int>(index)] > PadConst::ThresholdHigh)		return AnalogStrength::High;
	if (analogValueCurrent[static_cast<int>(index)] > PadConst::ThresholdMiddle)	return AnalogStrength::Middle;
	if (analogValueCurrent[static_cast<int>(index)] > PadConst::ThresholdLow)		return AnalogStrength::Low;

	return AnalogStrength::Zero;
}

float XInput::GetAnalogValue(PadInputAnalog padInput) const {

	//未接続なら入力0
	if (!isConnect) return 0.0f;

	auto index = PadInputIndexTableAnalog[static_cast<int>(padInput)];

	return analogValueCurrent[index];

}

float XInput::GetAnalogDelta(PadInputAnalog padInput) const {

	auto index = PadInputIndexTableAnalog[static_cast<int>(padInput)];

	return analogValueDelta[index];

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

float XInput::ConvertAnalogValue(PadInputAnalog padInput) const {

	auto gamepad = xInputState.Gamepad;

	float result;

	switch (padInput) {

	case PadInputAnalog::LStickX:	result = static_cast<float>(gamepad.sThumbLX) / PadConst::StickMax;			break;
	case PadInputAnalog::LStickY:	result = static_cast<float>(gamepad.sThumbLY) / PadConst::StickMax;			break;

	case PadInputAnalog::RStickX:	result = static_cast<float>(gamepad.sThumbRX) / PadConst::StickMax;			break;
	case PadInputAnalog::RStickY:	result = static_cast<float>(gamepad.sThumbRY) / PadConst::StickMax;			break;

	case PadInputAnalog::LT:		result = static_cast<float>(gamepad.bLeftTrigger)	/ PadConst::TriggerMax; break;
	case PadInputAnalog::RT:		result = static_cast<float>(gamepad.bRightTrigger)	/ PadConst::TriggerMax;	break;

	default:						result = 0.0f;

	}

	//-1～1の範囲に正規化結果を収める
	result = std::clamp(result, -1.0f, 1.0f);

	//デッドゾーン適用
	result = ApplyDeadzone(result);

	return result;

}

DirectX::SimpleMath::Vector2 XInput::ApplyDeadzoneRadial(DirectX::SimpleMath::Vector2 vector) const {

	float length = vector.Length();

	auto deadzone = PadConst::Deadzone;

	if (length < deadzone) {

		return { 0.0f,0.0f };

	}

	float scale = (length - deadzone) / (1.0f - deadzone);

	return vector / length * scale;

}

float XInput::ApplyDeadzone(float value) const {
	
	float absValue = std::abs(value);
	auto deadzone = PadConst::Deadzone;

	if (absValue < deadzone) {

		return 0.0f;

	}

	return value;

}