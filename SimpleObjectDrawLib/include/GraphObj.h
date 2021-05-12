#pragma once
#include "DrawableObjectClass.h"
#include "DrawingManager.h"

namespace SmplObjDrwLib {

	// �O���t�v���b�g�͈͂̐�����@�񋓌^
	enum struct GRAPH_RANGE_CTL {
		AUTO	= 0,
		FIXED	= 1,
	};

	/** ***************************************************************
	 * @brief �O���t�I�u�W�F�N�g
	 * <pre>  GraphObj
	 * </pre>
	 ******************************************************************/
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
			std::weak_ptr<CamClass> cam,
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

		Eigen::Vector3f rangeMax = Eigen::Vector3f(1, 1, 1);	// �v���b�g�f�[�^�̍ő�l�ێ��p(�n��ԋ���)
		Eigen::Vector3f rangeMin = Eigen::Vector3f(0, 0, 0);	// �v���b�g�f�[�^�̍ŏ��l�ێ��p(�n��ԋ���)
		long dataNumMax = 1000;
		bool normarize_direcVec = true;

		// �O���t�v���b�g�͈͂̐�����@
		struct {
			GRAPH_RANGE_CTL x = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL y = GRAPH_RANGE_CTL::AUTO;
			GRAPH_RANGE_CTL z = GRAPH_RANGE_CTL::AUTO;
		}rangeCtl;

		virtual int AddPlotLine( std::string lineName );
		virtual void DeletePlotLine( std::string lineName );

		virtual void AddData( 							// �v���b�g�f�[�^�̒ǉ�
			const Eigen::Vector3f &point,				// �v���b�g�f�[�^(���W�l)
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		int AddAtr(										// �A�g���r���[�g�f�[�^��̒ǉ�
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void AddAtrData( 								// �A�g���r���[�g�f�[�^�̒ǉ�
			int atrIdx,									// �A�g���r���[�gindex
			const float& atrDat,						// �A�g���r���[�g�f�[�^
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void AddPtVct( 									// �v���b�g�_�̕����x�N�g���ǉ�
			const Eigen::Vector3f &direcVector,			// �v���b�g�_�̕����x�N�g��
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void CnfgAtrDisp_PtColorIdx( 					// �_��̐F�Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
			int atrIdx,									// �A�g���r���[�gindex
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void CnfgAtrDisp_PtLnWdtIdx( 					// �v���b�g�_�̕��Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
			int atrIdx,									// �A�g���r���[�gindex
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void CnfgAtrDisp_BarIdx( 						// �o�[�����Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
			int atrIdx,									// �A�g���r���[�gindex
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void CnfgAtrDisp_BarColorIdx( 					// �o�[�̐F�Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
			int atrIdx,									// �A�g���r���[�gindex
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void CnfgAtrDisp_BarWidthIdx( 					// �o�[�̕��Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
			int atrIdx,									// �A�g���r���[�gindex
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void SetPlotLineColor( 							// �v���b�g�n��Ƀf�t�H���g�\���F��ݒ肷��
			const ST_COLOR& color,						// �ݒ�F
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void SetPlotLineOffset( 						// �v���b�g�n��̕\���ʒu�̃I�t�Z�b�g��ݒ肷��
			const Eigen::Vector3f& ofs,					// �I�t�Z�b�g�x�N�g��
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void SetPlotLineWidth( 							// �v���b�g�n��̐�����ݒ肷��
			const float& width,							// ����
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void SetPlotLineDrawType( 						// �v���b�g�n��̕`��^�C�v��ݒ肷��
			DRAWTYPE type,								// �`��^�C�v
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		void SetBarWidth( 								// �o�[�̃f�t�H���g�������w�肷��
			const float& width,							// ��
			std::string pltLineName						// �v���b�g�f�[�^�n��
		);

		int GetNumPlotLines(); 							// �v���b�g�n��̐����擾����


	protected:
		//std::deque< Eigen::Vector3f>									_data;			// �O���t�̃v���b�g�f�[�^
		std::map< std::string, std::deque<Eigen::Vector3f> >			_lines;			// �O���t�̃v���b�g�f�[�^
		//std::shared_ptr<PointsObj>									_dataToDraw;	// �O���t�̃v���b�g�f�[�^(�`��p)
		std::map< std::string, std::shared_ptr<PointsWithAttributes> >	_linesToDraw;	// �O���t�̃v���b�g�f�[�^(�`��p)

		std::shared_ptr<LabelObj>		xMaxLabel;		// x�ő�l���x��
		std::shared_ptr<LabelObj>		xMinLabel;		// x�ő�l���x��
		std::shared_ptr<LabelObj>		yMaxLabel;		// y�ŏ��l���x��
		std::shared_ptr<LabelObj>		yMinLabel;		// y�ŏ��l���x��

		virtual void initSelf(std::weak_ptr<CamClass> cam = std::weak_ptr<CamClass>() );

	};

}

