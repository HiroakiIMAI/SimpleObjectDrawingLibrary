#include "include/IDrawableObjBase.h"

namespace SmplObjDrwLib {
	namespace color4fv {
		const float LLGRAY[4]	= { 0.9, 0.9, 0.9, 1.0 };
		const float LGRAY[4]	= { 0.8, 0.8, 0.8, 1.0 };
		const float GRAY[4]	= { 0.5, 0.5, 0.5, 1.0 };
		const float DGRAY[4]	= { 0.2, 0.2, 0.2, 1.0 };
		const float DDGRAY[4]	= { 0.1, 0.1, 0.1, 1.0 };

		const float BLACK[4]	= { 0.0, 0.0, 0.0, 1.0 };
		const float WHITE[4]	= { 1.0, 1.0, 1.0, 1.0 };

		const float RED[4]		= { 1.0, 0.0, 0.0, 1.0 };
		const float GREEN[4]	= { 0.0, 1.0, 0.0, 1.0 };
		const float BLUE[4]		= { 0.0, 0.0, 1.0, 1.0 };
	}
}

void SmplObjDrwLib::copyColor4fv(const float src[4], float* dst)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = src[3];
}

/** ***************************************************************
 * @brief HSV��RGB �F�ϊ�
 * <pre>
 * [�⑫]
 * �E�F��(h)�̒l���0.0�`360.0[deg]�As��v�̒l���0.0�`1.0
 * �E�F�����l��𒴂���ꍇ�͔���Ԃ�
 * </pre>
 ******************************************************************/
void SmplObjDrwLib::CvtHsv2Rgb(
	const float& h,	///< hue			[0,0, 360.0]
	const float& s,	///< saturation		[0.0, 1.0]
	const float& v,	///< value			[0.0, 1.0]
	const float& a,	///< alpha channel	[0.0, 1.0]
	float rgba_fv4[4]
)
{
	ST_COLOR* p = (ST_COLOR*)rgba_fv4;
	p->a = a;

	if(s < 0.0001)
	{
		p->r = p->g = p->b = v;
	}
	else
	{
		int		tmp_i = (int)floor(h/60.f);
		float	tmp_f = h/60.f - (float)tmp_i;
		float	tmp_m = v*(1.f - s);
		float	tmp_n = v*(1.f - s * tmp_f);
		float	tmp_k = v*(1.f - s * (1.f - tmp_f));

		switch(tmp_i)
		{
		case 0:
			p->r = v;
			p->g = tmp_k;
			p->b = tmp_m;
			break;
		case 1:
			p->r = tmp_n;
			p->g = v;
			p->b = tmp_m;
			break;
		case 2:
			p->r = tmp_m;
			p->g = v;
			p->b = tmp_k;
			break;
		case 3:
			p->r = tmp_m;
			p->g = tmp_n;
			p->b = v;
			break;
		case 4:
			p->r = tmp_k;
			p->g = tmp_m;
			p->b = v;
			break;
		case 5:
			p->r = v;
			p->g = tmp_m;
			p->b = tmp_n;
			break;
		default:
			p->r = 1.0;
			p->g = 1.0;
			p->b = 1.0;
			break;
		}
	}
}


/** ***************************************************************
 * @brief �ő�A�ŏ��l�Ő��������ꂽ�q�[�g�}�b�v�J���[���擾����
 * <pre>
 * [�⑫]
 * �E�A���t�@�l��1.0�ŌŒ�
 * �E�ő�-�ŏ� < 0.0001 �̏ꍇ�͗�O�Ƃ��ĐԌŒ��Ԃ�(0����΍�)
 * </pre>
 ******************************************************************/
void SmplObjDrwLib::GetNormalizedHeatColor(
	const float& dat,
	const float& max,
	const float& min,
	float rgba_fv4[4]
	)
{
	ST_COLOR* p = (ST_COLOR*)rgba_fv4;
	float dynmcRng = max - min;

	if( 0.0001 < dynmcRng )											// 0����΍�
	{
		// dat �����Ƀq�[�g�}�b�v�l�𐶐�����
		float dt = (dat - min) / dynmcRng * 270.f;					// �f�[�^��[0,270]�Ő�����
		dt = 270.f - dt;											// �������f�[�^��HSV�̐F��270�`0�x(����)�Ƀ}�b�s���O
		SmplObjDrwLib::CvtHsv2Rgb( dt, 1.0, 1.0, 1.0, rgba_fv4 );	// HSV�̐F���ɉ�����RGB�l���Z�b�g����
	}
	else															// 0����ɂȂ�ꍇ
	{
		// �����Z�b�g���Ă���
		p->r = p->a = 1.0;
		p->g = p->b = 0.0;
	}
}



using namespace SmplObjDrwLib;

IDrawableObjBase::IDrawableObjBase(std::string)
	: ISodlObjBase(name)
	, visible(true)
{
}


IDrawableObjBase::~IDrawableObjBase()
{
}

void IDrawableObjBase::RemoveSelf_FromDrawingSpace()
{
	if( auto sPtr = drawingSpace_belongTo.lock() )
	{
		(*sPtr).erase( this->id_readOnly );
		drawingSpace_belongTo.reset();
		drawingSpaceNum_belongTo = -1;
	}
}


