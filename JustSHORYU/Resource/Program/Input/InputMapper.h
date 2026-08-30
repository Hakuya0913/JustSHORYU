#pragma once

/*

InputContextに応じて
GameLogiとInputBindを紐付け
入力状況をInputStateなどで返す

*/

#include"ConstantInput.h"
#include"InputBindDigital.h"
#include"KeyMouseInput.h"
#include"XInput.h"

#include<array>
#include<cstddef>

class InputMapper {
public:

	InputMapper(KeyMouseInput& keymouse,XInput& xInput);

	//コンテキストをセット
	void SetContext(InputContext context);

	InputContext GetContext() const;

private:

	//初期マッピング
	void InitGamePlayMapping();
	void InitMenuMapping();

	//バインドから入力状態取得
	InputState GetStateFromBind(const InputBindDigital& bind) const;

	//入力状態を結合
	InputState CombineState(InputState first, InputState second);

	//各入力端末
	KeyMouseInput& keyMouseInput;
	XInput& xInput;

	InputContext contextCurrent;

	//各ゲーム状況のマッピング
	//std::array<InputBindDigital, static_cast<size_t>(GameLogic::GamePlayCommandCount)> gamePlayBinds;
	//std::array<InputBindDigital, static_cast<size_t>(GameLogic::MenuCommandCount)> menuBinds;

};