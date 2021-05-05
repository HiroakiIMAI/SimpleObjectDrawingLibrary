#include "include/GraphObj.h"
#include <iomanip>

using namespace SmplObjDrwLib;



//================================================================
//
//	<Summary>		コンストラクタ
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
//	<Summary>		デストラクタ
//	<Description>
//================================================================
GraphObj::~GraphObj()
{

}


//================================================================
//
//	<Summary>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr < GraphObj::TypeOfSelf > GraphObj::create(
	std::string name,
	std::weak_ptr<CamClass> cam,
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
	ptr->initSelf(cam);

	return  ptr;
}

//================================================================
//
//	<Summary>		メンバ初期化関数
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
	// メンバ DrawableObj の初期化
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
	this->AddPlotLine();

	const int DEPTH_BACK				= -100;
	const int DEPTH_AREA_FROM_BACK		= 10;
	const int DEPTH_CONTENTS_FROM_AREA	= 10;

	//-------------------------------
	// 背景の初期化
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
	// グラフエリアの初期化
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
	// グラフタイトルの初期化
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
	// 軸の初期化
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
			DEPTH_CONTENTS_FROM_AREA
		);
	copyColor4fv(color4fv::BLACK, this->xAxisLabel->color.fv4);

	// x軸最小ラベル
	this->xMinLabel->text = "xMin";
	this->xMinLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			-xMaxLabel->size,
			DEPTH_CONTENTS_FROM_AREA
		);

	// x軸最大ラベル
	this->xMaxLabel->text = "xMax";
	this->xMaxLabel->align = LabelAlign::RIGHT;
	this->xMaxLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			this->area->boxSize.x(),
			-xMaxLabel->size,
			DEPTH_CONTENTS_FROM_AREA
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
			DEPTH_CONTENTS_FROM_AREA
		);
	this->yAxisLabel->CrdTrs.linear() =
		Eigen::AngleAxisf(M_PI / 2, UnitZ).matrix();

	copyColor4fv(color4fv::BLACK, this->yAxisLabel->color.fv4);

	// y軸最小ラベル
	this->yMinLabel->text = "xMin";
	this->yMinLabel->align = LabelAlign::LEFT;
	this->yMinLabel->CrdTrs.translation() =
		Eigen::Vector3f(
			0,
			0,
			DEPTH_CONTENTS_FROM_AREA
		);

	// y軸最大ラベル
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
 * @brief プロットデータ系列の追加
 *
 ******************************************************************/
int GraphObj::AddPlotLine()
{

	// I/F配列にデータ系列を追加
	_lines.push_back( std::deque< Eigen::Vector3f>() );

	// データ系列の生成
	int sz = _linesToDraw.size();
	auto ptr =	PointsWithAttributes::create(
					this->name+"_plot"+std::to_string(sz),
					this->area
				);

	//----------------------
	// データ系列の初期化
	//----------------------
	// プロットタイプ
	ptr->drawType = DRAWTYPE::WIRE;
	// 系列のデフォルト描画色
	//  hsv空間でindexに応じて色相を進めた色を作成
	float color_hue   = (float)( (sz * 60) % 360 );
	float color_value = 1.f - ( (float)(sz / 6) / 4.f );
	ST_COLOR color;
	CvtHsv2Rgb( color_hue, 1.0, color_value, 1.0, color.fv4 );
	copyColor4fv(color.fv4, ptr->colorWire.fv4);
	// area より10.0だけ浅い深度にプロットする
	ptr->CrdTrs.translation() = Eigen::Vector3f(0,0,10);

	// データ系列を配列にセットする
	_linesToDraw.push_back( ptr	);

	return sz;
}



/** ***************************************************************
 * @brief プロットデータ(座標値)の追加
 *
 ******************************************************************/
