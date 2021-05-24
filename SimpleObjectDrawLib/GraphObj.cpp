#include "include/GraphObj.h"
#include <iomanip>

using namespace SmplObjDrwLib;



//================================================================
//
//	<Summary>		�R���X�g���N�^
//	<Description>
//================================================================
GraphObj::GraphObj(std::string name, std::weak_ptr<CoordChainObj> parent)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
}

//================================================================
//
//	<Summary>		�f�X�g���N�^
//	<Description>
//================================================================
GraphObj::~GraphObj()
{

}


//================================================================
//
//	<Summary>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr < GraphObj::TypeOfSelf > GraphObj::create(
	std::string name,
	std::weak_ptr<CamClass> cam,
	std::weak_ptr<CoordChainObj> parent
)
{
	// �C���X�^���X�̐���
	auto ptr = std::shared_ptr< TypeOfSelf >(
		new TypeOfSelf(name, parent)
		);

	// �e�q�֌W�̍\�z
	auto prnt = parent.lock();
	if (prnt != NULL)
	{
		prnt->wPtr_chldrn.push_back(ptr);
	}

	// �����o�̏�����
	ptr->initSelf(cam);

	return  ptr;
}

//================================================================
//
//	<Summary>		�����o�������֐�
//	<Description>
//================================================================
void GraphObj::initSelf(std::weak_ptr<CamClass> cam)
{
	float size_x = 100.f;
	float size_y = 100.f;
	if (auto sPtr_cam = cam.lock())
	{
		size_x = sPtr_cam->prjMtxOrthWdt;
		size_y = sPtr_cam->prjMtxOrthHgt;
	}

	//-----------------------------------
	// �����o DrawableObj �̏�����
	//-----------------------------------
	// create
	this->back = BoxObj::create(name + "_back", std::dynamic_pointer_cast< GraphObj>(shared_from_this()));
	this->area = BoxObj::create(name + "_area", back);
	this->title = LabelObj::create(name + "_title", back);

	this->xAxis = PointsObj::create(name + "_xAxis", area);
	this->yAxis = PointsObj::create(name + "_yAxis", area);

	this->xAxisLabel = LabelObj::create(name + "_xLabel", area);
	this->yAxisLabel = LabelObj::create(name + "_yLabel", area);

	this->xMaxLabel = LabelObj::create(name + "_xMaxLabel", area);
	this->xMinLabel = LabelObj::create(name + "_xMinLabel", area);

	this->yMaxLabel = LabelObj::create(name + "_yMaxLabel", area);
	this->yMinLabel = LabelObj::create(name + "_yMinLabel", area);

	//this->_linesToDraw = PointsWithAttributes::create(name + "_data", area);
	this->AddPlotLine( _DEFAULT_PLOT );

	CreatePointCursol( area );

	const int DEPTH_BACK				= -100;
	const int DEPTH_AREA_FROM_BACK		= 10;
	const int DEPTH_CONTENTS_FROM_AREA	= 10;

	//-------------------------------
	// �w�i�̏�����
	//-------------------------------
	auto grph_back = this->back;
	grph_back->boxSize.x() = size_x-2;
	grph_back->boxSize.y() = size_y-2;
	grph_back->boxSize.z() = 1;
	grph_back->CrdTrs.translation() =
		Eigen::Vector3f( 1, 1, DEPTH_BACK );
	grph_back->drawType = POLYGON;
	copyColor4fv(color4fv::LGRAY, grph_back->color.fv4);
//	grph_back->visible = false;

	//-------------------------------
	// �O���t�G���A�̏�����
	//-------------------------------
	auto grph_area = this->area;
	grph_area->boxSize.x() = 0.9 * back->boxSize.x();
	grph_area->boxSize.y() = 0.7 * back->boxSize.y();
	grph_area->boxSize.z() = 1;
	grph_area->CrdTrs.translation() =
		Eigen::Vector3f(
			0.05 * back->boxSize.x(),
			0.10 * back->boxSize.y(),
			DEPTH_AREA_FROM_BACK
		);
	grph_area->drawType = POLYGON;
	copyColor4fv(color4fv::WHITE, grph_area->color.fv4);
//	grph_area->visible = false;

	//-------------------------------
	// �O���t�^�C�g���̏�����
	//-------------------------------
	this->title->text = "\"" + name + "\"";
	this->title->size = 20;
	this->title->align = LabelAlign::CENTER;
	this->title->CrdTrs.translation() = Eigen::Vector3f(
		back->boxSize.x() / 2,
		back->boxSize.y() - this->title->size ,
		DEPTH_CONTENTS_FROM_AREA
	);
	copyColor4fv(color4fv::BLACK, this->title->color.fv4);

	//-------------------------------
	// ���̏�����
	//-------------------------------
	copyColor4fv(color4fv::BLACK, this->xAxis->colorWire.fv4);
	copyColor4fv(color4fv::BLACK, this->yAxis->colorWire.fv4);

	this->xAxis->points.push_back(Eigen::Vector3f(0, 0, 0));
	this->xAxis->points.push_back(Eigen::Vector3f(area->boxSize.x(), 0, 0));
	this->xAxis->CrdTrs.translation() = Eigen::Vector3f( 0,0, DEPTH_CONTENTS_FROM_AREA );

	this->yAxis->points.push_back(Eigen::Vector3f(0, 0, 0));
	this->yAxis->points.push_back(Eigen::Vector3f(0, area->boxSize.y(), 0));
	this->yAxis->CrdTrs.translation() = Eigen::Vector3f( 0,0, DEPTH_CONTENTS_FROM_AREA );


	//-------------------------------
	// ���x���Q�̈ꊇ������
	//-------------------------------
	LabelObj::SetPrjMtxSizeToChildrenLabel(back, back->boxSize.x(), back->boxSize.y());

	//-------------------------------
	// x�����x���̏�����
	//-------------------------------
	// x�����x��
	this->xAxisLabel->text = "xLabel [unit]";
	this->xAxisLabel->align = LabelAlign::CENTER;
	this->xAxisLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			this->area->boxSize.x() / 2,
			-xAxisLabel->size	,
			DEPTH_CONTENTS_FROM_AREA
		);
	copyColor4fv(color4fv::BLACK, this->xAxisLabel->color.fv4);

	// x���ŏ����x��
	this->xMinLabel->text = "xMin";
	this->xMinLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			-xMaxLabel->size,
			DEPTH_CONTENTS_FROM_AREA
		);

	// x���ő僉�x��
	this->xMaxLabel->text = "xMax";
	this->xMaxLabel->align = LabelAlign::RIGHT;
	this->xMaxLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			this->area->boxSize.x(),
			-xMaxLabel->size,
			DEPTH_CONTENTS_FROM_AREA
		);

	//-------------------------------
	// y�����x���̏�����
	//-------------------------------
	// y�����x��
	this->yAxisLabel->text = "yLabel [unit]";
	this->yAxisLabel->align = LabelAlign::CENTER;
	this->yAxisLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			this->area->boxSize.y()/2,
			DEPTH_CONTENTS_FROM_AREA
		);
	this->yAxisLabel->CrdTrs.linear() =
		Eigen::AngleAxisf(M_PI / 2, UnitZ).matrix();

	copyColor4fv(color4fv::BLACK, this->yAxisLabel->color.fv4);

	// y���ŏ����x��
	this->yMinLabel->text = "xMin";
	this->yMinLabel->align = LabelAlign::LEFT;
	this->yMinLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			0,
			DEPTH_CONTENTS_FROM_AREA
		);

	// y���ő僉�x��
	this->yMaxLabel->text = "xMax";
	this->yMaxLabel->align = LabelAlign::LEFT;
	this->yMaxLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			this->area->boxSize.y(),
			DEPTH_CONTENTS_FROM_AREA
		);
}

