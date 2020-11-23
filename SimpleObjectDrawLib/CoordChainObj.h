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
	//	�A�����W�n
	//

	class CoordChainObj : public virtual IDrawableObjBase
	{
	public:
		using sPtr_CoordObj = std::shared_ptr< CoordChainObj >;
		using wPtr_CoordObj = std::weak_ptr< CoordChainObj >;

		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		CoordChainObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~CoordChainObj();

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<CoordChainObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// DrawableObjBase�̎���
		//--------------------------------------------
	public:
		virtual void draw();

		//--------------------------------------------
		// CoordChainObj.h
		//--------------------------------------------
	protected:
		virtual void _drawShapeOfSelf();

		//--------------------------------------------
		// �ȉ��A�{�N���X�ŗL�̃����o
		//--------------------------------------------
	public:
		wPtr_CoordObj wPtr_prnt;
		std::vector< wPtr_CoordObj >wPtr_chldrn;
		Eigen::Affine3f CrdTrs;
		bool visible_CoordChain = true;

		void DetachSelf_fromParentsCoordChain();
		void AttachSelf_toParentsCoordChain(std::weak_ptr<CoordChainObj> parent);


	protected:
		Eigen::Affine3f GetTf_root2self();
	};


}