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
	void AddMouseAnalog(MouseInputAnalog1D input);
	void AddPadAnalog(	PadInputAnalog1D   input);
			
			std::vector<MouseInputAnalog1D>& GetMouseAnalog()			{ return mouseInputs; }
	const	std::vector<MouseInputAnalog1D>& GetMouseAnalog()	const	{ return mouseInputs; }

			std::vector<PadInputAnalog1D>& GetPadAnalog()			{ return padInputs; }
	const	std::vector<PadInputAnalog1D>& GetPadAnalog()	const	{ return padInputs; }

private:

	std::vector<MouseInputAnalog1D>	mouseInputs;

	std::vector<PadInputAnalog1D>	padInputs;

};
