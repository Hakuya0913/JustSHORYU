#pragma once

/*

各インプットクラスをまとめる

*/

#include"RawInput.h"

class InputManager {
public:

	inline static InputManager& GetInstance() {
		static InputManager instance;
		return instance;
	}

	void Update();

	RawInput& GetRawInput() { return rawInput; }

private:

	RawInput rawInput;

};