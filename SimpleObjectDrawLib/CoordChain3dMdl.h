#pragma once

#include "Drawable3dMdlClass.h"
#include "DrawableObjectClass.h"

namespace SmplObjDrwLib {

	class CoordChain3dMdl 
		: public Drawable3dMdlClass
		,public  CoordChainObj
	{
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		CoordChain3dMdl(
			std::string modelFilePath,
			std::string coordName,
			std::weak_ptr<CoordChainObj> parent
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~CoordChain3dMdl() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<CoordChain3dMdl> create
		(
			std::string modelFilePath,
			std::string coordName,
			std::weak_ptr<CoordChainObj> parent
		);


	public:

		// DrawableObjBase�̎���
		virtual void draw();

	};

};
