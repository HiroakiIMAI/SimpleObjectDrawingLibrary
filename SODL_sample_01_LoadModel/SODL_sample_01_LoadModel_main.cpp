#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <Windows.h>

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )


// ライブラリ名のエイリアスを設定する
namespace sodl = SmplObjDrwLib;

// アプリケーションのグローバルシンボルをappネームスペースに収める
namespace app {

	const int	WINDOW_SIZE_X = 640;
	const int	WINDOW_SIZE_Y = 480;

	// その他のサブ関数
	std::string GetModulePath();	// 実行ファイルのパスを取得

};


//================================================================
//
//	<Summary>		アプリケーションのエントリポイント
//	<Description>
//================================================================
int main(int argc, char ** argv) 
{
	//////////////////////////////////////////////////////
	//
	// アプリケーションの初期化
	//
	//////////////////////////////////////////////////////

	//-----------------------------------------------------
	// ライブラリの描画マネージャを初期化する
	//-----------------------------------------------------
	// この時点でライブラリによってOpenGLコンテキストが作成され、
	// OpenGLのウィンドウが表示される
	sodl::DrawingManager::initMngr(app::WINDOW_SIZE_X, app::WINDOW_SIZE_Y);

	//--------------------------------------------------------------------------------------
	// 描画対象オブジェクトをcreateし、描画マネージャの描画空間にセットする
	//	Createしたオブジェクトはshared_ptrとして取得される。
	//	描画空間にセットしたオブジェクトは描画マネージャ側にshared_ptrとして保持されるので、
	//	Create時に得られたshared_ptrは不要になった時点で破棄して構わない。
	//--------------------------------------------------------------------------------------
	{
		//-----------------------------------------------------
		// ワールド座標系原点を定義
		//-----------------------------------------------------
		// ワールド座標系原点を表すシンボル表示用のオブジェクトを生成する
		auto World_Origin = sodl::CoordChainObj::create("World_Origin");

		//-----------------------------------------------------
		// 3Dモデルオブジェクトを生成し、座標系オブジェクトにアタッチする
		//-----------------------------------------------------
		// 実行ファイルのパスを取得する(モデルファイルの位置を相対パスで指定するため)
		std::string exePath = app::GetModulePath();
		// 3Dモデル描画付き座標系オブジェクトを生成し、親座標系としてワールド座標系を指定する。
		auto model_Crd = sodl::CoordChain3dMdl::create(exePath + "3dModel\\teapot\\teapot.obj", "model_Crd", World_Origin);
		// アタッチした親座標系との相対位置(並進)を調整する
		model_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 20.f);
		// アタッチした親座標系との相対位置(回転)を調整する
		//   Y+方向を上方向としたモデルをロードしたので、
		//   Z+が上向きになるようにX軸回りに90°回転させる。
		model_Crd->CrdTrs.linear() = Eigen::AngleAxisf(M_PI / 2.f, UnitX).matrix();

		//-----------------------------------------------------
		// 生成したオブジェクトを描画対象として描画マネージャに登録
		//-----------------------------------------------------
		// 定義した座標系連鎖を描画マネージャの持つ描画空間にセット
		sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);
	}

	////////////////////////////////////////////////////// 
	//
	// メインループ
	//
	//////////////////////////////////////////////////////
	while(1)
	{
		//-----------------------------------------------------
		// 描画マネージャから描画更新を実行する
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

		Sleep(10);
	}

	return 0;
}


namespace app {

	//================================================================
	//
	//	<Summary>		実行ファイルのパスを取得
	//	<Description>
	//================================================================
	std::string GetModulePath()
	{
		// 実行ファイルのパス
		std::string modulePath = "";
		// ドライブ名、ディレクトリ名、ファイル名、拡張子
		char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

		// 実行ファイルのファイルパスを取得
		if (GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
		{
			// ファイルパスを分割
			_splitpath_s(path, drive, dir, fname, ext);
			// ドライブとディレクトリ名を結合して実行ファイルパスとする
			modulePath = std::string(drive) + std::string(dir);
		}

		return modulePath;
	}
};
