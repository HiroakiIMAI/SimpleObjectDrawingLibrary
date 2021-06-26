#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#include "VifClass_PlotCnfg.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glut.h"
#include "ImGui/backends/imgui_impl_opengl2.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"



#include <math.h>
#include <iostream>
#include <fstream>

#include <Windows.h>

namespace sodl = SmplObjDrwLib;

namespace app {
	// グローバル変数
	const int	WINDOW_SIZE_X = 640*2;
	const int	WINDOW_SIZE_Y = 480*2;

	int mouse_x = 0;
	int mouse_y = 0;

	//-------------------------------------------------
	// CSVファイルをアプリケーションロジック上で扱うためのI/F
	//-------------------------------------------------
	// ファイル毎の構造
	typedef struct ST_PLOT_FILE_tp{
		std::string filePath;
		std::ifstream ifs;
		std::streampos seekPos = 0;
		int seekRow = 0;

		bool fg_xyz = false;
		bool fg_xyzijk = false;
		bool fg_xyzbc = false;
		bool fg_xyzac = false;
		bool fg_xyzabc = false;

		int cX = -1;
		int cY = -1;
		int cZ = -1;
		int cI = -1;
		int cJ = -1;
		int cK = -1;
	}ST_PLOT_FILE;

	// ファイル関連操作I/Fルート構造
	typedef struct FILE_IF_tp{
		std::map< std::string, ST_PLOT_FILE > pltFiles;
	}FILE_IF;

	// ファイル関連操作I/Fルートオブジェクト
	FILE_IF csvIf;

	std::map< std::string, std::shared_ptr<sodl::GraphObj> > graphs2D;

	//-------------------------------------------------
	// UIとアプリケーションロジックを媒介するI/F変数
	//-------------------------------------------------
	appUi::VISUAL_IF visualIf;

	//-------------------------------------------------
	// Windowインタラクション関連コールバック関数
	//-------------------------------------------------
	void onReshape(int u, int v);
	void onKeyboard(unsigned char key, int u, int v);
	void onKeyboardUp(unsigned char key, int u, int v);
	void onKeyboardSp(unsigned char key, int u, int v);
	void onKeyboardSpUp(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);
	void onMouseWheel(int wheelNum, int dir, int u, int v);

	//-------------------------------------------------
	// UIライブラリImGuiを利用するアプリケーションコード(SODLのコールバックに登録する)
	//-------------------------------------------------
	void PreDraw_ImGui();
	void PostDraw_ImGui();

	//-------------------------------------------------
	// その他のアプリケーションロジック
	//-------------------------------------------------
	// ファイルを読出してグラフのプロット系列に加える
//	void Func_ReadFile_ToPlotLine(
//			int pltLnIdx,
//			std::string pltLnName,
//			std::string filePath,
//			std::shared_ptr<sodl::GraphObj> grph ,
//			app::VISUAL_IF& vif
//			);

	void ReadCsvHeader_CreatePlotLine(
			std::string csvHeader,
			std::string pltLnName,
			app::ST_PLOT_FILE& plotFile,
			std::shared_ptr<sodl::GraphObj> grph,
			appUi::Vif_PlotCnfg& uidat
			);

	void ReadCsvData_ToPlotLine(
			std::string pltRowStr,
			std::string pltLnName,
			app::ST_PLOT_FILE& plotFile,
			std::shared_ptr<sodl::GraphObj> grph
			);

	void Func_ReadFile_ToPlotLine(
			std::string pltLnName,
			std::shared_ptr<sodl::GraphObj> grph,
			app::ST_PLOT_FILE& plotFile,
			appUi::Vif_PlotCnfg& uidat
			);

	std::shared_ptr<sodl::GraphObj> Add2DPlot( std::string name );

	//-------------------------------------------------
	// その他のサブ関数
	//-------------------------------------------------
	// 実行ファイルのパスを取得
	std::string GetModulePath();

	// ファイルを開いて行ごとにstd::stringに詰める
	void ReadFile_IntoVctStr_PerLine(
		std::string path ,
		std::vector<std::string>& vctStr
	);
	// 文字列分割関数
	void SplitString(
		std::string str,
		std::string delim,
		std::vector<std::string>& vctStr
	);

};



