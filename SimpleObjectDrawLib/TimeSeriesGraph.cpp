#include "include/TimeSeriesGraph.h"

using namespace SmplObjDrwLib;


//================================================================
//
//	<Summry>		コンストラクタ
//	<Description>
//================================================================
TimeSeriesGraph::TimeSeriesGraph(std::string name, std::weak_ptr<CoordChainObj> parent)
	: GraphObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{

}

//================================================================
//
//	<Summry>		デストラクタ
//	<Description>
//================================================================
TimeSeriesGraph::~TimeSeriesGraph()
{

}


//================================================================
//
//	<Summry>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr < TimeSeriesGraph::TypeOfSelf > TimeSeriesGraph::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent,
	float size_x,
	float size_y
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
	ptr->initSelf(size_x, size_y);

	return  ptr;
}


//================================================================
//
//	<Summry>		メンバ初期化関数
//	<Description>
//================================================================
void TimeSeriesGraph::initSelf(float size_x, float size_y)
{
	GraphObj::initSelf(size_x, size_y);
}


//================================================================
//
//	<Summry>		自己形状描画
//	<Description>	
//================================================================
void TimeSeriesGraph::_drawShapeOfSelf()
{
	refleshRangeAsScroll();

	// 描画範囲外のデータを表示しないために
	// 一旦データを退避してから削減する
	auto tmp = _data;
	_data.clear();
	int sizeDraw = min(dataNumToDraw, tmp.size());
	auto itr = tmp.end();
	for (int i = 0; i < sizeDraw; i++)
	{
		--itr;
		_data.push_back( *itr );
	}
	
	// 描画する
	GraphObj::_drawShapeOfSelf();

	// 退避したデータを復帰させる
	_data = tmp;
}


//================================================================
//
//	<Summry>		時系列プロットをスクロールさせる
//	<Description>	
//================================================================
void TimeSeriesGraph::refleshRangeAsScroll()
{
	if (_data.size() > dataNumToDraw)
	{
		rangeMin.x() = rangeMax.x() - dataNumToDraw;
	}
}
