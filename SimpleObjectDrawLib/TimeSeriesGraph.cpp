#include "include/TimeSeriesGraph.h"

using namespace SmplObjDrwLib;


//================================================================
//
//	<Summary>		コンストラクタ
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
//	<Summary>		デストラクタ
//	<Description>
//================================================================
TimeSeriesGraph::~TimeSeriesGraph()
{

}


//================================================================
//
//	<Summary>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr < TimeSeriesGraph::TypeOfSelf > TimeSeriesGraph::create(
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
void TimeSeriesGraph::initSelf(std::weak_ptr<CamClass> cam)
{
	GraphObj::initSelf(cam);
}


//================================================================
//
//	<Summary>		自己形状描画
//	<Description>
//================================================================
void TimeSeriesGraph::_drawShapeOfSelf()
{
	refleshRangeAsScroll();

	for( auto mpIt_ln  = _lines.begin();
			  mpIt_ln != _lines.end();
			++mpIt_ln)
	{

		auto& line = mpIt_ln->second;
		// 描画範囲外のデータを表示しないために
		// 一旦データを退避してから削減する
		auto tmp = line;
		line.clear();
		int sizeDraw = MIN(dataNumToDraw, tmp.size());
		auto itr = tmp.end();
		for (int i = 0; i < sizeDraw; i++)
		{
			--itr;
			line.push_front( *itr );
		}

	//	// 属性群のサイズもチェックする-------------------------------------------------------debug
	//	for (auto atr = _dataToDraw->_sPtr_attributes.begin();
	//		atr != _dataToDraw->_sPtr_attributes.end();
	//		atr++
	//		)
	//	{
	//		if (*atr)
	//		{
	//			// データ保持数オーバの場合は最古のデータを捨てる
	//			if ((*atr)->data.size() > sizeDraw)
	//			{
	//				(*atr)->data.pop_front();
	//			}
	//		}
	//	}

		// 描画する
		GraphObj::_drawShapeOfSelf();

		// 退避したデータを復帰させる
		line = tmp;
	}
}


//================================================================
//
//	<Summary>		時系列プロットをスクロールさせる
//	<Description>
//================================================================
void TimeSeriesGraph::refleshRangeAsScroll()
{
	if (_lines.begin()->second.size() > dataNumToDraw)
	{
		drwRngMin.x() = drwRngMax.x() - dataNumToDraw;
	}
}
