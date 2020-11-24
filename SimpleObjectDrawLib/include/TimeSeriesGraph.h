#pragma once
#include "GraphObj.h"

namespace SmplObjDrwLib {

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
		void initSelf();
		void refleshRangeAsScroll();

	};

}