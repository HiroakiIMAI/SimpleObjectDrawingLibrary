#include "include/GraphObj3D.h"
#include <iomanip>

using namespace SmplObjDrwLib;



/** ***************************************************************
 * @brief �R���X�g���N�^
 *
 ******************************************************************/
GraphObj3D::GraphObj3D(std::string name, std::weak_ptr<CoordChainObj> parent)
	: GraphObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
}

/** ***************************************************************
 * @brief �f�X�g���N�^
 *
 ******************************************************************/
GraphObj3D::~GraphObj3D()
{

}


/** ***************************************************************
 * @brief �t�@�N�g���֐�
 *
 ******************************************************************/
std::shared_ptr < GraphObj3D::TypeOfSelf > GraphObj3D::create(
	std::string name,
	Eigen::Vector3f graphBoxSize,
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
	ptr->initSelf( graphBoxSize );

	return  ptr;
}



/** ***************************************************************
 * @brief �����o�������֐�
 *
 ******************************************************************/
void GraphObj3D::initSelf(Eigen::Vector3f graphBoxSize)
{

	//-----------------------------------
	// �����o DrawableObj �̏�����
	//-----------------------------------
	// create
	this->back = BoxObj::create(name + "_back", std::dynamic_pointer_cast< GraphObj>(shared_from_this()));
	this->area = BoxObj::create(name + "_area", back);
	this->title = LabelSimple::create(name + "_title", back);

	this->xAxis = PointsObj::create(name + "_xAxis", area);
	this->yAxis = PointsObj::create(name + "_yAxis", area);

	this->xAxisLabel = LabelSimple::create(name + "_xLabel", area);
	this->yAxisLabel = LabelSimple::create(name + "_yLabel", area);

	this->xMaxLabel = LabelSimple::create(name + "_xMaxLabel", area);
	this->xMinLabel = LabelSimple::create(name + "_xMinLabel", area);

	this->yMaxLabel = LabelSimple::create(name + "_yMaxLabel", area);
	this->yMinLabel = LabelSimple::create(name + "_yMinLabel", area);

	this->AddPlotLine( _DEFAULT_PLOT );

	CreatePointCursol( area, color4fv::WHITE );

	//-------------------------------
	// �w�i�̏�����
	//-------------------------------
	auto grph_back = this->back;
	grph_back->boxSize.x() = graphBoxSize.x();
	grph_back->boxSize.y() = graphBoxSize.y();
	grph_back->boxSize.z() = graphBoxSize.z();
	grph_back->drawType = DRAWTYPE::WIRE;
	grph_back->CrdTrs.translation() = Eigen::Vector3f(0,0,0);
	grph_back->visible = false;
	copyColor4fv(color4fv::LGRAY, grph_back->color.fv4);

	//-------------------------------
	// �O���t�G���A�̏�����
	//-------------------------------
	auto grph_area = this->area;
	grph_area->boxSize.x() = graphBoxSize.x();
	grph_area->boxSize.y() = graphBoxSize.y();
	grph_area->boxSize.z() = graphBoxSize.z();
	grph_area->drawType = DRAWTYPE::WIRE;
	grph_area->CrdTrs.translation() = Eigen::Vector3f(10,10,10);
	grph_area->visible = false;
	copyColor4fv(color4fv::WHITE, grph_area->color.fv4);

	//-------------------------------
	// �O���t�^�C�g���̏�����
	//-------------------------------
	this->title->text = "\"" + name + "\"";
	this->title->size = 20;
	this->title->align = LabelAlign::CENTER;
	this->title->CrdTrs.translation() = Eigen::Vector3f(
		back->boxSize.x() / 2,
		back->boxSize.y() - this->title->size ,
		0
	);

	//-------------------------------
	// ���̏�����
	//-------------------------------
	copyColor4fv(color4fv::WHITE, this->xAxis->colorWire.fv4);
	copyColor4fv(color4fv::WHITE, this->yAxis->colorWire.fv4);

	this->xAxis->points.push_back(Eigen::Vector3f(0, 0, 0));
	this->xAxis->points.push_back(Eigen::Vector3f(area->boxSize.x(), 0, 0));
	copyColor4fv(color4fv::WHITE, xAxis->color.fv4);

	this->yAxis->points.push_back(Eigen::Vector3f(0, 0, 0));
	this->yAxis->points.push_back(Eigen::Vector3f(0, area->boxSize.y(), 0));
	copyColor4fv(color4fv::WHITE, yAxis->color.fv4);


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
			0
		);

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
	this->yAxisLabel->align = LabelAlign::CENTER;
	this->yAxisLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			this->area->boxSize.y()/2,
			0
		);
	this->yAxisLabel->CrdTrs.linear() =
		Eigen::AngleAxisf(M_PI / 2, UnitZ).matrix();


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

}

/** ***************************************************************
 * @brief �v���b�g�f�[�^�n��̒ǉ�
 *
 ******************************************************************/
int GraphObj3D::AddPlotLine( std::string lineName )
{
	int idx = GraphObj::AddPlotLine( lineName );

	// area�̌��_����`�悷��悤�ɃI�t�Z�b�g���N���A����
	this->_linesToDraw[lineName]->CrdTrs.translation() = Eigen::Vector3f(0,0,0);

	return idx;
}


