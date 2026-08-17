#pragma once

namespace KeyMouseConst{

	constexpr int KeyMax		 = 256;
	constexpr int MouseButtonMax = 5;

}

namespace PadConst {

	constexpr float StickMax = 32767.0f;

}

enum class InputStateDegital {

	Press,
	Hold,
	Release,
	None

};

enum class InputStateAnalog {

	

};

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
	Cancel

};
