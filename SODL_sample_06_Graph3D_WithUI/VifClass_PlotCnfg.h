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
	 * @brief 描画パーツ間の相互作用クラス
	 * <pre>
	 * 描画パーツ間の相互作用を扱うデータと処理を定義するクラス
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
	 * @brief 2DプロットのViusualIF
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
	 * @brief プロットラインのViusualIF
	 * <pre>
	 * </pre>
	 ******************************************************************/
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

		//---------------------------------------------------------------------
		// 2Dプロット用のI/F
		//---------------------------------------------------------------------
		std::map< std::string, appUi::Vif_PlotCnfg2D > grps2DIf;

	public:

		void Func_AtrCnfg( std::string label, bool& fgEnable, std::vector<std::string>& atrs, int& selectedIdx  );
		void drawPltLnCnfg();
	};



	/** ***************************************************************
	 * @brief 3DグラフのViusualIFルート構造
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


