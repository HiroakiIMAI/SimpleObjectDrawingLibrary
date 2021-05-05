#include "include/DrawingManager.h"
#include "include/DrawableObjectClass.h"
#include "include/GraphObj.h"
#include "include/glDrawExt.h"

#include <cstdio>
#include <iostream>
#include <vector>

#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#pragma comment( lib, "glew32.lib" )
#pragma comment( lib, "freeglut.lib" )

namespace SmplObjDrwLib {

	// extern しない変数
	int mouseU_prv = 0;
	int mouseV_prv = 0;

	// extern で外部ファイルに公開する変数
	std::unique_ptr<DrawingManager> drwMngr;
	int mouseBtnSt[3];
	unsigned char key;

};

void voidFunc(){}

using namespace SmplObjDrwLib;

/////////////////////////////////////////////////////////////////////
//
//	CamClass
//

std::list<std::weak_ptr<CamClass> > CamClass::camInstanceList;

 // factory
std::shared_ptr<CamClass> CamClass::create(std::string name)
{
	auto ptr = std::shared_ptr<CamClass>(new CamClass(name));
	ptr->addSelfToCamList();

	// 作成したインスタンスが最初の一つでない場合、
	// 最後の作成済インスタンスが参照している描画グラウンドと
	// 同じものをデフォルト参照とする。
	auto itr_self = (--camInstanceList.end());
	if (itr_self != camInstanceList.begin())
	{
		ptr->spaceAttached = (--itr_self)->lock()->spaceAttached;
	}

	return ptr;
}


// constructor
CamClass::CamClass(std::string name)
	: ISodlObjBase(name)
{
	// set default external cam param
	camPos = Eigen::Vector3f(1000.f, -1000.f, 1000.f);
	camTgt = Eigen::Vector3f(0.f, 0.f, -0.f);
	camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
}

void CamClass::addSelfToCamList()
{
	camInstanceList.push_back(
		std::dynamic_pointer_cast<CamClass>(
			shared_from_this()
		)
	);
}

