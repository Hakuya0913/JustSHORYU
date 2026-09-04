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

//初期マッピング
void InputMapper::InitGamePlayMapping() {

	//デジタル入力のマッピング

	auto& jumpBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Jump)];
	jumpBind.AddKey(VK_SHIFT);
	jumpBind.AddPadButton(PadInputDigital::A);

	auto& dashBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Dash)];
	dashBind.AddKey(VK_LSHIFT);
	dashBind.AddMouseButton(KeyMouseConst::MouseButtonR);
	dashBind.AddPadButton(PadInputDigital::B);

	auto& attackBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Attack)];
	attackBind.AddKey('J');
	attackBind.AddMouseButton(KeyMouseConst::MouseButtonL);
	attackBind.AddPadButton(PadInputDigital::X);

	auto& guardBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Guard)];
	guardBind.AddKey('R');
	guardBind.AddPadButton(PadInputDigital::Y);

	auto& lockOnBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::LockOn)];
	lockOnBind.AddKey('B');
	lockOnBind.AddPadButton(PadInputDigital::RThumb);

	auto& menuOpenBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::MenuOpen)];
	menuOpenBind.AddKey(VK_ESCAPE);
	menuOpenBind.AddPadButton(PadInputDigital::Start);

	//アナログ入力のマッピング

	auto& moveBind = gamePlayBindsAnalog[static_cast<size_t>(GamePlayCommandAnalog::Move)];
	moveBind.AddPadAnalogX(PadInputAnalog::LStickX);
	moveBind.AddPadAnalogY(PadInputAnalog::LStickY);

	auto& guardBindAnalog = gamePlayBindsAnalog[static_cast<size_t>(GamePlayCommandAnalog::Guard)];

	

}

void InputMapper::InitMenuMapping() {

	//デジタル入力のマッピング

	auto& upBind = menuBindsDigital[static_cast<size_t>(MenuCommandDigital::Up)];
	upBind.AddKey(VK_UP);
	upBind.AddKey('W');
	upBind.AddPadButton(PadInputDigital::Up);



}

//コマンドを受け取り、Contextに応じた入力状態を返す

//GamePlayCommand
InputState InputMapper::GetState(GamePlayCommandDigital command) const {

	//ContextがGamePlayではない時無効
	if (contextCurrent != InputContext::GamePlay) {
	
		return InputState::None;

	}

	const size_t index = static_cast<size_t>(command);

	return GetStateFromBind(gamePlayBindsDigital[index]);

}

DirectX::SimpleMath::Vector2 InputMapper::GetValue(GamePlayCommandAnalog command) const {
	
	//ContextがGamePlayではない時無効
	if (contextCurrent != InputContext::GamePlay) {

		return DirectX::SimpleMath::Vector2(0.0f, 0.0f);

	}

	const size_t index = static_cast<size_t>(command);

	return GetValueFromBind(gamePlayBindsAnalog[index]);

}

//MenuCommand
InputState InputMapper::GetState(MenuCommandDigital command) const {
	
	//ContextがMenuではない時無効
	if (contextCurrent != InputContext::Menu) {

		return InputState::None;

	}

	const size_t index = static_cast<size_t>(command);

	return GetStateFromBind(menuBindsDigital[index]);

}

DirectX::SimpleMath::Vector2 InputMapper::GetValue(MenuCommandAnalog command) const {

	//ContextがMenuではない時無効
	if (contextCurrent != InputContext::Menu) {

		return DirectX::SimpleMath::Vector2(0.0f, 0.0f);

	}

	const size_t index = static_cast<size_t>(command);

	return GetValueFromBind(menuBindsAnalog[index]);

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

DirectX::SimpleMath::Vector2 InputMapper::GetValueFromBind(const InputBindAnalog& bind) const {

	DirectX::SimpleMath::Vector2 value = DirectX::SimpleMath::Vector2::Zero;

	//X軸
	
	//マウス
	for (auto mouseInput : bind.GetMouseAnalogX()) {
		
		switch (mouseInput) {
		case MouseInputAnalog::Position:

			value.x += static_cast<float>(keyMouseInput.GetMousePos().x);

			break;
		case MouseInputAnalog::Delta:

			value.x += static_cast<float>(keyMouseInput.GetMouseDelta().x);

			break;
		case MouseInputAnalog::Wheel:

			value.x += static_cast<float>(keyMouseInput.GetWheelDelta().x);

			break;
		default:


			break;
		}

	}

	//パッド
	for (auto padInput : bind.GetPadAnalogX()) {

		value.x += xInput.GetAnalogValue(padInput);

	}

	//Y軸

	//マウス
	for (auto mouseInput : bind.GetMouseAnalogY()) {

		switch (mouseInput) {
		case MouseInputAnalog::Position:

			value.y += static_cast<float>(keyMouseInput.GetMousePos().y);

			break;
		case MouseInputAnalog::Delta:

			value.y += static_cast<float>(keyMouseInput.GetMouseDelta().y);

			break;
		case MouseInputAnalog::Wheel:

			value.y += static_cast<float>(keyMouseInput.GetWheelDelta().y);

			break;
		default:

			break;
		}

	}

	//パッド
	for (auto padInput : bind.GetPadAnalogY()) {
		value.y += xInput.GetAnalogValue(padInput);
	}

	return value;

}

InputState InputMapper::CombineState(InputState first, InputState second) const {

	//優先度: Trigger > Hold > Release > None

	if (first == InputState::Trigger || second == InputState::Trigger) {

		return InputState::Trigger;

	}
	
	if (first == InputState::Hold || second == InputState::Hold) {

		return InputState::Hold;

	}
	
	if (first == InputState::Release || second == InputState::Release) {

		return InputState::Release;

	}

	return InputState::None;

}