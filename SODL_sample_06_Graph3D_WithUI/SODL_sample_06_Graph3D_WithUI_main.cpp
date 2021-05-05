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

	//-------------------------------------------------
	// UIとアプリケーションロジックを媒介するI/F変数
	//-------------------------------------------------
	int mouse_x = 0;
	int mouse_y = 0;

	typedef struct VISUAL_IF_tp{
		std::unordered_map< std::string, appUi::Vif_PlotCnfg > pltLines;
		std::string newFilePath;
		std::string newPlotLineName;
	}VISUAL_IF;
	VISUAL_IF visualIf;

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
	void Func_ReadFile_ToPlotLine(
			int pltLnIdx,
			std::string pltLnName,
			std::string filePath,
			std::shared_ptr<sodl::GraphObj> grph ,
			app::VISUAL_IF& vif
			);


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
	// コマンドライン引数で指定されたファイルを順に読みだしてグラフのプロット系列とする
	for( int i=0; i<vct_plotDataPath.size(); ++i )
	{
		// i番目のプロット系列を追加
		graph3D->AddPlotLine( "initial plot" + std::to_string(i) );

		// i番目の系列にファイルから読み出したデータを流し込む
		app::Func_ReadFile_ToPlotLine( i, "initial plot" + std::to_string(i), vct_plotDataPath[i], graph3D, app::visualIf );
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
	//while( count < 100 )
	while( 1 )
	{
		//-----------------------------------------------------
		// 周期実行アプリケーションロジック
		//-----------------------------------------------------
		// UIからの入力結果を描画内容に反映させる

		// プロット系列の追加
		// 追加ファイル名称がUIによってセットされている場合
		if( 0 != app::visualIf.newFilePath.size() )
		{
			int i = graph3D->GetNumPlotLines();
			graph3D->AddPlotLine(app::visualIf.newPlotLineName);

			// i番目の系列にファイルから読み出したデータを流し込む
			app::Func_ReadFile_ToPlotLine( i, app::visualIf.newPlotLineName, app::visualIf.newFilePath,	graph3D, app::visualIf );

			// 追加ファイル名称をクリアする
			app::visualIf.newFilePath.clear();
			app::visualIf.newPlotLineName.clear();
		}

		// プロット系列の表示設定をグラフオブジェクトに反映する
		for(auto	vifPltLineMapItm  = app::visualIf.pltLines.begin();
					vifPltLineMapItm != app::visualIf.pltLines.end();
				  ++vifPltLineMapItm)
		{
			auto& vifPltLine = vifPltLineMapItm->second;
			graph3D->CnfgAtrDisp_PtColorIdx		(vifPltLine.idx_pltColorAtr,	vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_PtLnWdtIdx		(vifPltLine.idx_pltWdtAtr,		vifPltLine.pltLnName);

			graph3D->CnfgAtrDisp_BarIdx			(vifPltLine.idx_barAtr,			vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_BarColorIdx	(vifPltLine.idx_barColorAtr,	vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_BarWidthIdx	(vifPltLine.idx_barWidthAtr,	vifPltLine.pltLnName);

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
				// 処理できなかった文のループは、諦めて次の表示周期に処理する。
				break;
			}
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
	 * @brief	ファイルを読出してグラフのプロット系列に加える
	 * 			Func_ReadFile_ToPlotLine()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void Func_ReadFile_ToPlotLine(
			int pltLnIdx,
			std::string pltLnName,
			std::string filePath,
			std::shared_ptr<sodl::GraphObj> grph,
			app::VISUAL_IF& vif
			)
	{
		appUi::Vif_PlotCnfg uidat;
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
						grph->AddAtr(pltLnName);

						// アトリビュート名をUI表示用に保持する
						uidat.vct_labelsOfAttribetes.push_back( tokens[col] );
					}

					// アトリビュートデータをグラフオブジェクトにセットする
					grph->AddAtrData( col - atrTopCol, fDat , pltLnName);
				}
			}

			//-----------------------------------------------------
			// 1.2 読み出しだデータを系列1のプロット対象座標としてセットする
			//-----------------------------------------------------
			if( dataOfLine.size() > 6 )
			{
				// 0～2列目データをプロット座標としてグラフオブジェクトにセットする
				grph->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]), pltLnName);
				// 3～5列目を方向ベクトルとしてグラフオブジェクトにセットする
				grph->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]), pltLnName);
			}
		}

		vif.pltLines.insert( std::make_pair( pltLnName, uidat ) );
	};



	/** ***************************************************************
	 *
	 * @brief	ImGuiのメイン描画処理
	 * 			my_display_code()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void ImGui_DrawAndOperate()
	{
		static int openFileCtr = 0;

		// アトリビュートをプロットにどのように反映するかを設定する
		{
			const char KEYSTR_DIALOG_FILE_OPEN[] = "KeyStr_Dialog_FileOpen";

			// ImGuiウィンドウ開始
			ImGui::Begin("Plot Configuration");

			//------------------------------------------------------
			// プロット系列数分の設定項目を表示する
			//------------------------------------------------------
			for (auto vifMapItm = visualIf.pltLines.begin(); vifMapItm != visualIf.pltLines.end(); ++vifMapItm)
			{
				auto& vifPltLn = vifMapItm->second;
				vifPltLn.Updt();
			}

			//------------------------------------------------------
			// プロット系列を追加するためのダイアログ表示ボタン
			//------------------------------------------------------
			if (ImGui::Button("Open CSV File, and Add Plot") )
			{
				const char* filters = ".csv,.*";
				ImGuiFileDialog::Instance()->OpenDialog(
					KEYSTR_DIALOG_FILE_OPEN,							// key dialog
					" Choose a File", 									// title
					filters, 											// filters
					".", 												// path
					"" 													// defaut file name
				);
			}

			//------------------------------------------------------
			// プロット系列を追加するためのダイアログ処理
			//------------------------------------------------------
			ImVec2 minSize = ImVec2(0, 0);
			ImVec2 maxSize = ImVec2(FLT_MAX, FLT_MAX);
			// ダイアログを表示する( Openされているかは内部で自動判断 )
			if ( ImGuiFileDialog::Instance()->Display( KEYSTR_DIALOG_FILE_OPEN ) )
			{
				// ファイルが選択された場合
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					// 選択されたファイルパスをI/F領域にセットする
					app::visualIf.newFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
					app::visualIf.newPlotLineName = "file plot " + std::to_string(++openFileCtr);
				}
				ImGuiFileDialog::Instance()->Close();
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


