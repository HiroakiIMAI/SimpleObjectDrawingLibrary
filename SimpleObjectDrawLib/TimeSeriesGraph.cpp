#include "include/TimeSeriesGraph.h"

using namespace SmplObjDrwLib;


//================================================================
//
//	<Summry>		�R���X�g���N�^
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
//	<Summry>		�f�X�g���N�^
//	<Description>
//================================================================
TimeSeriesGraph::~TimeSeriesGraph()
{

}


//================================================================
//
//	<Summry>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr < TimeSeriesGraph::TypeOfSelf > TimeSeriesGraph::create(
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
void TimeSeriesGraph::initSelf(float size_x, float size_y)
{
	GraphObj::initSelf(size_x, size_y);
}


//================================================================
//
//	<Summry>		���Ȍ`��`��
//	<Description>	
//================================================================
void TimeSeriesGraph::_drawShapeOfSelf()
{
	refleshRangeAsScroll();

	// �`��͈͊O�̃f�[�^��\�����Ȃ����߂�
	// ��U�f�[�^��ޔ����Ă���팸����
	auto tmp = _data;
	_data.clear();
	int sizeDraw = min(dataNumToDraw, tmp.size());
	auto itr = tmp.end();
	for (int i = 0; i < sizeDraw; i++)
	{
		--itr;
		_data.push_back( *itr );
	}
	
	// �`�悷��
	GraphObj::_drawShapeOfSelf();

	// �ޔ������f�[�^�𕜋A������
	_data = tmp;
}


//================================================================
//
//	<Summry>		���n��v���b�g���X�N���[��������
//	<Description>	
//================================================================
void TimeSeriesGraph::refleshRangeAsScroll()
{
	if (_data.size() > dataNumToDraw)
	{
		rangeMin.x() = rangeMax.x() - dataNumToDraw;
	}
}
