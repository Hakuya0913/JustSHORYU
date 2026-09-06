#include"InputMapper.h"

/*

GetStrength(GameCommandAnalog)関数で1D,2Dの強さを合成している部分懸念点
改善案を模索必要

*/

InputMapper::InputMapper(KeyMouseInput& keyMouse, XInput& xInput)
	:keyMouseInput(keyMouse), xInput(xInput),
	contextCurrent(InputContext::Menu)
{

	InitGamePlayMapping();
	InitMenuMapping();

}

void InputMapper::SetContext(InputContext context) 
{

	contextCurrent = context;

}

//初期マッピング
void InputMapper::InitGamePlayMapping() 
{

	//デジタル入力のマッピング

	//Jump
	auto& jumpBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Jump)];
	jumpBind.AddKey(VK_SHIFT);
	jumpBind.AddPadButton(PadInputDigital::A);

	//Dash
	auto& dashBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Dash)];
	dashBind.AddKey(VK_LSHIFT);
	dashBind.AddMouseButton(KeyMouseConst::MouseButtonR);
	dashBind.AddPadButton(PadInputDigital::B);

	//Attack
	auto& attackBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Attack)];
	attackBind.AddKey('J');
	attackBind.AddMouseButton(KeyMouseConst::MouseButtonL);
	attackBind.AddPadButton(PadInputDigital::X);

	//Guard
	auto& guardBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::Guard)];
	guardBind.AddKey('R');

	//LockOn
	auto& lockOnBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::LockOn)];
	lockOnBind.AddKey('B');
	lockOnBind.AddPadButton(PadInputDigital::RThumb);

	//MenuOpen
	auto& menuOpenBind = gamePlayBindsDigital[static_cast<size_t>(GamePlayCommandDigital::MenuOpen)];
	menuOpenBind.AddKey(VK_ESCAPE);
	menuOpenBind.AddPadButton(PadInputDigital::Start);

	//アナログ入力のマッピング

	

}

void InputMapper::InitMenuMapping() 
{

	//デジタル入力のマッピング

	auto& upBind = menuBindsDigital[static_cast<size_t>(MenuCommandDigital::Up)];
	upBind.AddKey(VK_UP);
	upBind.AddKey('W');
	upBind.AddPadButton(PadInputDigital::Up);



}

#pragma region GamePlayCommandから各入力の取得

InputState InputMapper::GetState(GamePlayCommandDigital command) const
{

	//現在のContextがGamePlayでなければ無効
	if (contextCurrent != InputContext::GamePlay) return InputState::None;

	//Countが指定されたら無効
	if (command == GamePlayCommandDigital::Count) return InputState::None;

	const auto& bind = gamePlayBindsDigital[static_cast<size_t>(command)];
	return GetStateFromBind(bind);

}

AnalogStrength InputMapper::GetStrength(GamePlayCommandAnalog command) const
{

	//現在のContextがGamePlayでなければ無効
	if (contextCurrent != InputContext::GamePlay) return AnalogStrength::Zero;
	
	//Countが指定されたら無効
	if (command == GamePlayCommandAnalog::Count) return AnalogStrength::Zero;

	const size_t index = static_cast<size_t>(command);

	const auto& bind1D = gamePlayBindsAnalog1D[index];
	const auto& bind2D = gamePlayBindsAnalog2D[index];

	AnalogStrength strength1D = GetStrengthFromBind(bind1D);
	AnalogStrength strength2D = GetStrengthFromBind(bind2D);

	return CombineStrength(strength1D, strength2D);

}

float InputMapper::GetValue1D(GamePlayCommandAnalog command) const
{
	//現在のContextがGamePlayでなければ無効
	if (contextCurrent != InputContext::GamePlay) return 0.0f;
	//Countが指定されたら無効
	if (command == GamePlayCommandAnalog::Count) return 0.0f;

	const size_t index = static_cast<size_t>(command);
	const auto& bind1D = gamePlayBindsAnalog1D[index];

	return GetValueFromBind(bind1D);

}

