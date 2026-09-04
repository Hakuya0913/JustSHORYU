#include"InputBindAnalog1D.h"

InputBindAnalog1D::InputBindAnalog1D() {

	mouseInputs.clear();

	padInputs.clear();

}

void InputBindAnalog1D::AddMouseAnalog(MouseInputAnalog input) {

	mouseInputs.emplace_back(input);

}

void InputBindAnalog1D::AddPadAnalog(PadInputAnalog input) {

	padInputs.emplace_back(input);

}
