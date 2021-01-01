#pragma once
#include <windows.h>

#include <memory>
#include <vector>
#include <map>
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "IDrawableObjBase.h"

#include  <iostream>

#define			UnitX			Eigen::Vector3f::UnitX()
#define			UnitY			Eigen::Vector3f::UnitY()
#define			UnitZ			Eigen::Vector3f::UnitZ()

namespace SmplObjDrwLib {

	/////////////////////////////////////////////////////////////////////
	//
	//	CoordChainObj
	//	連鎖座標系
	//

	class CoordChainObj : public virtual IDrawableObjBase
	{
	public:
		using sPtr_CoordObj = std::shared_ptr< CoordChainObj >;
		using wPtr_CoordObj = std::weak_ptr< CoordChainObj >;

		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		CoordChainObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~CoordChainObj();

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<CoordChainObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// DrawableObjBaseの実装
		//--------------------------------------------
	public:
		virtual void draw();

		//--------------------------------------------
		// CoordChainObj.h
		//--------------------------------------------
	protected:
		virtual void _drawShapeOfSelf();

		//--------------------------------------------
		// 以下、本クラス固有のメンバ
		//--------------------------------------------
	public:
		wPtr_CoordObj wPtr_prnt;
		std::vector< wPtr_CoordObj >wPtr_chldrn;
		bool visible_CoordChain = true;
		Eigen::Affine3f CrdTrs;
		Eigen::Affine3f GetTf_root2self();

		void DetachSelf_fromParentsCoordChain();
		void AttachSelf_toParentsCoordChain(std::weak_ptr<CoordChainObj> parent);

		//--------------------------------------------
		// Eigen::Vector 等をメンバに持つクラスのための
		// New演算子オーバライドマクロ
		//->>>----------------------------------------
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		//-<<<----------------------------------------

	};


}