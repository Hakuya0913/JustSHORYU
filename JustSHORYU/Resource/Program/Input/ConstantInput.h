#pragma once

namespace KeyMouseConst{

	constexpr int KeyMax		 = 256;
	constexpr int MouseButtonMax = 5;

}

namespace PadConst {

	constexpr float StickMax = 32767.0f;

}

enum class PhysicalKey {

	W, A, S, D,
	Space,

	LeftMouse,
	RightMouse,

	PadUp,
	PadDown,
	PadLeft,
	PadRight,
	PadA,
	PadB,
	PadX,
	PadY

};

enum class PhysicalAxis {

	MouseX,
	MouseY,

	PadLX,
	PadLY,
	PadRX,
	PadRY,
	PadLT,
	PadRT

};
