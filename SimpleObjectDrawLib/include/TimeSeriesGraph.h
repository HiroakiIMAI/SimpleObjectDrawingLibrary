#pragma once
#include "GraphObj.h"
#include "DrawingManager.h"

namespace SmplObjDrwLib {

	/** ***************************************************************
	 * @brief 時系列グラフ
	 * <pre>  TimeSeriesGraph
	 *  ・ToDo 複数プロットライン対応は未確認
	 *  ・ToDo アトリビュート対応は未確認
	 * </pre>
	 ******************************************************************/
	class TimeSeriesGraph :
		public GraphObj
	{
		typedef TimeSeriesGraph TypeOfSelf;
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
	protected:
		TimeSeriesGraph(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~TimeSeriesGraph();

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

		int dataNumToDraw = 100;

	protected:
		void initSelf(std::weak_ptr<CamClass> cam = std::weak_ptr<CamClass>() );
		void refleshRangeAsScroll();

	};

}
