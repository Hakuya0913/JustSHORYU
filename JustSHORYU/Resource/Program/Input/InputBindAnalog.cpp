#include"InputBindAnalog.h"

InputBindAnalog::InputBindAnalog() {

	mouseInputs.clear();
	padInputs.clear();

}

void InputBindAnalog::AddMouseAnalog(MouseInputAnalog input) {

	mouseInputs.emplace_back(input);

}

void InputBindAnalog::AddPadAnalog(PadInputAnalog input) {

	padInputs.emplace_back(input);

}
