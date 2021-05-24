#define  _USE_MATH_DEFINES

#include "VifClass_PlotCnfg.h"


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
void appUi::Vif_PlotCnfg::Updt()
{
	// 系列名で折り畳み項目を作成
	if ( ImGui::CollapsingHeader( this->pltLnName.c_str() ) )
	{
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

		// 系列の削除ボタン
		ImGui::Text("                                  ");
		ImGui::SameLine();
		if( ImGui::Button( ("[Delete] " + this->pltLnName).c_str() ) )
		{
			this->fg_delete = true;
		}

		ImGui::Unindent();
		ImGui::PopID();
	}
}




