#pragma once

namespace KeyMouseConst{

	constexpr int KeyMax		 = 256;
	constexpr int MouseButtonMax = 5;

}

constexpr enum class PhysicalKey {

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

constexpr enum class PhysicalAxis {

	MouseX,
	MouseY,

	PadLX,
	PadLY,
	PadRX,
	PadRY,
	PadLT,
	PadRT

};
