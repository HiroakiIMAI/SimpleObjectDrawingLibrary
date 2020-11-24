#pragma once
#include "GraphObj.h"

namespace SmplObjDrwLib {

	class TimeSeriesGraph :
		public GraphObj
	{
		typedef TimeSeriesGraph TypeOfSelf;
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
	protected:
		TimeSeriesGraph(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:
		virtual ~TimeSeriesGraph();

		//--------------------------------------------
		// �t�@�N�g���֐�
		//--------------------------------------------
	public:
		static std::shared_ptr<TypeOfSelf> create(
			std::string name,
			std::weak_ptr<CoordChainObj> parent = std::weak_ptr<CoordChainObj>()
		);

		//--------------------------------------------
		// DrawableObjBase�̎���
		//--------------------------------------------
	public:
		//virtual void draw();

		//--------------------------------------------
		// CoordChainObj.h
		//--------------------------------------------
	protected:
		virtual void _drawShapeOfSelf();

		//--------------------------------------------
		// �ȉ��A�{�N���X�ŗL�̃����o
		//--------------------------------------------
	public:

		int dataNumToDraw = 100;

	protected:
		void initSelf();
		void refleshRangeAsScroll();

	};

}