void GraphObj::AddData(
		const Eigen::Vector3f &point,				// プロットデータ(座標値)
		const int& pltLineIdx						// プロットデータ系列index
		)
{
	if( _lines.size() > pltLineIdx)					// プロットデータ系列の存在チェック
	{
		// グラフプロット範囲がAUTOに設定されている場合、最大値を自動更新する
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { rangeMax.x() = MAX(rangeMax.x(), point.x()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { rangeMax.y() = MAX(rangeMax.y(), point.y()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { rangeMax.z() = MAX(rangeMax.z(), point.z()); }

		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { rangeMin.x() = MIN(rangeMin.x(), point.x()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { rangeMin.y() = MIN(rangeMin.y(), point.y()); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { rangeMin.z() = MIN(rangeMin.z(), point.z()); }


		_lines[pltLineIdx].push_back( point );

		// データ保持数オーバの場合は最古のデータを捨てる
		if (_lines[pltLineIdx].size() > dataNumMax)
		{
			_lines[pltLineIdx].pop_front();
		}
	}
}



/** ***************************************************************
 * @brief アトリビュートデータ列を追加する
 *
 ******************************************************************/
int GraphObj::AddAtr(
	const int& pltLineIdx							// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)					// プロットデータ系列の存在チェック
	{
		// アトリビュート列をインスタンス化
		auto sPtr = std::make_shared< SmplObjDrwLib::AttributeClass<float> >();

		// 最大サイズを設定
		sPtr->dataNumMax = dataNumMax;

		// 列の配列にセット
		_linesToDraw[pltLineIdx]->_sPtr_attributes.push_back(
			sPtr
		);
		// indexを返す
		return _linesToDraw[pltLineIdx]->_sPtr_attributes.size() - 1;
	}
	else
	{
		return -1;
	}
}



/** ***************************************************************
 * @brief アトリビュートデータ列にデータを追加する
 *
 ******************************************************************/
void GraphObj::AddAtrData(
		int atrIdx,									// アトリビュートindex
		const float& atrDat,						// アトリビュートデータ
		const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		if( _linesToDraw[pltLineIdx]->_sPtr_attributes.size() > atrIdx )
		{
			_linesToDraw[pltLineIdx]->_sPtr_attributes[atrIdx]->AddAtrData(atrDat);
		}
	}
}

/** ***************************************************************
 * @brief プロット点の方向ベクトル追加を追加する
 *
 ******************************************************************/
void GraphObj::AddPtVct(
		const Eigen::Vector3f &direcVector,			// プロット点の方向ベクトル
		const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		auto dirVec = direcVector;
		if(normarize_direcVec)
		{
			dirVec.normalize();
		}
		_linesToDraw[pltLineIdx]->_sPtr_ptVctrs->push_back(dirVec);

		// データ数が最大値を超えていたら最古データを捨てる
		for( ; _linesToDraw[pltLineIdx]->_sPtr_ptVctrs->size() > dataNumMax; )
		{
			_linesToDraw[pltLineIdx]->_sPtr_ptVctrs->pop_front();
		}
	}
}


/** ***************************************************************
 * @brief 点列の色に割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_PtColorIdx(
		int atrIdx,									// アトリビュートindex
		const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->atrIdx_pointColor = atrIdx;
	}
}



/** ***************************************************************
 * @brief 点列の線幅に割り当てるアトリビュートindexを設定する
 * <pre>
 * 線幅の値域は0.5～10.0 [pix]
 * </pre>
 ******************************************************************/
void GraphObj::CnfgAtrDisp_PtLnWdtIdx( 			// プロット点の幅に割り当てるアトリビュートindexを設定する
	int atrIdx,									// アトリビュートindex
	const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)		// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->atrIdx_pointTickness = atrIdx;
	}
}



/** ***************************************************************
 * @brief バー長さに割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarIdx(
		int atrIdx,									// アトリビュートindex
		const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->atrIdx_bar = atrIdx;
	}
}

/** ***************************************************************
 * @brief バーの色に割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarColorIdx(
		int atrIdx,									// アトリビュートindex
		const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->atrIdx_barColor = atrIdx;
	}
}


/** ***************************************************************
 * @brief バーの幅に割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarWidthIdx(
		int atrIdx,									// アトリビュートindex
		const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->atrIdx_barWidth = atrIdx;
	}
}



/** ***************************************************************
 * @brief プロット系列にデフォルト表示色を設定する
 *
 ******************************************************************/
void GraphObj::SetPlotLineColor( 					// プロット系列にデフォルト表示色を設定する
	const ST_COLOR& color,							// 設定色
	const int& pltLineIdx							// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->color = color;
		this->_linesToDraw[pltLineIdx]->colorWire = color;
	}
}

/** ***************************************************************
 * @brief プロット系列の表示位置のオフセット
 *
 ******************************************************************/
void GraphObj::SetPlotLineOffset( 					// プロット系列の表示位置のオフセット
	const Eigen::Vector3f& ofs,						// オフセットベクトル
	const int& pltLineIdx							// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)			// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->CrdTrs.translation() = ofs;
	}
}



/** ***************************************************************
 * @brief プロット系列の線幅を設定する
 * <pre>
 * 線幅の値域は0.5～10.0 [pix]
 * </pre>
 ******************************************************************/
void GraphObj::SetPlotLineWidth( 				// プロット系列の線幅を設定する
	const float& width,							// 線幅
	const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)		// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->pointTickness = width;
	}
}


