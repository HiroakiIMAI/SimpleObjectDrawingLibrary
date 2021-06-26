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

	this->AddPlotLine( _DEFAULT_PLOT );

	const int DEPTH_BACK				= -100;
	const int DEPTH_AREA_FROM_BACK		= 10;
	const int DEPTH_CONTENTS_FROM_AREA	= 10;

	//-------------------------------
	// カーソルの初期化
	//-------------------------------
	_CreatePointCursol( CURSOL_SEL::MAIN, xAxis, color4fv::RED );
	_CreatePointCursol( CURSOL_SEL::SUB1, xAxis, color4fv::GREEN );
	_CreatePointCursol( CURSOL_SEL::SUB2, xAxis, color4fv::BLUE );
	_cursol			.ptCenterLab->CrdTrs.translation() = Eigen::Vector3f(0,0,DEPTH_CONTENTS_FROM_AREA);
	_cursolSub[0]	.ptCenterLab->CrdTrs.translation() = Eigen::Vector3f(0,0,DEPTH_CONTENTS_FROM_AREA);
	_cursolSub[1]	.ptCenterLab->CrdTrs.translation() = Eigen::Vector3f(0,0,DEPTH_CONTENTS_FROM_AREA);


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
			DEPTH_CONTENTS_FROM_AREA + DEPTH_AREA_FROM_BACK
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
int GraphObj::AddPlotLine( std::string lineName )
{

	// I/F配列にデータ系列を追加
	_lines.insert( std::make_pair( lineName, std::deque< Eigen::Vector3f>() ) );

	// データ系列の生成
	int sz = _linesToDraw.size();
	auto ptr =	PointsWithAttributes::create(
					lineName,
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
	_linesToDraw.insert( std::make_pair( lineName, ptr ) );

	// グラフが既に描画空間に登録されている場合、
	// 追加したプロット系列も同一の描画空間に追加する。
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
 * @brief プロットデータ系列の削除
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
 * @brief プロットデータ(座標値)の追加
 *
 ******************************************************************/
void GraphObj::AddData(
	const Eigen::Vector3f& point,				// プロットデータ(座標値)
	std::string pltLineName						// プロットデータ系列名
	)
{
	if( _lines.count( pltLineName ) )					// プロットデータ系列の存在チェック
	{
		// データの最大/最小値を更新する
		datRngMax.x() = MAX(datRngMax.x(), point.x());
		datRngMax.y() = MAX(datRngMax.y(), point.y());
		datRngMax.z() = MAX(datRngMax.z(), point.z());

		datRngMin.x() = MIN(datRngMin.x(), point.x());
		datRngMin.y() = MIN(datRngMin.y(), point.y());
		datRngMin.z() = MIN(datRngMin.z(), point.z());

		// グラフプロット範囲がAUTOに設定されている場合、最大値を自動更新する
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { drwRngMax.x() = datRngMax.x(); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { drwRngMax.y() = datRngMax.y(); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { drwRngMax.z() = datRngMax.z(); }

		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { drwRngMin.x() = datRngMin.x(); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { drwRngMin.y() = datRngMin.y(); }
		if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { drwRngMin.z() = datRngMin.z(); }

		_lines[pltLineName].push_back( point );

		// データ保持数オーバの場合は最古のデータを捨てる
		if (_lines[pltLineName].size() > dataNumMax)
		{
			_lines[pltLineName].pop_front();
		}
	}
}



/** ***************************************************************
 * @brief アトリビュートデータ列を追加する
 *
 ******************************************************************/
int GraphObj::AddAtr(
	std::string attributeName,					// アトリビュート名
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		// アトリビュート列をインスタンス化
		auto sPtr = std::make_shared< SmplObjDrwLib::AttributeClass<float> >();

		// 最大サイズを設定
		sPtr->dataNumMax = dataNumMax;
		sPtr->atrName = attributeName;

		// 列の配列にセット
		_linesToDraw[pltLineName]->_sPtr_attributes.push_back(
			sPtr
		);
		// indexを返す
		return _linesToDraw[pltLineName]->_sPtr_attributes.size() - 1;
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
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		if( _linesToDraw[pltLineName]->_sPtr_attributes.size() > atrIdx )
		{
			_linesToDraw[pltLineName]->_sPtr_attributes[atrIdx]->AddAtrData(atrDat);
		}
	}
}

/** ***************************************************************
 * @brief プロット点の方向ベクトル追加を追加する
 *
 ******************************************************************/
void GraphObj::AddPtVct(
	const Eigen::Vector3f &direcVector,			// プロット点の方向ベクトル
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		auto dirVec = direcVector;
		if(normarize_direcVec)
		{
			dirVec.normalize();
		}
		_linesToDraw[pltLineName]->_sPtr_ptVctrs->push_back(dirVec);

		// データ数が最大値を超えていたら最古データを捨てる
		for( ; _linesToDraw[pltLineName]->_sPtr_ptVctrs->size() > dataNumMax; )
		{
			_linesToDraw[pltLineName]->_sPtr_ptVctrs->pop_front();
		}
	}
}


/** ***************************************************************
 * @brief 点列の色に割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_PtColorIdx(
	int atrIdx,									// アトリビュートindex
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->atrIdx_pointColor = atrIdx;
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
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->atrIdx_pointTickness = atrIdx;
	}
}



/** ***************************************************************
 * @brief バー長さに割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarIdx(
	int atrIdx,									// アトリビュートindex
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->atrIdx_bar = atrIdx;
	}
}

/** ***************************************************************
 * @brief バーの色に割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarColorIdx(
	int atrIdx,									// アトリビュートindex
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->atrIdx_barColor = atrIdx;
	}
}


/** ***************************************************************
 * @brief バーの幅に割り当てるアトリビュートindexを設定する
 *
 ******************************************************************/
void GraphObj::CnfgAtrDisp_BarWidthIdx(
	int atrIdx,									// アトリビュートindex
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->atrIdx_barWidth = atrIdx;
	}
}



/** ***************************************************************
 * @brief プロット系列にデフォルト表示色を設定する
 *
 ******************************************************************/
void GraphObj::SetPlotLineColor( 				// プロット系列にデフォルト表示色を設定する
	const ST_COLOR& color,						// 設定色
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->color = color;
		this->_linesToDraw[pltLineName]->colorWire = color;
	}
}

/** ***************************************************************
 * @brief プロット系列の表示位置のオフセット
 *
 ******************************************************************/
void GraphObj::SetPlotLineOffset( 				// プロット系列の表示位置のオフセット
	const Eigen::Vector3f& ofs,					// オフセットベクトル
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->CrdTrs.translation() = ofs;
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
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->pointTickness = width;
	}
}


