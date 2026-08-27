#pragma once
#include<Windows.h>
#include<Xinput.h>
#include<array>

namespace KeyMouseConst{

	// キーの数
	constexpr int KeyCount		 = 256;

	//取得したいRawInputデバイス数
	//今回は「キーボード＋マウス」の2つ
	constexpr UINT RawInputDeviceCount = 2;
	
	//マウスボタンの割付
	constexpr UINT MouseButtonNum = 5;
	constexpr UINT MouseButtonL	  = 0;
	constexpr UINT MouseButtonR   = 1;
	constexpr UINT MouseButtonM   = 2;
	constexpr UINT MouseButtonX1  = 3;
	constexpr UINT MouseButtonX2  = 4;

}

namespace PadConst {



	//入力の最大値
	constexpr float StickMax	= 32767.0f;
	constexpr UINT TriggerMax	= 255;

	//デッドゾーン
	//「これ未満」ならデッドゾーン圏内として扱う
	constexpr float Deadzone = 0.1f;

	//アナログ入力正規化後の閾値
	//「それ以上」ならその強さとして扱う
	constexpr float ThresholdLow	= 0.1f;
	constexpr float ThresholdMiddle = 0.4f;
	constexpr float ThresholdHigh	= 0.7f;

}

//使用する入力の名前
//連番前提で動作する
enum class PadInputDigital {

	Up,
	Down,
	Left,
	Right,

	A,
	B,
	X,
	Y,

	LB,
	RB,

	LThumb,
	RThumb,

	Start,
	View

};

//使用するアナログ入力の名前
//連番前提で動作する
enum class PadInputAnalog {

	LStickX, 
	LStickY,

	RStickX,
	RStickY,

	LT, 
	RT,

};

static constexpr std::array<PadInputDigital, 14> DigitalList =
{

	PadInputDigital::Up,
	PadInputDigital::Down,
	PadInputDigital::Left,
	PadInputDigital::Right,

	PadInputDigital::A,
	PadInputDigital::B,
	PadInputDigital::X,
	PadInputDigital::Y,

	PadInputDigital::LB,
	PadInputDigital::RB,

	PadInputDigital::LThumb,
	PadInputDigital::RThumb,

	PadInputDigital::Start,
	PadInputDigital::View

};

//XInputクラスが保持するデジタル入力の配列の添え字
//コメントは現状のPadInputでの並び順
static constexpr std::array<size_t, 14> PadInputIndexTableDigital =
{
	0, 1, 2, 3,	//Up, Down, Left, Right

	4, 5, 6, 7,	//A, B, X, Y

	8, 9,		//LB, RB

	10, 11,		//LThumb, RThumb

	12, 13		//Start, View
};

static constexpr std::array<PadInputAnalog, 6> AnalogList =
{

	PadInputAnalog::LStickX,
	PadInputAnalog::LStickY,

	PadInputAnalog::RStickX,
	PadInputAnalog::RStickY,

	PadInputAnalog::LT,
	PadInputAnalog::RT

};

//XInputクラスが保持するアナログ入力の配列の添え字
//コメントは現状のPadInputでの並び順
static constexpr std::array<size_t, 6> PadInputIndexTableAnalog =
{

	0,1,	//LStickX,LStickY,

	2,3,	//RStickX,RStickY

	4,5		//LT,RT

};

//入力の有無(変化時と継続)
enum class InputState {

	Trigger,	//押下の瞬間
	Hold,		//押下継続
	Release,	//離上の瞬間
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