/** ***************************************************************
 * @brief プロット系列の描画タイプを設定する
 ******************************************************************/
void GraphObj::SetPlotLineDrawType( 						// プロット系列の描画タイプを設定する
	DRAWTYPE type,								// 描画タイプ
	const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)		// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->drawType = type;
	}
}


/** ***************************************************************
 * @brief バーのデフォルト長さを指定する
 ******************************************************************/
void GraphObj::SetBarWidth( 					// バーのデフォルト長さを指定する
	const float& width,							// 幅
	const int& pltLineIdx						// プロットデータ系列index
)
{
	if( _linesToDraw.size() > pltLineIdx)		// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineIdx]->barWidth = width;
	}
}


/** ***************************************************************
 * @brief プロット系列の数を取得する
 ******************************************************************/
int GraphObj::GetNumPlotLines()
{
	return  _linesToDraw.size();
}



//================================================================
//
//	<Summary>		自己形状描画
//	<Description>	実際の描画はChildrenCoordObjに任せる。
//					ここではCheldrenCoordObjのdraw()の前処理を実施する。
//================================================================
void GraphObj::_drawShapeOfSelf()
{
	// 軸をリサイズ
	xAxis->points[0].x() = 0;
	xAxis->points[1].x() = area->boxSize.x();
	yAxis->points[0].y() = 0;
	yAxis->points[1].y() = area->boxSize.y();

	// 軸を再配置
	xAxis->points[0].y() =
	xAxis->points[1].y() = -rangeMin.y() / (rangeMax.y() - rangeMin.y())* area->boxSize.y();

	yAxis->points[0].x() =
	yAxis->points[1].x() = -rangeMin.x() / (rangeMax.x() - rangeMin.x())* area->boxSize.x();

	// 軸最[大/小]値の表示を更新
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2);
	ss << rangeMax.x(); xMaxLabel->text = ss.str();	ss.str("");
	ss << rangeMax.y(); yMaxLabel->text = ss.str(); ss.str("");
	ss << rangeMin.x();	xMinLabel->text = ss.str(); ss.str("");
	ss << rangeMin.y();	yMinLabel->text = ss.str(); ss.str("");

	// プロットデータ系列ループ
	for( int i = 0; i<_linesToDraw.size(); ++i)
	{
		auto pltLine = _linesToDraw[i];
		// データ系列の有効チェック
		if (auto sPtr_points = pltLine->_sPtr_points)
		{
			// 内部プロットデータをクリア
			sPtr_points->clear();

			// 内部プロットに対応するI/Fデータの存在チェック
			if( _lines.size() > i)
			{
				// 内部プロットデータをI/Fデータから再構成
				for (auto itr = _lines[i].begin();
					itr != _lines[i].end();
					++itr
					)
				{
					auto point = *itr;

					// グラフプロット範囲がAUTOに設定されている場合
					// pointの軸座標値を軸最大値で正規化して描画エリアサイズに合わせる。
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { point.x() = ((point.x() - rangeMin.x()) / (rangeMax.x() - rangeMin.x())) * area->boxSize.x(); }
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { point.y() = ((point.y() - rangeMin.y()) / (rangeMax.y() - rangeMin.y())) * area->boxSize.y(); }
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { point.z() = ((point.z() - rangeMin.z()) / (rangeMax.z() - rangeMin.z())) * area->boxSize.z(); }

					sPtr_points->push_back(point);
				}
			}
		}
	}
}
