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


	/** ***************************************************************
	 *
	 * @brief	属性データクラス
	 * <pre>
	 * </pre>
	 ******************************************************************/
	template <typename T>
	class AttributeClass
	{
	public:
		T max;
		T min;
		T dynmcRange;
		virtual ~AttributeClass(){};
		int dataNumMax = 1000;

	protected:
		std::deque<T> _data;

	public:
		const std::deque<T>& data = _data;

		// アトリビュートデータ追加メソッド
		virtual void AddAtrData( T dat )
		{
			// max, min の作成
			if( 0 == _data.size() )
			{
				max = min = dat;
			}
			else
			{
				max = MAX(max, dat);
				min = MIN(min, dat);
			}
			dynmcRange = max - min;
			// データの保持
			_data.push_back( dat );

			// データ数が最大値を超えていたら最古データを捨てる
			for( ; _data.size() > dataNumMax; )
			{
				_data.pop_front();
			}
		}

	};

	/** ***************************************************************
	 *
	 * @brief	属性データ付き点列
	 * <pre>	PointsWithAttibutes
	 * </pre>
	 ******************************************************************/
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
		virtual void _drawShapeOfSelf() override;


		//--------------------------------------------
		// その他
		//--------------------------------------------
	public:
		std::shared_ptr< std::deque<Eigen::Vector3f> > _sPtr_points;
		std::shared_ptr< std::deque<Eigen::Vector3f> > _sPtr_ptVctrs;
		//std::vector< std::shared_ptr< std::deque<float> > > _sPtr_attributes;
		std::vector< std::shared_ptr< AttributeClass<float> > > _sPtr_attributes;



		DRAWTYPE drawType = WIRE;
		float pointTickness = 1.0;

		const int ATRIDX_NONE = -1;
		int atrIdx_pointColor	= ATRIDX_NONE;
		int atrIdx_pointTickness= ATRIDX_NONE;
		int atrIdx_bar			= ATRIDX_NONE;
		int atrIdx_barColor		= ATRIDX_NONE;
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
		typedef LabelObj TypeOfSelf;

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
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
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
		static void SetPrjMtxSizeToChildrenLabel(
			std::shared_ptr<CoordChainObj> obj,
			float size_x,
			float size_y
		);

		std::string text;						// ラベル内容のテキスト
		float size = 15;						// ラベルのフォントサイズ(縦)[pix]
		float prjMtxRangeX = 100.f;				// プロジェクションマトリックスの投影幅(ortho前提)
		float prjMtxRangeY = 100.f;				// プロジェクションマトリックスの投影高(ortho前提)
		LabelAlign align = LabelAlign::LEFT;	// ラベルのアラインメント
	};

	/////////////////////////////////////////////////////////////////////
	//
	//	LabelSimple
	//	テキストラベル(軽量版)
	//

	class LabelSimple : public LabelObj
	{
		typedef LabelSimple TypeOfSelf;
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		//   インスタンスを保持させない。
	protected:
		LabelSimple(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~LabelSimple() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<LabelSimple> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// LabelObjの描画関数の実装
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf() override;

		//--------------------------------------------
		// LabelObjの実装を隠蔽
		//--------------------------------------------
	private:
		static void SetPrjMtxSizeToChildrenLabel(
			std::shared_ptr<CoordChainObj> obj,
			float size_x,
			float size_y
		){}
		float prjMtxRangeX = 100.f;				// プロジェクションマトリックスの投影幅(ortho前提)
		float prjMtxRangeY = 100.f;				// プロジェクションマトリックスの投影高(ortho前提)
		LabelAlign align = LabelAlign::LEFT;	// ラベルのアラインメント
	};

}

