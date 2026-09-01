#include"InputMapper.h"

InputMapper::InputMapper(KeyMouseInput& keyMouse, XInput& xInput)
	:keyMouseInput(keyMouse), xInput(xInput),
	contextCurrent(InputContext::Menu)
{

	InitGamePlayMapping();
	InitMenuMapping();

}

void InputMapper::SetContext(InputContext context) {

	contextCurrent = context;

}

void InputMapper::InitGamePlayMapping() {


}

void InputMapper::InitMenuMapping() {



}

InputState InputMapper::GetStateFromBind(const InputBindDigital& bind) const {

	InputState state = InputState::None;

	//キーボード
	for (const auto& key : bind.GetKeys()) {

		state = CombineState(state, keyMouseInput.GetKeyState(key));

	}

	//マウスボタン
	for (const auto& button : bind.GetMouseButtons()) {

		state = CombineState(state, keyMouseInput.GetMouseButtonState(button));

	}

	//パッドボタン
	for (const auto& padButton : bind.GetPadButtons()) {

		state = CombineState(state, xInput.GetDigitalState(padButton));

	}

	return state;
}

InputState InputMapper::CombineState(InputState first, InputState second) const {

	if (first == InputState::Trigger || second == InputState::Trigger) {

		return InputState::Trigger;

	}
	else if (first == InputState::Hold || second == InputState::Hold) {

		return InputState::Hold;

	}
	else if (first == InputState::Release || second == InputState::Release) {

		return InputState::Release;

	}
	else{

		return InputState::None;

	}

	return InputState::None;

}

InputState InputMapper::GetState(GamePlayCommandDigital command) const {

	//ContextがGamePlayではない時無効
	if (contextCurrent != InputContext::GamePlay) {
	
		return InputState::None;

	}

	const size_t index = static_cast<size_t>(command);

	return GetStateFromBind(gamePlayBindsDigital[index]);

}

float InputMapper::GetValueFromBind(const InputBindAnalog& bind) const {
	float value = 0.0f;
	
	//マウスアナログ
	for (const auto& button : bind.GetMouseAnalog()) {

		value += static_cast<float>(keyMouseInput.GetMouseButtonState(button) == InputState::Hold);

	}

	//パッドアナログ
	for (const auto& padButton : bind.GetPadAnalog()) {

		value += xInput.GetAnalogValue(padButton);

	}

	return value;
}