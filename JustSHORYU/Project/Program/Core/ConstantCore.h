#pragma once
#include<Windows.h>

namespace ConstVal {

	namespace Window {

		//ウィンドウクラスネームとウィンドウネーム
		constexpr LPCWSTR ClassName = L"DX12_Class";
		constexpr LPCWSTR WindowName = ClassName;

		//スクリーンサイズのデフォルト設定
		constexpr UINT ScreenW = 960;
		constexpr UINT ScreenH = 540;

	}

	namespace D3D {

		//マルチバッファリング数とフレームリソース数
		constexpr UINT BufferringCount = 2;
		constexpr UINT FrameResourceCount = BufferringCount;

		//解像度
		constexpr UINT ResolutionW = Window::ScreenW;
		constexpr UINT ResolutionH = Window::ScreenH;

		//スクリーンクリアカラー
		constexpr float ClearColor[4] = { 0.0f, 1.0f, 6.f,1.0f };

	}

}