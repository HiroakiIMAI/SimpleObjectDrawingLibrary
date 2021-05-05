#pragma once
#include "DrawableObjectClass.h"
#include "DrawingManager.h"

namespace SmplObjDrwLib {

	// グラフプロット範囲の制御方法列挙型
	enum struct GRAPH_RANGE_CTL {
		AUTO	= 0,
		FIXED	= 1,
	};

	/** ***************************************************************
	 * @brief グラフオブジェクト
	 * <pre>  GraphObj
	 * </pre>
	 ******************************************************************/
	class GraphObj :
		public CoordChainObj
	{
		typedef GraphObj TypeOfSelf;
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		GraphObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~GraphObj();

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			std::weak_ptr<CamClass> cam,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// DrawableObjBaseの実装
		//--------------------------------------------
	public:
		//virtual void draw();

		//--------------------------------------------
		// CoordChainObj.h
		//--------------------------------------------
	protected:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// 以下、本クラス固有のメンバ
		//--------------------------------------------
	public:
		std::shared_ptr<BoxObj>			back;			// グラフエリアの背景
		std::shared_ptr<BoxObj>			area;			// グラフエリアの塗りつぶし
		std::shared_ptr<LabelObj>		title;			// グラフタイトル
		std::shared_ptr<LabelObj>		xAxisLabel;		// x軸ラベル
		std::shared_ptr<LabelObj>		yAxisLabel;		// y軸ラベル
		//std::shared_ptr<LabelObj>		zAxisLabel;		// z軸ラベル

		std::shared_ptr<PointsObj>		xAxis;			// x軸
		std::shared_ptr<PointsObj>		yAxis;			// y軸

		Eigen::Vector3f rangeMax = Eigen::Vector3f(1, 1, 1);	// プロットデータの最大値保持用(系列間共通)
		Eigen::Vector3f rangeMin = Eigen::Vector3f(0, 0, 0);	// プロットデータの最小値保持用(系列間共通)
		long dataNumMax = 1000;
		bool normarize_direcVec = true;

		// グラフプロット範囲の制御方法
		struct {
			GRAPH_RANGE_CTL x = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL y = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL z = GRAPH_RANGE_CTL::AUTO;
		}rangeCtl;

		virtual int AddPlotLine( std::string lineName );
		virtual void DeletePlotLine( std::string lineName );

		virtual void AddData( 							// プロットデータの追加
			const Eigen::Vector3f &point,				// プロットデータ(座標値)
			std::string pltLineName						// プロットデータ系列名
		);

		int AddAtr(										// アトリビュートデータ列の追加
			std::string pltLineName						// プロットデータ系列名
		);

		void AddAtrData( 								// アトリビュートデータの追加
			int atrIdx,									// アトリビュートindex
			const float& atrDat,						// アトリビュートデータ
			std::string pltLineName						// プロットデータ系列名
		);

		void AddPtVct( 									// プロット点の方向ベクトル追加
			const Eigen::Vector3f &direcVector,			// プロット点の方向ベクトル
			std::string pltLineName						// プロットデータ系列名
		);

		void CnfgAtrDisp_PtColorIdx( 					// 点列の色に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName						// プロットデータ系列名
		);

		void CnfgAtrDisp_PtLnWdtIdx( 					// プロット点の幅に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName						// プロットデータ系列名
		);

		void CnfgAtrDisp_BarIdx( 						// バー長さに割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName						// プロットデータ系列名
		);

		void CnfgAtrDisp_BarColorIdx( 					// バーの色に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName						// プロットデータ系列名
		);

		void CnfgAtrDisp_BarWidthIdx( 					// バーの幅に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName						// プロットデータ系列名
		);

		void SetPlotLineColor( 							// プロット系列にデフォルト表示色を設定する
			const ST_COLOR& color,						// 設定色
			std::string pltLineName						// プロットデータ系列名
		);

		void SetPlotLineOffset( 						// プロット系列の表示位置のオフセットを設定する
			const Eigen::Vector3f& ofs,					// オフセットベクトル
			std::string pltLineName						// プロットデータ系列名
		);

		void SetPlotLineWidth( 							// プロット系列の線幅を設定する
			const float& width,							// 線幅
			std::string pltLineName						// プロットデータ系列名
		);

		void SetPlotLineDrawType( 						// プロット系列の描画タイプを設定する
			DRAWTYPE type,								// 描画タイプ
			std::string pltLineName						// プロットデータ系列名
		);

		void SetBarWidth( 								// バーのデフォルト長さを指定する
			const float& width,							// 幅
			std::string pltLineName						// プロットデータ系列名
		);

		int GetNumPlotLines(); 							// プロット系列の数を取得する


	protected:
		//std::deque< Eigen::Vector3f>									_data;			// グラフのプロットデータ
		std::map< std::string, std::deque<Eigen::Vector3f> >			_lines;			// グラフのプロットデータ
		//std::shared_ptr<PointsObj>									_dataToDraw;	// グラフのプロットデータ(描画用)
		std::map< std::string, std::shared_ptr<PointsWithAttributes> >	_linesToDraw;	// グラフのプロットデータ(描画用)

		std::shared_ptr<LabelObj>		xMaxLabel;		// x最大値ラベル
		std::shared_ptr<LabelObj>		xMinLabel;		// x最大値ラベル
		std::shared_ptr<LabelObj>		yMaxLabel;		// y最小値ラベル
		std::shared_ptr<LabelObj>		yMinLabel;		// y最小値ラベル

		virtual void initSelf(std::weak_ptr<CamClass> cam = std::weak_ptr<CamClass>() );

	};

}

