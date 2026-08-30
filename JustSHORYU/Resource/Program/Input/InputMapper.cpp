#include"InputMapper.h"

InputMapper::InputMapper(KeyMouseInput& keyMouse, XInput& xInput)
	:keyMouseInput(keyMouse), xInput(xInput),
	contextCurrent(InputContext::Menu)
{

	InitGamePlayMapping();
	InitMenuMapping();

}

void InputMapper::InitGamePlayMapping() {


}

void InputMapper::InitMenuMapping() {



}