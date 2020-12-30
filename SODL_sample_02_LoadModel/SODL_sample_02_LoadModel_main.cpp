#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )


// ライブラリ名のエイリアスを設定する
namespace sodl = SmplObjDrwLib;

// アプリケーションのグローバルシンボルをappネームスペースに収める
namespace app {

	const int	WINDOW_SIZE_X = 640;
	const int	WINDOW_SIZE_Y = 480;
	float ax_X = 100; // [mm]
	float ax_Y = 100; // [mm]
	float ax_Z = 100; // [mm]

	void keyFunc(unsigned char key, int u, int v);

	// その他のサブ関数
	std::string GetModulePath();	// 実行ファイルのパスを取得

};


//================================================================
//
//	<Summry>		アプリケーションのエントリポイント
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
	sodl::DrawingManager::initMngr( &argc, argv, app::WINDOW_SIZE_X, app::WINDOW_SIZE_Y);

	// 描画マネージャにコールバック関数を設定する
	sodl::drwMngr->SetKeyboardFunc(app::keyFunc);

	//-----------------------------------------------------
	// ワールド座標系原点から連鎖するJ1~4座標系オブジェクトを定義
	//-----------------------------------------------------
	// ワールド座標系原点を定義
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f,0.f);

	// J1~4座標系を定義
	auto X_Link = sodl::CoordChainObj::create("X_Link", World_Origin);
	auto Y_Link = sodl::CoordChainObj::create("Y_Link", X_Link);
	auto Z_Link = sodl::CoordChainObj::create("Z_Link", Y_Link);

	//-----------------------------------------------------
	// 3Dモデルオブジェクトを座標系オブジェクトにアタッチする
	//-----------------------------------------------------
	// 実行ファイルのパスを取得する(モデルファイルの位置を相対パスで指定するため)
	std::string exePath = app::GetModulePath();
	// 3Dモデル描画付き座標系オブジェクトを生成
	auto model_Crd = sodl::CoordChain3dMdl::create(exePath+"3dModel\\miku.obj", "model_Crd", Z_Link);
	// アタッチしたZ_Linkとの相対位置を調整する
	model_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 10.f);

	//-----------------------------------------------------
	// 定義したオブジェクトを描画対象として描画マネージャに登録
	//-----------------------------------------------------
	// 定義した座標系連鎖を描画マネージャの持つ描画空間にセット
	sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);

	////////////////////////////////////////////////////// 
	//
	// メインループ
	//
	//////////////////////////////////////////////////////
	while(1)
	{
		//-----------------------------------------------------
		// キー入力で更新される変数値によって、描画オブジェクトの座標変換行列を更新する
		//-----------------------------------------------------
		X_Link->CrdTrs.translation() = Eigen::Vector3f(app::ax_X, 0, 0);
		Y_Link->CrdTrs.translation() = Eigen::Vector3f(0, app::ax_Y, 0);
		Z_Link->CrdTrs.translation() = Eigen::Vector3f(0, 0, app::ax_Z);

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
	//	<Summry>		キー操作時のコールバック
	//	<Description>
	//================================================================
	void keyFunc(unsigned char key, int u, int v)
	{
		const float		KEY_MOT_UNIT	=	5.f;
		const float		CNV_DEG2RAD		=	((float)M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' は ESC の ASCII コード
			exit(0);

		case '1':
			ax_X += KEY_MOT_UNIT;
			break;
		case '2':
			ax_Y += KEY_MOT_UNIT;
			break;
		case '3':
			ax_Z += KEY_MOT_UNIT;
			break;

		case 'q':
			ax_X -= KEY_MOT_UNIT;
			break;
		case 'w':
			ax_Y -= KEY_MOT_UNIT;
			break;
		case 'e':
			ax_Z -= KEY_MOT_UNIT;
			break;

		default:
			break;
		}
	}

	//================================================================
	//
	//	<Summry>		実行ファイルのパスを取得
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
