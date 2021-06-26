#define  _USE_MATH_DEFINES

#include "VifClass_PlotCnfg.h"


appUi::Vif_InterVisualPartsAction ivpa;	//


void appUi::Vif_PlotCnfg2D::draw2DPlotIf()
{
	// 2Dプロットの数だけ折り畳み項目を生成
	if ( ImGui::CollapsingHeader( ("2D plot config  " + plt2dName).c_str(), ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		ImGui::Indent();
		ImGui::PushID(plt2dName.c_str());

		//--------------------------------------------------------------------
		// スケール操作
		//--------------------------------------------------------------------
		// スケールセット
		if( ImGui::Checkbox( "Set Scale With Current SubCursol 1 and 2", &fg_cBox_scalefitToSubCursols ) )
		{
			if(	!ivpa.fg_subCursol1_enable
			||	!ivpa.fg_subCursol2_enable
			)
			{
				fg_cBox_scalefitToSubCursols = false;
			}
		}

		ImGui::PopID();
		ImGui::Unindent();
	}
}



/** ***************************************************************
 * @brief 可視化アトリビュート設定
 * <pre>
 * 「可視化項目に対してソースとするアトリビュート列を設定するUI」
 * を表示し、ユーザの設定結果をメンバ変数に保持する。
 * </pre>
 ******************************************************************/
void appUi::Vif_PlotCnfg::Func_AtrCnfg( std::string label, bool& fgEnable, std::vector<std::string>& atrs, int& selectedIdx  )
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


/** ***************************************************************
 * @brief プロット設定更新
 * <pre>
 * プロット設定用のUIを表示し、
 * ユーザの設定結果をメンバ変数に保持する。
 * </pre>
 ******************************************************************/
void appUi::Vif_PlotCnfg::drawPltLnCnfg()
{
	// 系列名で折り畳み項目を作成
	if ( ImGui::CollapsingHeader( this->pltLnName.c_str() , ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		// 系列の削除ボタン
		ImGui::Text("                                  ");
		ImGui::SameLine();
		if( ImGui::Button( ("[Delete] " + this->pltLnName).c_str() ) )
		{
			this->fg_delete = true;
		}

		//--------------------------------------------------------------------------
		// 3D Plot Line Config
		//--------------------------------------------------------------------------
		ImGui::PushID(pltLnName.c_str());

		ImGui::Indent();

		// プロット色
		Func_AtrCnfg("[Color]",	fg_cBox_plotColorByAtr,	vct_labelsOfAttribetes, idx_pltColorAtr	);
		// プロット幅
		Func_AtrCnfg("[Width]",	fg_cBox_plotWidthByAtr,	vct_labelsOfAttribetes, idx_pltWdtAtr	);

		// バー表示有効チェックボックス
		ImGui::Checkbox( "Enable [Bar] ", &fg_cBox_barEnable);
		// バー表示有効の場合、バー表示の設定項目を描画する
		if( fg_cBox_barEnable )
		{
			ImGui::Indent();

			// バー長さ
			Func_AtrCnfg("[Bar Length]",	fg_cBox_barLengAtr,		vct_labelsOfAttribetes, idx_barAtr		);
			// バー色
			Func_AtrCnfg("[Bar Color]",		fg_cBox_barColorByAtr,	vct_labelsOfAttribetes, idx_barColorAtr	);
			// バー幅
			Func_AtrCnfg("[Bar Width]",		fg_cBox_barWidthByAtr,	vct_labelsOfAttribetes, idx_barWidthAtr	);

			ImGui::Unindent();
		}

		ImGui::NewLine();

		// リアルタイムファイルリロード
		ImGui::Checkbox( "[Realtime File Reload] ", &fg_cBox_updateCyclic );

		//--------------------------------------------------------------------------
		// 2D Plot Config
		//->>>----------------------------------------------------------------------
		if( ImGui::Button( "Add 2DPlotArea" ) )
		{
			SetSlctDmnd( DMND_SRC_TP::AREA, 0 );
		}
		for( int i=0; i < vct_2dPlots.size(); ++i )
		{
			ImGui::PushID( (pltLnName + std::to_string(i) ).c_str() );
			if( ImGui::Button( "Add 2DPlotLine" ) )
			{
				SetSlctDmnd( DMND_SRC_TP::LINE, i );
			}
			ImGui::PopID();
		}
		ShowSlctWndw();

		// 2DプロットのUIを描画する
		for( auto plt2dIf_itr  = grps2DIf.begin();
				  plt2dIf_itr != grps2DIf.end();
				++plt2dIf_itr)
		{
			auto& plt2dIf = plt2dIf_itr->second;
			plt2dIf.draw2DPlotIf();
		}

		//-<<<----------------------------------------------------------------------
		// 2D Plot Config
		//--------------------------------------------------------------------------


		ImGui::Unindent();
		ImGui::PopID();
	}
}



/** ***************************************************************
 * @brief アトリビュート選択Windowに要求をセットする
 * <pre>
 * </pre>
 ******************************************************************/
void appUi::Vif_PlotCnfg::SetSlctDmnd( DMND_SRC_TP tp, int num )
{
	ST_atrSlctWindowCtl.lastDmndSrcTp = tp;
	ST_atrSlctWindowCtl.lastDmndSrcIdx = num;
	ST_atrSlctWindowCtl.fg_showAtrSlctWndw = true;
}


/** ***************************************************************
 * @brief アトリビュート選択結果2Dプロット反映処理
 * <pre>
 * アトリビュート選択Windowの選択結果を2Dプロット[エリア/ライン]に追加する
 * </pre>
 ******************************************************************/
void appUi::Vif_PlotCnfg::PrcSelected()
{
	auto & t = ST_atrSlctWindowCtl;

	if ( "" != t.selectedAtrName )
	{
		switch( t.lastDmndSrcTp )
		{
		case DMND_SRC_TP::AREA:
			{
				std::vector<std::string> tmp;
				tmp.push_back( t.selectedAtrName );
				vct_2dPlots.emplace_back( tmp );
			}
			break;
		case DMND_SRC_TP::LINE:
			vct_2dPlots[t.lastDmndSrcIdx].push_back( t.selectedAtrName );
			break;
		}
		t.selectedAtrName = "";
	}
}


/** ***************************************************************
 * @brief アトリビュート選択Window表示処理
 * <pre>
 * </pre>
 ******************************************************************/
void appUi::Vif_PlotCnfg::ShowSlctWndw()
{
	auto & t = ST_atrSlctWindowCtl;

	if(t.fg_showAtrSlctWndw)
	{
		ImGui::Begin("SelectWindow");
		{
			// コンボボックス表示用の横幅を表示オブジェクト一般に設定
			ImGui::PushItemWidth(100); // [pix]

			// コンボボックス表示内容を作成
			std::vector< const char* > pList;
			// アトリビュート数分ループ
			for(int i = 0; i<vct_labelsOfAttribetes.size(); ++i)
			{
				pList.push_back( vct_labelsOfAttribetes[i].c_str() );
			}

			// コンボボックス表示
			ImGui::Combo( "[select]", &t.selectedAtrIdx, (char**)&pList[0], pList.size(), 10 );

			// コンボボックス表示用の横幅の設定を表示オブジェクト一般に対して解除する
			ImGui::PopItemWidth();

			if(ImGui::Button("OK"))
			{
				t.selectedAtrName = pList[t.selectedAtrIdx];
				t.fg_showAtrSlctWndw = false;
			}
		}
		ImGui::End();
	}
}


void appUi::VISUAL_IF::drawVif()
{
	// アトリビュートをプロットにどのように反映するかを設定する

	static int openFileCtr = 0;

	const char KEYSTR_DIALOG_FILE_OPEN[] = "KeyStr_Dialog_FileOpen";

	// ImGuiウィンドウ開始
	ImGui::Begin("Plot Configuration");

	//------------------------------------------------------
	// プロット系列数分の設定項目を表示する
	//------------------------------------------------------
	for (auto vifMapItm = pltLines.begin(); vifMapItm != pltLines.end(); ++vifMapItm)
	{
		auto& vifPltLn = vifMapItm->second;
		vifPltLn.drawPltLnCnfg();
	}

	ImGui::NewLine();
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
			newFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
			newPlotLineName = "plot" + std::to_string(++openFileCtr);
		}
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::NewLine();
	//------------------------------------------------------
	// カーソル操作
	//------------------------------------------------------
	// プロット系列が存在する場合のみ表示する。
	if( 0 != pltLines.size() )
	{
		// 系列名で折り畳み項目を作成
		if ( ImGui::CollapsingHeader( "Cursol Operation" ) )
		{
			ImGui::Indent();

			// カーソル有効チェックボックス
			ImGui::Checkbox( "cursol ehable", &cursol_enable );
			if( cursol_enable )
			{
				//------------------------------------------------------
				// カーソルを置くプロット系列の選択
				//------------------------------------------------------
				// コンボボックス表示内容を作成
				std::vector< const char* > pList;
				// アトリビュート数分ループ
				for(auto	mpIt_ptLn  = pltLines.begin();
							mpIt_ptLn != pltLines.end();
						  ++mpIt_ptLn )
				{
					pList.push_back( mpIt_ptLn->first.c_str() );
				}

				if( 0 != pList.size() )
				{
					//------------------------------------------------------
					// カーソルを置くプロット系列の選択
					//------------------------------------------------------
					// コンボボックス表示
					ImGui::PushItemWidth(80); 																// 要素幅指定 [pix]
					ImGui::Combo(
						"[Plot Line] put cursol to ",
						&cursol_pltLn_selIdx,
						(char**)&pList[0],
						pList.size()
					);
					ImGui::PopItemWidth();																	// 要素幅指定キャンセル

					// コンボボックスで選択されたプロット系列の名称を取得
					if( pList.size() > cursol_pltLn_selIdx )
					{
						cursol_pltLnName = pList[cursol_pltLn_selIdx];
					}

					//------------------------------------------------------
					// カーソル位置の操作
					//------------------------------------------------------
					ImGui::SliderInt("[Cursol Pos] slider", &cursol_idx, 0, cursol_idxMax);
					if( ImGui::Button("<") )
					{
						cursol_idx -= cursolAdjUnit;
					}
					ImGui::SameLine();
					if( ImGui::Button(">") )
					{
						cursol_idx += cursolAdjUnit;
					}
					ImGui::SameLine();
					ImGui::Text("[Cursol Pos] adjust         ");
					ImGui::SameLine();
					ImGui::PushItemWidth(80); 																// 要素幅指定 [pix]
					ImGui::InputInt( "adj unit", &cursolAdjUnit );											// 数値入力ボックス
					ImGui::PopItemWidth();																	// 要素幅指定キャンセル
					ImGui::NewLine();																		// 改行

					//------------------------------------------------------
					// サブカーソル操作
					//------------------------------------------------------
					// サブカーソル1を有効化
					if( ImGui::Checkbox( "subCursol 1 ehable", &subCursol_enable[0] ) )
					{
						subCursol_idx[0] = cursol_idx;
					}
					// 現在位置にサブカーソル1をセットする
					if( subCursol_enable[0] )
					{
						ImGui::SameLine();
						if( ImGui::Button("Set Sub Cursol 1 here") )
						{
							subCursol_idx[0] = cursol_idx;
						}
					}
					ivpa.fg_subCursol1_enable = subCursol_enable[0];

					// サブカーソル2を有効化
					if( ImGui::Checkbox( "subCursol 2 ehable", &subCursol_enable[1] ) )
					{
						subCursol_idx[1] = cursol_idx;
					}
					// 現在位置にサブカーソル2をセットする
					if( subCursol_enable[1] )
					{
						ImGui::SameLine();
						if( ImGui::Button("Set Sub Cursol 2 here") )
						{
							subCursol_idx[1] = cursol_idx;
						}
					}
					ivpa.fg_subCursol2_enable = subCursol_enable[1];
				}
			}

			ImGui::Unindent();
		}
	}

	// ImGuiウィンドウ終了
	ImGui::End();


}



