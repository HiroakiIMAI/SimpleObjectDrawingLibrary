#pragma once

#include "IDrawableObjBase.h"
#include "GLTypedef.h"
#include <vector>

//-----------------------------------
// 3Dモデルローダライブラリ rx
//#include "rx_vec.h"
//#include "rx_obj.h"
// 3Dモデルローダライブラリ rx
//-----------------------------------

//-----------------------------------
// 3Dモデルローダライブラリ Assimp
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment( lib, "assimp.lib" )
// 3Dモデルローダライブラリ Assimp
//-----------------------------------

#include "assimpGlUtil.h"

namespace SmplObjDrwLib {

	class Drawable3dMdlClass : public virtual IDrawableObjBase
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		Drawable3dMdlClass(std::string modelFilePath);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:
		virtual ~Drawable3dMdlClass() {};

		//--------------------------------------------
		// ファクトリ関数
		//--------------------------------------------
	public:
		static std::shared_ptr<Drawable3dMdlClass> create
		(
			std::string modelFilePath
		);


	protected:
		GLuint vao;
		GLuint vboVtx;
		GLuint vboColor;
		GLuint vboNrm;
		GLuint vboIdx;

		/*
		//-----------------------------------
		// 3Dモデルローダライブラリ rx依存データ型オブジェクト
		std::vector<Vec3> modelVtx;
		std::vector<Vec3> modelNrm;
		std::vector<rxFace> modelFace;
		rxMTL objMtl;
		rxOBJ cRxObj;
		// 3Dモデルローダライブラリ rx依存データ型オブジェクト
		//-----------------------------------
		*/

		//-----------------------------------
		// 3Dモデルローダライブラリ Assimp依存データ型オブジェクト
		const aiScene* modelScene = NULL;
		// 3Dモデルローダライブラリ Assimp依存データ型オブジェクト
		//-----------------------------------

		bool isAssimpModel = false;

		std::vector<int> modelFaceIdx;
		std::vector<float> modelFaceColor;

	public:


		// DrawableObjBaseの実装
		virtual void draw();

		void initVbo();
		void initVao();
		void SetVao();
		void destructVao();
	};

};
