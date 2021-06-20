#include "include/GraphObj3D.h"
#include <iomanip>

using namespace SmplObjDrwLib;



/** ***************************************************************
 * @brief コンストラクタ
 *
 ******************************************************************/
GraphObj3D::GraphObj3D(std::string name, std::weak_ptr<CoordChainObj> parent)
	: GraphObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
}

/** ***************************************************************
 * @brief デストラクタ
 *
 ******************************************************************/
GraphObj3D::~GraphObj3D()
{

}


/** ***************************************************************
 * @brief ファクトリ関数
 *
 ******************************************************************/
std::shared_ptr < GraphObj3D::TypeOfSelf > GraphObj3D::create(
	std::string name,
	Eigen::Vector3f graphBoxSize,
	std::weak_ptr<CoordChainObj> parent
)
{
	// インスタンスの生成
	auto ptr = std::shared_ptr< TypeOfSelf >(
		new TypeOfSelf(name, parent)
		);

	// 親子関係の構築
	auto prnt = parent.lock();
	if (prnt != NULL)
	{
		prnt->wPtr_chldrn.push_back(ptr);
	}

	// メンバの初期化
	ptr->initSelf( graphBoxSize );

	return  ptr;
}



/** ***************************************************************
 * @brief メンバ初期化関数
 *
 ******************************************************************/
void GraphObj3D::initSelf(Eigen::Vector3f graphBoxSize)
{

	//-----------------------------------
	// メンバ DrawableObj の初期化
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
	// 背景の初期化
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
	// グラフエリアの初期化
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
	// グラフタイトルの初期化
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
	// 軸の初期化
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
	// ラベル群の一括初期化
	//-------------------------------
	LabelObj::SetPrjMtxSizeToChildrenLabel(back, back->boxSize.x(), back->boxSize.y());

	//-------------------------------
	// x軸ラベルの初期化
	//-------------------------------
	// x軸ラベル
	this->xAxisLabel->text = "xLabel [unit]";
	this->xAxisLabel->align = LabelAlign::CENTER;
	this->xAxisLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			this->area->boxSize.x() / 2,
			-xAxisLabel->size	,
			0
		);

	// x軸最小ラベル
	this->xMinLabel->text = "xMin";
	this->xMinLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			-xMaxLabel->size,
			0
		);

	// x軸最大ラベル
	this->xMaxLabel->text = "xMax";
	this->xMaxLabel->align = LabelAlign::RIGHT;
	this->xMaxLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			this->area->boxSize.x(),
			-xMaxLabel->size,
			0
		);

	//-------------------------------
	// y軸ラベルの初期化
	//-------------------------------
	// y軸ラベル
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


	// y軸最小ラベル
	this->yMinLabel->text = "xMin";
	this->yMinLabel->align = LabelAlign::LEFT;
	this->yMinLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			0,
			0
		);

	// y軸最大ラベル
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
 * @brief プロットデータ系列の追加
 *
 ******************************************************************/
int GraphObj3D::AddPlotLine( std::string lineName )
{
	int idx = GraphObj::AddPlotLine( lineName );

	// areaの原点から描画するようにオフセットをクリアする
	this->_linesToDraw[lineName]->CrdTrs.translation() = Eigen::Vector3f(0,0,0);

	return idx;
}


