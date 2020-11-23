#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "fGlutTest.lib" )


#include <math.h>
#include <iostream>

#define			KEY_MOT_UNIT	5.f
#define			CNV_DEG2RAD		(M_PI/180.f)

#define			PATH_TESTMODEL	"C:/Program Files/Assimp/test/models/STL/Spider_ascii.stl"

//-----------------
// debug


// debug
//-----------------

namespace sodLib = SmplObjDrwLib;

float Axez_X = 10;
float Axez_Y = -200;
float Axez_Z = -300;
float Axez_B = 0;
float Axez_C = 0;

namespace app {

	int mouseBtnSt[3];
	int mouseU_prv = 0;
	int mouseV_prv = 0;

	void keyFunc(unsigned char key, int u, int v);
	//void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);
};

int main(int argc, char ** argv) 
{
	sodLib::DrawingManager::initMngr( &argc, argv);
	
	//sodLib::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodLib::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodLib::drwMngr->SetPassiveMotionFunc(app::onMouseHover);
	sodLib::drwMngr->SetKeyboardFunc(app::keyFunc);
	

	//////////////////////////////////////////////////////
	//
	// ビューポートとカメラの設定
	//

	// ビューポート1
	auto vp1 = sodLib::drwMngr->viewPorts[0];
	vp1->setVpSize(
		0,			// left
		0,			// bottom
		VP_SIZE_X,	// width
		VP_SIZE_Y	// height
	);
	auto cam1 = vp1->getCam();
	cam1->camPos = Eigen::Vector3f(600.f, -2000.f, -0.f);
	cam1->camTgt = Eigen::Vector3f(300.f, -300.f, -300.f);
	cam1->zoomRatio = 2.0;
	vp1->attachCam(cam1);

	// ビューポート2
	auto vp2 = sodLib::drwMngr->addViewPort("vp2");
	vp2->setVpSize(
		VP_SIZE_X,	// left
		0,			// bottom
		VP_SIZE_X,	// width
		VP_SIZE_Y	// height
	);
	auto cam2 = vp2->camAttached;
	cam2->camPos = Eigen::Vector3f(300.f, -300.f, 5000.f);
	cam2->camTgt = Eigen::Vector3f(300.f, -300.f, -300.f);
	cam2->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);



	// 時系列グラフの作成
	auto timeSerialGraph = sodLib::TimeSeriesGraph::create("TimeSerealGraph");
	timeSerialGraph->rangeMax.x() = 0;
	timeSerialGraph->rangeMin.x() = 0;
	timeSerialGraph->rangeMax.y() = 100;
	timeSerialGraph->rangeMin.y() = -100;
	{
		// グラフ用に描画空間を追加
		auto spaceGrph = sodLib::drwMngr->addDrawingSpace();
		{
			// ビューポート3
			auto vpGrph = sodLib::drwMngr->addViewPort("vp3");
			vpGrph->spaceAttached = spaceGrph;
			vpGrph->setVpSize(
				VP_SIZE_X,		// left
				VP_SIZE_Y / 2,	// bottom
				VP_SIZE_X/2,		// width
				VP_SIZE_Y / 2		// height
			);
			auto camGrph = vpGrph->getCam();
			camGrph->camPos = Eigen::Vector3f(0.f, 0.f, 10.f);
			camGrph->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
			camGrph->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
			camGrph->SetPrjMtx_As2DView(100, 100);
		}
	}
	// 描画空間にグラフを追加
	sodLib::drwMngr->AddObjTree_ToDrwSpace(timeSerialGraph, 1);


	// 散布図グラフの作成
	auto scatterGraph = sodLib::GraphObj::create("ScatterGraph");
	scatterGraph->rangeMax.x() =  100;
	scatterGraph->rangeMin.x() = -100;
	scatterGraph->rangeMax.y() =  100;
	scatterGraph->rangeMin.y() = -100;
	{
		// グラフ用に描画空間を追加
		auto spaceGrph = sodLib::drwMngr->addDrawingSpace();
		{
			// ビューポート4
			auto vpGrph = sodLib::drwMngr->addViewPort("vp4");
			vpGrph->spaceAttached = spaceGrph;
			vpGrph->setVpSize(
				VP_SIZE_X,			// left
				0,					// bottom
				VP_SIZE_Y / 2,		// width
				VP_SIZE_Y / 2		// height
			);
			auto camGrph = vpGrph->getCam();
			camGrph->camPos = Eigen::Vector3f(0.f, 0.f, 10.f);
			camGrph->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
			camGrph->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
			camGrph->SetPrjMtx_As2DView(100, 100);
		}
	}
	// 描画空間にグラフを追加
	sodLib::drwMngr->AddObjTree_ToDrwSpace(scatterGraph, 2);



	//////////////////////////////////////////////////////
	//
	// 描画対象オブジェクトの生成
	//

	// 機械座標原点を定義
	auto Mch_Origin = sodLib::CoordChainObj::create("Mch_Origin");
	Mch_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f,0.f);

	// 機械座標原点から連鎖するXYZリンクを定義
	auto Y_Link = sodLib::CoordChainObj::create("Y_Link", Mch_Origin);
	auto X_Link = sodLib::CoordChainObj::create("X_Link", Y_Link );
	auto Z_Link = sodLib::CoordChainObj::create("Z_Link", X_Link);

	// 機械座標原点から連鎖するBCリンクを定義
	auto B_Link = sodLib::CoordChainObj::create("B_Link", Mch_Origin);
	B_Link->CrdTrs.translation() = Eigen::Vector3f(300.f, 0.f, -300.f);
	auto C_Link = sodLib::CoordChainObj::create("C_Link", B_Link);
	C_Link->CrdTrs.translation() = Eigen::Vector3f(0.f, -300.f, 0.f);	

	// 3Dモデルオブジェクトをリンクにアタッチする
	auto Mch_OriginModel = sodLib::CoordChain3dMdl::create("3dModel/Y_Rail.stl", "Y_Rail", Mch_Origin);
	Mch_OriginModel->CrdTrs.translation() = Eigen::Vector3f(-100.f,-800.f,-100.f);
	
	auto Y_LinkModel = sodLib::CoordChain3dMdl::create("3dModel/X_Rail.stl", "X_Rail", Y_Link);
	Y_LinkModel->CrdTrs.translation() = Eigen::Vector3f(-100.f, 0.f, 0.f);

	auto X_LinkModel = sodLib::CoordChain3dMdl::create("3dModel/Z_Rail.stl", "Z_Rail", X_Link);
	X_LinkModel->CrdTrs.translation() = Eigen::Vector3f(-50.f, -10.f, 0.f);

	auto Z_LinkModel = sodLib::CoordChain3dMdl::create("3dModel/head_cone.stl", "head_cone", Z_Link);
	



	////////////////////////////////////////////////////// 
	//
	// 定義した描画対象オブジェクトを描画マネージャに登録
	//

	// 定義した座標系連鎖を描画空間にセット
	sodLib::drwMngr->AddObjTree_ToDrwSpace(Mch_Origin);

	int count = 0;
	while(1)
	{
		//app::keyFunc(sodLib::key); 
		//sodLib::key = 0;

		X_Link->CrdTrs.translation() = Eigen::Vector3f(Axez_X, 0, 0);
		Y_Link->CrdTrs.translation() = Eigen::Vector3f(0, Axez_Y, 0);
		Z_Link->CrdTrs.translation() = Eigen::Vector3f(0, 0, Axez_Z);

		B_Link->CrdTrs.linear() = Eigen::AngleAxisf( Axez_B, UnitY ).matrix();
		C_Link->CrdTrs.linear() = Eigen::AngleAxisf( Axez_C, UnitZ ).matrix();
		

		//---------------------------------
		// debug  
		
		timeSerialGraph->addData(Eigen::Vector3f(count, Axez_X + rand()%100 - 50, -3));

		// debug
		//---------------------------------

		sodLib::drwMngr->drawUpdt();

		++count;
		Sleep(10);
	}

	return 0;
}