DirectX::SimpleMath::Vector2 InputMapper::GetValue2D(GamePlayCommandAnalog command) const
{
	//現在のContextがGamePlayでなければ無効
	if (contextCurrent != InputContext::GamePlay) return DirectX::SimpleMath::Vector2::Zero;
	//Countが指定されたら無効
	if (command == GamePlayCommandAnalog::Count) return DirectX::SimpleMath::Vector2::Zero;

	const size_t index = static_cast<size_t>(command);
	const auto& bind2D = gamePlayBindsAnalog2D[index];

	return GetValueFromBind(bind2D);

}

#pragma endregion

#pragma region MenuCommandから各入力の取得

InputState InputMapper::GetState(MenuCommandDigital command) const
{
	
	//現在のContextがMenuでなければ無効
	if (contextCurrent != InputContext::Menu) return InputState::None;
	//Countが指定されたら無効
	if (command == MenuCommandDigital::Count) return InputState::None;

	const auto& bind = menuBindsDigital[static_cast<size_t>(command)];
	return GetStateFromBind(bind);

}

AnalogStrength InputMapper::GetStrength(MenuCommandAnalog command) const
{

	//現在のContextがMenuでなければ無効
	if (contextCurrent != InputContext::Menu) return AnalogStrength::Zero;
	//Countが指定されたら無効
	if (command == MenuCommandAnalog::Count) return AnalogStrength::Zero;

	const size_t index = static_cast<size_t>(command);
	const auto& bind1D = menuBindsAnalog1D[index];
	const auto& bind2D = menuBindsAnalog2D[index];

	AnalogStrength strength1D = GetStrengthFromBind(bind1D);
	AnalogStrength strength2D = GetStrengthFromBind(bind2D);

	return CombineStrength(strength1D, strength2D);

}

float InputMapper::GetValue1D(MenuCommandAnalog command) const
{
	//現在のContextがMenuでなければ無効
	if (contextCurrent != InputContext::Menu) return 0.0f;
	//Countが指定されたら無効
	if (command == MenuCommandAnalog::Count) return 0.0f;

	const size_t index = static_cast<size_t>(command);
	const auto& bind1D = menuBindsAnalog1D[index];

	return GetValueFromBind(bind1D);

}

DirectX::SimpleMath::Vector2 InputMapper::GetValue2D(MenuCommandAnalog command) const
{
	//現在のContextがMenuでなければ無効
	if (contextCurrent != InputContext::Menu) return DirectX::SimpleMath::Vector2::Zero;
	//Countが指定されたら無効
	if (command == MenuCommandAnalog::Count) return DirectX::SimpleMath::Vector2::Zero;

	const size_t index = static_cast<size_t>(command);
	const auto& bind2D = menuBindsAnalog2D[index];

	return GetValueFromBind(bind2D);
}

#pragma endregion

#pragma region BindからAnalogStrengthの取得

//1Dアナログ入力の強さを返す
AnalogStrength InputMapper::GetStrengthFromBind(const InputBindAnalog1D& bind) const
{
	AnalogStrength strength = AnalogStrength::Zero;
	
	//マウスのアナログ入力は強さを返すのは困難
	//Zeroのまま返す

	//Padのアナログ入力の強さを返す
	for (const auto& padInput : bind.GetPadAnalog())
	{

		AnalogStrength padStrength = xInput.GetAnalogStrength(padInput);
		strength = CombineStrength(strength, padStrength);

	}

	return strength;

}

//2Dアナログ入力の強さを返す
AnalogStrength InputMapper::GetStrengthFromBind(const InputBindAnalog2D& bind) const
{
	AnalogStrength strength = AnalogStrength::Zero;
	
	//マウスのアナログ入力は強さを返すのは困難
	//Zeroのまま返す
	
	//Padのアナログ入力の強さを返す
	for (const auto& padInput : bind.GetPadAnalog())
	{
		
		AnalogStrength padStrength = xInput.GetAnalogStrength(padInput);
		strength = CombineStrength(strength, padStrength);

	}

	return strength;
}

