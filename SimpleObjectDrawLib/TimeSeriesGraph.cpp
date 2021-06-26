#include "include/TimeSeriesGraph.h"

using namespace SmplObjDrwLib;


//================================================================
//
//	<Summary>		�R���X�g���N�^
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
//	<Summary>		�f�X�g���N�^
//	<Description>
//================================================================
TimeSeriesGraph::~TimeSeriesGraph()
{

}


//================================================================
//
//	<Summary>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr < TimeSeriesGraph::TypeOfSelf > TimeSeriesGraph::create(
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
void TimeSeriesGraph::initSelf(std::weak_ptr<CamClass> cam)
{
	GraphObj::initSelf(cam);
}


//================================================================
//
//	<Summary>		���Ȍ`��`��
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
		// �`��͈͊O�̃f�[�^��\�����Ȃ����߂�
		// ��U�f�[�^��ޔ����Ă���팸����
		auto tmp = line;
		line.clear();
		int sizeDraw = MIN(dataNumToDraw, tmp.size());
		auto itr = tmp.end();
		for (int i = 0; i < sizeDraw; i++)
		{
			--itr;
			line.push_front( *itr );
		}

	//	// �����Q�̃T�C�Y���`�F�b�N����-------------------------------------------------------debug
	//	for (auto atr = _dataToDraw->_sPtr_attributes.begin();
	//		atr != _dataToDraw->_sPtr_attributes.end();
	//		atr++
	//		)
	//	{
	//		if (*atr)
	//		{
	//			// �f�[�^�ێ����I�[�o�̏ꍇ�͍ŌẪf�[�^���̂Ă�
	//			if ((*atr)->data.size() > sizeDraw)
	//			{
	//				(*atr)->data.pop_front();
	//			}
	//		}
	//	}

		// �`�悷��
		GraphObj::_drawShapeOfSelf();

		// �ޔ������f�[�^�𕜋A������
		line = tmp;
	}
}


//================================================================
//
//	<Summary>		���n��v���b�g���X�N���[��������
//	<Description>
//================================================================
void TimeSeriesGraph::refleshRangeAsScroll()
{
	if (_lines.begin()->second.size() > dataNumToDraw)
	{
		drwRngMin.x() = drwRngMax.x() - dataNumToDraw;
	}
}
