/******************************************************************
/**
 * @file GraphObi3D.h
 * <pre>
 * 3Dグラフオブジェクトクラスの宣言
 * </pre>
 ******************************************************************/

#pragma once
#include "DrawableObjectClass.h"
#include "DrawingManager.h"
#include "GraphObj.h"

/******************************************************************
/**
 * @brief Simple Object Drawing Library ネームスペース
 *
 ******************************************************************/
namespace SmplObjDrwLib {


	/******************************************************************/
	/**
	 * @brief 3Dグラフオブジェクトクラス
	 *
	 ******************************************************************/
	class GraphObj3D :
		public GraphObj
	{
		typedef GraphObj3D TypeOfSelf;
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		GraphObj3D(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~GraphObj3D();

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			Eigen::Vector3f graphBoxSize,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// DrawableObjBaseの実装
		//--------------------------------------------
	public:
		// virtual void draw();

		//--------------------------------------------
		// CoordChainObj.h
		//--------------------------------------------
	protected:
		// virtual void _drawShapeOfSelf() override;


		//--------------------------------------------
		// GraphObj.h のオーバライド
		//--------------------------------------------
	public:
		virtual int AddPlotLine( std::string lineName ) override;

	protected:
		virtual void initSelf(std::weak_ptr<CamClass> cam = std::weak_ptr<CamClass>() )override{};

		//--------------------------------------------
		// 以下、本クラス固有のメンバ
		//--------------------------------------------
	public:
		bool direc = false;

	public:
		virtual void initSelf(Eigen::Vector3f graphBoxSize);

	};

}