/** ***************************************************************
 * @brief プロット系列の描画タイプを設定する
 ******************************************************************/
void GraphObj::SetPlotLineDrawType( 						// プロット系列の描画タイプを設定する
	DRAWTYPE type,								// 描画タイプ
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->drawType = type;
	}
}


/** ***************************************************************
 * @brief バーの表示有効フラグを設定する
 ******************************************************************/
void GraphObj::SetBarEnable( 					// バーの表示有効フラグをセットする
	const bool& enable,							// 有効フラグ
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))				// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->barEnable = enable;
	}
}


/** ***************************************************************
 * @brief バーのデフォルト幅を指定する
 ******************************************************************/
void GraphObj::SetBarWidth( 					// バーのデフォルト長さを指定する
	const float& width,							// 幅
	std::string pltLineName						// プロットデータ系列名
)
{
	if (_lines.count(pltLineName))					// プロットデータ系列の存在チェック
	{
		this->_linesToDraw[pltLineName]->barWidth = width;
	}
}


/** ***************************************************************
 * @brief プロット系列の数を取得する
 ******************************************************************/
int GraphObj::GetNumPlotLines()
{
	return  _linesToDraw.size();
}


/** ***************************************************************
 * @brief プロット系列の数を取得する
 ******************************************************************/
std::vector< std::shared_ptr< AttributeClass<float> > >
GraphObj::GetAttributes_Shared(
	std::string pltLineName					// プロットデータ系列名
)
{
	return _linesToDraw[ pltLineName ]->_sPtr_attributes;
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
	xAxis->points[1].y() = -drwRngMin.y() / (drwRngMax.y() - drwRngMin.y())* area->boxSize.y();

	yAxis->points[0].x() =
	yAxis->points[1].x() = -drwRngMin.x() / (drwRngMax.x() - drwRngMin.x())* area->boxSize.x();

	// 軸最[大/小]値の表示を更新
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2);
	ss << drwRngMax.x(); xMaxLabel->text = ss.str();	ss.str("");
	ss << drwRngMax.y(); yMaxLabel->text = ss.str(); ss.str("");
	ss << drwRngMin.x();	xMinLabel->text = ss.str(); ss.str("");
	ss << drwRngMin.y();	yMinLabel->text = ss.str(); ss.str("");

	// プロットデータ系列ループ
	for(auto pltMapItm = _linesToDraw.begin(); pltMapItm != _linesToDraw.end(); ++pltMapItm)
	{
		//auto pltLine = _linesToDraw[i];
		// データ系列の有効チェック
		if (auto sPtr_points = pltMapItm->second->_sPtr_points)
		{
			// 内部プロットデータをクリア
			sPtr_points->clear();

			// 内部プロットに対応するI/Fデータの存在チェック
			if( _lines.count( pltMapItm->first ) )
			{
				auto line = _lines[pltMapItm->first];
				// 内部プロットデータをI/Fデータから再構成
				for (auto	itr  = line.begin();
							itr != line.end();
						  ++itr
					)
				{
					auto point = *itr;

					// グラフプロット範囲がAUTOに設定されている場合
					// pointの軸座標値を軸最大値で正規化して描画エリアサイズに合わせる。
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.x) { point.x() = ((point.x() - drwRngMin.x()) / (drwRngMax.x() - drwRngMin.x())) * area->boxSize.x(); }
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.y) { point.y() = ((point.y() - drwRngMin.y()) / (drwRngMax.y() - drwRngMin.y())) * area->boxSize.y(); }
					if (GRAPH_RANGE_CTL::AUTO == rangeCtl.z) { point.z() = ((point.z() - drwRngMin.z()) / (drwRngMax.z() - drwRngMin.z())) * area->boxSize.z(); }

					sPtr_points->push_back(point);
				}
			}

			_cursol.pltLnName = pltMapItm->first;
		}
	}

}

