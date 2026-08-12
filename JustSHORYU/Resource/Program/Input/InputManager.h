#pragma once

/*

各インプットクラスをまとめる

*/

#include"RawInput.h"

class InputManager {
public:

	RawInput& GetRawInput() { return rawInput; }

private:

	RawInput rawInput;

};