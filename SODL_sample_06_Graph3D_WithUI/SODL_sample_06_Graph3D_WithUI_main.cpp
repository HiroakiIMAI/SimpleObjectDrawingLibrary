#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glut.h"
#include "ImGui/backends/imgui_impl_opengl2.h"

#include <math.h>
#include <iostream>
#include <fstream>

#include <Windows.h>

namespace sodl = SmplObjDrwLib;

namespace app {
	// グローバル変数
	const int	WINDOW_SIZE_X = 640*2;
	const int	WINDOW_SIZE_Y = 480*2;

	// UIとアプリケーションロジックを媒介するI/F変数
	int mouse_x = 0;
	int mouse_y = 0;
	bool Flg_UiWindowShow = true;
	class Vif_PlotCnfg
	{
	public:
		std::string pltLnName;

		// チェックボックス状態保持用static変数宣言
		bool fg_cBox_plotColorByAtr	= false;
		bool fg_cBox_plotWidthByAtr	= false;
		bool fg_cBox_barEnable		= false;
		bool fg_cBox_barLengAtr		= false;
		bool fg_cBox_barColorByAtr	= false;
		bool fg_cBox_barWidthByAtr	= false;

		int		idx_pltWdtAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_pltColorAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;

		bool	fg_BarEnable		= false;
		int		idx_barAtr			= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_barColorAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_barWidthAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;

		std::vector<std::string> vct_labelsOfAttribetes;

	public:

		auto Func_AtrCnfg( std::string label, bool& fgEnable, std::vector<std::string>& atrs, int& selectedIdx  )
		{
			// チェックボックス表示
			ImGui::Checkbox( (label + " by attribute.").c_str(), &fgEnable);
			// チェックONの場合、コンボボックスを表示する
			if( fgEnable )
			{
				ImGui::SameLine();
				// コンボボックス表示用の横幅を表示オブジェクト一般に設定
				ImGui::PushItemWidth(50); // [pix]

				// コンボボックス表示内容を作成
				std::vector< const char* > pList;
				// アトリビュート数分ループ
				for(int i = 0; i<atrs.size(); ++i)
				{
					pList.push_back( atrs[i].c_str() );
				}

				// コンボボックス表示
				ImGui::Combo( label.c_str(), &selectedIdx, (char**)&pList[0], pList.size(), 10 );

				// コンボボックス表示用の横幅の設定を表示オブジェクト一般に対して解除する
				ImGui::PopItemWidth();
			}
			// チェックボックスOFFの場合
			else
			{
				// indexに無効値を設定する
				selectedIdx = sodl::PointsWithAttributes::ATRIDX_NONE;
			}
		}


		void Updt()
		{
			// 系列名で折り畳み項目を作成
			if ( ImGui::CollapsingHeader( this->pltLnName.c_str() ) )
			{
				ImGui::Indent();

				// プロット色
				Func_AtrCnfg(pltLnName + " color ",	fg_cBox_plotColorByAtr,	vct_labelsOfAttribetes, idx_pltColorAtr	);
				// プロット幅
				Func_AtrCnfg(pltLnName + " width ",	fg_cBox_plotWidthByAtr,	vct_labelsOfAttribetes, idx_pltWdtAtr	);

				// バー表示有効チェックボックス
				ImGui::Checkbox( (pltLnName + " Enable bar for each plot.").c_str(), &fg_cBox_barEnable);
				// バー表示有効の場合、バー表示の設定項目を描画する
				if( fg_cBox_barEnable )
				{
					ImGui::Indent();

					// バー長さ
					Func_AtrCnfg(pltLnName + " bar length  ",	fg_cBox_barLengAtr,		vct_labelsOfAttribetes, idx_barAtr		);
					// バー色
					Func_AtrCnfg(pltLnName + " bar color  ",	fg_cBox_barColorByAtr,	vct_labelsOfAttribetes, idx_barColorAtr	);
					// バー幅
					Func_AtrCnfg(pltLnName + " bar width ",		fg_cBox_barWidthByAtr,	vct_labelsOfAttribetes, idx_barWidthAtr	);

					ImGui::Unindent();
				}

				ImGui::Unindent();
			}
		}
	};

	typedef struct VISUAL_IF_tp{
		std::vector< Vif_PlotCnfg > pltLines;
	}VISUAL_IF;
	VISUAL_IF visualIf;

	// Windowインタラクション関連コールバック関数
	void onReshape(int u, int v);
	void onKeyboard(unsigned char key, int u, int v);
	void onKeyboardUp(unsigned char key, int u, int v);
	void onKeyboardSp(unsigned char key, int u, int v);
	void onKeyboardSpUp(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);
	void onMouseWheel(int wheelNum, int dir, int u, int v);

	// UIライブラリImGuiを利用するアプリケーションコード(SODLのコールバックに登録する)
	void PreDraw_ImGui();
	void PostDraw_ImGui();

	// その他のサブ関数
	std::string GetModulePath();							// 実行ファイルのパスを取得

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
		std::string exePath = app::GetModulePath();
		vct_plotDataPath.push_back(exePath + "plotData\\plotData.csv");
		vct_plotDataPath.push_back(exePath + "plotData\\plotData2.csv");
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
			app::WINDOW_SIZE_Y		// height
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

