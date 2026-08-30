#include"InputBindDigital.h"

InputBindDigital::InputBindDigital() {

	keys.clear();
	mouseButtons.clear();
	padButtons.clear();

}

void InputBindDigital::AddKey(USHORT vKey) {

	keys.emplace_back(vKey);

}

void InputBindDigital::AddMouseButton(UINT button) {

	mouseButtons.emplace_back(button);

}

void InputBindDigital::AddPadButton(PadInputDigital button) {

	padButtons.emplace_back(button);

}
