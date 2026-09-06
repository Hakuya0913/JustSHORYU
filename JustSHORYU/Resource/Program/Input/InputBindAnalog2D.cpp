#include"InputBindAnalog2D.h"

InputBindAnalog2D::InputBindAnalog2D() {

	mouseInputs.clear();
	padInputs.clear();

}

void InputBindAnalog2D::AddMouseAnalog(MouseInputAnalog2D input) {

	mouseInputs.emplace_back(input);

}

void InputBindAnalog2D::AddPadAnalog(PadInputAnalog2D input) {

	padInputs.emplace_back(input);

}