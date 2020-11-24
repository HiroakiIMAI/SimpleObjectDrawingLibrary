#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "include/SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawLib.lib" )


// ライブラリ名のエイリアスを設定する
namespace sodl = SmplObjDrwLib;

// アプリケーションのグローバルシンボルをappネームスペースに収める
namespace app {

	const int	WINDOW_SIZE_X = 640;
	const int	WINDOW_SIZE_Y = 480;
	float ax_J1 = 0; // [rad]
	float ax_J2 = 0; // [rad]
	float ax_J3 = 0; // [rad]
	float ax_J4 = 0; // [rad]

	void keyFunc(unsigned char key, int u, int v);
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
	// 機械座標原点を定義
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f,0.f);

	// J1~4座標系を定義
	auto J1_Crd = sodl::CoordChainObj::create("J1_Crd", World_Origin);
	J1_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 10.f);

	auto J2_Crd = sodl::CoordChainObj::create("J2_Crd", J1_Crd );
	J2_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 10.f);

	auto J3_Crd = sodl::CoordChainObj::create("J3_Crd", J2_Crd);
	J3_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 100.f);

	auto J4_Crd = sodl::CoordChainObj::create("J4_Crd", J3_Crd);
	J4_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 80.f);

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
		J1_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J1, UnitZ ).matrix();
		J2_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J2, UnitX ).matrix();
		J3_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J3, UnitX ).matrix();
		J4_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J4, UnitX ).matrix();

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
		const float		CNV_DEG2RAD		=	(M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' は ESC の ASCII コード
			exit(0);

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

		default:
			break;
		}
	}


};
