#pragma once

/*

1つの物理的アナログ入力に紐づく
入力の設定クラス

*/

#include"ConstantInput.h"
#include<vector>

class InputBindAnalog1D {
public:

	InputBindAnalog1D();

	//入力の追加
	void AddMouseAnalog(MouseInputAnalog input);

	void AddPadAnalog(PadInputAnalog input);
			
			std::vector<MouseInputAnalog>& GetMouseAnalog()			{ return mouseInputs; }
	const	std::vector<MouseInputAnalog>& GetMouseAnalog()	const	{ return mouseInputs; }

			std::vector<PadInputAnalog>& GetPadAnalog()			{ padInputs; }
	const	std::vector<PadInputAnalog>& GetPadAnalog()	const	{ return padInputs; }

private:

	std::vector<MouseInputAnalog>	mouseInputs;

	std::vector<PadInputAnalog>		padInputs;

};
