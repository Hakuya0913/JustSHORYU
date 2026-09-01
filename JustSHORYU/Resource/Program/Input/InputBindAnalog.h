#pragma once

/*

物理的アナログ入力を、GameLogiAnalogと紐付ける

*/

#include"ConstantInput.h"
#include<vector>

class InputBindAnalog {
public:

	InputBindAnalog();

	void AddMouseAnalog(MouseInputAnalog input);
	void AddPadAnalog(PadInputAnalog input);

	const std::vector<MouseInputAnalog>& GetMouseAnalog() const { return mouseInputs; }
	const std::vector<PadInputAnalog>& GetPadAnalog()	  const { return padInputs; }

private:

	std::vector<MouseInputAnalog>	mouseInputs;
	std::vector<PadInputAnalog>		padInputs;

};