/** ***************************************************************
 * @brief �v���b�g�f�[�^�n��̒ǉ�
 *
 ******************************************************************/
int GraphObj::AddPlotLine( std::string lineName )
{

	// I/F�z��Ƀf�[�^�n���ǉ�
	_lines.insert( std::make_pair( lineName, std::deque< Eigen::Vector3f>() ) );

	// �f�[�^�n��̐���
	int sz = _linesToDraw.size();
	auto ptr =	PointsWithAttributes::create(
					lineName,
					this->area
				);

	//----------------------
	// �f�[�^�n��̏�����
	//----------------------
	// �v���b�g�^�C�v
	ptr->drawType = DRAWTYPE::WIRE;
	// �n��̃f�t�H���g�`��F
	//  hsv��Ԃ�index�ɉ����ĐF����i�߂��F���쐬
	float color_hue   = (float)( (sz * 60) % 360 );
	float color_value = 1.f - ( (float)(sz / 6) / 4.f );
	ST_COLOR color;
	CvtHsv2Rgb( color_hue, 1.0, color_value, 1.0, color.fv4 );
	copyColor4fv(color.fv4, ptr->colorWire.fv4);
	// area ���10.0�����󂢐[�x�Ƀv���b�g����
	ptr->CrdTrs.translation() = Eigen::Vector3f(0,0,10);

	// �f�[�^�n���z��ɃZ�b�g����
	_linesToDraw.insert( std::make_pair( lineName, ptr ) );

	// �O���t�����ɕ`���Ԃɓo�^����Ă���ꍇ�A
	// �ǉ������v���b�g�n�������̕`���Ԃɒǉ�����B
	if( -1 != drawingSpaceNum_belongTo )
	{
		drwMngr->AddObj_ToDrwSpace( ptr, drawingSpaceNum_belongTo );
	}
//	if( auto sPtr_dSpace = drawingSpace_belongTo.lock() )
//	{
//		//sPtr_dSpace->insert( std::make_pair(ptr->id_readOnly, ptr) );
//
//	}

	return sz;
}


