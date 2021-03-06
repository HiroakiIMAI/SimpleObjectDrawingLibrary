#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )


// ライブラリ名のエイリアスを設定する
namespace sodl = SmplObjDrwLib;

// アプリケーションのグローバルシンボルをappネームスペースに収める
namespace app {

	const int	WINDOW_SIZE_X = 1280;
	const int	WINDOW_SIZE_Y = 960;
	float ax_J1 =  0.0 * (M_PI / 180); // [rad]
	float ax_J2 = 45.0 * (M_PI / 180); // [rad]
	float ax_J3 = 90.0 * (M_PI / 180); // [rad]
	float ax_J4 =  0.0 * (M_PI / 180); // [rad]
	float ax_J5 = 45.0 * (M_PI / 180); // [rad]
	float ax_J6 =  0.0 * (M_PI / 180); // [rad]

	void keyFunc(unsigned char key, int u, int v);

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
	sodl::DrawingManager::initMngr( &argc, argv, app::WINDOW_SIZE_X, app::WINDOW_SIZE_Y);

	// 描画マネージャにコールバック関数を設定する
	sodl::drwMngr->SetKeyboardFunc(app::keyFunc);


	//-----------------------------------------------------
	// ビューポートとカメラの設定
	//-----------------------------------------------------
	// ビューポートとカメラは描画マネージャdrwMngrのメソッドを介して追加できる。
	// その時点で描画マネージャ側にshared_ptrとして保持されるので
	// mainソース側のshared_ptrは不要になった時点で破棄して構わない。
	{
		//-----------------------------------------------------
		// ビューポート1
		//-----------------------------------------------------
		{
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
			// カメラ設定を反映するために、調整したカメラを再度ビューポート1に関連付けし直す
			vp1->attachCam(cam1);
		}

		//-----------------------------------------------------
		// ビューポート2
		//-----------------------------------------------------
		{
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
			// カメラのズーム比率を設定する
			cam2->zoomRatio = 0.3;
			// カメラ設定を反映するために、調整したカメラを再度ビューポート1に関連付けし直す
			vp2->attachCam(cam2);
		}

		//-----------------------------------------------------
		// ビューポート3
		//-----------------------------------------------------
		{
			// ビューポートを追加する。
			auto vp3 = sodl::drwMngr->addViewPort("vp3");

			// ウィンドウを4分割した左下領域にビューポートを張る
			vp3->setVpSize(
				0,								// left
				0,								// bottom
				app::WINDOW_SIZE_X / 2,			// width
				app::WINDOW_SIZE_Y / 2			// height
			);

			// ビューポート3に関連付けられたカメラへのshared_ptrを取得する
			auto cam3 = vp3->getCam();
			// 側面図を撮影する
			cam3->camPos = Eigen::Vector3f(-1000.f, 0.f, 0.f);
			cam3->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
			cam3->camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
			// カメラのズーム比率を設定する
			cam3->zoomRatio = 0.3;
			// カメラ設定を反映するために、調整したカメラを再度ビューポート1に関連付けし直す
			vp3->attachCam(cam3);
		}

		//-----------------------------------------------------
		// ビューポート4
		//-----------------------------------------------------
		{
			// ビューポートを追加する。
			auto vp4 = sodl::drwMngr->addViewPort("vp4");

			// ウィンドウを4分割した右下領域にビューポートを張る
			vp4->setVpSize(
				app::WINDOW_SIZE_X / 2, 		// left
				0,								// bottom
				app::WINDOW_SIZE_X / 2,			// width
				app::WINDOW_SIZE_Y / 2			// height
			);

			// ビューポート4に関連付けられたカメラへのshared_ptrを取得する
			auto cam4 = vp4->getCam();
			// 側面図を撮影する
			cam4->camPos = Eigen::Vector3f(0.f, -1000.f, 0.f);
			cam4->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
			cam4->camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
			// カメラのズーム比率を設定する
			cam4->zoomRatio = 0.3;
			// カメラ設定を反映するために、調整したカメラを再度ビューポート1に関連付けし直す
			vp4->attachCam(cam4);
		}
	}


