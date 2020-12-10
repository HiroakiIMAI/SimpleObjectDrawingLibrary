#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "include/SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawLib.lib" )


// ライブラリ名のエイリアスを設定する
namespace sodl = SmplObjDrwLib;

// アプリケーションのグローバルシンボルをappネームスペースに収める
namespace app {

	const int	WINDOW_SIZE_X = 1280;
	const int	WINDOW_SIZE_Y = 960;
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
	// ビューポートとカメラの設定
	//-----------------------------------------------------
	// ビューポート1
	//-----------------------------------------------------
	//	デフォルトのビューポートは描画マネージャの初期化時にインスタンス化され、
	//	viewPorts[0]にstd::shared_ptrで保持されている。
	//	これを一時変数に受け取ってサイズや関連付けられたカメラ設定を操作する

	// ビューポート1へのshared_ptr取得
	auto vp1 = sodl::drwMngr->viewPorts[0];

	// ビューポート1のサイズを設定する
	//	ウィンドウを4分割した左上領域にビューポートを張る
	//	(OpenGL画像座標系に従うので、ウィンドウ左下原点、上方向がY+、右方向がX+)
	vp1->setVpSize(
		0,								// left
		app::WINDOW_SIZE_Y / 2,			// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// ビューポート1に関連付けられたカメラへのshared_ptrを取得する
	auto cam1 = vp1->getCam();

	// カメラの位置を設定する
	cam1->camPos = Eigen::Vector3f(600.f, -2000.f, 300.f);
	// カメラの撮影対象位置を設定する
	cam1->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	// カメラのズーム比率を設定する
	cam1->zoomRatio = 0.8;
	vp1->attachCam(cam1);

	//-----------------------------------------------------
	// ビューポート2
	//-----------------------------------------------------
	// ビューポートを追加する。
	//	追加されたビューポートインスタンスは
	//	drwMngr->viewPorts[]にもshared_ptrを介して保持される
	auto vp2 = sodl::drwMngr->addViewPort("vp2");

	// ウィンドウを4分割した右上領域にビューポートを張る
	vp2->setVpSize(
		app::WINDOW_SIZE_X / 2,			// left
		app::WINDOW_SIZE_Y / 2,			// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// ビューポート2に関連付けられたカメラへのshared_ptrを取得する
	auto cam2 = vp2->getCam();
	// 上面図を撮影する
	cam2->camPos = Eigen::Vector3f(0.f, -0.f, 5000.f);
	cam2->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	cam2->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
	vp2->attachCam(cam2);

	//-----------------------------------------------------
	// ビューポート3
	//-----------------------------------------------------
	// ビューポートを追加する。
	auto vp3 = sodl::drwMngr->addViewPort("vp3");

	// ウィンドウを4分割した左下領域にビューポートを張る
	vp3->setVpSize(
		0,								// left
		0,								// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// ビューポート2に関連付けられたカメラへのshared_ptrを取得する
	auto cam3 = vp3->getCam();
	// 側面図を撮影する
	cam3->camPos = Eigen::Vector3f(-1000.f, 0.f, 0.f);
	cam3->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	cam3->camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
	vp3->attachCam(cam3);

	//-----------------------------------------------------
	// ビューポート4
	//-----------------------------------------------------
	// ビューポートを追加する。
	auto vp4 = sodl::drwMngr->addViewPort("vp4");

	// ウィンドウを4分割した右下領域にビューポートを張る
	vp4->setVpSize(
		app::WINDOW_SIZE_X / 2, 		// left
		0,								// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// ビューポート2に関連付けられたカメラへのshared_ptrを取得する
	auto cam4 = vp4->getCam();
	// 側面図を撮影する
	cam4->camPos = Eigen::Vector3f(0.f, -1000.f, 0.f);
	cam4->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	cam4->camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
	vp4->attachCam(cam4);


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
