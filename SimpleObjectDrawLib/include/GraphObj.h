#pragma once
#include "DrawableObjectClass.h"
#include "DrawingManager.h"

namespace SmplObjDrwLib {

	// グラフプロット範囲の制御方法列挙型
	enum struct GRAPH_RANGE_CTL {
		AUTO	= 0,
		FIXED	= 1,
	};


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

		Eigen::Vector3f rangeMax = Eigen::Vector3f(1, 1, 1);	// プロットデータの最大値保持用
		Eigen::Vector3f rangeMin = Eigen::Vector3f(0, 0, 0);	// プロットデータの最小値保持用
		long dataNumMax = 1000;

		virtual void addData( Eigen::Vector3f &point );	// プロットデータの追加


		// グラフプロット範囲の制御方法
		struct {
			GRAPH_RANGE_CTL x = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL y = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL z = GRAPH_RANGE_CTL::AUTO;
		}rangeCtl;

		void AddAtrData(std::shared_ptr < std::deque<float> > atr);

	protected:
		std::deque< Eigen::Vector3f>			_data;			// グラフのプロットデータ
		//std::shared_ptr<PointsObj>			_dataToDraw;	// グラフのプロットデータ(描画用)
		std::shared_ptr<PointsWithAttributes>	_dataToDraw;	// グラフのプロットデータ(描画用)

		std::shared_ptr<LabelObj>		xMaxLabel;		// x最大値ラベル
		std::shared_ptr<LabelObj>		xMinLabel;		// x最大値ラベル
		std::shared_ptr<LabelObj>		yMaxLabel;		// y最小値ラベル
		std::shared_ptr<LabelObj>		yMinLabel;		// y最小値ラベル

		void initSelf(std::weak_ptr<CamClass> cam = std::weak_ptr<CamClass>() );

	public:

	};

}

