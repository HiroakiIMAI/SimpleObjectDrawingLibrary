#include "include/DrawableObjectClass.h"
#include "include/glDrawExt.h"


#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#pragma comment( lib, "glew32.lib" )



using namespace SmplObjDrwLib;



/////////////////////////////////////////////////////////////////////
//
//	PlaneObj
//	����
//

//================================================================
//
//	<Summry>		�R���X�g���N�^
//	<Description>
//================================================================
PlaneObj::PlaneObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summry>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr<PlaneObj> PlaneObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< PlaneObj >(
		new PlaneObj(name, parent)
		);

	// �e�̘A�����W�n�ɍ쐬�����I�u�W�F�N�g���q�Ƃ��ēo�^����
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summry>		���Ȍ`��̕`��
//	<Description>
//================================================================
void PlaneObj::_drawShapeOfSelf()
{
	glBegin(GL_POLYGON);
	glColor4fv( this->color.fv4 );
	glVertex3f(0, 0, 0);
	glVertex3f(width, 0, 0);
	glVertex3f(width, height, 0);
	glVertex3f(0, height, 0);
	glEnd();

}




/////////////////////////////////////////////////////////////////////
//
//	Box
//	������
//

//================================================================
//
//	<Summry>		�R���X�g���N�^
//	<Description>
//================================================================
BoxObj::BoxObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summry>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr<BoxObj> BoxObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< BoxObj >(
		new BoxObj(name, parent)
		);

	// �e�̘A�����W�n�ɍ쐬�����I�u�W�F�N�g���q�Ƃ��ēo�^����
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summry>		���Ȍ`��̕`��
//	<Description>
//================================================================
void BoxObj::_drawShapeOfSelf()
{
	if ((drawType == POLYGON)
	||	(drawType == WIRE_POLYGON)
	)
	{
		glColor4fv(this->color.fv4);
		Eigen::Vector3f& b = boxSize;

		//����
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(0, b.y(), 0);
		glVertex3f(b.x(), b.y(), 0);
		glVertex3f(b.x(), 0, 0);
		glEnd();

		//���
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, b.z());
		glVertex3f(b.x(), 0, b.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(0, b.y(), b.z());
		glEnd();

		//y0��
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, b.z());
		glVertex3f(0, 0, 0);
		glVertex3f(b.x(), 0, 0);
		glVertex3f(b.x(), 0, b.z());
		glEnd();

		//y+��
		glBegin(GL_POLYGON);
		glVertex3f(0, b.y(), b.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(b.x(), b.y(), 0);
		glVertex3f(0, b.y(), 0);
		glEnd();

		//x0��
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, b.z());
		glVertex3f(0, b.y(), b.z());
		glVertex3f(0, b.y(), 0);
		glVertex3f(0, 0, 0);
		glEnd();

		//x+��
		glBegin(GL_POLYGON);
		glVertex3f(b.x(), 0, b.z());
		glVertex3f(b.x(), 0, 0);
		glVertex3f(b.x(), b.y(), 0);
		glVertex3f(b.x(), b.y(), b.z());
		glEnd();
	}

	if( (drawType == WIRE)
	||	(drawType == WIRE_POLYGON)
	)
	{
		glColor4fv(this->colorWire.fv4);
		drawBox(Eigen::Vector3f(0, 0, 0), boxSize);
	}

}


/////////////////////////////////////////////////////////////////////
//
//	Points
//	�_��
//

//================================================================
//
//	<Summry>		�R���X�g���N�^
//	<Description>
//================================================================
PointsObj::PointsObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summry>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr<PointsObj> PointsObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< PointsObj >(
		new PointsObj(name, parent)
		);

	// �e�̘A�����W�n�ɍ쐬�����I�u�W�F�N�g���q�Ƃ��ēo�^����
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summry>		���Ȍ`��̕`��
//	<Description>
//================================================================
void PointsObj::_drawShapeOfSelf()
{
	if(drawType == POLYGON)
	{
		glBegin(GL_POLYGON);
		glColor4fv(this->color.fv4);
	}
	else if(drawType == WIRE)
	{
		glBegin(GL_LINE_STRIP);
		glColor4fv(this->colorWire.fv4);
	}


	for (auto itr = points.begin();
		itr != points.end();
		itr++)
	{
		glVertex3f(itr->x(), itr->y(), itr->z());
	}

	glEnd();

}


/////////////////////////////////////////////////////////////////////
//
//	Label
//	�e�L�X�g���x��
//

//================================================================
//
//	<Summry>		�R���X�g���N�^
//	<Description>
//================================================================
LabelObj::LabelObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	copyColor4fv( color4fv::BLACK, color.fv4 );
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summry>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr<LabelObj> LabelObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< LabelObj >(
		new LabelObj(name, parent)
		);

	// �e�̘A�����W�n�ɍ쐬�����I�u�W�F�N�g���q�Ƃ��ēo�^����
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summry>		���Ȍ`��̕`��
//	<Description>
//================================================================
void LabelObj::_drawShapeOfSelf()
{
	
	////////////////////////////////////////////////
	// �X�g���[�N�T�C�Y�����]�ɂȂ�悤��
	// �ꎞ�I��projectionMatrix���C������
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// �������ň󎚂����ꍇ�̕����T�C�Y���擾����
	float rawHgt = glutStrokeHeight(GLUT_STROKE_ROMAN);

	// �����T�C�Y�����p�̊g�k�{�����擾����
	float rate = rawHgt / size;
	// �g�k�̒��S�ʒu���擾
	float x = GetTf_root2self().translation().x();
	float y = GetTf_root2self().translation().y();	
	glOrtho(
		x-(x*rate), x+((100-x)*rate), 
		y - (y*rate), y + ((100 - y)*rate),
		-1000, 1000);
	
	

	// name ���x���̕`��
	glColor4fv(color.fv4);			// �����F�̐ݒ�
	glRasterPos3d(0, 0, 0.);		// 0,0,0 �ʒu���X�^�[�g�ʒu�ɂ���

	////////////////////////////////////////////////
	// �����񃌃��_�����O�̓����ŃJ�[�\����i�߂邽�߂�
	// ModelViewMatrix�ɕ��A����
	glMatrixMode(GL_MODELVIEW);

	// �e�L�X�g�A���C�������g
	float rawLength = glutStrokeLengthf(GLUT_STROKE_ROMAN, (unsigned char*)text.c_str());
	float offset = 0;
	switch ( align )
	{
	case LabelAlign::RIGHT:
		offset = -rawLength;
		break;
	case LabelAlign::CENTER:
		offset = -rawLength / 2;
		break;
	case LabelAlign::LEFT:
	default:
		break;
	}
	glTranslatef(offset, 0, 0);

	// �����񃌃��_�����O
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(text.c_str()) );

	////////////////////////////////////////////////
	// projectionMatrix�𕜌�����
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	////////////////////////////////////////////////
	// ModelViewMatrix�ɕ��A����
	glMatrixMode(GL_MODELVIEW);

}



////////////////
//debug func
/*
void print_glMatlix(GLfloat* m)
{
printf("%f, %f, %f, %f\n", m[0], m[4], m[8], m[12]);
printf("%f, %f, %f, %f\n", m[1], m[5], m[9], m[13]);
printf("%f, %f, %f, %f\n", m[2], m[6], m[10], m[14]);
printf("%f, %f, %f, %f\n", m[3], m[7], m[11], m[15]);
printf("\n");
}
*/
