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


	/** ***************************************************************
	 *
	 * @brief	�����f�[�^�N���X
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

		// �A�g���r���[�g�f�[�^�ǉ����\�b�h
		virtual void AddAtrData( T dat )
		{
			// max, min �̍쐬
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
			// �f�[�^�̕ێ�
			_data.push_back( dat );

			// �f�[�^�����ő�l�𒴂��Ă�����ŌÃf�[�^���̂Ă�
			for( ; _data.size() > dataNumMax; )
			{
				_data.pop_front();
			}
		}

	};

	/** ***************************************************************
	 *
	 * @brief	�����f�[�^�t���_��
	 * <pre>	PointsWithAttibutes
	 * </pre>
	 ******************************************************************/
	class PointsWithAttributes : public virtual CoordChainObj
	{
		typedef PointsWithAttributes TypeOfSelf;

		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		PointsWithAttributes(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~PointsWithAttributes() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<TypeOfSelf>()
		);

		//--------------------------------------------
		// CoordChainObj�̕`��֐��̎���
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf() override;


		//--------------------------------------------
		// ���̑�
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
	//	�e�L�X�g���x��
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
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
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
		static void SetPrjMtxSizeToChildrenLabel(
			std::shared_ptr<CoordChainObj> obj,
			float size_x,
			float size_y
		);

		std::string text;						// ���x�����e�̃e�L�X�g
		float size = 15;						// ���x���̃t�H���g�T�C�Y(�c)[pix]
		float prjMtxRangeX = 100.f;				// �v���W�F�N�V�����}�g���b�N�X�̓��e��(ortho�O��)
		float prjMtxRangeY = 100.f;				// �v���W�F�N�V�����}�g���b�N�X�̓��e��(ortho�O��)
		LabelAlign align = LabelAlign::LEFT;	// ���x���̃A���C�������g
	};

	/////////////////////////////////////////////////////////////////////
	//
	//	LabelSimple
	//	�e�L�X�g���x��(�y�ʔ�)
	//

	class LabelSimple : public LabelObj
	{
		typedef LabelSimple TypeOfSelf;
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		//   �C���X�^���X��ێ������Ȃ��B
	protected:
		LabelSimple(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~LabelSimple() {};

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<LabelSimple> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// LabelObj�̕`��֐��̎���
		//--------------------------------------------
	public:
		virtual void _drawShapeOfSelf() override;

		//--------------------------------------------
		// LabelObj�̎������B��
		//--------------------------------------------
	private:
		static void SetPrjMtxSizeToChildrenLabel(
			std::shared_ptr<CoordChainObj> obj,
			float size_x,
			float size_y
		){}
		float prjMtxRangeX = 100.f;				// �v���W�F�N�V�����}�g���b�N�X�̓��e��(ortho�O��)
		float prjMtxRangeY = 100.f;				// �v���W�F�N�V�����}�g���b�N�X�̓��e��(ortho�O��)
		LabelAlign align = LabelAlign::LEFT;	// ���x���̃A���C�������g
	};

}

