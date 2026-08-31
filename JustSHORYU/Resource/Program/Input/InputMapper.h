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

	/// <summary>
	/// コンテクスト
	/// </summary>
	/// <param name="keymouse"></param>
	/// <param name="xInput"></param>
	InputMapper(KeyMouseInput& keymouse,XInput& xInput);

	//コンテキストをセット
	void SetContext(InputContext context);

	//Getter
	InputContext GetContext() const { return contextCurrent; }

private:

	//初期マッピング
	void InitGamePlayMapping();
	void InitMenuMapping();

	//バインドから入力状態取得
	InputState GetStateFromBind(const InputBindDigital& bind) const;
	float GetValueFromBind(const InputBindAnalog& bind) const;

	//入力状態を結合
	InputState CombineState(InputState first, InputState second);

	//各入力端末
	KeyMouseInput& keyMouseInput;
	XInput& xInput;

	InputContext contextCurrent;

	//各ゲーム状況のマッピング(デジタル＋アナログ)
	std::array<InputBindDigital, static_cast<size_t>()>	gamePlayBindsDigital;
	std::array<InputBindAnalog, static_cast<size_t>(GameLogicAnalog::Count)>	gamePlayBindsAnalog;

	std::array<InputBindDigital, static_cast<size_t>(GameLogicAnalog::Count)> menuBindsDigital;
	std::array<InputBindAnalog, static_cast<size_t>(GameLogicAnalog::Count)> menuBindsAnalog;

};