#include "include/GraphObj.h"

using namespace SmplObjDrwLib;



//================================================================
//
//	<Summry>		�R���X�g���N�^
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
//	<Summry>		�f�X�g���N�^
//	<Description>
//================================================================
GraphObj::~GraphObj()
{

}


//================================================================
//
//	<Summry>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr < GraphObj::TypeOfSelf > GraphObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent,
	float size_x,
	float size_y
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
	ptr->initSelf(size_x, size_y);

	return  ptr;
}

//================================================================
//
//	<Summry>		�����o�������֐�
//	<Description>
//================================================================
void GraphObj::initSelf(float size_x, float size_y)
{
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

	this->_dataToDraw = PointsWithAttributes::create(name + "_data", area);
	_dataToDraw->atrIdx_bar = 0;

	//-------------------------------
	// �w�i�̏�����
	//-------------------------------
	auto grph_back = this->back;
	grph_back->boxSize.x() = size_x;
	grph_back->boxSize.y() = size_y;
	grph_back->drawType = DRAWTYPE::POLYGON;
	copyColor4fv(color4fv::LGRAY, grph_back->color.fv4);

	//-------------------------------
	// �O���t�G���A�̏�����
	//-------------------------------
	auto grph_area = this->area;
	grph_area->boxSize.x() = 0.9 * size_x;
	grph_area->boxSize.y() = 0.7 * size_y;
	grph_area->boxSize.z() = 0;
	grph_area->CrdTrs.translation() = Eigen::Vector3f(0.05 * size_x, 0.10 * size_y, 0);
	grph_area->drawType = DRAWTYPE::POLYGON;
	copyColor4fv(color4fv::WHITE, grph_area->color.fv4);

	//-------------------------------
	// �O���t�^�C�g���̏�����
	//-------------------------------
	this->title->text = "\"" + name + "\"";
	this->title->size = 10;
	this->title->align = LabelAlign::CENTER;
	this->title->CrdTrs.translation() = Eigen::Vector3f(
		back->boxSize.x() / 2,
		back->boxSize.y() - this->title->size ,
		0
	);
	copyColor4fv(color4fv::BLACK, this->title->color.fv4);

	//-------------------------------
	// ���̏�����
	//-------------------------------
	copyColor4fv(color4fv::BLACK, this->xAxis->colorWire.fv4);
	copyColor4fv(color4fv::BLACK, this->yAxis->colorWire.fv4);

	this->xAxis->points.push_back(Eigen::Vector3f(0, 0, 0));
	this->xAxis->points.push_back(Eigen::Vector3f(area->boxSize.x(), 0, 0));

	this->yAxis->points.push_back(Eigen::Vector3f(0, 0, 0));
	this->yAxis->points.push_back(Eigen::Vector3f(0, area->boxSize.y(), 0));

	
	//-------------------------------
	// ���x���Q�̈ꊇ������
	//-------------------------------
	LabelObj::SetVpSizeToChildrenLabel(back, back->boxSize.x(), back->boxSize.y());

	//-------------------------------
	// x�����x���̏�����
	//-------------------------------
	// x�����x��
	this->xAxisLabel->text = "xLabel [unit]";
	this->xAxisLabel->size = 5;
	this->xAxisLabel->align = LabelAlign::CENTER;
	this->xAxisLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			this->area->boxSize.x() / 2,
			-xAxisLabel->size	,
			0
		);
	copyColor4fv(color4fv::BLACK, this->xAxisLabel->color.fv4);

	// x���ŏ����x��
	this->xMinLabel->text = "xMin";
	this->xMinLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			-xMaxLabel->size,
			0
		);

	// x���ő僉�x��
	this->xMaxLabel->text = "xMax";
	this->xMaxLabel->align = LabelAlign::RIGHT;
	this->xMaxLabel->CrdTrs.translation() = 
		Eigen::Vector3f(
			this->area->boxSize.x(),
			-xMaxLabel->size,
			0
		);

	//-------------------------------
	// y�����x���̏�����
	//-------------------------------
	// y�����x��
	this->yAxisLabel->text = "yLabel [unit]";
	this->yAxisLabel->size = 5;
	this->yAxisLabel->align = LabelAlign::CENTER;
	this->yAxisLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			this->area->boxSize.y()/2,
			0
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
			0
		);

	// y���ő僉�x��
	this->yMaxLabel->text = "xMax";
	this->yMaxLabel->align = LabelAlign::LEFT;
	this->yMaxLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			this->area->boxSize.y(),
			0
		);


	//-------------------------------
	// �v���b�g�ݒ�̏�����
	//-------------------------------
	// �v���b�g�F
	this->_dataToDraw->drawType = DRAWTYPE::WIRE;
	copyColor4fv(color4fv::BLUE, this->_dataToDraw->colorWire.fv4);

}

//================================================================
//
//	<Summry>		�v���b�g�ɑ΂���A�g���r���[�g�z���ǉ�����
//	<Description>
//================================================================
void GraphObj::AddAtrData(std::shared_ptr < std::deque<float> > atr)
{
	_dataToDraw->_sPtr_attributes.push_back(atr);
}


//================================================================
//
//	<Summry>		�v���b�g�f�[�^�̒ǉ�
//	<Description>	
//================================================================
void GraphObj::addData( Eigen::Vector3f &point )
{
	
	// �O���t�v���b�g�͈͂�AUTO�ɐݒ肳��Ă���ꍇ�A�ő�l�������X�V����
	if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { rangeMax.x() = max(rangeMax.x(), point.x()); }
	if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { rangeMax.y() = max(rangeMax.y(), point.y()); }
	if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { rangeMax.z() = max(rangeMax.z(), point.z()); }

	if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { rangeMin.x() = min(rangeMin.x(), point.x()); }
	if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { rangeMin.y() = min(rangeMin.y(), point.y()); }
	if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { rangeMin.z() = min(rangeMin.z(), point.z()); }


	_data.push_back( point );

	// �f�[�^�ێ����I�[�o�̏ꍇ�͍ŌẪf�[�^���̂Ă�
	if (_data.size() > dataNumMax)
	{
		_data.pop_front();
	}
}


//================================================================
//
//	<Summry>		���Ȍ`��`��
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
	xMaxLabel->text = std::to_string(rangeMax.x());
	xMinLabel->text = std::to_string(rangeMin.x());
	yMaxLabel->text = std::to_string(rangeMax.y());
	yMinLabel->text = std::to_string(rangeMin.y());

	// �����v���b�g�f�[�^���N���A
	if (auto sPtr_points = _dataToDraw->_sPtr_points)
	{
		sPtr_points->clear();

		// �����v���b�g�f�[�^���č\��
		for (auto itr = _data.begin();
			itr != _data.end();
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
}