//================================================================
//
//	<Summary>		カメラに正射影マトリクスを設定する
//	<Description>
//================================================================
void CamClass::SetPrjMtx_As2DView(
	float width,
	float height,
	float _near,
	float _far
)
{

	prjMtxOrthWdt = width  / zoomRatio;
	prjMtxOrthHgt = height / zoomRatio;
	widthHalfZoomed  = prjMtxOrthWdt / 2.f;
	heightHalfZoomed = prjMtxOrthHgt / 2.f;

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (is2DGraphMode)
	{
		glOrtho(
			0,
			prjMtxOrthWdt,
			0,
			prjMtxOrthHgt,
			CULLING_FORE,
			CULLING_BACK
		);
	}
	else
	{
		glOrtho(
			-widthHalfZoomed,
			widthHalfZoomed,
			-heightHalfZoomed,
			heightHalfZoomed,
			CULLING_FORE,
			CULLING_BACK
		);
	}

	glGetFloatv(GL_PROJECTION_MATRIX, m_prjection);

	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////
//
//	ViewPortClass
//

std::list<std::weak_ptr<ViewPortClass> > ViewPortClass::vpInstanceList;

//--------------------------------------------
// ファクトリ関数
//--------------------------------------------
std::shared_ptr<ViewPortClass> ViewPortClass::create(std::string name, int vpSizeX, int vpSizeY)
{
	auto ptr = std::shared_ptr<ViewPortClass>(new ViewPortClass(name, vpSizeX, vpSizeY));
	ptr->addSelfToVpList();


	return ptr;
}

//--------------------------------------------
// コンストラクタ
//--------------------------------------------
ViewPortClass::ViewPortClass(std::string name, int vpSizeX, int vpSizeY)
	:ISodlObjBase(name)
	,camAttached(NULL)
{
	vp_id = (int)vpInstanceList.size();
	//vpInstanceList.push_back( shared_from_this() );

	int num = vp_id + 1;

	width	= vpSizeX /num;
	height	= vpSizeY /num;
	left	= vpSizeX - width;
	bottom	= vpSizeY - height;

	attachCam( CamClass::create(DEFAULT_CAM) );
}

void ViewPortClass::addSelfToVpList()
{
	vpInstanceList.push_back(
		std::dynamic_pointer_cast<ViewPortClass>(
			shared_from_this()
			)
	);

}

void ViewPortClass::setVpSize(int left_, int bottom_, int width_, int height_)
{
	left	= left_;
	bottom	= bottom_;
	width	= width_;
	height	= height_;

	if (camAttached->isOrthoMode)
	{
		fitCamMatrixToOrthView();
	}
}

 std::shared_ptr<CamClass> ViewPortClass::getCam()
{
	 return camAttached;
}

void ViewPortClass::attachCam(std::shared_ptr<CamClass> cam)
{
	camAttached = cam;

	if (cam->is2DGraphMode)
	{
		// 2DGraphに関連付けられたカメラは、グラフ平面を固定的に撮影するので
		// vpにattachしたタイミングで位置姿勢を初期化する。
		// ユーザコード側でいちいちカメラの位置姿勢を設定しなくてよい。
		cam->camPos = Eigen::Vector3f(0.f, 0.f, 10.f);
		cam->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
		cam->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
	}

	if (camAttached->isOrthoMode)
	{
		fitCamMatrixToOrthView();
	}
}

void ViewPortClass::fitCamMatrixToOrthView()
{
	camAttached->SetPrjMtx_As2DView(width, height);
}


/////////////////////////////////////////////////////////////////////
//
//	DrawingManager
//

//================================================================
//
//	<Summary>		コンストラクタ
//	<Description>
//================================================================
DrawingManager::DrawingManager(int windowSizeX, int windowSizeY, std::string windowTitle) :
	_windowSizeX(windowSizeX),
	_windowSizeY(windowSizeY)
{
	//char arg1[] = "exeName";
	char arg2[] = "-gldebug";
	char* argArr[] = { arg2 };
	//argv = (char**)&arg;

	int num = 1;
	int* argc = &num;

	// init Glut
	glutInit(argc, argArr);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(windowSizeX, windowSizeY);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);	// test for exit program
	glutCreateWindow( windowTitle.c_str() );

	// init GLEW
	glewInit();

	glEnableClientState(GL_VERTEX_ARRAY);

	// コールバック関数の設定
	// 注意：displayfuncのコールバック関数を登録しておかないと、
	// glutMainLoopEvent()をコールしたときにエラーが発生してプログラムが終了してしまう。
	glutDisplayFunc( OnDispFunc );
//	glutDisplayFunc( voidFunc );
	glutReshapeFunc( OnReshapeFunc);
	glutKeyboardFunc( OnKeyboardFunc );
	glutMouseFunc( OnMouseBtn );
	glutMotionFunc( OnMouseDrag );
	glutPassiveMotionFunc( OnMouseHover );
	glutMouseWheelFunc( OnMouseWheel);

	// 描画空間ベクタを初期化する
	drawingSpace = std::make_shared< std::vector< sPtr_map< T_ID, sPtr_IDrawableObjBase > > >();

	// デフォルト描画空間を作成
	addDrawingSpace();

	//--------------------------------------------------------
	// init ViewPortData

	auto vpDiag = addViewPort(DIAG_VIEW);
	vpDiag->camAttached->spaceAttached = (*drawingSpace)[0];

	// init ViewPortData
	//--------------------------------------------------------

	//ブレンドの有効化
	glEnable(GL_BLEND);
	// アルファチャンネルを使ったカラーブレンドを選択
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glEnable(GL_LIGHTING);				// ライティングの有効化
//	glEnable(GL_COLOR_MATERIAL);		// ライティング有効中のglColor3d()の有効化

}


//================================================================
//
//	<Summary>		ファクトリ関数(シングルトンパターン)
//	<Description>
//================================================================
DrawingManager::TypeOfSelf* DrawingManager::initMngr(int windowSizeX, int windowSizeY, std::string windowTitle)
{
	static auto ptr = new TypeOfSelf( windowSizeX, windowSizeY, windowTitle);
	SmplObjDrwLib::drwMngr.reset(ptr);

	return  ptr;

}


