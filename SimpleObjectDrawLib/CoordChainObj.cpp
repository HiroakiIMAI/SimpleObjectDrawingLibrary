#include "include/DrawableObjectClass.h"
#include "include/glDrawExt.h"

#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#pragma comment( lib, "glew32.lib" )

using namespace SmplObjDrwLib;


/** ***************************************************************
 * @brief �R���X�g���N�^
 *
 ******************************************************************/
CoordChainObj::CoordChainObj(std::string name, std::weak_ptr<CoordChainObj> parent)
	: ISodlObjBase(name)
	, IDrawableObjBase(name)
	, wPtr_prnt(parent)
	, wPtr_chldrn()
{
	CrdTrs.linear() = Eigen::Matrix3f::Identity();
	CrdTrs.translation() = Eigen::Vector3f(0, 0, 0);
}

/** ***************************************************************
 * @brief �f�X�g���N�^
 *
 ******************************************************************/
CoordChainObj::~CoordChainObj()
{

}


/** ***************************************************************
 * @brief �t�@�N�g���֐�
 *
 ******************************************************************/
std::shared_ptr<CoordChainObj> CoordChainObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< CoordChainObj >(
		new CoordChainObj(name, parent)
		);

	// �e�̘A�����W�n�ɍ쐬�����I�u�W�F�N�g���q�Ƃ��ēo�^����
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

/** ***************************************************************
 * @brief draw()
 *
 ******************************************************************/
void CoordChainObj::draw()
{
	if (!visible)
	{
		return;
	}

	GLfloat m[16];
	cvtMatrix_Eigen2GL(GetTf_root2self().matrix(), m);

	glPushMatrix();
	glMultMatrixf(m);

	// line parent to self
	if (visible_CoordChain)
	{
		auto prnt = this->wPtr_prnt.lock();
		if (NULL != prnt)
		{
			auto Tf_inv = CrdTrs.inverse();
			auto pos = Tf_inv.translation();

			float x = pos.x();
			float y = pos.y();
			float z = pos.z();

			glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(0, 0, 0);
			glVertex3f(pos.x(), pos.y(), pos.z());
			glEnd();
		}
	}

	// draw Shape of self
	_drawShapeOfSelf();

	glPopMatrix();

}


/** ***************************************************************
 * @brief ���W�n�A���̐e�Ƃ̊֌W���폜����
 *
 ******************************************************************/
void CoordChainObj::DetachSelf_fromParentsCoordChain()
{
	auto sPtr_prnt = wPtr_prnt.lock();
	if (NULL != sPtr_prnt)
	{
		// �e�̎q�����X�g��T��
		for (auto itr = sPtr_prnt->wPtr_chldrn.begin();
			itr != sPtr_prnt->wPtr_chldrn.end();
			itr++
			)
		{
			// �������g��������?
			if (itr->lock().get() == this)
			{
				// ���������X�g����폜���ă��[�v�𔲂���
				sPtr_prnt->wPtr_chldrn.erase(itr);
				break;
			}
		}
	}
}


/** ***************************************************************
 * @brief ���W�n�A���̐e�q�֌W�Ɏq��ǉ�����
 *
 ******************************************************************/
void CoordChainObj::AttachSelf_toParentsCoordChain(std::weak_ptr<CoordChainObj> parent)
{
	auto prnt = parent.lock();
	if (prnt != NULL)
	{
		prnt->wPtr_chldrn.push_back(std::dynamic_pointer_cast<CoordChainObj>(shared_from_this()));
		this->wPtr_prnt = parent;
	}
}


/** ***************************************************************
 * @brief ���Ȍ`��̕`��
 *
 ******************************************************************/
void CoordChainObj::_drawShapeOfSelf()
{
	drawCoordSymbol((unsigned char*)name.c_str(), 100.0);
}

//================================================================
//
//	<Summary>		GetTf_root2self()
//	<Description>
//================================================================
Eigen::Affine3f CoordChainObj::GetTf_root2self()
{
	auto prnt = wPtr_prnt.lock();
	if (prnt != NULL)
	{
		return (prnt->GetTf_root2self() * this->CrdTrs);
	}
	else
	{
		return this->CrdTrs;
	}
}

/** ***************************************************************
 * @brief �`���Ԃւ̃I�u�W�F�N�g�̒ǉ�
 *
 ******************************************************************/
void CoordChainObj::RemoveSelfRecursive_FromDrawingSpace()
{
	// ���g��`���Ԃ���폜
	this->RemoveSelf_FromDrawingSpace();

	// �q�ւ̍ċA����
	for( auto	it = wPtr_chldrn.begin();
				it != wPtr_chldrn.end();
			  ++it)
	{
		auto sPtr_child = it->lock();
		sPtr_child->RemoveSelfRecursive_FromDrawingSpace();
	}
}




