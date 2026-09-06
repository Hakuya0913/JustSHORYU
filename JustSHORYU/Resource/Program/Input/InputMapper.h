#pragma once

/*
 
InputContextとコマンドに応じて
入力状況をInputStateなどで返す

*/

#include<DirectXMath.h>
#include<SimpleMath.h>
#include<array>
#include<cstddef>

#include"ConstantInput.h"
#include"InputBindDigital.h"
#include"InputBindAnalog1D.h"
#include"InputBindAnalog2D.h"
#include"KeyMouseInput.h"
#include"XInput.h"

class InputMapper {
public:

	InputMapper(KeyMouseInput& keymouse,XInput& xInput);

	//Contextをセット
	void SetContext(InputContext context);

	//Getter
	InputContext GetContext() const { return contextCurrent; }

	//Contextに応じて各Commandの入力状態を返す
	InputState						GetState(	GamePlayCommandDigital command) const;
	AnalogStrength					GetStrength(GamePlayCommandAnalog  command) const;
	float							GetValue1D(	GamePlayCommandAnalog  command) const;
	DirectX::SimpleMath::Vector2	GetValue2D(	GamePlayCommandAnalog  command) const;

	InputState						GetState(	MenuCommandDigital command) const;
	AnalogStrength					GetStrength(MenuCommandAnalog  command) const;
	float 							GetValue1D(	MenuCommandAnalog  command) const;
	DirectX::SimpleMath::Vector2	GetValue2D(	MenuCommandAnalog  command) const;

private:

	//初期マッピング
	void InitGamePlayMapping();
	void InitMenuMapping();

	//バインドから入力状態取得
	InputState						GetStateFromBind(const InputBindDigital& bind) const;
	AnalogStrength					GetStrengthFromBind(const InputBindAnalog1D& bind) const;
	AnalogStrength					GetStrengthFromBind(const InputBindAnalog2D& bind) const;
	float							GetValueFromBind(const InputBindAnalog1D& bind) const;
	DirectX::SimpleMath::Vector2	GetValueFromBind(const InputBindAnalog2D&  bind) const;

	//入力状態を結合
	InputState		CombineState(	InputState		first, InputState		second) const;
	AnalogStrength	CombineStrength(AnalogStrength	first, AnalogStrength	second) const;

	//各入力端末への参照
	KeyMouseInput& keyMouseInput;
	XInput& xInput;

	//現在のContext
	InputContext contextCurrent;

	//各ContextでのInputBind配列
	std::array<InputBindDigital, static_cast<size_t>(GamePlayCommandDigital::Count)>	gamePlayBindsDigital;
	std::array<InputBindAnalog1D,	 static_cast<size_t>(GamePlayCommandAnalog::Count)>	gamePlayBindsAnalog1D;
	std::array<InputBindAnalog2D, static_cast<size_t>(GamePlayCommandAnalog::Count)>	gamePlayBindsAnalog2D;

	std::array<InputBindDigital, static_cast<size_t>(MenuCommandDigital::Count)>	menuBindsDigital;
	std::array<InputBindAnalog1D,  static_cast<size_t>(MenuCommandAnalog::Count)>	menuBindsAnalog1D;
	std::array<InputBindAnalog2D, static_cast<size_t>(MenuCommandAnalog::Count)>	menuBindsAnalog2D;

};