//================================================================
//
//	<Summary>		アプリケーションのエントリポイント
//	<Description>
//================================================================
int main(int argc, char ** argv)
{
	std::vector<std::string> vct_plotDataPath;

	//------------------------------------------------------
	// コマンドライン引数の処理
	//	各引数をプロットデータファイルへのパスとみなす
	//------------------------------------------------------
	if (argc > 2)
	{
		for (int i = 1; i < argc; ++i)
		{
			vct_plotDataPath.push_back(argv[i]);
			std::cout << vct_plotDataPath[i - 1] << std::endl; // debug out
		}
	}

	// デバッグ処理
	if (0 == vct_plotDataPath.size())
	{
		// コマンドライン引数が指定されていない場合はハードコーディングで所定のファイルを指定する。
//		std::string exePath = app::GetModulePath();
//		vct_plotDataPath.push_back(exePath + "plotData\\plotData.csv");
//		vct_plotDataPath.push_back(exePath + "plotData\\plotData2.csv");
	}

	//------------------------------------------------------
	// SODLの初期化
	//------------------------------------------------------
	sodl::DrawingManager::initMngr( app::WINDOW_SIZE_X,app::WINDOW_SIZE_Y);
	sodl::drwMngr->depthBufferIsEnable = true;

	sodl::drwMngr->SetReshapeFunc(app::onReshape);
	sodl::drwMngr->SetKeyboardFunc(app::onKeyboard);
	sodl::drwMngr->SetKeyboardUpFunc(app::onKeyboardUp);
	sodl::drwMngr->SetKeyboardSpFunc(app::onKeyboardSp);
	sodl::drwMngr->SetKeyboardSpUpFunc(app::onKeyboardSpUp);
	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetMouseHover(app::onMouseHover);
	sodl::drwMngr->SetMouseWheelFunc(app::onMouseWheel);
	// drawの前後でImGuiのための処理を呼び出すように登録する
	sodl::drwMngr->Func_PreDraw = app::PreDraw_ImGui;
	sodl::drwMngr->Func_PostDraw = app::PostDraw_ImGui;

	//------------------------------------------------------
	// ビューポート1
	//------------------------------------------------------
	{
		auto vp1 = sodl::drwMngr->viewPorts[0];
		vp1->setVpSize(
			0,						// left
			0,						// bottom
			app::WINDOW_SIZE_X / 2,	// width
			app::WINDOW_SIZE_Y 		// height
		);
		auto cam1 = vp1->getCam();
		cam1->camPos = Eigen::Vector3f(400.f, -400.f, 400.f);
		cam1->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
		cam1->zoomRatio = 0.8;
		vp1->attachCam(cam1);
	}

	//-----------------------------------------------------
	// ワールド座標系原点から連鎖する座標系オブジェクトを定義
	//-----------------------------------------------------
	// ワールド座標原点を定義
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);

	//-----------------------------------------------------
	// 3Dグラフオブジェクトを生成する
	//-----------------------------------------------------
	auto graph3D = sodl::GraphObj3D::create("graph3D", Eigen::Vector3f(200,200,200), World_Origin);
	// コマンドライン引数で指定されたファイルを順に読みだしてグラフのプロット系列とする
	for( int i=0; i<vct_plotDataPath.size(); ++i )
	{
		// i番目のプロット系列を追加
		std::string pltName = "p" + std::to_string(i);
		graph3D->AddPlotLine( pltName );
		app::csvIf.pltFiles.	emplace( std::make_pair( pltName, app::ST_PLOT_FILE() ) );
		app::visualIf.pltLines.	emplace( std::make_pair( pltName,appUi::Vif_PlotCnfg() ) );

		// i番目のファイルをオープン
		app::csvIf.pltFiles[pltName].ifs.open( vct_plotDataPath[i] );
		app::csvIf.pltFiles[pltName].filePath = vct_plotDataPath[i];

		// i番目の系列にファイルから読み出したデータを流し込む
		app::Func_ReadFile_ToPlotLine(
			pltName,graph3D,
			app::csvIf.pltFiles[pltName],
			app::visualIf.pltLines[pltName]
		);
	}

	// 生成した描画対象オブジェクトツリーをデフォルトの描画空間に配置する
	sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);

	//------------------------------------------------------
	// UIライブラリ ImGuiの初期化
	//------------------------------------------------------
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	// ImGuiのコールバック関数をGLUTに登録させない。
	// 代わりにSODLのコールバックの中でImGuiのコールバックを呼び出す。
	//ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	// ImGuiに画面サイズを通知
	{
		auto& io = ImGui::GetIO();
		io.DisplaySize.x = sodl::drwMngr->viewPorts[0]->width;
		io.DisplaySize.y = sodl::drwMngr->viewPorts[0]->height;
	}

	//------------------------------------------------------
	// UIライブラリ ImGuiFileDialogの初期化
	//------------------------------------------------------
	ImGuiFileDialog::Instance()->SetExtentionInfos(".csv", ImVec4(1.0f, 1.0f, 0.0f, 0.9f));



	//////////////////////////////////////////////////////
	//
	// 周期処理の実施
	//

	int count = 0;
	while( 1 )
	{
		//-----------------------------------------------------
		// 周期実行アプリケーションロジック
		//-----------------------------------------------------
		// UIからの入力結果を描画内容に反映させる

		//-------------------------------------------------------------------------------------------------
		// C1 プロット系列の追加
		//-------------------------------------------------------------------------------------------------
		// 追加ファイル名称がUIによってセットされている場合
		if( 0 != app::visualIf.newFilePath.size() )
		{
			std::string pltName = app::visualIf.newPlotLineName;
			// プロット系列を追加
			graph3D->AddPlotLine(app::visualIf.newPlotLineName);
			app::csvIf.pltFiles.	emplace( std::make_pair( pltName, app::ST_PLOT_FILE() ) );
			app::visualIf.pltLines.	emplace( std::make_pair( pltName, appUi::Vif_PlotCnfg() ) );

			// ファイルをオープン
			app::csvIf.pltFiles[pltName].ifs.open( app::visualIf.newFilePath );
			app::csvIf.pltFiles[pltName].filePath = app::visualIf.newFilePath;

			// 系列にファイルから読み出したデータを流し込む
			app::Func_ReadFile_ToPlotLine(
				pltName,graph3D,
				app::csvIf.pltFiles[pltName],
				app::visualIf.pltLines[pltName]
			);

			// 追加ファイル名称をクリアする
			app::visualIf.newFilePath.clear();
			app::visualIf.newPlotLineName.clear();
		}

		//-------------------------------------------------------------------------------------------------
		// C2 2Dプロットの追加
		//-------------------------------------------------------------------------------------------------
		// プロット系列ループ
		for(auto	vifPltLineMapItm  = app::visualIf.pltLines.begin();
					vifPltLineMapItm != app::visualIf.pltLines.end();
				  ++vifPltLineMapItm)
		{
			auto& vifPltLine = vifPltLineMapItm->second;
			auto& atrSlct = vifPltLine.ST_atrSlctWindowCtl;

			// 新規2Dプロット名が要求されている場合
			if( "" != atrSlct.selectedAtrName )
			{
				std::string structredName = vifPltLine.pltLnName + "." + atrSlct.selectedAtrName;

				// 2Dプロットの実体を生成してアプリケーション共通の2Dプロット保持用リストに加える
				auto grph2D = app::Add2DPlot( structredName );

				// UIで選択された3Dプロットのアトリビュート列を
				// 生成した2Dプロットにプロットデータとして流し込む
				auto attributes = graph3D->GetAttributes_Shared( vifPltLine.pltLnName );
				auto atr = attributes[atrSlct.selectedAtrIdx];
				for( int i=0; i<atr->data.size(); ++i )
				{
					// X:データindex
					// Y:アトリビュート値
					// Z:固定値
					grph2D->AddData( Eigen::Vector3f( i, atr->data[i], 10) );
				}

				// 生成した2Dプロットに対応する操作用のUIを生成する
				appUi::Vif_PlotCnfg2D plt2DVif;
				plt2DVif.plt2dName = structredName;
				vifPltLine.grps2DIf.insert( std::make_pair( structredName, plt2DVif ) );

				atrSlct.selectedAtrName = "";
			}
		}

		//-------------------------------------------------------------------------------------------------
		// C3 プロット系列の表示設定をグラフオブジェクトに反映する
		//-------------------------------------------------------------------------------------------------
		// プロット系列ループ
		for(auto	vifPltLineMapItm  = app::visualIf.pltLines.begin();
					vifPltLineMapItm != app::visualIf.pltLines.end();
				  ++vifPltLineMapItm)
		{
			auto& vifPltLine = vifPltLineMapItm->second;
			graph3D->SetBarEnable				( vifPltLine.fg_cBox_barEnable, vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_PtColorIdx		(vifPltLine.idx_pltColorAtr,	vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_PtLnWdtIdx		(vifPltLine.idx_pltWdtAtr,		vifPltLine.pltLnName);

			graph3D->CnfgAtrDisp_BarIdx			(vifPltLine.idx_barAtr,			vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_BarColorIdx	(vifPltLine.idx_barColorAtr,	vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_BarWidthIdx	(vifPltLine.idx_barWidthAtr,	vifPltLine.pltLnName);

			if( vifPltLine.fg_cBox_updateCyclic )
			{
				// ファイル読出しを繰り返し実行する
				app::Func_ReadFile_ToPlotLine( vifPltLine.pltLnName , graph3D, app::csvIf.pltFiles[ vifPltLine.pltLnName ], vifPltLine );
			}

			//-----------------------------------------------------
			// 2Dプロットへの設定更新
			//-----------------------------------------------------
			for( auto	itr_plt2dIf  = vifPltLine.grps2DIf.begin();
						itr_plt2dIf != vifPltLine.grps2DIf.end();
					  ++itr_plt2dIf )
			{
				auto& plt2dIf =  itr_plt2dIf->second;

				// 対象の2Dグラフを取得・存在チェック
				auto itr_grph2D = app::graphs2D.find( plt2dIf.plt2dName );
				if( app::graphs2D.end() != itr_grph2D )
				{
					auto& grph2D = itr_grph2D->second;

					// グラフのプロットラインのデータを取得
					auto p_line = grph2D->GetCpyPltLn();
					const auto sz = p_line->size();

					// 引数で与えた範囲におけるy座標の最大/最小を取得する。
					auto func_GetYMax = [&p_line](
						const int idxMin,
						const int idxMax,
						float& yMin,
						float& yMax
						)
					{
						yMax = -999999;
						yMin = +999999;
						for( int ln_idx  = idxMin;
								 ln_idx <= idxMax;
							  ++ ln_idx )
						{
							const float y = (*p_line)[ln_idx].y();
							// 最大値保持
							if( y > yMax )
							{
								yMax = y;
							}
							// 最小値保持
							if( y < yMin )
							{
								yMin = y;
							}
						}
					};

					// サブカーソルによるスケーリング有効状態が前回値と異なる場合
					if( plt2dIf.fg_cBox_scalefitToSubCursols
					!=  plt2dIf.fg_cBox_scalefitToSubCursols_prv
					)
					{
						// サブカーソルによるスケーリング有効の場合
						if( plt2dIf.fg_cBox_scalefitToSubCursols )
						{
							// カーソルの大小関係を正規化
							const int lnIdx_min = MIN( app::visualIf.subCursol_idx[0], app::visualIf.subCursol_idx[1] );
							const int lnIdx_max = MAX( app::visualIf.subCursol_idx[0], app::visualIf.subCursol_idx[1] );

							// カーソルがプロットライン上にあることをチェック
							if(	( sz > lnIdx_min )
							&&	( sz > lnIdx_max )
							)
							{
								// カーソル上のプロット点を取得
								const auto p1 = (*grph2D->GetCpyPltLn())[ lnIdx_min ];
								const auto p2 = (*grph2D->GetCpyPltLn())[ lnIdx_max ];

								// カーソル間のy座標の最大/最小値を取得
								float yMax;
								float yMin;
								func_GetYMax( lnIdx_min, lnIdx_max, yMin, yMax );

								// 2Dグラフの描画範囲を再設定
								grph2D->drwRngMin.x() = lnIdx_min;
								grph2D->drwRngMax.x() = lnIdx_max;
								grph2D->drwRngMin.y() = yMin;
								grph2D->drwRngMax.y() = yMax;
							}
						}
						// サブカーソルによるスケーリング無効の場合
						else
						{
							// 2Dグラフの描画範囲を再設定
							grph2D->drwRngMin.x() = grph2D->datRngMin.x();
							grph2D->drwRngMax.x() = grph2D->datRngMax.x();
							grph2D->drwRngMin.y() = grph2D->datRngMin.y();
							grph2D->drwRngMax.y() = grph2D->datRngMax.y();
						}
					}
					// サブカーソルによるスケーリング有効状態 前回値更新
					plt2dIf.fg_cBox_scalefitToSubCursols_prv = plt2dIf.fg_cBox_scalefitToSubCursols;
				}
			}


			//-----------------------------------------------------
			// プロット系列の削除
			//-----------------------------------------------------
			if( vifPltLine.fg_delete )
			{
				// 描画オブジェクト削除
				graph3D->DeletePlotLine(vifPltLine.pltLnName);
				// visualIfの削除
				app::visualIf.pltLines.erase(vifPltLine.pltLnName);

				// for文の繰り返し処理で空のイテレータを++しないようにループを抜ける
				// 処理できなかった分のループは、諦めて次の表示周期に処理する。
				break;
			}
		}

		//-------------------------------------------------------------------------------------------------
		// C4 カーソル関連
		//-------------------------------------------------------------------------------------------------
		if( auto cursoledLine = graph3D->GetCpyPltLn( app::visualIf.cursol_pltLnName ) )
		{
			app::visualIf.cursol_idxMax = cursoledLine->size();
		}
		else
		{
			app::visualIf.cursol_idxMax = 0;
		}
		graph3D->PutCursolToLine( app::visualIf.cursol_pltLnName,		sodl::GraphObj::CURSOL_SEL::MAIN );
		graph3D->SetCursolViible( app::visualIf.cursol_enable,			sodl::GraphObj::CURSOL_SEL::MAIN );
		graph3D->UpdtCursol		( app::visualIf.cursol_idx,				sodl::GraphObj::CURSOL_SEL::MAIN );

		bool subCursolEnable_1 = ( app::visualIf.cursol_enable && app::visualIf.subCursol_enable[0] );
		graph3D->PutCursolToLine( app::visualIf.cursol_pltLnName,		sodl::GraphObj::CURSOL_SEL::SUB1 );
		graph3D->SetCursolViible( subCursolEnable_1,					sodl::GraphObj::CURSOL_SEL::SUB1 );
		graph3D->UpdtCursol		( app::visualIf.subCursol_idx[0],		sodl::GraphObj::CURSOL_SEL::SUB1 );

		bool subCursolEnable_2 = ( app::visualIf.cursol_enable && app::visualIf.subCursol_enable[1] );
		graph3D->SetCursolViible( subCursolEnable_2,					sodl::GraphObj::CURSOL_SEL::SUB2 );
		graph3D->PutCursolToLine( app::visualIf.cursol_pltLnName,		sodl::GraphObj::CURSOL_SEL::SUB2 );
		graph3D->UpdtCursol		( app::visualIf.subCursol_idx[1],		sodl::GraphObj::CURSOL_SEL::SUB2 );

		// 2Dプロットのカーソルを描画する
		for( auto grph2Ditr = app::graphs2D.begin();
				  grph2Ditr != app::graphs2D.end();
				++grph2Ditr )
		{
			auto grph2D = grph2Ditr->second;
			grph2D->SetCursolViible	( app::visualIf.cursol_enable,			sodl::GraphObj::CURSOL_SEL::MAIN );
			grph2D->UpdtCursol		( app::visualIf.cursol_idx,				sodl::GraphObj::CURSOL_SEL::MAIN );

			grph2D->SetCursolViible	( subCursolEnable_1,					sodl::GraphObj::CURSOL_SEL::SUB1 );
			grph2D->UpdtCursol		( app::visualIf.subCursol_idx[0],		sodl::GraphObj::CURSOL_SEL::SUB1 );

			grph2D->SetCursolViible	( subCursolEnable_2,					sodl::GraphObj::CURSOL_SEL::SUB2 );
			grph2D->UpdtCursol		( app::visualIf.subCursol_idx[1],		sodl::GraphObj::CURSOL_SEL::SUB2 );
		}

		//-----------------------------------------------------
		// 描画マネージャから描画更新を実行する
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

		++count;
		Sleep(30);
	}

	//------------------------------------------------------
	// UIライブラリ ImGuiの終了処理
	//------------------------------------------------------
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

	return 0;
}


namespace app {

	void onReshape(int u, int v)
	{
		ImGui_ImplGLUT_ReshapeFunc(u, v);
	}


	//================================================================
	//
	//	<Summary>		キー操作時のコールバック
	//	<Description>
	//================================================================
	void onKeyboard(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_KeyboardFunc( key, u, v );

		const float		KEY_MOT_UNIT	=	5.f;
		const float		CNV_DEG2RAD		=	(M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' は ESC の ASCII コード
			exit(0);
			break;

		default:
			break;
		}
	}

	void onKeyboardUp(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_KeyboardUpFunc( key, u, v);
	}

	void onKeyboardSp(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_SpecialFunc( key, u, v);
	}

	void onKeyboardSpUp(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_SpecialUpFunc( key, u, v);
	}


	//================================================================
	//
	//	<Summary>		GLウィンドウ上でマウスボタン操作時のコールバック
	//	<Description>
	//================================================================
	void onMouseBtn(int button, int state, int x, int y)
	{
		ImGui_ImplGLUT_MouseFunc( button, state, x, y );
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

		// ImGuiのコールバックをコールする
		ImGui_ImplGLUT_MotionFunc(u,v);
	}

	//================================================================
	//
	//	<Summary>		GLウィンドウ上でマウスホバー時のコールバック
	//	<Description>
	//================================================================
	void onMouseHover(int u, int v)
	{
		// ImGuiのコールバックをコールする
		ImGui_ImplGLUT_MotionFunc(u,v);
	}

	void onMouseWheel(int wheelNum, int dir, int u, int v)
	{
		ImGui_ImplGLUT_MouseWheelFunc( wheelNum, dir, u, v );
	}


	/** ***************************************************************
	 *
	 * @brief	iniファイルからデフォルトプロット列のヘッダ名取得
	 * 			GetDefaultPlotHeaderNameFromIni()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void GetDefaultPlotHeaderNameFromIni(
		std::string 				iniFilePath,
		std::vector<std::string>&	headers
		)
	{
		char buf[100];
		if( 6 <= headers.size() )
		{
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "X", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[0] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "Y", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[1] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "Z", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[2] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "I", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[3] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "J", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[4] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "K", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[5] = buf;
		}
	}


	/** ***************************************************************
	 *
	 * @brief	csvのヘッダ行を受けて、グラフプロットの系列を構築する
	 * 			Func_ReadFile_ToPlotLine()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void ReadCsvHeader_CreatePlotLine(
			std::string csvHeader,
			std::string pltLnName,
			app::ST_PLOT_FILE& plotFile,
			std::shared_ptr<sodl::GraphObj> grph,
			appUi::Vif_PlotCnfg& uidat
			)
	{
		uidat.pltLnName = pltLnName;


		// ヘッダ行データを','区切りのtokenに分割する
		std::vector<std::string> tokens;
		app::SplitString(csvHeader, ",", tokens);

		// 1行分のトークンをループ処理する
		std::vector<float> dataOfLine;
		for (int col = 0; col < tokens.size(); ++col)
		{
			// グラフのアトリビュート配列にアトリビュート列を追加する。
			grph->AddAtr(tokens[col], pltLnName );

			// アトリビュート名をUI表示用に保持する
			uidat.vct_labelsOfAttribetes.emplace_back( tokens[col] );
		}

		//-------------------------------------------------------------------
		// iniファイルからデフォルトプロット候補の列名称を取得する
		//-------------------------------------------------------------------
		std::vector< std::string > vct_defaultPltColNms(6);
		GetDefaultPlotHeaderNameFromIni( ".\\plot.ini", vct_defaultPltColNms );

		//-------------------------------------------------------------------
		// 1.2.1 列名称が特定の並びに一致するとき、特別処理を実施するためのフラグを立てる
		//-------------------------------------------------------------------
		// iniファイル設定のデフォルトプロット列XYZ相当が存在するかチェックする
		for (int col = 0; col < tokens.size(); ++col)
		{
			if( std::string::npos != tokens[col].find( vct_defaultPltColNms[0] ) )
			{
				plotFile.cX = col;
			}
			if( std::string::npos != tokens[col].find( vct_defaultPltColNms[1] ) )
			{
				plotFile.cY = col;
			}
			if( std::string::npos != tokens[col].find( vct_defaultPltColNms[2] ) )
			{
				plotFile.cZ = col;
			}
		}
		if( ( -1 != plotFile.cX )
		&&	( -1 != plotFile.cY )
		&&	( -1 != plotFile.cZ )
		)
		{
			// XYZ列存在フラグON
			plotFile.fg_xyz = true;

			// iniファイル設定のデフォルトプロット列IJK相当が存在するかチェックする
			for (int col = 0; col < tokens.size(); ++col)
			{
				if( std::string::npos != tokens[col].find( vct_defaultPltColNms[3] ) )
				{
					plotFile.cI = col;
				}
				if( std::string::npos != tokens[col].find( vct_defaultPltColNms[4] ) )
				{
					plotFile.cJ = col;
				}
				if( std::string::npos != tokens[col].find( vct_defaultPltColNms[5] ) )
				{
					plotFile.cK = col;
				}
			}
			if( ( -1 != plotFile.cI )
			&&	( -1 != plotFile.cJ )
			&&	( -1 != plotFile.cK )
			)
			{
				// XYZIJK列存在フラグON
				plotFile.fg_xyzijk = true;
				uidat.fg_cBox_barEnable = true;
			}
		}
		// デフォルトプロット列XYZ相当が存在するかチェックする
		else
		{
			// 列の先頭が x, y, z である場合
			if( ( tokens.size() >= 3 )
			&&	(	("x" == tokens[0] )
				||  ("X" == tokens[0] )
				)
			&&  (	("y" == tokens[1] )
				||  ("Y" == tokens[1] )
				)
			&&  (	("z" == tokens[2] )
				||  ("Z" == tokens[2] )
				)
			)
			{
				// 先頭列から順に x, y, z の名称がcsvに記載されているフラグ ON
				plotFile.fg_xyz = true;
				plotFile.cX = 0;
				plotFile.cY = 1;
				plotFile.cZ = 2;

				// 後続の列が i, j, k である場合、これを方向ベクトルとして扱う
				if( ( tokens.size() >= 6 )
				&&	(	("i" == tokens[3] )
					||  ("I" == tokens[3] )
					)
				&&  (	("j" == tokens[4] )
					||  ("J" == tokens[4] )
					)
				&&  (	("k" == tokens[5] )
					||  ("K" == tokens[5] )
					)
				)
				{
					// x, y, z 後続して i, j, k の名称がcsvに記載されているフラグ ON
					plotFile.fg_xyzijk = true;
					uidat.fg_cBox_barEnable = true;
					plotFile.cI = 3;
					plotFile.cJ = 4;
					plotFile.cK = 5;
				}
			}
		}
	}



	/** ***************************************************************
	 *
	 * @brief	csvの行ベクトルを受けて、グラフプロットの行に加える
	 * 			Func_ReadFile_ToPlotLine()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void ReadCsvData_ToPlotLine(
		std::string pltRowStr,
		std::string pltLnName,
		app::ST_PLOT_FILE& plotFile,
		std::shared_ptr<sodl::GraphObj> grph
		)
	{
		// 1行データを','区切りのtokenに分割する
		std::vector<std::string> tokens;
		app::SplitString(pltRowStr, ",", tokens);

		//-------------------------------------------------------------------
		// 1.2 1行目はヘッダなので特別処理を実施する
		//-------------------------------------------------------------------
		if( plotFile.seekRow == 0 )
		{

		}
		//-------------------------------------------------------------------
		// 1.3 ヘッダ行以降のデータを処理する
		//-------------------------------------------------------------------
		else
		{
			// 1行分のトークンを処理する
			std::vector<float> dataOfLine;
			for (int col = 0; col < tokens.size(); ++col)
			{
				float fDat = 0;

				// トークンを数値データに変換する
				try
				{
					fDat = std::stof(tokens[col]);
				}
				// 数値変換に失敗した場合は初期値の0が採用される
				catch( std::exception& e )
				{
				}
				// トークンを変換して得られた数値データを配列に詰める
				dataOfLine.push_back( fDat );

				// アトリビュートデータをグラフオブジェクトにセットする
				grph->AddAtrData( col, fDat , pltLnName);
			}

			//-----------------------------------------------------
			// 1.4 読み出しだデータを系列1のプロット対象座標としてセットする
			//-----------------------------------------------------
			// 先頭列から順に x, y, z の名称がcsvに記載されている場合
			if( plotFile.fg_xyz )
			{
				// 0～2列目データをプロット座標 x, y, z としてグラフオブジェクトにセットする
				grph->AddData( Eigen::Vector3f(dataOfLine[plotFile.cX], dataOfLine[plotFile.cY], dataOfLine[plotFile.cZ]), pltLnName);

				// x, y, z 後続して i, j, k の名称がcsvに記載されている場合
				if( plotFile.fg_xyzijk )
				{
					// 3～5列目を方向ベクトルとしてグラフオブジェクトにセットする
					grph->AddPtVct( Eigen::Vector3f(dataOfLine[plotFile.cI], dataOfLine[plotFile.cJ], dataOfLine[plotFile.cK]), pltLnName);
				}

			}
			// 先頭列から順に x, y, z の名称がcsvに記載されていない場合
			else
			{
				// 0列目データをy座標としてプロット(n, y, 0)をグラフオブジェクトにセットする
				grph->AddData( Eigen::Vector3f( plotFile.seekRow, dataOfLine[0], 0 ), pltLnName);
			}
		}
	}



	/** ***************************************************************
	 *
	 * @brief	ファイルを読出してグラフのプロット系列に加える
	 * 			Func_ReadFile_ToPlotLine()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void Func_ReadFile_ToPlotLine(
			std::string pltLnName,
			std::shared_ptr<sodl::GraphObj> grph,
			app::ST_PLOT_FILE& plotFile,
			appUi::Vif_PlotCnfg& uidat
			)
	{

		//-----------------------------------------------------
		// 1. プロットデータをcsvファイルから読み出して系列1としてグラフにセット
		//-----------------------------------------------------
		// EOFビット等をクリアしておく
		plotFile.ifs.clear();
		if( 0 != plotFile.seekPos  )
		{
			plotFile.ifs.seekg( plotFile.seekPos );
		}
		std::string line;
		while( std::getline( plotFile.ifs, line ) )
		{
			if( 0 == plotFile.seekRow )
			{
				ReadCsvHeader_CreatePlotLine(
						line,
						pltLnName,
						plotFile,
						grph,
						uidat
						);
			}
			else
			{
				ReadCsvData_ToPlotLine(
						line,
						pltLnName,
						plotFile,
						grph
						);
			}

			++plotFile.seekRow;									// 読出し行数のカウント
			plotFile.seekPos = plotFile.ifs.tellg();			// 次の読出し位置をメモしておく
		}
	};


	/** ***************************************************************
	 *
	 * @brief	2Dプロットを追加する
	 * 			Add2DPlot()
	 * <pre>
	 * 2Dプロットの実体を生成し、
	 * アプリケーションスコープの2Dプロット保持用構造体に保持する。
	 * 明示的に削除しない限り、アプリケーションの終了まで保持される。
	 * </pre>
	 ******************************************************************/
	std::shared_ptr<sodl::GraphObj> Add2DPlot( std::string name )
	{
		//----------------------------------------------------
		// 描画空間、ビューポート、カメラの追加と設定
		//----------------------------------------------------

		// グラフ用に描画空間を追加
		int idx_ds = sodl::drwMngr->drawingSpace->size();
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		// ビューポート作成
		const int hgt = 200;
		auto vpGrph = sodl::drwMngr->addViewPort("vp" + name);
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X / 2,		// left
			hgt * (idx_ds-1),			// bottom
			app::WINDOW_SIZE_X / 2,		// width
			hgt							// height
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
		auto grph = sodl::GraphObj::create(
			name,									// オブジェクト名称 = グラフタイトル
			camGrph									// 関連付けるカメラ(カメラ投影行列などの設定が済んでいること)
		);
		// ラベルの設定
		grph->title->text = name;
		grph->xAxisLabel->text = "plot data idx";
		grph->yAxisLabel->text = name;
		// 描画空間[2]にグラフを追加
		sodl::drwMngr->AddObjTree_ToDrwSpace(grph, idx_ds);

		// 作成したグラフをアプリケーションの2Dグラフ配列に保持する
		app::graphs2D.insert( std::make_pair( name, grph ) );

		return grph;
	}



	/** ***************************************************************
	 *
	 * @brief	ImGuiのメイン描画処理
	 * 			my_display_code()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void ImGui_DrawAndOperate()
	{
		visualIf.drawVif();
	}



	/** ***************************************************************
	 *
	 * @brief	SODL描画前 ImGui処理
	 * 			PreDraw_ImGui()
	 * <pre>
	 * UIライブラリImGuiの描画前処理とメイン描画処理を実施する
	 * </pre>
	 ******************************************************************/
	void PreDraw_ImGui()
	{
	    // Start the Dear ImGui frame
	    ImGui_ImplOpenGL2_NewFrame();
	    ImGui_ImplGLUT_NewFrame();

	    ImGui_DrawAndOperate();

	    // Rendering
	    ImGui::Render();
	    ImGuiIO& io = ImGui::GetIO();

	}

	/** ***************************************************************
	 *
	 * @brief	SODL描画後 ImGui処理
	 * 			PostDraw_ImGui()
	 * <pre>
	 * UIライブラリImGuiの描画後処理を実施する
	 * </pre>
	 ******************************************************************/
	void PostDraw_ImGui()
	{
	    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
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

	/** **************************************************************
	*
	* @brief			ファイルを開いて行ごとにstd::stringに詰める
	* <pre></pre>
	******************************************************************/
	void ReadFile_IntoVctStr_PerLine(	// ファイルを開いて行ごとにstd::stringに詰める
		std::string path,
		std::vector<std::string>& vctStr
	)
	{
		std::ifstream ifs(path);
		if (ifs.is_open())
		{
			std::string line;
			while ( std::getline(ifs, line) )
			{
				vctStr.push_back(line);
			}
		}
	}

	/** **************************************************************
	*
	* @brief			文字列分割処理
	* <pre>
	* string をデリミタで区切って vector に詰める
	* </pre>
	******************************************************************/
	void SplitString(
		std::string str,
		std::string delim,
		std::vector<std::string>& vctStr
	)
	{
		vctStr.clear();
		while (1)
		{
			// 引数文字列に対してデリミタを検索
			auto pos = str.find(delim);
			// デリミタがみつからなければ現在の文字列を出力配列に追加してループを抜ける。
			if (std::string::npos == pos)
			{
				vctStr.push_back(str);
				break;
			}

			// デリミタまでを切り出して出力配列に追加
			vctStr.push_back( str.substr(0, pos) );

			// デリミタ以降の文字列で引数文字列を更新
			str = str.substr(pos + 1, std::string::npos);

		}
	}

};