/** ***************************************************************
 * @brief �v���b�g�f�[�^�n��̍폜
 *
 ******************************************************************/
void GraphObj::DeletePlotLine( std::string lineName )
{
	_lines.erase(lineName);
	if( _linesToDraw.count( lineName ) )
	{
		_linesToDraw[lineName]->RemoveSelfRecursive_FromDrawingSpace();
		_linesToDraw.erase(lineName);
	}
}




/** ***************************************************************
 * @brief �v���b�g�f�[�^(���W�l)�̒ǉ�
 *
 ******************************************************************/
void GraphObj::AddData(
	const Eigen::Vector3f& point,				// �v���b�g�f�[�^(���W�l)
	std::string pltLineName						// �v���b�g�f�[�^�n��
	)
{
	if( _lines.count( pltLineName ) )					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		// �O���t�v���b�g�͈͂�AUTO�ɐݒ肳��Ă���ꍇ�A�ő�l�������X�V����
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { rangeMax.x() = MAX(rangeMax.x(), point.x()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { rangeMax.y() = MAX(rangeMax.y(), point.y()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { rangeMax.z() = MAX(rangeMax.z(), point.z()); }

		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { rangeMin.x() = MIN(rangeMin.x(), point.x()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { rangeMin.y() = MIN(rangeMin.y(), point.y()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { rangeMin.z() = MIN(rangeMin.z(), point.z()); }


		_lines[pltLineName].push_back( point );

		// �f�[�^�ێ����I�[�o�̏ꍇ�͍ŌẪf�[�^���̂Ă�
		if (_lines[pltLineName].size() > dataNumMax)
		{
			_lines[pltLineName].pop_front();
		}
	}
}



/** ***************************************************************
 * @brief �A�g���r���[�g�f�[�^���ǉ�����
 *
 ******************************************************************/
int GraphObj::AddAtr(
	std::string attributeName,					// �A�g���r���[�g��
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		// �A�g���r���[�g����C���X�^���X��
		auto sPtr = std::make_shared< SmplObjDrwLib::AttributeClass<float> >();

		// �ő�T�C�Y��ݒ�
		sPtr->dataNumMax = dataNumMax;
		sPtr->atrName = attributeName;

		// ��̔z��ɃZ�b�g
		_linesToDraw[pltLineName]->_sPtr_attributes.push_back(
			sPtr
		);
		// index��Ԃ�
		return _linesToDraw[pltLineName]->_sPtr_attributes.size() - 1;
	}
	else
	{
		return -1;
	}
}



/** ***************************************************************
 * @brief �A�g���r���[�g�f�[�^��Ƀf�[�^��ǉ�����
 *
 ******************************************************************/
void GraphObj::AddAtrData(
	int atrIdx,									// �A�g���r���[�gindex
	const float& atrDat,						// �A�g���r���[�g�f�[�^
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		if( _linesToDraw[pltLineName]->_sPtr_attributes.size() > atrIdx )
		{
			_linesToDraw[pltLineName]->_sPtr_attributes[atrIdx]->AddAtrData(atrDat);
		}
	}
}

/** ***************************************************************
 * @brief �v���b�g�_�̕����x�N�g���ǉ���ǉ�����
 *
 ******************************************************************/
void GraphObj::AddPtVct(
	const Eigen::Vector3f &direcVector,			// �v���b�g�_�̕����x�N�g��
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		auto dirVec = direcVector;
		if(normarize_direcVec)
		{
			dirVec.normalize();
		}
		_linesToDraw[pltLineName]->_sPtr_ptVctrs->push_back(dirVec);

		// �f�[�^�����ő�l�𒴂��Ă�����ŌÃf�[�^���̂Ă�
		for( ; _linesToDraw[pltLineName]->_sPtr_ptVctrs->size() > dataNumMax; )
		{
			_linesToDraw[pltLineName]->_sPtr_ptVctrs->pop_front();
		}
	}
}


/** ***************************************************************
 * @brief �_��̐F�Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_PtColorIdx(
	int atrIdx,									// �A�g���r���[�gindex
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->atrIdx_pointColor = atrIdx;
	}
}



/** ***************************************************************
 * @brief �_��̐����Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
 * <pre>
 * �����̒l���0.5�`10.0 [pix]
 * </pre>
 ******************************************************************/
void GraphObj::CnfgAtrDisp_PtLnWdtIdx( 			// �v���b�g�_�̕��Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
	int atrIdx,									// �A�g���r���[�gindex
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->atrIdx_pointTickness = atrIdx;
	}
}



/** ***************************************************************
 * @brief �o�[�����Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarIdx(
	int atrIdx,									// �A�g���r���[�gindex
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->atrIdx_bar = atrIdx;
	}
}

/** ***************************************************************
 * @brief �o�[�̐F�Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarColorIdx(
	int atrIdx,									// �A�g���r���[�gindex
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->atrIdx_barColor = atrIdx;
	}
}


/** ***************************************************************
 * @brief �o�[�̕��Ɋ��蓖�Ă�A�g���r���[�gindex��ݒ肷��
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarWidthIdx(
	int atrIdx,									// �A�g���r���[�gindex
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->atrIdx_barWidth = atrIdx;
	}
}



/** ***************************************************************
 * @brief �v���b�g�n��Ƀf�t�H���g�\���F��ݒ肷��
 *
 ******************************************************************/
void GraphObj::SetPlotLineColor( 				// �v���b�g�n��Ƀf�t�H���g�\���F��ݒ肷��
	const ST_COLOR& color,						// �ݒ�F
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->color = color;
		this->_linesToDraw[pltLineName]->colorWire = color;
	}
}

/** ***************************************************************
 * @brief �v���b�g�n��̕\���ʒu�̃I�t�Z�b�g
 *
 ******************************************************************/
void GraphObj::SetPlotLineOffset( 				// �v���b�g�n��̕\���ʒu�̃I�t�Z�b�g
	const Eigen::Vector3f& ofs,					// �I�t�Z�b�g�x�N�g��
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->CrdTrs.translation() = ofs;
	}
}



/** ***************************************************************
 * @brief �v���b�g�n��̐�����ݒ肷��
 * <pre>
 * �����̒l���0.5�`10.0 [pix]
 * </pre>
 ******************************************************************/
void GraphObj::SetPlotLineWidth( 				// �v���b�g�n��̐�����ݒ肷��
	const float& width,							// ����
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->pointTickness = width;
	}
}


