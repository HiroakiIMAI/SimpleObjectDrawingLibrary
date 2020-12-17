#pragma once

#include "CoordChainObj.h"

namespace SmplObjDrwLib {

	/////////////////////////////////////////////////////////////////////
	//
	//	PlaneObj
	//	平面
	//

	class PlaneObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		PlaneObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~PlaneObj() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<PlaneObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<PlaneObj>()
		);

		//--------------------------------------------
		// CoordChainObjの描画関数の実装
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// その他
		//--------------------------------------------
	public:
		float width  = 30.;
		float height = 20.;


	};

	/////////////////////////////////////////////////////////////////////
	//
	//	Box
	//	直方体
	//

	class BoxObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		BoxObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~BoxObj() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<BoxObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<BoxObj>()
		);

		//--------------------------------------------
		// CoordChainObjの描画関数の実装
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// その他
		//--------------------------------------------
	public:
		Eigen::Vector3f boxSize;
		DRAWTYPE drawType = WIRE;
	};

	/////////////////////////////////////////////////////////////////////
	//
	//	Points
	//	点列
	//

	class PointsObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		PointsObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~PointsObj() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<PointsObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<PointsObj>()
		);

		//--------------------------------------------
		// CoordChainObjの描画関数の実装
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// その他
		//--------------------------------------------
	public:
		std::deque<Eigen::Vector3f> points;
		DRAWTYPE drawType = WIRE;
	};


	/////////////////////////////////////////////////////////////////////
	//
	//	PointsWithAttibutes
	//	属性データ付き点列
	//

	class PointsWithAttributes : public virtual CoordChainObj
	{
		typedef PointsWithAttributes TypeOfSelf;

		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		PointsWithAttributes(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~PointsWithAttributes() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<TypeOfSelf>()
		);

		//--------------------------------------------
		// CoordChainObjの描画関数の実装
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// その他
		//--------------------------------------------
	public:
		std::shared_ptr< std::deque<Eigen::Vector3f> > _sPtr_points;
		std::vector< std::shared_ptr< std::deque<float> > > _sPtr_attributes;

		DRAWTYPE drawType = WIRE;
		int atrIdx_pointColor = -1;
		int atrIdx_bar = -1;
		int atrIdx_Size = -1;
		Eigen::Vector3f atrBarDirec = UnitY;

	};


	/////////////////////////////////////////////////////////////////////
	//
	//	Label
	//	テキストラベル
	//

	enum struct LabelAlign {
		LEFT = 0,
		CENTER,
		RIGHT,
	};

	class LabelObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		LabelObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~LabelObj() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<LabelObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<PointsObj>()
		);

		//--------------------------------------------
		// LabelObjの描画関数の実装
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// その他
		//--------------------------------------------
	public:
		static void SetVpSizeToChildrenLabel(
			std::shared_ptr<CoordChainObj> obj,
			float size_x,
			float size_y
		);

		std::string text;						// ラベル内容のテキスト
		float size = 5;							// ラベルのフォントサイズ
		float vpSizeX = 100.f;					// ビューポートサイズ X
		float vpSizeY = 100.f;					// ビューポートサイズ Y
		LabelAlign align = LabelAlign::LEFT;	// ラベルのアラインメント
	};
}

