#pragma once

/*

各インプットクラスをまとめる

*/

#include"KeyMouseInput.h"

class InputManager {
public:

	inline static InputManager& GetInstance() {
		static InputManager instance;
		return instance;
	}

	void Update();

	KeyMouseInput& GetKeyMouseInput() { return keyMouseInput; }

private:

	KeyMouseInput keyMouseInput;

};