/** ***************************************************************
 * @brief �v���b�g�n��̕`��^�C�v��ݒ肷��
 ******************************************************************/
void GraphObj::SetPlotLineDrawType( 						// �v���b�g�n��̕`��^�C�v��ݒ肷��
	DRAWTYPE type,								// �`��^�C�v
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->drawType = type;
	}
}


/** ***************************************************************
 * @brief �o�[�̕\���L���t���O��ݒ肷��
 ******************************************************************/
void GraphObj::SetBarEnable( 					// �o�[�̕\���L���t���O���Z�b�g����
	const bool& enable,							// �L���t���O
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))				// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->barEnable = enable;
	}
}


/** ***************************************************************
 * @brief �o�[�̃f�t�H���g�����w�肷��
 ******************************************************************/
void GraphObj::SetBarWidth( 					// �o�[�̃f�t�H���g�������w�肷��
	const float& width,							// ��
	std::string pltLineName						// �v���b�g�f�[�^�n��
)
{
	if (_lines.count(pltLineName))					// �v���b�g�f�[�^�n��̑��݃`�F�b�N
	{
		this->_linesToDraw[pltLineName]->barWidth = width;
	}
}


/** ***************************************************************
 * @brief �v���b�g�n��̐����擾����
 ******************************************************************/