#pragma endregion

#pragma region BindからInputStateの取得

InputState InputMapper::GetStateFromBind(const InputBindDigital& bind) const
{

	InputState state = InputState::None;
	
	//キーボード入力の状態を取得
	for (const auto& key : bind.GetKeys())
	{
		InputState keyState = keyMouseInput.GetKeyState(key);
		state = CombineState(state, keyState);
	}

	//マウスボタン入力の状態を取得
	for (const auto& mouseButton : bind.GetMouseButtons())
	{
		InputState mouseButtonState = keyMouseInput.GetMouseButtonState(mouseButton);
		state = CombineState(state, mouseButtonState);
	}

	//Padボタン入力の状態を取得
	for (const auto& padButton : bind.GetPadButtons())
	{
		InputState padButtonState = xInput.GetDigitalState(padButton);
		state = CombineState(state, padButtonState);
	}

	return state;
}

#pragma endregion

#pragma region BindからValueの取得

float InputMapper::GetValueFromBind(const InputBindAnalog1D& bind) const
{
	
	float value = 0.0f;

	//マウス
	for (const auto& mouseInput : bind.GetMouseAnalog())
	{
		
		switch (mouseInput)
		{

		case MouseInputAnalog1D::Wheel:

			value += static_cast<float>(keyMouseInput.GetWheelDelta());

			break;
		default:
			break;
		}

	}

	//パッド
	for (const auto& padInput : bind.GetPadAnalog())
	{

		value += xInput.GetAnalogValue(padInput);

	}

	return value;

}

DirectX::SimpleMath::Vector2 InputMapper::GetValueFromBind(const InputBindAnalog2D& bind) const
{
	
	DirectX::SimpleMath::Vector2 value = DirectX::SimpleMath::Vector2::Zero;

	//マウス
	for (const auto& mouseInput : bind.GetMouseAnalog())
	{
		
		switch (mouseInput)
		{
		case MouseInputAnalog2D::Position:

			value += keyMouseInput.GetMousePos();

			break;
		case MouseInputAnalog2D::Delta:

			auto delta = keyMouseInput.GetMouseDelta();

			value.x = static_cast<float>(delta.x);
			value.y = static_cast<float>(delta.y);

		default:
			break;
		}

	}

	//パッド
	for (const auto& padInput : bind.GetPadAnalog())
	{

		value += xInput.GetAnalogValue(padInput);

	}

	return value;

}

#pragma endregion

#pragma region 入力状態の結合

//デジタル入力の状態を結合
//長押し具合が長い方優先
InputState InputMapper::CombineState(InputState first, InputState second) const
{
	if (first == InputState::Hold || second == InputState::Hold)
	{
		return InputState::Hold;
	}
	else if (first == InputState::Trigger || second == InputState::Trigger)
	{
		return InputState::Trigger;
	}
	else if (first == InputState::Release || second == InputState::Release)
	{
		return InputState::Release;
	}
	else
	{
		return InputState::None;
	}
}

//アナログ入力の強さを結合
//強さが高い方優先
AnalogStrength InputMapper::CombineStrength(AnalogStrength first, AnalogStrength second) const
{

	if (first == AnalogStrength::High || second == AnalogStrength::High)
	{
		return AnalogStrength::High;
	}
	else if (first == AnalogStrength::Middle || second == AnalogStrength::Middle)
	{
		return AnalogStrength::Middle;
	}
	else if (first == AnalogStrength::Low || second == AnalogStrength::Low)
	{
		return AnalogStrength::Low;
	}
	else
	{
		return AnalogStrength::Zero;
	}

}

#pragma endregion