namespace app {


	void keyFunc(unsigned char key, int u, int v)
	{
		switch (key) {
		case '\033':
			exit(0);  /* '\033' は ESC の ASCII コード */

		case '1':
			Axez_X += KEY_MOT_UNIT;
			break;
		case '2':
			Axez_Y += KEY_MOT_UNIT;
			break;
		case '3':
			Axez_Z += KEY_MOT_UNIT;
			break;
		case '4':
			Axez_B += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '5':
			Axez_C += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;

		case 'q':
			Axez_X -= KEY_MOT_UNIT;
			break;
		case 'w':
			Axez_Y -= KEY_MOT_UNIT;
			break;
		case 'e':
			Axez_Z -= KEY_MOT_UNIT;
			break;
		case 'r':
			Axez_B -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 't':
			Axez_C -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;


		default:
			break;
		}
	}

	/*
	void onMouseBtn(int button, int state, int x, int y)
	{
		mouseBtnSt[button] = state;
	}
	*/

	void onMouseDrag(int u, int v)
	{
		/*
		const int du = u - mouseU_prv;
		const int dv = v - mouseV_prv;

		Eigen::Vector3f mv;

		if (mouseBtnSt[sodLib::MOUSE_LEFT_BUTTON] == sodLib::MOUSE_DOWN)
		{
			Eigen::Vector3f* cPos = &SmplObjDrwLib::drwMngr->viewPorts[0]->camAttached->camPos;
			Eigen::Vector3f* tPos = &SmplObjDrwLib::drwMngr->viewPorts[0]->camAttached->camTgt;
			Eigen::Vector3f dir_p2t = *cPos - *tPos;
			dir_p2t.normalize();
			Eigen::Vector3f uDir3D = dir_p2t.cross(UnitZ);
			Eigen::Vector3f vDir3D = dir_p2t.cross(uDir3D);

			*cPos += 2 * ((uDir3D * du) + (-vDir3D * dv));
		}

		mouseU_prv = u;
		mouseV_prv = v;
		*/
	}

	void onMouseHover(int u, int v)
	{
		//mouseU_prv = u;
		//mouseV_prv = v;
	}

};
