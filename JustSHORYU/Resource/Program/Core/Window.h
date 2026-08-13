#pragma once

/*

Window生成とウィンドウプロシージャの定義
ウィンドウハンドル等の提供を行う

*/

#include<Windows.h>

class Window {
public:

	void Init();

	HINSTANCE GetHInst() const { return hInstance; }
	HWND GetHWND() const { return hwnd; }

private:

	HINSTANCE hInstance;
	HWND hwnd;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);

};
