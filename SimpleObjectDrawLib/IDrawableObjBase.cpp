#include "include/IDrawableObjBase.h"

namespace SmplObjDrwLib {
	namespace color4fv {
		const float LLGRAY[4]	= { 0.9, 0.9, 0.9, 1.0 };
		const float LGRAY[4]	= { 0.8, 0.8, 0.8, 1.0 };
		const float GRAY[4]		= { 0.5, 0.5, 0.5, 1.0 };
		const float DGRAY[4]	= { 0.2, 0.2, 0.2, 1.0 };
		const float DDGRAY[4]	= { 0.1, 0.1, 0.1, 1.0 };

		const float BLACK[4]	= { 0.0, 0.0, 0.0, 1.0 };
		const float WHITE[4]	= { 1.0, 1.0, 1.0, 1.0 };

		const float RED[4]		= { 0.0, 0.0, 1.0, 1.0 };
		const float GREEN[4]	= { 0.0, 0.0, 1.0, 1.0 };
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


using namespace SmplObjDrwLib;

IDrawableObjBase::IDrawableObjBase(std::string)
	: ISodlObjBase(name)
	, visible(true)
{
}


IDrawableObjBase::~IDrawableObjBase()
{
}

