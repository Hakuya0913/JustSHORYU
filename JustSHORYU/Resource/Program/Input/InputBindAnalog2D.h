#pragma once

/*

InputBindAnalog1D‚Ì2D”Å

*/

#include"ConstantInput.h"
#include<vector>

class InputBindAnalog2D {
public:

	InputBindAnalog2D();

	//“ü—Í‚Ì’Ç‰Á
	void AddMouseAnalog(MouseInputAnalog2D input);
	void AddPadAnalog(	PadInputAnalog2D   input);

	//ŠeŽ²‚ÌGetter
			std::vector<MouseInputAnalog2D>& GetMouseAnalog()		{ return mouseInputs; }
	const	std::vector<MouseInputAnalog2D>& GetMouseAnalog() const { return mouseInputs; }

			std::vector<PadInputAnalog2D>& GetPadAnalog()		{ return padInputs; }
	const	std::vector<PadInputAnalog2D>& GetPadAnalog() const { return padInputs; }

private:

	std::vector<MouseInputAnalog2D> mouseInputs;
	std::vector<PadInputAnalog2D>	padInputs;

};