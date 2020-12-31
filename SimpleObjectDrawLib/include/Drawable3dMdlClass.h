#pragma once

#include "IDrawableObjBase.h"
#include "GLTypedef.h"
#include <vector>

//-----------------------------------
// 3D���f�����[�_���C�u���� rx
//#include "rx_vec.h"
//#include "rx_obj.h"
// 3D���f�����[�_���C�u���� rx
//-----------------------------------

//-----------------------------------
// 3D���f�����[�_���C�u���� Assimp
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment( lib, "assimp.lib" )
// 3D���f�����[�_���C�u���� Assimp
//-----------------------------------

#include "assimpGlUtil.h"

namespace SmplObjDrwLib {

	class Drawable3dMdlClass : public virtual IDrawableObjBase
	{
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		Drawable3dMdlClass(std::string modelFilePath);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~Drawable3dMdlClass() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
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
		// 3D���f�����[�_���C�u���� rx�ˑ��f�[�^�^�I�u�W�F�N�g
		std::vector<Vec3> modelVtx;
		std::vector<Vec3> modelNrm;
		std::vector<rxFace> modelFace;
		rxMTL objMtl;
		rxOBJ cRxObj;
		// 3D���f�����[�_���C�u���� rx�ˑ��f�[�^�^�I�u�W�F�N�g
		//-----------------------------------
		*/

		//-----------------------------------
		// 3D���f�����[�_���C�u���� Assimp�ˑ��f�[�^�^�I�u�W�F�N�g
		const aiScene* modelScene = NULL;
		// 3D���f�����[�_���C�u���� Assimp�ˑ��f�[�^�^�I�u�W�F�N�g
		//-----------------------------------

		bool isAssimpModel = false;

		std::vector<int> modelFaceIdx;
		std::vector<float> modelFaceColor;

	public:


		// DrawableObjBase�̎���
		virtual void draw();

		void initVbo();
		void initVao();
		void SetVao();
		void destructVao();
	};

};
