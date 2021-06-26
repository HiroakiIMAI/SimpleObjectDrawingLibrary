#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glut.h"
#include "ImGui/backends/imgui_impl_opengl2.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"



#include <math.h>
#include <iostream>
#include <fstream>

#include <Windows.h>

namespace sodl = SmplObjDrwLib;

namespace appUi {


	/** ***************************************************************
	 * @brief �`��p�[�c�Ԃ̑��ݍ�p�N���X
	 * <pre>
	 * �`��p�[�c�Ԃ̑��ݍ�p�������f�[�^�Ə������`����N���X
	 * </pre>
	 ******************************************************************/
	class Vif_InterVisualPartsAction
	{
	public:

		bool fg_subCursol1_enable = false;
		bool fg_subCursol2_enable = false;

	private:

	};


	/** ***************************************************************
	 * @brief 2D�v���b�g��ViusualIF
	 * <pre>
	 * </pre>
	 ******************************************************************/
	class Vif_PlotCnfg2D
	{
	public:
		std::string plt2dName;

		bool fg_cBox_scalefitToSubCursols = false;
		bool fg_cBox_scalefitToSubCursols_prv = false;

		void draw2DPlotIf();
	};


	/** ***************************************************************
	 * @brief �v���b�g���C����ViusualIF
	 * <pre>
	 * </pre>
	 ******************************************************************/
	class Vif_PlotCnfg
	{
	public:
		std::string pltLnName;
		std::string pltFilePath;
		bool fg_delete = false;

		// �`�F�b�N�{�b�N�X��ԕێ��p�����o
		bool fg_cBox_plotColorByAtr	= false;
		bool fg_cBox_plotWidthByAtr	= false;
		bool fg_cBox_barEnable		= false;
		bool fg_cBox_barLengAtr		= false;
		bool fg_cBox_barColorByAtr	= false;
		bool fg_cBox_barWidthByAtr	= false;
		bool fg_cBox_updateCyclic	= false;

		int		idx_pltWdtAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_pltColorAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;

		int		idx_barAtr			= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_barColorAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_barWidthAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;

		std::vector<std::string> vct_labelsOfAttribetes;

		std::vector< std::vector<std::string> > vct_2dPlots;

		// 2D�v���b�g�I��p�q�E�B���h�E�\����ԕێ��p�����o
		enum struct DMND_SRC_TP{ AREA, LINE };
		struct
		{
			bool 		fg_showAtrSlctWndw		= false;
			std::string selectedAtrName			= "";
			int			selectedAtrIdx			= 0;
			DMND_SRC_TP lastDmndSrcTp;
			int 		lastDmndSrcIdx;
		}ST_atrSlctWindowCtl;

		void SetSlctDmnd( DMND_SRC_TP tp, int num );
		void PrcSelected();
		void ShowSlctWndw();

		//---------------------------------------------------------------------
		// 2D�v���b�g�p��I/F
		//---------------------------------------------------------------------
		std::map< std::string, appUi::Vif_PlotCnfg2D > grps2DIf;

	public:

		void Func_AtrCnfg( std::string label, bool& fgEnable, std::vector<std::string>& atrs, int& selectedIdx  );
		void drawPltLnCnfg();
	};



	/** ***************************************************************
	 * @brief 3D�O���t��ViusualIF���[�g�\��
	 * <pre>
	 * </pre>
	 ******************************************************************/
	class VISUAL_IF{
	public:
		//-------------------------------------------------
		// Visual to Model
		//-------------------------------------------------
		std::map< std::string, appUi::Vif_PlotCnfg > pltLines;
		std::string newFilePath;
		std::string newPlotLineName;

		bool		cursol_enable		= false;
		std::string cursol_pltLnName;
		int			cursol_pltLn_selIdx = 0;
		int			cursol_idx			= 0;
		int			cursolAdjUnit		= 1;

		int			subCursol_idx[2]	= { 0, 0 };
		bool		subCursol_enable[2]	= { false, false };



		//-------------------------------------------------
		// Model to Visual
		//-------------------------------------------------
		int cursol_idxMax = 0;


		//-------------------------------------------------
		// function I/F
		//-------------------------------------------------
		void drawVif();

	};


};