//================================================================
//
//	<Summary>		描画関数
//	<Description>
//================================================================
void DrawingManager::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto itr = viewPorts.begin();
		itr != viewPorts.end();
		itr++)
	{
		auto vp = *itr;
		auto cam = vp->camAttached;

		//------------------------
		// ビューポートの配置
		//------------------------
		glViewport(vp->left, vp->bottom, vp->width, vp->height);

		//------------------------
		// 透視投影の設定(カメラの内部パラ設定)
		//------------------------
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(cam->m_prjection);

		//------------------------
		// カメラの配置(カメラの外部パラ設定)
		//------------------------
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		{
			// カメラを配置する前にviewPortに縁を描く
			float _w = cam->widthHalfZoomed;
			float _h = cam->heightHalfZoomed;
			// ズーム比に応じて、最終的に1[pix]で線が書けるように調整
			float _a = 1.0 / (cam->zoomRatio);
			glColor3d(1.0, 1.0, 1.1); // white
			glRectf(-_w, -_h, _w, _w);
			glColor3d(0.0, 0.0, 0.1); // navy
			glRectf(-_w + _a, -_h + _a, _w - _a, _w - _a);
		}

		// カメラの配置
		gluLookAt(
			cam->camPos.x(),		cam->camPos.y(),		cam->camPos.z(),	// camPos
			cam->camTgt.x(),		cam->camTgt.y(),		cam->camTgt.z(),	// camTargetCentor
			cam->camUpVec.x(),		cam->camUpVec.y(),		cam->camUpVec.z()	// upVec
		);

		//------------------------
		// 描画対象オブジェクトを描画する
		//------------------------
		if( depthBufferIsEnable )
		{
			// Zバッファを有効化
			glEnable( GL_DEPTH_TEST );
		}
		// 描画対象のグラウンドが有効に設定されているかをチェックする
		// (ポインタ参照先が未設定or破棄されていいないか?)
		auto grnd = cam->spaceAttached.lock();
		{
			// グラウンドに配置されたオブジェクトを順に描画する
			for (auto obj = grnd->begin();
				obj != grnd->end();
				obj++)
			{
				// 描画
				obj->second->draw();
			}
		}
		glDisable( GL_DEPTH_TEST );
	}
}

//================================================================
//
//	<Summary>		描画更新前処理 グラフ専用
//	<Description>
//================================================================
/*
void DrawingManager::preDrawProc4GraphObj(
	sPtr_IDrawableObjBase obj,
	std::weak_ptr<CamClass> cam
)
{
	if (auto grph = std::dynamic_pointer_cast<GraphObj>(obj))
	{
	}
}
*/

//================================================================
//
//	<Summary>		描画更新
//	<Description>
//================================================================
void DrawingManager::drawUpdt(void)
{

	// アプリケーション側でセット可能な描画前処理を実施する
	if( Func_PreDraw )
	{
		Func_PreDraw();
	}

	// SODLの描画処理メインルーチンを実施する
	draw();

	// アプリケーション側でセット可能な描画後処理を実施する
	if( Func_PostDraw )
	{
		Func_PostDraw();
	}

	glutSwapBuffers();
	glutMainLoopEvent(); // glutMainLoop() 等価な内容を実施してくれる
}

//================================================================
//
//	<Summary>		カメラ移動
//	<Description>
//================================================================
void DrawingManager::mvCam( Eigen::Vector3f mv )
{
	viewPorts[0]->camAttached->camPos += mv;
}

//================================================================
//
//	<Summary>		ビューポートの追加
//	<Description>
//================================================================
std::shared_ptr<ViewPortClass> DrawingManager::addViewPort(std::string name, int vpSizeX, int vpSizeY)
{
	// 引数のvpSizeが省略された場合は、windowSizeを使用する
	(vpSizeX == -1) ? vpSizeX = _windowSizeX : vpSizeX = vpSizeX;
	(vpSizeY == -1) ? vpSizeY = _windowSizeY : vpSizeY = vpSizeY;

	auto vp = ViewPortClass::create(name, vpSizeX, vpSizeY);
	viewPorts.push_back(vp);
	return vp;
}


//================================================================
//
//	<Summary>		描画空間の追加
//	<Description>
//================================================================
sPtr_map< T_ID, sPtr_IDrawableObjBase > DrawingManager::addDrawingSpace()
{
	drawingSpace->emplace_back(
		std::make_shared< std::map< T_ID, sPtr_IDrawableObjBase > >()
	);
	return drawingSpace->back();
}

