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

	Up		= XINPUT_GAMEPAD_DPAD_UP,			//0x0001
	Down	= XINPUT_GAMEPAD_DPAD_DOWN,			//0x0002
	Left	= XINPUT_GAMEPAD_DPAD_LEFT,			//0x0004
	Right	= XINPUT_GAMEPAD_DPAD_RIGHT,		//0x0008

	A		= XINPUT_GAMEPAD_A,					//0x1000
	B		= XINPUT_GAMEPAD_B,					//0x2000
	X		= XINPUT_GAMEPAD_X,					//0x4000
	Y		= XINPUT_GAMEPAD_Y,					//0x8000	

	LB		= XINPUT_GAMEPAD_LEFT_SHOULDER,		//0x0100
	RB		= XINPUT_GAMEPAD_RIGHT_SHOULDER,	//0x0200

	LThumb	= XINPUT_GAMEPAD_LEFT_THUMB,		//0x0040
	RThumb	= XINPUT_GAMEPAD_RIGHT_THUMB,		//0x0080

	Start	= XINPUT_GAMEPAD_START,				//0x0010
	View	= XINPUT_GAMEPAD_BACK,				//0x0020
	XBox,

	//アナログ識別用(これより値が大きければアナログ入力)
	ThresholdAnalog,	

	LStickX, LStickY,
	RStickX, RStickY,

	LT, RT,

	//例外用
	None

};

//forループ用
static constexpr std::array < PadInput, 15 > DegitalList =
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
	PadInput::View,
	PadInput::XBox,

};

static constexpr std::array<PadInput, 6> AnalogList =
{

	PadInputIndexAnalog::LStickX,
	PadInputIndexAnalog::LStickY,

	PadInputIndexAnalog::RStickX,
	PadInputIndexAnalog::RStickY,

	PadInputIndexAnalog::LT,
	PadInputIndexAnalog::RT

};

//入力の有無(変化時と継続)
enum class InputState {

	Trigger,	//押下の瞬間
	Active,		//押下継続
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
