#define  _USE_MATH_DEFINES

#include "VifClass_PlotCnfg.h"


appUi::Vif_InterVisualPartsAction ivpa;	//


void appUi::Vif_PlotCnfg2D::draw2DPlotIf()
{
	// 2D�v���b�g�̐������܂��ݍ��ڂ𐶐�
	if ( ImGui::CollapsingHeader( ("2D plot config  " + plt2dName).c_str(), ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		ImGui::Indent();
		ImGui::PushID(plt2dName.c_str());

		//--------------------------------------------------------------------
		// �X�P�[������
		//--------------------------------------------------------------------
		// �X�P�[���Z�b�g
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
 * @brief �����A�g���r���[�g�ݒ�
 * <pre>
 * �u�������ڂɑ΂��ă\�[�X�Ƃ���A�g���r���[�g���ݒ肷��UI�v
 * ��\�����A���[�U�̐ݒ茋�ʂ������o�ϐ��ɕێ�����B
 * </pre>
 ******************************************************************/
void appUi::Vif_PlotCnfg::Func_AtrCnfg( std::string label, bool& fgEnable, std::vector<std::string>& atrs, int& selectedIdx  )
{
	// �`�F�b�N�{�b�N�X�\��
	ImGui::Checkbox( (label + " by attribute.").c_str(), &fgEnable);
	// �`�F�b�NON�̏ꍇ�A�R���{�{�b�N�X��\������
	if( fgEnable )
	{
		ImGui::SameLine();
		// �R���{�{�b�N�X�\���p�̉�����\���I�u�W�F�N�g��ʂɐݒ�
		ImGui::PushItemWidth(50); // [pix]

		// �R���{�{�b�N�X�\�����e���쐬
		std::vector< const char* > pList;
		// �A�g���r���[�g�������[�v
		for(int i = 0; i<atrs.size(); ++i)
		{
			pList.push_back( atrs[i].c_str() );
		}

		// �R���{�{�b�N�X�\��
		ImGui::Combo( label.c_str(), &selectedIdx, (char**)&pList[0], pList.size(), 10 );

		// �R���{�{�b�N�X�\���p�̉����̐ݒ��\���I�u�W�F�N�g��ʂɑ΂��ĉ�������
		ImGui::PopItemWidth();
	}
	// �`�F�b�N�{�b�N�XOFF�̏ꍇ
	else
	{
		// index�ɖ����l��ݒ肷��
		selectedIdx = sodl::PointsWithAttributes::ATRIDX_NONE;
	}
}


/** ***************************************************************
 * @brief �v���b�g�ݒ�X�V
 * <pre>
 * �v���b�g�ݒ�p��UI��\�����A
 * ���[�U�̐ݒ茋�ʂ������o�ϐ��ɕێ�����B
 * </pre>
 ******************************************************************/
void appUi::Vif_PlotCnfg::drawPltLnCnfg()
{
	// �n�񖼂Ő܂��ݍ��ڂ��쐬
	if ( ImGui::CollapsingHeader( this->pltLnName.c_str() , ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		// �n��̍폜�{�^��
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

		// �v���b�g�F
		Func_AtrCnfg("[Color]",	fg_cBox_plotColorByAtr,	vct_labelsOfAttribetes, idx_pltColorAtr	);
		// �v���b�g��
		Func_AtrCnfg("[Width]",	fg_cBox_plotWidthByAtr,	vct_labelsOfAttribetes, idx_pltWdtAtr	);

		// �o�[�\���L���`�F�b�N�{�b�N�X
		ImGui::Checkbox( "Enable [Bar] ", &fg_cBox_barEnable);
		// �o�[�\���L���̏ꍇ�A�o�[�\���̐ݒ荀�ڂ�`�悷��
		if( fg_cBox_barEnable )
		{
			ImGui::Indent();

			// �o�[����
			Func_AtrCnfg("[Bar Length]",	fg_cBox_barLengAtr,		vct_labelsOfAttribetes, idx_barAtr		);
			// �o�[�F
			Func_AtrCnfg("[Bar Color]",		fg_cBox_barColorByAtr,	vct_labelsOfAttribetes, idx_barColorAtr	);
			// �o�[��
			Func_AtrCnfg("[Bar Width]",		fg_cBox_barWidthByAtr,	vct_labelsOfAttribetes, idx_barWidthAtr	);

			ImGui::Unindent();
		}

		ImGui::NewLine();

		// ���A���^�C���t�@�C�������[�h
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

		// 2D�v���b�g��UI��`�悷��
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
 * @brief �A�g���r���[�g�I��Window�ɗv�����Z�b�g����
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
 * @brief �A�g���r���[�g�I������2D�v���b�g���f����
 * <pre>
 * �A�g���r���[�g�I��Window�̑I�����ʂ�2D�v���b�g[�G���A/���C��]�ɒǉ�����
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
 * @brief �A�g���r���[�g�I��Window�\������
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
			// �R���{�{�b�N�X�\���p�̉�����\���I�u�W�F�N�g��ʂɐݒ�
			ImGui::PushItemWidth(100); // [pix]

			// �R���{�{�b�N�X�\�����e���쐬
			std::vector< const char* > pList;
			// �A�g���r���[�g�������[�v
			for(int i = 0; i<vct_labelsOfAttribetes.size(); ++i)
			{
				pList.push_back( vct_labelsOfAttribetes[i].c_str() );
			}

			// �R���{�{�b�N�X�\��
			ImGui::Combo( "[select]", &t.selectedAtrIdx, (char**)&pList[0], pList.size(), 10 );

			// �R���{�{�b�N�X�\���p�̉����̐ݒ��\���I�u�W�F�N�g��ʂɑ΂��ĉ�������
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
	// �A�g���r���[�g���v���b�g�ɂǂ̂悤�ɔ��f���邩��ݒ肷��

	static int openFileCtr = 0;

	const char KEYSTR_DIALOG_FILE_OPEN[] = "KeyStr_Dialog_FileOpen";

	// ImGui�E�B���h�E�J�n
	ImGui::Begin("Plot Configuration");

	//------------------------------------------------------
	// �v���b�g�n�񐔕��̐ݒ荀�ڂ�\������
	//------------------------------------------------------
	for (auto vifMapItm = pltLines.begin(); vifMapItm != pltLines.end(); ++vifMapItm)
	{
		auto& vifPltLn = vifMapItm->second;
		vifPltLn.drawPltLnCnfg();
	}

	ImGui::NewLine();
	//------------------------------------------------------
	// �v���b�g�n���ǉ����邽�߂̃_�C�A���O�\���{�^��
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
	// �v���b�g�n���ǉ����邽�߂̃_�C�A���O����
	//------------------------------------------------------
	ImVec2 minSize = ImVec2(0, 0);
	ImVec2 maxSize = ImVec2(FLT_MAX, FLT_MAX);
	// �_�C�A���O��\������( Open����Ă��邩�͓����Ŏ������f )
	if ( ImGuiFileDialog::Instance()->Display( KEYSTR_DIALOG_FILE_OPEN ) )
	{
		// �t�@�C�����I�����ꂽ�ꍇ
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// �I�����ꂽ�t�@�C���p�X��I/F�̈�ɃZ�b�g����
			newFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
			newPlotLineName = "plot" + std::to_string(++openFileCtr);
		}
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::NewLine();
	//------------------------------------------------------
	// �J�[�\������
	//------------------------------------------------------
	// �v���b�g�n�񂪑��݂���ꍇ�̂ݕ\������B
	if( 0 != pltLines.size() )
	{
		// �n�񖼂Ő܂��ݍ��ڂ��쐬
		if ( ImGui::CollapsingHeader( "Cursol Operation" ) )
		{
			ImGui::Indent();

			// �J�[�\���L���`�F�b�N�{�b�N�X
			ImGui::Checkbox( "cursol ehable", &cursol_enable );
			if( cursol_enable )
			{
				//------------------------------------------------------
				// �J�[�\����u���v���b�g�n��̑I��
				//------------------------------------------------------
				// �R���{�{�b�N�X�\�����e���쐬
				std::vector< const char* > pList;
				// �A�g���r���[�g�������[�v
				for(auto	mpIt_ptLn  = pltLines.begin();
							mpIt_ptLn != pltLines.end();
						  ++mpIt_ptLn )
				{
					pList.push_back( mpIt_ptLn->first.c_str() );
				}

				if( 0 != pList.size() )
				{
					//------------------------------------------------------
					// �J�[�\����u���v���b�g�n��̑I��
					//------------------------------------------------------
					// �R���{�{�b�N�X�\��
					ImGui::PushItemWidth(80); 																// �v�f���w�� [pix]
					ImGui::Combo(
						"[Plot Line] put cursol to ",
						&cursol_pltLn_selIdx,
						(char**)&pList[0],
						pList.size()
					);
					ImGui::PopItemWidth();																	// �v�f���w��L�����Z��

					// �R���{�{�b�N�X�őI�����ꂽ�v���b�g�n��̖��̂��擾
					if( pList.size() > cursol_pltLn_selIdx )
					{
						cursol_pltLnName = pList[cursol_pltLn_selIdx];
					}

					//------------------------------------------------------
					// �J�[�\���ʒu�̑���
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
					ImGui::PushItemWidth(80); 																// �v�f���w�� [pix]
					ImGui::InputInt( "adj unit", &cursolAdjUnit );											// ���l���̓{�b�N�X
					ImGui::PopItemWidth();																	// �v�f���w��L�����Z��
					ImGui::NewLine();																		// ���s

					//------------------------------------------------------
					// �T�u�J�[�\������
					//------------------------------------------------------
					// �T�u�J�[�\��1��L����
					if( ImGui::Checkbox( "subCursol 1 ehable", &subCursol_enable[0] ) )
					{
						subCursol_idx[0] = cursol_idx;
					}
					// ���݈ʒu�ɃT�u�J�[�\��1���Z�b�g����
					if( subCursol_enable[0] )
					{
						ImGui::SameLine();
						if( ImGui::Button("Set Sub Cursol 1 here") )
						{
							subCursol_idx[0] = cursol_idx;
						}
					}
					ivpa.fg_subCursol1_enable = subCursol_enable[0];

					// �T�u�J�[�\��2��L����
					if( ImGui::Checkbox( "subCursol 2 ehable", &subCursol_enable[1] ) )
					{
						subCursol_idx[1] = cursol_idx;
					}
					// ���݈ʒu�ɃT�u�J�[�\��2���Z�b�g����
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

	// ImGui�E�B���h�E�I��
	ImGui::End();


}



