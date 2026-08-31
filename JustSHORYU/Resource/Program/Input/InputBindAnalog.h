#pragma once

/*

物理的アナログ入力を、GameLogiAnalogと紐付ける

*/

#include"ConstantInput.h"
#include<vector>

class InputBindAnalog {
public:

	void SetPadLeftStick() {};
	void SetPadRightStick() {};

	PadInputAnalog GetPadAnalog() const {};

private:

	std::vector<PadInputAnalog> inputs;

};