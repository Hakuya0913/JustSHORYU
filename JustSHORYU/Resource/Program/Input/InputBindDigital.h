#pragma once

/*

物理的デジタル入力とGameLogicを紐づける

*/

#include"ConstantInput.h"
#include<vector>
#include<Windows.h>

class InputBindDigital {
public:

	InputBindDigital();

	//割り当てる入力の追加
	void AddKey(USHORT vKey);
	void AddMouseButton(UINT button);
	void AddPadButton(PadInputDigital button);

	//Getter
	const std::vector<USHORT>&			GetKeys()			const { return keys; }
	const std::vector<UINT>&			GetMouseButtons()	const { return mouseButtons; }
	const std::vector<PadInputDigital>& GetPadButtons()		const { return padButtons; }

private:

	//割り当てを記憶する変数
	std::vector<USHORT> keys;
	std::vector<UINT> mouseButtons;
	std::vector<PadInputDigital> padButtons;

};