int GraphObj::GetNumPlotLines()
{
	return  _linesToDraw.size();
}



//================================================================
//
//	<Summary>		���Ȍ`��`��
//	<Description>	���ۂ̕`���ChildrenCoordObj�ɔC����B
//					�����ł�CheldrenCoordObj��draw()�̑O���������{����B
//================================================================
void GraphObj::_drawShapeOfSelf()
{
	// �������T�C�Y
	xAxis->points[0].x() = 0;
	xAxis->points[1].x() = area->boxSize.x();
	yAxis->points[0].y() = 0;
	yAxis->points[1].y() = area->boxSize.y();

	// �����Ĕz�u
	xAxis->points[0].y() =
	xAxis->points[1].y() = -rangeMin.y() / (rangeMax.y() - rangeMin.y())* area->boxSize.y();

	yAxis->points[0].x() =
	yAxis->points[1].x() = -rangeMin.x() / (rangeMax.x() - rangeMin.x())* area->boxSize.x();

	// ����[��/��]�l�̕\�����X�V
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2);
	ss << rangeMax.x(); xMaxLabel->text = ss.str();	ss.str("");
	ss << rangeMax.y(); yMaxLabel->text = ss.str(); ss.str("");
	ss << rangeMin.x();	xMinLabel->text = ss.str(); ss.str("");
	ss << rangeMin.y();	yMinLabel->text = ss.str(); ss.str("");

	// �v���b�g�f�[�^�n�񃋁[�v
	for(auto pltMapItm = _linesToDraw.begin(); pltMapItm != _linesToDraw.end(); ++pltMapItm)
	{
		//auto pltLine = _linesToDraw[i];
		// �f�[�^�n��̗L���`�F�b�N
		if (auto sPtr_points = pltMapItm->second->_sPtr_points)
		{
			// �����v���b�g�f�[�^���N���A
			sPtr_points->clear();

			// �����v���b�g�ɑΉ�����I/F�f�[�^�̑��݃`�F�b�N
			if( _lines.count( pltMapItm->first ) )
			{
				auto line = _lines[pltMapItm->first];
				// �����v���b�g�f�[�^��I/F�f�[�^����č\��
				for (auto	itr  = line.begin();
							itr != line.end();
						  ++itr
					)
				{
					auto point = *itr;

					// �O���t�v���b�g�͈͂�AUTO�ɐݒ肳��Ă���ꍇ
					// point�̎����W�l�����ő�l�Ő��K�����ĕ`��G���A�T�C�Y�ɍ��킹��B
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { point.x() = ((point.x() - rangeMin.x()) / (rangeMax.x() - rangeMin.x())) * area->boxSize.x(); }
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { point.y() = ((point.y() - rangeMin.y()) / (rangeMax.y() - rangeMin.y())) * area->boxSize.y(); }
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { point.z() = ((point.z() - rangeMin.z()) / (rangeMax.z() - rangeMin.z())) * area->boxSize.z(); }

					sPtr_points->push_back(point);
				}
			}

			_cursol.pltLnName = pltMapItm->first;
		}
	}

}

