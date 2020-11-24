#pragma once

#include "Drawable3dMdlClass.h"
#include "DrawableObjectClass.h"

namespace SmplObjDrwLib {

	class CoordChain3dMdl 
		: public Drawable3dMdlClass
		,public  CoordChainObj
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		CoordChain3dMdl(
			std::string modelFilePath,
			std::string coordName,
			std::weak_ptr<CoordChainObj> parent
		);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~CoordChain3dMdl() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<CoordChain3dMdl> create
		(
			std::string modelFilePath,
			std::string coordName,
			std::weak_ptr<CoordChainObj> parent
		);


	public:

		// DrawableObjBaseの実装
		virtual void draw();

	};

};
