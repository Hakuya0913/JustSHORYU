#pragma once

namespace KeyMouseConst{

	constexpr int KeyMax		 = 256;

	constexpr UINT RowInputDeviceCount = 2;
	
	constexpr UINT MouseButtonMax = 5;
	constexpr UINT MouseButtonL	  = 0;
	constexpr UINT MouseButtonR   = 1;
	constexpr UINT MouseButtonM   = 2;
	constexpr UINT MouseButtonX1  = 3;
	constexpr UINT MouseButtonX2  = 4;

}

namespace PadConst {

	constexpr float StickMax = 32767.0f;

}

//入力の有無(変化時と継続)
enum class InputState {

	Active,		//押下の瞬間
	Hold,		//押下継続
	DeActive,	//離上の瞬間
	None		//入力なし

};

//アナログ入力の強さ定義(4種)
enum class AnalogStrength {

	Zero,		//入力なし
	Low,		//弱
	Middle,		//中
	High		//強

};

//ゲームで使用するコマンド一覧
enum class GameLogic {

	MoveForward,
	MoveBack,
	MoveLeft,
	MoveRight,
	Dash,
	Jump,
	Atack,
	
	MenuUp,
	MenuDown,
	MenuLeft,
	MenuRight,

	Confirm,
	Cancel,

	None

};