	auto Crd1 = sodl::CoordChainObj::create("Crd1", World_Origin);
	Crd1->CrdTrs.translation() = Eigen::Vector3f(10.f, 10.f, 0.f);

	//-----------------------------------------------------
	// 3Dグラフオブジェクトを生成する
	//-----------------------------------------------------
	auto graph3D = sodl::GraphObj3D::create("graph3D", Eigen::Vector3f(200,200,200), Crd1);

	auto Func_ReadFile_ToPlotLine = [](
			int pltLnIdx,
			std::string pltLnName,
			std::string filePath,
			std::shared_ptr<sodl::GraphObj> grph ,
			app::VISUAL_IF& vif
			)
	{
		app::Vif_PlotCnfg uidat;
		uidat.pltLnName = pltLnName;

		//-----------------------------------------------------
		// 1. プロットデータをcsvファイルから読み出して系列1としてグラフにセット
		//-----------------------------------------------------
		// 1.1 プロットデータをcsvファイルから読み出す
		//-----------------------------------------------------
		// ファイルを開いて行ごとにstringに詰める
		std::vector<std::string> plotDatStr;
		app::ReadFile_IntoVctStr_PerLine( filePath, plotDatStr);

		for (int row = 0; row < plotDatStr.size(); ++row) // 1行目はヘッダなので読み飛ばす
		{
			auto str = plotDatStr[row];
			// 1行データを','区切りのtokenに分割する
			std::vector<std::string> tokens;
			app::SplitString(str, ",", tokens);

			// 1行分のトークンを処理する
			std::vector<float> dataOfLine;
			for (int col = 0; col < tokens.size(); ++col)
			{
				float fDat = 0;

				// 1行目はヘッダなので飛ばす
				if( row != 0 )
				{
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
				}

				// 6列目以降をアトリビュートとしてグラフオブジェクトにセットする
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// 最初の行だけの処理
					if( 0 == row )
					{
						// アトリビュート列を追加する
						grph->AddAtr(pltLnIdx);

						// アトリビュート名をUI表示用に保持する
						uidat.vct_labelsOfAttribetes.push_back( tokens[col] );
					}

					// アトリビュートデータをグラフオブジェクトにセットする
					grph->AddAtrData( col - atrTopCol, fDat , pltLnIdx);
				}
			}

			//-----------------------------------------------------
			// 1.2 読み出しだデータを系列1のプロット対象座標としてセットする
			//-----------------------------------------------------
			if( dataOfLine.size() > 6 )
			{
				// 0～2列目データをプロット座標としてグラフオブジェクトにセットする
				grph->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]), pltLnIdx );
				// 3～5列目を方向ベクトルとしてグラフオブジェクトにセットする
				grph->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]), pltLnIdx );
			}
		}

		vif.pltLines.push_back( uidat );
	};

	// コマンドライン引数で指定されたファイルを順に読みだしてグラフのプロット系列とする
	for( int i=0; i<vct_plotDataPath.size(); ++i )
	{
		if( i!=0 )
		{
			// プロット系列を追加
			graph3D->AddPlotLine();
		}

		// i 番目の系列にファイルから読み出したデータを流し込む
		Func_ReadFile_ToPlotLine( i,"plot"+std::to_string(i) , vct_plotDataPath[i],	graph3D, app::visualIf );
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


	//////////////////////////////////////////////////////
	//
	// 周期処理の実施
	//

	int count = 0;
	//while( count < 100 )
	while( 1 )
	{
		//-----------------------------------------------------
		// 周期実行 アプリケーションロジック
		//-----------------------------------------------------
		for( int i=0; i < app::visualIf.pltLines.size(); ++i )
		{
			graph3D->CnfgAtrDisp_PtColorIdx		( app::visualIf.pltLines[i].idx_pltColorAtr,	i);
			graph3D->CnfgAtrDisp_PtLnWdtIdx		( app::visualIf.pltLines[i].idx_pltWdtAtr,		i);

			graph3D->CnfgAtrDisp_BarIdx			( app::visualIf.pltLines[i].idx_barAtr,			i);
			graph3D->CnfgAtrDisp_BarColorIdx	( app::visualIf.pltLines[i].idx_barColorAtr,	i);
			graph3D->CnfgAtrDisp_BarWidthIdx	( app::visualIf.pltLines[i].idx_barWidthAtr,	i);
		}

		//-----------------------------------------------------
		// 描画マネージャから描画更新を実行する
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

		++count;
		Sleep(10);
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
	 * @brief	ImGuiのメイン描画処理
	 * 			my_display_code()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void my_display_code()
	{
		// アトリビュートをプロットにどのように反映するかを設定する
		{
			// ImGuiウィンドウ開始
			ImGui::Begin("Plot Configuration");

			// プロット系列数分ループ
			for (int pltLnIdx = 0; pltLnIdx < visualIf.pltLines.size(); ++pltLnIdx)
			{
				auto& vmifPltLn = visualIf.pltLines[pltLnIdx];
				vmifPltLn.Updt();
			}

			// ImGuiウィンドウ終了
			ImGui::End();
		}
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

	    my_display_code();

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


