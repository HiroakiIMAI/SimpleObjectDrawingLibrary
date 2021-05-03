#pragma once
#include "ISodlObjBase.h"

namespace SmplObjDrwLib {

	enum DRAWTYPE{
		WIRE = 0,
		POLYGON,
		WIRE_POLYGON,
		POINT,
	};

	namespace color4fv {
		extern const float LLGRAY[];
		extern const float LGRAY[];
		extern const float GRAY[];
		extern const float DGRAY[];
		extern const float DDGRAY[];

		extern const float BLACK[];
		extern const float WHITE[];
		extern const float RED[];
		extern const float GREEN[];
		extern const float BLUE[];
	}

	extern void copyColor4fv(const float src[4], float* dst);
	extern void CvtHsv2Rgb(
		const float& h,
		const float& s,
		const float& v,
		const float& a,
		float rgba_fv4[4]
	);

	extern void GetNormalizedHeatColor(
		const float& dat,
		const float& max,
		const float& min,
		float rgba_fv4[4]
		);


	union ST_COLOR{
		float fv4[4] = { 1,1,1,1 };
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};

	class IDrawableObjBase
		: public virtual ISodlObjBase
	{
		//--------------------------------------------
		// �R���X�g���N�^
		//--------------------------------------------
		// �p���N���X�O����̃C���X�^���X�����֎~����
		// ���t�@�N�g���̎g�p���������Ashared_ptr�ȊO��
		// �@�C���X�^���X��ێ������Ȃ��B
	protected:
		IDrawableObjBase(std::string name);

		//--------------------------------------------
		// �f�X�g���N�^
		//--------------------------------------------
	public:

		~IDrawableObjBase();

		//--------------------------------------------
		// DrawableObjBase�̎���
		//--------------------------------------------
	public:
		virtual void draw() = 0;

		//--------------------------------------------
		// �ȉ��A�{�N���X�ŗL�̃����o
		//--------------------------------------------
	public:
		bool visible;

		ST_COLOR color;
		ST_COLOR colorWire;

		DRAWTYPE drawType = WIRE;

	};

	//----------------------------------------------------
	// ���O��ԓ��ŗL���ȃG�C���A�X���`����
	//----------------------------------------------------
	// std::shared_ptr< IDrawableObjBase > �̒Z�k�`
	using sPtr_IDrawableObjBase = std::shared_ptr< IDrawableObjBase > ;

	// std::shared_ptr< std::vector<T> > �̒Z�k�`
	template <class T>
	using sPtr_vector = std::shared_ptr< std::vector<T> > ;

};