/** ***************************************************************
 * @brief カーソルの生成
 *
 ******************************************************************/
void GraphObj::_CreatePointCursol( CURSOL_SEL slct, sPtr_CoordObj prnt, const float color[] )
{
	auto& csl = _SelectCursol( slct );
	// オブジェクト生成
	csl.ptCenterToDraw	= PointsObj::create		( name + "_cursol_center",		prnt				);
	csl.ptCenterLab		= LabelSimple::create	( name + "_cursol_label",		csl.ptCenterToDraw	);
	csl.atrLab			= LabelSimple::create	( name + "_cursol_atrLabel", 	csl.ptCenterLab		);
	csl.lineX = PointsObj::create( name + "_cursol_lineX", prnt  );
	csl.lineY = PointsObj::create( name + "_cursol_lineY", prnt  );
	csl.lineZ = PointsObj::create( name + "_cursol_lineZ", prnt  );

	// カーソルポイントの初期化
	csl.ptCenterToDraw->visible = false;
	csl.ptCenterLab->CrdTrs.translation() = Eigen::Vector3f( 0,0,0 );
	csl.ptCenterToDraw->points.push_back( Eigen::Vector3f(0,0,0) );
	csl.ptCenterToDraw->drawType = DRAWTYPE::POINT;
	copyColor4fv( color4fv::RED, csl.ptCenterToDraw->color.fv4 );

	// ラベルの初期化
	csl.ptCenterLab->CrdTrs.translation() = Eigen::Vector3f( 0,0,0 );
	copyColor4fv( color, csl.ptCenterLab->color.fv4 );

	// X軸平行線の初期化
	csl.lineX->visible = false;
	csl.lineX->points.push_back( Eigen::Vector3f(0,0,0) );
	csl.lineX->points.push_back( Eigen::Vector3f(area->boxSize.x(),0,0) );
	csl.lineX->drawType = DRAWTYPE::WIRE;
	copyColor4fv( color, csl.lineX->colorWire.fv4 );
	copyColor4fv( color, csl.lineX->color.fv4 );

	// Y軸平行線の初期化
	csl.lineY->visible = false;
	csl.lineY->points.push_back( Eigen::Vector3f(0,0,0) );
	csl.lineY->points.push_back( Eigen::Vector3f(0,area->boxSize.y(),0) );
	csl.lineY->drawType = DRAWTYPE::WIRE;
	copyColor4fv( color, csl.lineY->colorWire.fv4 );
	copyColor4fv( color, csl.lineY->color.fv4 );

	// Z軸平行線の初期化
	csl.lineZ->visible = false;
	csl.lineZ->points.push_back( Eigen::Vector3f(0,0,0) );
	csl.lineZ->points.push_back( Eigen::Vector3f(0,0,area->boxSize.z()) );
	csl.lineZ->drawType = DRAWTYPE::WIRE;
	copyColor4fv( color, csl.lineZ->colorWire.fv4 );
	copyColor4fv( color, csl.lineZ->color.fv4 );

	// デフォルト系列にカーソルを置く
	csl.pltLnName = _linesToDraw.begin()->second->name;
}


/** ***************************************************************
 * @brief カーソル選択用内部I/F
 *
 ******************************************************************/
