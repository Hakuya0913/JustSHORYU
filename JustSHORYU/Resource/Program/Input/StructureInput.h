#pragma once
#include<Windows.h>

struct MouseState {

	LONG dx			 = 0;
	LONG dy			 = 0;
	bool leftButton  = false;
	bool rightButton = false;
	short wheelDelta = 0;

};

struct KeyState {

	bool isPress = false;
	USHORT makeCode = 0;
	USHORT vKey = 0;

};