/** ***************************************************************
 * @brief �J�[�\���̐���
 *
 ******************************************************************/
void GraphObj::CreatePointCursol( sPtr_CoordObj prnt )
{
	// �I�u�W�F�N�g����
	_cursol.ptCenterToDraw	= PointsObj::create		( name + "_cursol_center",		prnt					);
	_cursol.ptCenterLab		= LabelSimple::create	( name + "_cursol_label",		_cursol.ptCenterToDraw	);
	_cursol.atrLab			= LabelSimple::create	( name + "_cursol_atrLabel", 	_cursol.ptCenterLab		);
	_cursol.lineX = PointsObj::create( name + "_cursol_lineX", prnt  );
	_cursol.lineY = PointsObj::create( name + "_cursol_lineY", prnt  );
	_cursol.lineZ = PointsObj::create( name + "_cursol_lineZ", prnt  );

	// �J�[�\���|�C���g�̏�����
	_cursol.ptCenterToDraw->visible = false;
	_cursol.ptCenterToDraw->points.push_back( Eigen::Vector3f(0,0,0) );
	_cursol.ptCenterToDraw->drawType = DRAWTYPE::POINT;
	copyColor4fv( color4fv::RED, _cursol.ptCenterToDraw->color.fv4 );

	// ���x���̏�����
	//_cursol.ptCenterLab->CrdTrs.translation() = Eigen::Vector3f(10,10,10);

	// X�����s���̏�����
	_cursol.lineX->visible = false;
	_cursol.lineX->points.push_back( Eigen::Vector3f(0,0,0) );
	_cursol.lineX->points.push_back( Eigen::Vector3f(area->boxSize.x(),0,0) );
	_cursol.lineX->drawType = DRAWTYPE::WIRE;
	copyColor4fv( color4fv::WHITE, _cursol.lineX->colorWire.fv4 );
	copyColor4fv( color4fv::WHITE, _cursol.lineX->color.fv4 );

	// Y�����s���̏�����
	_cursol.lineY->visible = false;
	_cursol.lineY->points.push_back( Eigen::Vector3f(0,0,0) );
	_cursol.lineY->points.push_back( Eigen::Vector3f(0,area->boxSize.y(),0) );
	_cursol.lineY->drawType = DRAWTYPE::WIRE;
	copyColor4fv( color4fv::WHITE, _cursol.lineY->colorWire.fv4 );
	copyColor4fv( color4fv::WHITE, _cursol.lineY->color.fv4 );

	// Z�����s���̏�����
	_cursol.lineZ->visible = false;
	_cursol.lineZ->points.push_back( Eigen::Vector3f(0,0,0) );
	_cursol.lineZ->points.push_back( Eigen::Vector3f(0,0,area->boxSize.z()) );
	_cursol.lineZ->drawType = DRAWTYPE::WIRE;
	copyColor4fv( color4fv::WHITE, _cursol.lineZ->colorWire.fv4 );
	copyColor4fv( color4fv::WHITE, _cursol.lineZ->color.fv4 );

	// �f�t�H���g�n��ɃJ�[�\����u��
	_cursol.pltLnName = _linesToDraw.begin()->second->name;
}



/** ***************************************************************
 * @brief �J�[�\����u���v���b�g�n���I������
 *
 ******************************************************************/
void GraphObj::PutCursolToLine( const std::string &pltLineName )
{
	_cursol.pltLnName= pltLineName;
}

/** ***************************************************************
 * @brief �J�[�\���̏��X�V
 *
 ******************************************************************/
