#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include <math.h>
#include <iostream>
#include <Windows.h>

namespace sodl = SmplObjDrwLib;

namespace app {
	// グローバル変数
	const int	WINDOW_SIZE_X = 1280;
	const int	WINDOW_SIZE_Y = 480;

	int mouse_x = 0;
	int mouse_y = 0;

	float ax_J1 =  0.0 * (M_PI / 180); // [rad]
	float ax_J2 = 45.0 * (M_PI / 180); // [rad]
	float ax_J3 = 90.0 * (M_PI / 180); // [rad]
	float ax_J4 =  0.0 * (M_PI / 180); // [rad]
	float ax_J5 = 45.0 * (M_PI / 180); // [rad]
	float ax_J6 =  0.0 * (M_PI / 180); // [rad]

	// Windowインタラクション関連コールバック関数
	void onKeyboard(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);

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
	sodl::DrawingManager::initMngr(app::WINDOW_SIZE_X,app::WINDOW_SIZE_Y);

	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetMouseHover(app::onMouseHover);
	sodl::drwMngr->SetKeyboardFunc(app::onKeyboard);

	//////////////////////////////////////////////////////
	//
	// ビューポート1とカメラ1を調整する。(デフォルトの描画空間[0]を撮影する)
	// 3Dモデルをロードし、デフォルトの描画空間[0]に配置する。
	//
	//////////////////////////////////////////////////////
	//------------------------------------------------------
	// ビューポート1
	//------------------------------------------------------
	{
		auto vp1 = sodl::drwMngr->viewPorts[0];
		vp1->setVpSize(
			0,						// left
			0,						// bottom
			app::WINDOW_SIZE_X / 2,	// width
			app::WINDOW_SIZE_Y		// height
		);
		auto cam1 = vp1->getCam();
		cam1->camPos = Eigen::Vector3f(400.f, -400.f, 400.f);
		cam1->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
		cam1->zoomRatio = 0.8;
		vp1->attachCam(cam1);
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
		// 定義した座標系連鎖を描画マネージャの持つデフォルトの描画空間にセット
		sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);
	}

	// 時系列グラフ アクセス用shared_ptrを用意
	std::shared_ptr< sodl::TimeSeriesGraph > timeSerialGraph;
	//////////////////////////////////////////////////////
	//
	// 追加の描画空間[1]を作成し、ビューポート, カメラを設定する。
	//
	// 時系列グラフ(2次元)を作成し、描画空間[1]に配置する。
	//
	//////////////////////////////////////////////////////
	{
		//----------------------------------------------------
		// 描画空間、ビューポート、カメラの追加と設定
		//----------------------------------------------------

		//----------------------------------------------------
		// グラフ用に描画空間[1]を追加
		//----------------------------------------------------
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();

		//----------------------------------------------------
		// ビューポート作成
		//----------------------------------------------------
		auto vpGrph = sodl::drwMngr->addViewPort("vpTimeSerialGraph");
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X / 2,		// left
			app::WINDOW_SIZE_Y / 2,		// bottom
			app::WINDOW_SIZE_X / 2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		//----------------------------------------------------
		// カメラ設定
		//----------------------------------------------------
		auto camGrph = vpGrph->getCam();
		// カメラが描画空間[1]を撮影するように関連付ける
		camGrph->spaceAttached = spaceGrph;

		// カメラを2Dグラフモードに設定する
		//   is2DGraph = true に設定したカメラは、グラフ平面を固定的に撮影するように
		//   vp->attachCam()したとき、自動的に位置姿勢が初期化される。
		//   ユーザコード側でいちいちカメラの位置姿勢を設定しなくてよい。
		camGrph->is2DGraphMode = true;

		// ビューポートに設定を更新したカメラを関連付ける。
		vpGrph->attachCam(camGrph);

		//----------------------------------------------------
		// 時系列グラフ(2次元)の作成と描画空間[1]への配置
		//----------------------------------------------------
		// 時系列グラフの作成
		auto tsGrph = sodl::TimeSeriesGraph::create(
				"Ax_J1 Position[deg] + Noize",			// オブジェクト名称 = グラフタイトル
				camGrph									// 関連付けるカメラ(カメラ投影行列などの設定が済んでいること)
			);
		// ラベルの設定
		tsGrph->xAxisLabel->text = "time";
		tsGrph->yAxisLabel->text = "position[deg]";

		// 描画空間[1]にグラフを追加
		sodl::drwMngr->AddObjTree_ToDrwSpace(tsGrph, 1);

		// グラフの内容をメインループ内で更新したいので、
		// {}の外で宣言したshared_ptrに作成したグラフのオブジェクトを渡しておく。
		timeSerialGraph = tsGrph;
	}


	// 散布図グラフ アクセス用shared_ptrを用意
	std::shared_ptr<sodl::GraphObj> scatterGraph_mouseDrag;
	//////////////////////////////////////////////////////
	//
	// 追加の描画空間[2]を作成し、ビューポート, カメラを設定する。
	//
	// 散布図グラフ"マウスドラッグ軌跡"を作成し、描画空間[2]に配置する。
	//
	//////////////////////////////////////////////////////
	{
		//----------------------------------------------------
		// 描画空間、ビューポート、カメラの追加と設定
		//----------------------------------------------------

		// グラフ用に描画空間[2]を追加
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		// ビューポート作成
		auto vpGrph = sodl::drwMngr->addViewPort("vpScatterGraph");
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X / 2,		// left
			0,							// bottom
			app::WINDOW_SIZE_Y / 2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		// カメラ設定
		auto camGrph = vpGrph->getCam();
		camGrph->spaceAttached = spaceGrph;
		camGrph->is2DGraphMode = true;
		vpGrph->attachCam(camGrph);

		//----------------------------------------------------
		// 散布図グラフ(2次元)の作成と描画空間[2]への配置
		//----------------------------------------------------
		// 散布図グラフの作成
		auto scttrGrph = sodl::GraphObj::create(
			"Mouse Drag Trajectory",				// オブジェクト名称 = グラフタイトル
			camGrph									// 関連付けるカメラ(カメラ投影行列などの設定が済んでいること)
		);
		// ラベルの設定
		scttrGrph->xAxisLabel->text = "X [pix]";
		scttrGrph->yAxisLabel->text = "Y [pix]";
		// 描画空間[2]にグラフを追加
		sodl::drwMngr->AddObjTree_ToDrwSpace(scttrGrph, 2);

		// グラフの内容をメインループ内で更新したいので、
		// {}の外で宣言したshared_ptrに作成したグラフのオブジェクトを渡しておく。
		scatterGraph_mouseDrag = scttrGrph;
	}


	// 散布図グラフ アクセス用shared_ptrを用意
	std::shared_ptr<sodl::GraphObj> scatterGraph_RobotHandXY;
	//////////////////////////////////////////////////////
	//
	// 追加の描画空間[3]を作成し、ビューポート, カメラを設定する。
	//
	// 散布図グラフ"ロボットハンドXY"を作成し、描画空間[3]に配置する。
	//
	//////////////////////////////////////////////////////
	{
		// グラフ用に描画空間[3]を追加
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		// ビューポート作成
		auto vpGrph = sodl::drwMngr->addViewPort("vpRobotHandXY");
		vpGrph->setVpSize(
			(app::WINDOW_SIZE_X / 2) + (app::WINDOW_SIZE_Y / 2),		// left
			0,							// bottom
			app::WINDOW_SIZE_Y / 2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		// カメラ設定
		auto camGrph = vpGrph->getCam();
		camGrph->spaceAttached = spaceGrph;
		camGrph->is2DGraphMode = true;
		vpGrph->attachCam(camGrph);

		//----------------------------------------------------
		// 散布図グラフ(2次元)の作成と描画空間[3]への配置
		//----------------------------------------------------
		// 散布図グラフの作成
		auto scttrGrph = sodl::GraphObj::create(
			"Robot Hand XY",						// オブジェクト名称 = グラフタイトル
			camGrph									// 関連付けるカメラ(カメラ投影行列などの設定が済んでいること)
		);
		// ラベルの設定
		scttrGrph->xAxisLabel->text = "X [mm]";
		scttrGrph->yAxisLabel->text = "Y [mm]";
		// 描画空間[3]にグラフを追加
		sodl::drwMngr->AddObjTree_ToDrwSpace(scttrGrph, 3);

		// グラフの内容をメインループ内で更新したいので、
		// {}の外で宣言したshared_ptrに作成したグラフのオブジェクトを渡しておく。
		scatterGraph_RobotHandXY = scttrGrph;
	}


	//////////////////////////////////////////////////////
	//
	// 周期処理の実施
	//

	int count = 0;
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
		// J1関節角度＋ランダムノイズを時系列グラフにプロット
		//-----------------------------------------------------
		timeSerialGraph->AddData(Eigen::Vector3f(count, app::ax_J1*180.0/M_PI + ((rand()%100 - 50)/10.0), 0));

		//-----------------------------------------------------
		// ロボットハンド位置X,Yを散布図グラフにプロット
		//-----------------------------------------------------
		Eigen::Vector3f handPos = J6_Crd->GetTf_root2self().translation();
		scatterGraph_RobotHandXY->AddData( handPos );

		//-----------------------------------------------------
		// ドラッグ中のマウス位置X,Yを散布図グラフにプロット
		//-----------------------------------------------------
		scatterGraph_mouseDrag->AddData(Eigen::Vector3f(app::mouse_x, app::mouse_y, 0));

		//-----------------------------------------------------
		// 描画マネージャから描画更新を実行する
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

		++count;
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
	void onKeyboard(unsigned char key, int u, int v)
	{
		const float		KEY_MOT_UNIT	=	5.f;
		const float		CNV_DEG2RAD		=	(M_PI / 180.f);

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
	//	<Summary>		GLウィンドウ上でマウスボタン操作時のコールバック
	//	<Description>
	//================================================================
	void onMouseBtn(int button, int state, int x, int y)
	{
	}

	//================================================================
	//
	//	<Summary>		GLウィンドウ上でマウスドラッグ時のコールバック
	//	<Description>
	//================================================================
	void onMouseDrag(int u, int v)
	{
		mouse_x = u;
		mouse_y = WINDOW_SIZE_Y - v;
	}

	//================================================================
	//
	//	<Summary>		GLウィンドウ上でマウスホバー時のコールバック
	//	<Description>
	//================================================================
	void onMouseHover(int u, int v)
	{
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
