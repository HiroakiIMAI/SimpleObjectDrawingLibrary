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

	protected:
		static const std::string _DEFAULT_PLOT;

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
		long dataNumMax = 100000;
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
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		int AddAtr(										// アトリビュートデータ列の追加
			std::string attributeName,					// アトリビュート名
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void AddAtrData( 								// アトリビュートデータの追加
			int atrIdx,									// アトリビュートindex
			const float& atrDat,						// アトリビュートデータ
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void AddPtVct( 									// プロット点の方向ベクトル追加
			const Eigen::Vector3f &direcVector,			// プロット点の方向ベクトル
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void CnfgAtrDisp_PtColorIdx( 					// 点列の色に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void CnfgAtrDisp_PtLnWdtIdx( 					// プロット点の幅に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void CnfgAtrDisp_BarIdx( 						// バー長さに割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void CnfgAtrDisp_BarColorIdx( 					// バーの色に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void CnfgAtrDisp_BarWidthIdx( 					// バーの幅に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void CnfgAtrDisp_BarWidthIdx( 					// バーの幅に割り当てるアトリビュートindexを設定する
			int atrIdx,									// アトリビュートindex
			const int& pltLineIdx = 0					// プロットデータ系列index
		);

		void SetPlotLineColor( 							// プロット系列にデフォルト表示色を設定する
			const ST_COLOR& color,						// 設定色
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void SetPlotLineOffset( 						// プロット系列の表示位置のオフセットを設定する
			const Eigen::Vector3f& ofs,					// オフセットベクトル
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void SetPlotLineWidth( 							// プロット系列の線幅を設定する
			const float& width,							// 線幅
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void SetPlotLineDrawType( 						// プロット系列の描画タイプを設定する
			DRAWTYPE type,								// 描画タイプ
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		void SetBarEnable( 								// バーの表示有効フラグをセットする
			const bool& enable,							// 有効フラグ
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);


		void SetBarWidth( 								// バーのデフォルト長さを指定する
			const float& width,							// 幅
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		int GetNumPlotLines(); 							// プロット系列の数を取得する

		// プロット系列のattributeへのshared_ptr配列を取得する
		std::vector< std::shared_ptr< AttributeClass<float> > >
		GetAttributes_Shared(
			std::string pltLineName =_DEFAULT_PLOT		// プロットデータ系列名
		);

		// プロット系列のコピーデータを取得する
		const std::deque<Eigen::Vector3f>*
		GetCpyPltLn(
			std::string pltName
		);

	protected:
		std::unordered_map< std::string, std::deque<Eigen::Vector3f> >				_lines;			// グラフのプロットデータ
		std::unordered_map< std::string, std::shared_ptr<PointsWithAttributes> >	_linesToDraw;	// グラフのプロットデータ(描画用)

		std::shared_ptr<LabelObj>		xMaxLabel;		// x最大値ラベル
		std::shared_ptr<LabelObj>		xMinLabel;		// x最大値ラベル
		std::shared_ptr<LabelObj>		yMaxLabel;		// y最小値ラベル
		std::shared_ptr<LabelObj>		yMinLabel;		// y最小値ラベル

		virtual void initSelf(std::weak_ptr<CamClass> cam = std::weak_ptr<CamClass>() );


		//----------------------------------------------------------------------
		// カーソル関連の定義
		//----------------------------------------------------------------------
		// 外部I/F
	public:
		enum struct CURSOL_SEL { MAIN, SUB1, SUB2 };
		void UpdtCursol(int idx = -1, CURSOL_SEL slct = CURSOL_SEL::MAIN);
		void PutCursolToLine(const std::string& pltLineName, CURSOL_SEL slct = CURSOL_SEL::MAIN);
		void SetCursolViible(const bool fg, CURSOL_SEL slct = CURSOL_SEL::MAIN);

		// 内部I/F・データ
	protected:
		// クラス内構カーソル造体宣言
		typedef struct ST_PointCursol_tp
		{
		public:
			std::shared_ptr<PointsObj>	lineX;							///< カーソル位置を表現するX軸方向のライン
			std::shared_ptr<PointsObj>	lineY;							///< カーソル位置を表現するY軸方向のライン
			std::shared_ptr<PointsObj>	lineZ;							///< カーソル位置を表現するZ軸方向のライン
			std::shared_ptr<PointsObj>	ptCenterToDraw;					///< カーソル位置を表現する点(スケール調整後)
			std::shared_ptr<LabelObj>	ptCenterLab;					///< カーソル位置に表示するラベル
			std::shared_ptr<LabelObj>	atrLab;							///< カーソル位置に表示するアトリビュート情報表示ラベル
			Eigen::Vector3f				ptCenter;						///< カーソル位置を表現する点

			std::string					pltLnName;						///< カーソルを置くプロット系列の名称
			int idx_currentPt = 0;										///< 現在のカーソル表示に対応するデータindex

		} ST_PointCursol;
		// カーソル実体定義
		ST_PointCursol _cursol;
		ST_PointCursol _cursolSub[2];
		// カーソル操作内部I/F関数
		ST_PointCursol& _SelectCursol( const CURSOL_SEL& sel );
		void _CreatePointCursol( CURSOL_SEL slct, sPtr_CoordObj prnt, const float color[] );

	};

}

