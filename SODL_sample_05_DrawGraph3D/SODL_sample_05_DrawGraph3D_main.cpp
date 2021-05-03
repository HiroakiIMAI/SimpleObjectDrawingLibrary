#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include <math.h>
#include <iostream>
#include <fstream>

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
	void keyFunc(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);

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
	sodl::DrawingManager::initMngr( &argc, argv, app::WINDOW_SIZE_X,app::WINDOW_SIZE_Y);
	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetPassiveMotionFunc(app::onMouseHover);
	sodl::drwMngr->SetKeyboardFunc(app::keyFunc);

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
	{
		//-----------------------------------------------------
		// 1. プロットデータをcsvファイルから読み出して系列1としてグラフにセット
		//-----------------------------------------------------
		// 1.1 プロットデータをcsvファイルから読み出す
		//-----------------------------------------------------
		// ファイルを開いて行ごとにstringに詰める
		std::vector<std::string> plotDatStr;
		std::string exePath = app::GetModulePath();
		app::ReadFile_IntoVctStr_PerLine(exePath + "plotData\\plotData.csv", plotDatStr);

		for (int row = 1; row < plotDatStr.size(); ++row) // 1行目はヘッダなので読み飛ばす
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

				// 6列目以降をアトリビュートとしてグラフオブジェクトにセットする
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// 最初の行だけの処理
					if( 1 == row )
					{
						// アトリビュート列を追加する
						graph3D->AddAtr();
					}

					// アトリビュートデータをグラフオブジェクトにセットする
					graph3D->AddAtrData( col - atrTopCol, fDat );
				}
			}

			//-----------------------------------------------------
			// 1.2 読み出しだデータを系列1のプロット対象座標としてセットする
			//-----------------------------------------------------
			// 0～2列目データをプロット座標としてグラフオブジェクトにセットする
			graph3D->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]) );
			// 3～5列目を方向ベクトルとしてグラフオブジェクトにセットする
			graph3D->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]) );
		}

		//-----------------------------------------------------
		// 1.3 系列1のグラフ表示に追加するアトリビュートデータを設定する
		//-----------------------------------------------------
		//graph3D->CnfgAtrDisp_BarIdx(1);
		//graph3D->CnfgAtrDisp_BarColorIdx(1);
		graph3D->CnfgAtrDisp_PtColorIdx(2);
		graph3D->CnfgAtrDisp_PtLnWdtIdx(0);
		graph3D->SetPlotLineDrawType( sodl::DRAWTYPE::POINT );
		graph3D->SetPlotLineWidth(10);

	}

	{
		//-----------------------------------------------------
		// 2. プロットデータ2をcsvファイルから読み出して系列2としてグラフにセット
		//-----------------------------------------------------
		// グラフにプロットする系列を追加する
		int pltIdx = graph3D->AddPlotLine();
		//-----------------------------------------------------
		// 2.1 プロットデータ2をcsvファイルから読み出す
		//-----------------------------------------------------
		// ファイルを開いて行ごとにstringに詰める
		std::vector<std::string> plotDatStr;
		std::string exePath = app::GetModulePath();
		app::ReadFile_IntoVctStr_PerLine(exePath + "plotData\\plotData2.csv", plotDatStr);

		for (int row = 1; row < plotDatStr.size(); ++row) // 1行目はヘッダなので読み飛ばす
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

				// 6列目以降をアトリビュートとしてグラフオブジェクトにセットする
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// 最初の行だけの処理
					if( 1 == row )
					{
						// アトリビュート列を追加する
						graph3D->AddAtr(pltIdx);
					}

					// アトリビュートデータをグラフオブジェクトにセットする
					graph3D->AddAtrData( col - atrTopCol, fDat, pltIdx );
				}
			}

			//-----------------------------------------------------
			// 2.2 読み出しだデータを系列1のプロット対象座標としてセットする
			//-----------------------------------------------------
			// 0～2列目データをプロット座標としてグラフオブジェクトにセットする
			graph3D->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]), pltIdx );
			// 3～5列目を方向ベクトルとしてグラフオブジェクトにセットする
			graph3D->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]), pltIdx );
		}

		//-----------------------------------------------------
		// 2.3 系列1のグラフ表示に追加するアトリビュートデータを設定する
		//-----------------------------------------------------
		graph3D->CnfgAtrDisp_BarIdx(1, pltIdx);
		//graph3D->CnfgAtrDisp_BarColorIdx(1, pltIdx);
		//graph3D->CnfgAtrDisp_PtColorIdx(2, pltIdx);
	}


	{
		//-----------------------------------------------------
		// 2. プロットデータ2をcsvファイルから読み出して系列2としてグラフにセット
		//-----------------------------------------------------
		// グラフにプロットする系列を追加する
		int pltIdx = graph3D->AddPlotLine();
		//-----------------------------------------------------
		// 2.1 プロットデータ2をcsvファイルから読み出す
		//-----------------------------------------------------
		// ファイルを開いて行ごとにstringに詰める
		std::vector<std::string> plotDatStr;
		std::string exePath = app::GetModulePath();
		app::ReadFile_IntoVctStr_PerLine(exePath + "plotData\\plotData2.csv", plotDatStr);

		for (int row = 1; row < plotDatStr.size(); ++row) // 1行目はヘッダなので読み飛ばす
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

				// 6列目以降をアトリビュートとしてグラフオブジェクトにセットする
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// 最初の行だけの処理
					if( 1 == row )
					{
						// アトリビュート列を追加する
						graph3D->AddAtr(pltIdx);
					}

					// アトリビュートデータをグラフオブジェクトにセットする
					graph3D->AddAtrData( col - atrTopCol, fDat, pltIdx );
				}
			}

			//-----------------------------------------------------
			// 2.2 読み出しだデータを系列1のプロット対象座標としてセットする
			//-----------------------------------------------------
			// 0～2列目データをプロット座標としてグラフオブジェクトにセットする
			graph3D->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]), pltIdx );
			// 3～5列目を方向ベクトルとしてグラフオブジェクトにセットする
			graph3D->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]), pltIdx );
		}

		//-----------------------------------------------------
		// 2.3 系列1のグラフ表示に追加するアトリビュートデータを設定する
		//-----------------------------------------------------
		graph3D->SetPlotLineWidth(5, pltIdx);
		graph3D->SetPlotLineOffset( Eigen::Vector3f(10,0,0) , pltIdx );
		graph3D->CnfgAtrDisp_BarIdx(1, pltIdx);

		//graph3D->CnfgAtrDisp_BarColorIdx(1, pltIdx);
		//graph3D->CnfgAtrDisp_PtColorIdx(2, pltIdx);
	}



//	auto Crd2 = sodl::CoordChainObj::create("Crd2", World_Origin);
//	Crd2->CrdTrs.translation() = Eigen::Vector3f(200.f, 100.f, 0.f);
//
//	auto graph3D2 = sodl::GraphObj3D::create( "graph3D", Eigen::Vector3f(100, 100, 100), Crd2 );
//	graph3D2->addData(Eigen::Vector3f(0, 0, 0));
//	graph3D2->addData(Eigen::Vector3f(100, 100, 100));


	sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);


	//////////////////////////////////////////////////////
	//
	// 周期処理の実施
	//

	int count = 0;
	//while( count < 100 )
	while( 1 )
	{
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
	void keyFunc(unsigned char key, int u, int v)
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