GraphObj::ST_PointCursol& GraphObj::_SelectCursol( const CURSOL_SEL& sel )
{
	switch( sel )
	{
	case CURSOL_SEL::MAIN:
		return _cursol;
	case CURSOL_SEL::SUB1:
		return _cursolSub[0];
	case CURSOL_SEL::SUB2:
		return _cursolSub[1];
	}

	return *(GraphObj::ST_PointCursol*)NULL;
}




/** ***************************************************************
 * @brief カーソルを置くプロット系列を選択する
 *
 ******************************************************************/
void GraphObj::PutCursolToLine( const std::string &pltLineName , CURSOL_SEL slct )
{
	_SelectCursol( slct ).pltLnName = pltLineName;
}

/** ***************************************************************
 * @brief カーソルの情報更新
 *
 ******************************************************************/
void GraphObj::UpdtCursol( int idx , CURSOL_SEL slct )
{
	auto& csl = _SelectCursol( slct );

	if( idx == -1 )
	{
		idx = csl.idx_currentPt;
	}
	else
	{
		csl.idx_currentPt = idx;
	}

	//------------------------------------------
	// カーソルが指すデータを取得
	//------------------------------------------
	{
		// プロット系列名からプロット系列を取得
		auto line = _lines.find( csl.pltLnName );
		if( line != _lines.end() )
		{
			// プロット系列からプロット点を取得
			if( line->second.size() > idx )
			{
				csl.ptCenter = line->second[idx];
			}
		}
		else
		{
			// プロット系列がない場合は(0,0,0)で更新
			csl.ptCenter = Eigen::Vector3f(0,0,0);
		}
	}

	//------------------------------------------
	// カーソルが指すデータ(スケール調整済)を取得
	//------------------------------------------
	{
		// プロット系列名からプロット系列を取得
		auto lineToDraw = _linesToDraw.find( csl.pltLnName );
		if( lineToDraw != _linesToDraw.end() )
		{
			if( lineToDraw->second->_sPtr_points->size() > idx )
			{
				// プロット系列からプロット点を取得
				csl.ptCenterToDraw->points[0] = (*lineToDraw->second->_sPtr_points)[idx];

				// カーソルライン、カーソルポイント位置の更新
				csl.lineX->points[0] =
				csl.lineX->points[1] =

				csl.lineY->points[0] =
				csl.lineY->points[1] =

				csl.lineZ->points[0] =
				csl.lineZ->points[1] =
				csl.ptCenterToDraw->CrdTrs.translation() =
				csl.ptCenterToDraw->points[0];

				// カーソルラインの長さを再設定
				csl.lineX->points[0].x() = 0;
				csl.lineX->points[1].x() = area->boxSize.x();

				csl.lineY->points[0].y() = 0;
				csl.lineY->points[1].y() = area->boxSize.y();

				csl.lineZ->points[0].z() = 0;
				csl.lineZ->points[1].z() = area->boxSize.z();

				// カーソルラベルの更新
				csl.ptCenterLab->text =
						"( " +
						std::to_string( csl.ptCenter.x() ) + ", " +
						std::to_string( csl.ptCenter.y() ) + ", " +
						std::to_string( csl.ptCenter.z() ) + ")";

				// プロット系列に関連付けられたアトリビュートの値をラベルに書き出す
				csl.atrLab->text = "";												// 空文字列で初期化
				for( auto mpIt_atr  = lineToDraw->second->_sPtr_attributes.begin();
						  mpIt_atr != lineToDraw->second->_sPtr_attributes.end();
						++mpIt_atr )
				{
					if( (*mpIt_atr)->data.size() > idx )
					{
						csl.atrLab->text += "\n " +
							(*mpIt_atr)->atrName + " : " + std::to_string( (*mpIt_atr)->data[idx] );
					}
				}
			}
		}
		else
		{
			// プロット系列がない場合は(0,0,0)で更新
			csl.ptCenterToDraw->points[0] = Eigen::Vector3f(0,0,0);
		}
	}



}


/** ***************************************************************
 * @brief カーソルの表示設定
 *
 ******************************************************************/
void GraphObj::SetCursolViible( const bool fg, CURSOL_SEL slct )
{
	auto& csl = _SelectCursol( slct );

	csl.lineX->visible = fg;
	csl.lineY->visible = fg;
	csl.lineZ->visible = fg;
	csl.ptCenterToDraw->visible = fg;
	csl.ptCenterLab->visible = fg;
	csl.ptCenterToDraw->visible = fg;
	csl.atrLab->visible = fg;
}


/** ***************************************************************
 * @brief プロット系列のコピーデータを取得する
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
