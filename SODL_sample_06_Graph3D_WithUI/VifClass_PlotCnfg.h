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

	class Vif_PlotCnfg
	{
	public:
		std::string pltLnName;
		std::string pltFilePath;
		bool fg_delete = false;

		// チェックボックス状態保持用メンバ
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

		// 2Dプロット選択用子ウィンドウ表示状態保持用メンバ
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


	public:

		void Func_AtrCnfg( std::string label, bool& fgEnable, std::vector<std::string>& atrs, int& selectedIdx  );
		void Updt();
	};

};