/** ***************************************************************
 * @brief 描画空間へのオブジェクトの追加
 *
 ******************************************************************/
void DrawingManager::AddObj_ToDrwSpace(sPtr_IDrawableObjBase obj, int numGround)
{
	if (numGround < drawingSpace->size())
	{
		(*drawingSpace)[numGround]->insert( std::make_pair(obj->id_readOnly , obj) );
		obj->drawingSpace_belongTo = (*drawingSpace)[numGround];
		obj->drawingSpaceNum_belongTo = numGround;
	}
}


/** ***************************************************************
 * @brief 描画空間からオブジェクトを削除
 *
 * <pre>
 * オブジェクトIDを指定して削除する
 * </pre>
 ******************************************************************/
void DrawingManager::DeleteObj_FromDrwSpace(T_ID id, int numGround )
{
	if (numGround < drawingSpace->size())
	{
		// 対象オブジェクトの所属空間をクリア
		(*drawingSpace)[numGround]->at(id)->drawingSpace_belongTo.reset();
		(*drawingSpace)[numGround]->at(id)->drawingSpaceNum_belongTo = -1;
		// 描画空間からオブジェクトを削除
		(*drawingSpace)[numGround]->erase( id );
	}
}



//================================================================
//
//	<Summary>		描画空間へのオブジェクトツリーの追加
//	<Description>
//================================================================
void DrawingManager::AddObjTree_ToDrwSpace(std::shared_ptr<CoordChainObj> obj, int numGround)
{
	if (numGround < drawingSpace->size())
	{
		// 受け取ったobjを描画空間に追加する
		AddObj_ToDrwSpace(obj, numGround);
		std::cout << obj->name << std::endl;

		// 子要素に対して再帰する
		for (auto itr = obj->wPtr_chldrn.begin();
			itr != obj->wPtr_chldrn.end();
			itr++)
		{
			auto sPtr_child =  itr->lock();
			AddObjTree_ToDrwSpace( sPtr_child, numGround);
		}
	}
}

void DrawingManager::OnDispFunc()
{
	;
}

void DrawingManager::OnReshapeFunc(int x, int y)
{
	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	(*usrReshapeFunc)( x, y );
}

void DrawingManager::OnKeyboardFunc(unsigned char key, int u, int v)
{
	SmplObjDrwLib::key = key;

	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	if (drwMngr->usrKeyboardFunc)
	{
		(*usrKeyboardFunc)(key, u, v);
	}
}

void DrawingManager::OnKeyboardUpFunc(unsigned char key, int u, int v)
{
	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	if (drwMngr->usrKeyboardUpFunc)
	{
		(*usrKeyboardUpFunc)(key, u, v);
	}
}

void DrawingManager::OnKeyboardSpFunc(unsigned char key, int u, int v)
{
	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	if (drwMngr->usrKeyboardSpFunc)
	{
		(*usrKeyboardSpFunc)(key, u, v);
	}
}

void DrawingManager::OnKeyboardSpUpFunc(unsigned char key, int u, int v)
{
	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	if (drwMngr->usrKeyboardSpUpFunc)
	{
		(*usrKeyboardSpUpFunc)(key, u, v);
	}
}

void DrawingManager::OnMouseBtn(int button, int state, int u, int v)
{
	mouseBtnSt[button] = state;

	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	if (usrMouseBtnFunc)
	{
		(*usrMouseBtnFunc)(button, state, u, v);
	}
}