void GraphObj::UpdtCursol( int idx )
{
	if( idx == -1 )
	{
		idx = _cursol.idx_currentPt;
	}
	else
	{
		_cursol.idx_currentPt = idx;
	}

	//------------------------------------------
	// �J�[�\�����w���f�[�^���擾
	//------------------------------------------
	{
		// �v���b�g�n�񖼂���v���b�g�n����擾
		auto line = _lines.find( _cursol.pltLnName );
		if( line != _lines.end() )
		{
			// �v���b�g�n�񂩂�v���b�g�_���擾
			if( line->second.size() > idx )
			{
				_cursol.ptCenter = line->second[idx];
			}
		}
		else
		{
			// �v���b�g�n�񂪂Ȃ��ꍇ��(0,0,0)�ōX�V
			_cursol.ptCenter = Eigen::Vector3f(0,0,0);
		}
	}

	//------------------------------------------
	// �J�[�\�����w���f�[�^(�X�P�[��������)���擾
	//------------------------------------------
	{
		// �v���b�g�n�񖼂���v���b�g�n����擾
		auto lineToDraw = _linesToDraw.find( _cursol.pltLnName );
		if( lineToDraw != _linesToDraw.end() )
		{
			if( lineToDraw->second->_sPtr_points->size() > idx )
			{
				// �v���b�g�n�񂩂�v���b�g�_���擾
				_cursol.ptCenterToDraw->points[0] = (*lineToDraw->second->_sPtr_points)[idx];

				// �J�[�\�����C���A�J�[�\���|�C���g�ʒu�̍X�V
				_cursol.lineX->points[0] =
				_cursol.lineX->points[1] =

				_cursol.lineY->points[0] =
				_cursol.lineY->points[1] =

				_cursol.lineZ->points[0] =
				_cursol.lineZ->points[1] =
				_cursol.ptCenterToDraw->CrdTrs.translation() =
				_cursol.ptCenterToDraw->points[0];

				// �J�[�\�����C���̒������Đݒ�
				_cursol.lineX->points[0].x() = 0;
				_cursol.lineX->points[1].x() = area->boxSize.x();

				_cursol.lineY->points[0].y() = 0;
				_cursol.lineY->points[1].y() = area->boxSize.y();

				_cursol.lineZ->points[0].z() = 0;
				_cursol.lineZ->points[1].z() = area->boxSize.z();

				// �J�[�\�����x���̍X�V
				_cursol.ptCenterLab->text =
						"( " +
						std::to_string( _cursol.ptCenter.x() ) + ", " +
						std::to_string( _cursol.ptCenter.y() ) + ", " +
						std::to_string( _cursol.ptCenter.z() ) + ")";

				// �v���b�g�n��Ɋ֘A�t����ꂽ�A�g���r���[�g�̒l�����x���ɏ����o��
				_cursol.atrLab->text = "";												// �󕶎���ŏ�����
				for( auto mpIt_atr  = lineToDraw->second->_sPtr_attributes.begin();
						  mpIt_atr != lineToDraw->second->_sPtr_attributes.end();
						++mpIt_atr )
				{
					if( (*mpIt_atr)->data.size() > idx )
					{
						_cursol.atrLab->text += "\n " +
							(*mpIt_atr)->atrName + " : " + std::to_string( (*mpIt_atr)->data[idx] );
					}
				}
			}
		}
		else
		{
			// �v���b�g�n�񂪂Ȃ��ꍇ��(0,0,0)�ōX�V
			_cursol.ptCenterToDraw->points[0] = Eigen::Vector3f(0,0,0);
		}
	}



}


/** ***************************************************************
 * @brief �J�[�\���̕\���ݒ�
 *
 ******************************************************************/
void GraphObj::SetCursolViible( const bool fg )
{
	_cursol.lineX->visible = fg;
	_cursol.lineY->visible = fg;
	_cursol.lineZ->visible = fg;
	_cursol.ptCenterToDraw->visible = fg;
}


/** ***************************************************************
 * @brief �v���b�g�n��̃R�s�[�f�[�^���擾����
 *
 ******************************************************************/
const std::deque<Eigen::Vector3f>* GraphObj::GetCpyPltLn( std::string pltName )
{
	auto line = _lines.find(pltName);
	if( line != _lines.end() )
	{
		return &(*line).second;
	}
	else
	{
		return NULL;
	}
}

const std::string GraphObj::_DEFAULT_PLOT = "default_plot";
