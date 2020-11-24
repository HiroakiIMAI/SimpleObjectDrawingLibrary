#pragma once
#include "DrawableObjectClass.h"

namespace SmplObjDrwLib {

	// �O���t�v���b�g�͈͂̐�����@�񋓌^
	enum struct GRAPH_RANGE_CTL {
		AUTO	= 0,
		FIXED	= 1,
	};


	class GraphObj :
		public CoordChainObj
	{
		typedef GraphObj TypeOfSelf;
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		GraphObj(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~GraphObj();

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// DrawableObjBase�̎���
		//--------------------------------------------
	public:
		//virtual void draw();

		//--------------------------------------------
		// CoordChainObj.h
		//--------------------------------------------
	protected:
		virtual void _drawShapeOfSelf();


		//--------------------------------------------
		// �ȉ��A�{�N���X�ŗL�̃����o
		//--------------------------------------------
	public:
		std::shared_ptr<BoxObj>			back;			// �O���t�G���A�̔w�i
		std::shared_ptr<BoxObj>			area;			// �O���t�G���A�̓h��Ԃ�
		std::shared_ptr<LabelObj>		title;			// �O���t�^�C�g��
		std::shared_ptr<LabelObj>		xAxisLabel;		// x�����x��
		std::shared_ptr<LabelObj>		yAxisLabel;		// y�����x��
		//std::shared_ptr<LabelObj>		zAxisLabel;		// z�����x��

		std::shared_ptr<PointsObj>		xAxis;			// x��
		std::shared_ptr<PointsObj>		yAxis;			// y��

		Eigen::Vector3f rangeMax = Eigen::Vector3f(1, 1, 1);	// �v���b�g�f�[�^�̍ő�l�ێ��p
		Eigen::Vector3f rangeMin = Eigen::Vector3f(0, 0, 0);	// �v���b�g�f�[�^�̍ŏ��l�ێ��p
		long dataNumMax = 1000;

		virtual void addData( Eigen::Vector3f &point );	// �v���b�g�f�[�^�̒ǉ�


		// �O���t�v���b�g�͈͂̐�����@
		struct {
			GRAPH_RANGE_CTL x = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL y = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL z = GRAPH_RANGE_CTL::AUTO;
		}rangeCtl;

	protected:
		std::deque< Eigen::Vector3f>	_data;			// �O���t�̃v���b�g�f�[�^
		std::shared_ptr<PointsObj>		_dataToDraw;	// �O���t�̃v���b�g�f�[�^(�`��p)
		
		std::shared_ptr<LabelObj>		xMaxLabel;		// x�ő�l���x��
		std::shared_ptr<LabelObj>		xMinLabel;		// x�ő�l���x��
		std::shared_ptr<LabelObj>		yMaxLabel;		// y�ŏ��l���x��
		std::shared_ptr<LabelObj>		yMinLabel;		// y�ŏ��l���x��

		void initSelf();
	};

}