void DrawingManager::OnMouseDrag(int u, int v)
{
	const int du = u - mouseU_prv;
	const int dv = v - mouseV_prv;

	Eigen::Vector3f mv;

	auto& default_cam =SmplObjDrwLib::drwMngr->viewPorts[0]->camAttached;

	if( (mouseBtnSt[MOUSE_LEFT_BUTTON] == MOUSE_DOWN)
	||	(mouseBtnSt[MOUSE_RIGHT_BUTTON] == MOUSE_DOWN)
	)
	{
		Eigen::Vector3f* cPos = &default_cam->camPos;
		Eigen::Vector3f* tPos = &default_cam->camTgt;
		Eigen::Vector3f dir_p2t = *cPos - *tPos;
		dir_p2t.normalize();

		Eigen::Vector3f uDir3D = dir_p2t.cross(UnitZ);
		uDir3D.normalize();

		Eigen::Vector3f vDir3D = dir_p2t.cross(uDir3D);


		// 通常ドラッグの場合、視点を中心にカメラを回転させる
		if (mouseBtnSt[MOUSE_LEFT_BUTTON] == MOUSE_DOWN)
		{
			*cPos += 2 * ((uDir3D * du) + (-vDir3D * dv));
		}
		// 右ドラッグの場合、視点とカメラ位置を並行移動させる
		else if(mouseBtnSt[MOUSE_RIGHT_BUTTON] == MOUSE_DOWN)
		{
			float z = 1.f/default_cam->zoomRatio;
			*tPos += z * ((uDir3D * du) + (-vDir3D * dv));
			*cPos += z * ((uDir3D * du) + (-vDir3D * dv));
		}

	}


	mouseU_prv = u;
	mouseV_prv = v;

	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	if (usrMouseDragFunc)
	{
		(*usrMouseDragFunc)(u, v);
	}
}

void DrawingManager::OnMouseHover(int u, int v)
{
	mouseU_prv = u;
	mouseV_prv = v;

	//-------------------------------------
	// ライブラリユーザ用コールバック
	//-------------------------------------
	if (usrMouseHoverFunc)
	{
		(*usrMouseHoverFunc)(u, v);
	}
}

void DrawingManager::OnMouseWheel(int wheelNum, int dir, int u, int v)
{
	// デフォルト処理:
	//  デフォルトvp のデフォルトcam に対してズームイン、ズームアウトを実施する
	auto dfltCam = drwMngr->viewPorts[0]->getCam();

	// 正回転 → 拡大
	if (dir > 0)
	{
		dfltCam->zoomRatio *= 1.1;
	}
	// 負回転 → 縮小
	else
	{
		dfltCam->zoomRatio *= 0.9;
	}

	drwMngr->viewPorts[0]->attachCam(dfltCam);
}

void DrawingManager::SetReshapeFunc(void(*func)(int u, int v))
{
	usrReshapeFunc = func;
}

void DrawingManager::SetKeyboardFunc(void(*func)(unsigned char key, int u, int v))
{
	usrKeyboardFunc = func;
}

void DrawingManager::SetKeyboardUpFunc(void(*func)(unsigned char key, int u, int v))
{
	usrKeyboardUpFunc = func;
}

void DrawingManager::SetKeyboardSpFunc(void(*func)(unsigned char key, int u, int v))
{
	usrKeyboardSpFunc = func;
}

void DrawingManager::SetKeyboardSpUpFunc(void(*func)(unsigned char key, int u, int v))
{
	usrKeyboardSpUpFunc = func;
}

void DrawingManager::SetMouseFunc(void(*func)(int button, int state, int u, int v))
{
	usrMouseBtnFunc=func;
}

void DrawingManager::SetMouseDrag(void(*func)(int u, int v))
{
	usrMouseDragFunc=func;
}

void DrawingManager::SetMouseHover(void(*func)(int u, int v))
{
	usrMouseHoverFunc=func;
}

void DrawingManager::SetMouseWheelFunc(void(*func)(int wheelNum, int dir, int u, int v))
{
	usrMouseWheelFunc=func;
}


void (*DrawingManager::usrReshapeFunc)		(int u, int v)							= NULL;
void (*DrawingManager::usrKeyboardFunc)		(unsigned char key, int u, int v)		= NULL;
void (*DrawingManager::usrKeyboardUpFunc)	(unsigned char key, int u, int v)		= NULL;
void (*DrawingManager::usrKeyboardSpFunc)	(unsigned char key, int u, int v)		= NULL;
void (*DrawingManager::usrKeyboardSpUpFunc)	(unsigned char key, int u, int v)		= NULL;
void (*DrawingManager::usrMouseBtnFunc)		(int button, int state, int u, int v)	= NULL;
void (*DrawingManager::usrMouseDragFunc)	(int u, int v)							= NULL;
void (*DrawingManager::usrMouseHoverFunc)	(int u, int v)							= NULL;
void (*DrawingManager::usrMouseWheelFunc)	(int wheelNum, int dir, int u, int v)	= NULL;


//-- 以下メモ ----------------------------------
//		glGetFloatv(GL_MODELVIEW_MATRIX, m);
