#pragma once
#include<Windows.h>
#include<Xinput.h>
#include<array>

namespace KeyMouseConst{

	constexpr int KeyMax		 = 256;

	constexpr UINT RowInputDeviceCount = 2;
	
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

enum class PadInput {

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
	View,

	//アナログ識別用(これより値が大きければアナログ入力)
	ThresholdAnalog,

	LStickX, LStickY,
	RStickX, RStickY,

	LT, RT,

	//例外用
	None

};

static constexpr std::array<PadInput, 15> DegitalList =
{

	PadInput::Up,
	PadInput::Down,
	PadInput::Left,
	PadInput::Right,

	PadInput::A,
	PadInput::B,
	PadInput::X,
	PadInput::Y,

	PadInput::LB,
	PadInput::RB,

	PadInput::LThumb,
	PadInput::RThumb,

	PadInput::Start,
	PadInput::View

};

static constexpr std::array<PadInput, 6> AnalogList =
{

	PadInput::LStickX,
	PadInput::LStickY,

	PadInput::RStickX,
	PadInput::RStickY,

	PadInput::LT,
	PadInput::RT

};

static constexpr int ConvertArrayIndex(PadInput padInput) {

	switch (padInput) {

	//デジタル入力用配列インデックス

	case PadInput::Up:		return  0;
	case PadInput::Down:	return  1;
	case PadInput::Left:	return  2;
	case PadInput::Right:	return  3;

	case PadInput::A:		return  4;
	case PadInput::B:		return  5;
	case PadInput::X:		return  6;
	case PadInput::Y:		return  7;

	case PadInput::LB:		return  8;
	case PadInput::RB:		return  9;

	case PadInput::LThumb:	return 10;
	case PadInput::RThumb:	return 11;

	case PadInput::Start:	return 12;
	case PadInput::View:	return 13;

	//アナログ入力用配列インデックス

	case PadInput::LStickX: return  0;
	case PadInput::LStickY: return  1;

	case PadInput::RStickX: return  2;
	case PadInput::RStickY: return  3;

	case PadInput::LT:		return  4;
	case PadInput::RT:		return  5;

	default:				return -1;

	}

}

static constexpr WORD ConvertWORD(PadInput padInput) {

	switch (padInput) {

	case PadInput::Up:		return XINPUT_GAMEPAD_DPAD_UP;
	case PadInput::Down:	return XINPUT_GAMEPAD_DPAD_DOWN;
	case PadInput::Left:	return XINPUT_GAMEPAD_DPAD_LEFT;
	case PadInput::Right:	return XINPUT_GAMEPAD_DPAD_RIGHT;

	case PadInput::A:		return XINPUT_GAMEPAD_A;
	case PadInput::B:		return XINPUT_GAMEPAD_B;
	case PadInput::X:		return XINPUT_GAMEPAD_X;
	case PadInput::Y:		return XINPUT_GAMEPAD_Y;

	case PadInput::LB:		return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case PadInput::RB:		return XINPUT_GAMEPAD_RIGHT_SHOULDER;

	case PadInput::LThumb:	return XINPUT_GAMEPAD_LEFT_THUMB;
	case PadInput::RThumb:	return XINPUT_GAMEPAD_RIGHT_THUMB;

	case PadInput::Start:	return XINPUT_GAMEPAD_START;
	case PadInput::View:	return XINPUT_GAMEPAD_BACK;

	default:				return 0;

	}

}

static constexpr std::array<PadInput, 6> AnalogList =
{

	PadInput::LStickX,
	PadInput::LStickY,

	PadInput::RStickX,
	PadInput::RStickY,

	PadInput::LT,
	PadInput::RT

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