	//-----------------------------------------------------
	// ワールド座標系原点から連鎖するJ1~6座標系オブジェクトを定義
	//   J1~6座標系が6軸ロボットアームの各関節の位置姿勢を表現する
	//-----------------------------------------------------
	// ワールド座標原点を定義
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);

	// J1(Joint1)座標系オブジェクトを作成、ワールド座標系にアタッチする
	auto J1_Crd = sodl::CoordChainObj::create("J1_Crd", World_Origin);
	// J1座標系の親座標系に対するオフセット量を設定する
	J1_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 30.f);

	// J2座標系オブジェクトを作成、親(J1)座標系にアタッチする
	auto J2_Crd = sodl::CoordChainObj::create("J2_Crd", J1_Crd);
	// J2座標系の親座標系に対するオフセット量を設定する
	J2_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 230.f);

	// J3座標系オブジェクトを作成、親座標系にアタッチする
	auto J3_Crd = sodl::CoordChainObj::create("J3_Crd", J2_Crd);
	// J3座標系の親座標系に対するオフセット量を設定する
	J3_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 320.f);

	// J4座標系オブジェクトを作成、親座標系にアタッチする
	auto J4_Crd = sodl::CoordChainObj::create("J4_Crd", J3_Crd);
	// J4座標系の親座標系に対するオフセット量を設定する
	J4_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 200.f);

	// J5座標系オブジェクトを作成、親座標系にアタッチする
	auto J5_Crd = sodl::CoordChainObj::create("J5_Crd", J4_Crd);
	// J5座標系の親座標系に対するオフセット量を設定する
	J5_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 120.f);

	// J6座標系オブジェクトを作成
	auto J6_Crd = sodl::CoordChainObj::create("J6_Crd", J5_Crd);
	// J6座標系の親座標系に対するオフセット量を設定する
	J6_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 50.f);


	{
		//-----------------------------------------------------
		// 3Dモデル描画用のオブジェクトを作成する
		//   6軸ロボットアームの各関節を表現する各座標系に
		//   ロボットアームの各リンクの3Dモデルをアタッチする。
		//-----------------------------------------------------
		std::string exePath = app::GetModulePath();

		// ロボット台座のモデルをロードした描画オブジェクトを生成し、ワールド座標系原点にアタッチする
		auto RobotBase = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\BasePlate.stl", "RobotBase", World_Origin);

		// S1(sholder1)のモデルをロードした描画オブジェクトを生成し、J1座標系にアタッチする
		auto RobotSholder1 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\S1.stl", "S1", J1_Crd);

		// S2のモデルをロードした描画オブジェクトを生成し、J2座標系にアタッチする
		auto RobotSholder2 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\S2.stl", "S2", J2_Crd);
		// S2モデルの表示位置を調整するために、親(J2)座標系に対する座標系オフセットを設定する
		RobotSholder2->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, -90.f);

		// E1(elbow1)のモデルをロードした描画オブジェクトを生成し、J3座標系にアタッチする
		auto RobotElbow1 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\E1.stl", "E1", J3_Crd);
		// モデルの表示位置を調整するために、親座標系に対する座標系オフセットを設定する
		RobotElbow1->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, -70.f);

		// W1(Wrist1)のモデルをロードした描画オブジェクトを生成し、J4座標系にアタッチする
		auto RobotWrist1 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\W1.stl", "W1", J4_Crd);
		// モデルの表示位置を調整するために、親座標系に対する座標系オフセットを設定する
		RobotWrist1->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);

		// W2(Wrist2)のモデルをロードした描画オブジェクトを生成し、J5座標系にアタッチする
		auto RobotWrist2 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\W2.stl", "W2", J5_Crd);
		// モデルの表示位置を調整するために、親座標系に対する座標系オフセットを設定する
		RobotWrist2->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, -50.f);

		// W3(Wrist3)のモデルをロードした描画オブジェクトを生成し、J6座標系にアタッチする
		auto RobotWrist3 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\W3.stl", "W3", J6_Crd);
		// モデルの表示位置を調整するために、親座標系に対する座標系オフセットを設定する
		RobotWrist3->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);


		//-----------------------------------------------------
		// 定義したオブジェクトを描画対象として描画マネージャに登録
		//-----------------------------------------------------
		// 定義した座標系連鎖を描画マネージャの持つ描画空間にセット
		sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);
		// ここで描画空間に追加したオブジェクトは、shared_ptrとしてdrwMngr側に共有されるため、
		// 今後アクセスる必要がなければ、mainソース側のshared_ptrはここで破棄してしまっても構わない。
	}


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
		J1_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J1, UnitZ).matrix();
		J2_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J2, UnitX).matrix();
		J3_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J3, UnitX).matrix();
		J4_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J4, UnitZ).matrix();
		J5_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J5, UnitX).matrix();
		J6_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J6, UnitZ).matrix();

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
	//	<Summary>		キー操作時のコールバック
	//	<Description>
	//================================================================
	void keyFunc(unsigned char key, int u, int v)
	{
		const float		KEY_MOT_UNIT = 5.f;
		const float		CNV_DEG2RAD = (M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' は ESC の ASCII コード
			exit(0);
			break;

		case '1':
			ax_J1 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '2':
			ax_J2 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '3':
			ax_J3 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '4':
			ax_J4 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '5':
			ax_J5 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '6':
			ax_J6 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;

		case 'q':
			ax_J1 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'w':
			ax_J2 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'e':
			ax_J3 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'r':
			ax_J4 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 't':
			ax_J5 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'y':
			ax_J6 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;

		default:
			break;
		}
	}

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
