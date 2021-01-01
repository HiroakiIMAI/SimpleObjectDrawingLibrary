#ifndef __DRAWING_MANAGER_H__
#define __DRAWING_MANAGER_H__

#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <thread>

#include <Eigen\Core>
#include <Eigen\Geometry>


#include "DrawableObjectClass.h"
#include "GLTypedef.h"



#define			DIAG_VIEW				"DiagonalView"
#define			HEAD_VIEW				"HeadView"
	


namespace SmplObjDrwLib {

	const std::string DEFAULT_CAM = "defaultCam";
	const std::string DEFAULT_VP = "defaultView";

	class CamClass:
		public virtual ISodlObjBase
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		CamClass(std::string name);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~CamClass() {}

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<CamClass> create(std::string name);

		//--------------------------------------------
		// static リスト
		//--------------------------------------------
	public:
		static std::list<std::weak_ptr<CamClass> > camInstanceList;

		//--------------------------------------------
		// カメラパラメータ
		//--------------------------------------------
	public:
		// 外部パラ
		Eigen::Vector3f camTgt;
		Eigen::Vector3f camPos;
		Eigen::Vector3f camUpVec;

		// 内部パラ
		float zoomRatio = 1.0;
		GLfloat m_prjection[16];
		bool isOrthoMode = true;
		bool is2DGraphMode = false;

		float prjMtxOrthWdt = 100.f;
		float prjMtxOrthHgt = 100.f;
		float widthHalfZoomed = 50.f;
		float heightHalfZoomed = 50.f;

		// 正射影マトリクス設定メソッド
		void SetPrjMtx_As2DView(
			float width,
			float height,
			float near = 0,
			float far = 5000
		);

		//--------------------------------------------
		// その他
		//--------------------------------------------
	public:
		int cam_id;

		std::weak_ptr<
			std::vector<
			sPtr_IDrawableObjBase
			>
		> spaceAttached;

	private:
		void addSelfToCamList();
	};

	class ViewPortClass :
		public virtual ISodlObjBase
	{
	public:
		static std::shared_ptr<ViewPortClass> create(std::string name, int vpSizeX, int vpSizeY);
		static std::list<std::weak_ptr<ViewPortClass> > vpInstanceList;

		int vp_id;

		// protected に移動予定
		int left;
		int bottom;
		int width;
		int height;


//		std::string name;
		std::shared_ptr<CamClass> camAttached;

		void attachCam(std::shared_ptr<CamClass> cam);
		void setVpSize(int left_, int bottom_, int width_, int height_);
		std::shared_ptr<CamClass> getCam();

	protected:
		ViewPortClass(std::string name, int vpSizeX, int vpSizeY);

		void fitCamMatrixToOrthView();
		void addSelfToVpList();

	private:

	};

	class DrawingManager
	{
		typedef DrawingManager TypeOfSelf;
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
	private:
		DrawingManager(
			int* argc, 
			char** argv, 
			int windowSizeX, 
			int windowSizeY,
			std::string windowTitle 
		);

		//--------------------------------------------
		// ファクトリ関数(シングルトンパターン)
		//--------------------------------------------
	public:
		static TypeOfSelf* initMngr(
			int* argc, char** argv, 
			int windowSizeX = 640 * 2, 
			int windowSizeY = 480 * 2,
			std::string windowTitle = "SimpleObjectDrawingLibrary Window"
		);

		//--------------------------------------------
		// 以下、本クラス固有の公開メンバ
		//--------------------------------------------
	public:
		void drawUpdt();
		void mvCam(Eigen::Vector3f mv);

		// 描画空間のベクタ
		sPtr_vector< sPtr_vector< sPtr_IDrawableObjBase > > drawingSpace;

		// 描画空間の追加
		sPtr_vector< sPtr_IDrawableObjBase > addDrawingSpace();

		// 描画空間へのオブジェクトの追加
		void AddObj_ToDrwSpace(sPtr_IDrawableObjBase obj, int numGround = 0);

		// 描画空間へのオブジェクトツリーの追加
		void AddObjTree_ToDrwSpace(std::shared_ptr<CoordChainObj> obj, int numGround = 0);

		// ビューポートのベクタ
		std::vector< std::shared_ptr<ViewPortClass> >viewPorts;

		// ビューポートの追加
		std::shared_ptr<ViewPortClass> addViewPort(std::string, int vpSizeX = -1, int vpSizeY = -1);

		// コールバック関数の設定I/F
		void SetKeyboardFunc(void(*func)(unsigned char key, int u, int v));
		void SetMouseFunc(void(*func)(int button, int state, int u, int v));
		void SetMouseDrag(void(*func)(int u, int v));
		void SetPassiveMotionFunc(void(*func)(int u, int v));

		//--------------------------------------------
		// 以下、本クラス固有の非公開メンバ
		//--------------------------------------------
	private:

		void draw(void);

		// 描画オブジェクトの種別毎の描画前処理
		//void DrawingManager::preDrawProc4GraphObj(
		//	sPtr_IDrawableObjBase obj,
		//	std::weak_ptr<CamClass> cam
		//);

		int _windowSizeX;
		int _windowSizeY;

		// default call back
		static void OnDispFunc();
		static void OnReshapeFunc(int u, int v);
		static void OnKeyboardFunc (unsigned char key, int u, int v);
		static void OnMouseBtn (int button, int state, int u, int v);
		static void OnMouseDrag (int u, int v);
		static void OnMouseHover(int u, int v);
		static void OnMouseWheel(int wheelNum, int dir, int u, int v);

		// user call back
		static void(*usrKeyboardFunc)(unsigned char key, int u, int v);
		static void(*usrMouseBtnFunc)(int button, int state, int u, int v);
		static void(*usrMouseDragFunc)(int u, int v);
		static void(*usrMouseHoverFunc)(int u, int v);
	
	};

};

namespace SmplObjDrwLib {
	extern	std::unique_ptr<DrawingManager> drwMngr;
	extern	int mouseBtnSt[3];
	extern	unsigned char key;
	extern	int reshapeWindowSize[2];
};


#endif

