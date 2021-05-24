#define  _USE_MATH_DEFINES

#include "VifClass_PlotCnfg.h"


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
void appUi::Vif_PlotCnfg::Updt()
{
	// �n�񖼂Ő܂��ݍ��ڂ��쐬
	if ( ImGui::CollapsingHeader( this->pltLnName.c_str() ) )
	{
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

		// �n��̍폜�{�^��
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




