#pragma once

#include "CoordChainObj.h"

namespace SmplObjDrwLib {

	/////////////////////////////////////////////////////////////////////
	//
	//	PlaneObj
	//	����
	//

	class PlaneObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		PlaneObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~PlaneObj() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<PlaneObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<PlaneObj>()
		);

		//--------------------------------------------
		// CoordChainObj�̕`��֐��̎���
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// ���̑�
		//--------------------------------------------
	public:
		float width  = 30.;
		float height = 20.;


	};

	/////////////////////////////////////////////////////////////////////
	//
	//	Box
	//	������
	//

	class BoxObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		BoxObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~BoxObj() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<BoxObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<BoxObj>()
		);

		//--------------------------------------------
		// CoordChainObj�̕`��֐��̎���
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// ���̑�
		//--------------------------------------------
	public:
		Eigen::Vector3f boxSize;
		DRAWTYPE drawType = WIRE;
	};

	/////////////////////////////////////////////////////////////////////
	//
	//	Points
	//	�_��
	//

	class PointsObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		PointsObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~PointsObj() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<PointsObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<PointsObj>()
		);

		//--------------------------------------------
		// CoordChainObj�̕`��֐��̎���
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// ���̑�
		//--------------------------------------------
	public:
		std::deque<Eigen::Vector3f> points;
		DRAWTYPE drawType = WIRE;
	};

	/////////////////////////////////////////////////////////////////////
	//
	//	Label
	//	�e�L�X�g���x��
	//

	enum struct LabelAlign {
		LEFT = 0,
		CENTER,
		RIGHT,
	};

	class LabelObj : public virtual CoordChainObj
	{
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		LabelObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~LabelObj() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<LabelObj> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<PointsObj>()
		);

		//--------------------------------------------
		// LabelObj�̕`��֐��̎���
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// ���̑�
		//--------------------------------------------
	public:
		std::string text;						// ���x�����e�̃e�L�X�g
		float size = 5;								// ���x���̃t�H���g�T�C�Y
		LabelAlign align = LabelAlign::LEFT;	// ���x���̃A���C�������g
	};

}

