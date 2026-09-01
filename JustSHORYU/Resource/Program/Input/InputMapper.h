#pragma once

/*
 
InputContextに応じて
GameLogiとInputBindを紐付け
入力状況をInputStateなどで返す

*/

#include"ConstantInput.h"
#include"InputBindDigital.h"
#include"InputBindAnalog.h"
#include"KeyMouseInput.h"
#include"XInput.h"

#include<array>
#include<cstddef>

class InputMapper {
public:

	InputMapper(KeyMouseInput& keymouse,XInput& xInput);

	//Contextをセット
	void SetContext(InputContext context);

	//Getter
	InputContext GetContext() const { return contextCurrent; }

	//Contextに応じて各Commandの入力状態を返す
	InputState	GetState(GamePlayCommandDigital command) const;
	float		GetValue(GamePlayCommandAnalog command) const;

	InputState	GetState(MenuCommandDigital command) const;
	float		GetValue(MenuCommandAnalog command) const;

private:

	//初期マッピング
	void InitGamePlayMapping();
	void InitMenuMapping();

	//バインドから入力状態取得
	InputState	GetStateFromBind(const InputBindDigital& bind) const;
	float		GetValueFromBind(const InputBindAnalog& bind) const;

	//入力状態を結合
	InputState CombineState(InputState first, InputState second) const;

	//各入力端末
	KeyMouseInput& keyMouseInput;
	XInput& xInput;

	InputContext contextCurrent;

	//各ContextでのInputBind配列
	std::array<InputBindDigital, static_cast<size_t>(GamePlayCommandDigital::Count)>	gamePlayBindsDigital;
	std::array<InputBindAnalog,	 static_cast<size_t>(GamePlayCommandAnalog::Count)>		gamePlayBindsAnalog;

	std::array<InputBindDigital, static_cast<size_t>(MenuCommandDigital::Count)>	menuBindsDigital;
	std::array<InputBindAnalog,  static_cast<size_t>(MenuCommandAnalog::Count)>		menuBindsAnalog;

};