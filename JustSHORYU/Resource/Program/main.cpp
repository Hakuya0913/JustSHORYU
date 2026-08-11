/*

Appクラスの初期化,アップデートを呼ぶ

*/

#include<Windows.h>

#include"App/App.h"

int WINAPI WinMain(
	_In_		HINSTANCE,
	_In_opt_	HINSTANCE,
	_In_		LPSTR,
	_In_ int	nShowCmd
) {

	App app;

	app.Init();

	app.Update();

	return 0;

}