/******************************************************************
/**
 * @file GraphObi3D.h
 * <pre>
 * 3D�O���t�I�u�W�F�N�g�N���X�̐錾
 * </pre>
 ******************************************************************/

#pragma once
#include "DrawableObjectClass.h"
#include "DrawingManager.h"
#include "GraphObj.h"

/******************************************************************
/**
 * @brief Simple Object Drawing Library �l�[���X�y�[�X
 *
 ******************************************************************/
namespace SmplObjDrwLib {


	/******************************************************************/
	/**
	 * @brief 3D�O���t�I�u�W�F�N�g�N���X
	 *
	 ******************************************************************/
	class GraphObj3D :
		public GraphObj
	{
		typedef GraphObj3D TypeOfSelf;
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		GraphObj3D(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~GraphObj3D();

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			Eigen::Vector3f graphBoxSize,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// DrawableObjBase�̎���
		//--------------------------------------------
	public:
		// virtual void draw();

		//--------------------------------------------
		// CoordChainObj.h
		//--------------------------------------------
	protected:
		// virtual void _drawShapeOfSelf() override;


		//--------------------------------------------
		// GraphObj.h �̃I�[�o���C�h
		//--------------------------------------------
	public:
		virtual int AddPlotLine( std::string lineName ) override;

	protected:
		virtual void initSelf(std::weak_ptr<CamClass> cam = std::weak_ptr<CamClass>() )override{};

		//--------------------------------------------
		// �ȉ��A�{�N���X�ŗL�̃����o
		//--------------------------------------------
	public:
		bool direc = false;

	public:
		virtual void initSelf(Eigen::Vector3f graphBoxSize);

	};

}

