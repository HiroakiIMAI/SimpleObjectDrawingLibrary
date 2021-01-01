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
//	<Summary>		�R���X�g���N�^
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
//	<Summary>		�t�@�N�g���֐�
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
//	<Summary>		���Ȍ`��̕`��
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
//	<Summary>		�R���X�g���N�^
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
//	<Summary>		�t�@�N�g���֐�
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
//	<Summary>		���Ȍ`��̕`��
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
//	<Summary>		�R���X�g���N�^
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
//	<Summary>		�t�@�N�g���֐�
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
//	<Summary>		���Ȍ`��̕`��
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
//	PointsWithAttibutes
//	�����f�[�^�t���_��
//

//================================================================
//
//	<Summary>		�R���X�g���N�^
//	<Description>
//================================================================
PointsWithAttributes::PointsWithAttributes(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
	_sPtr_points = std::make_shared< std::deque< Eigen::Vector3f> >();
}

//================================================================
//
//	<Summary>		�t�@�N�g���֐�
//	<Description>
//================================================================
std::shared_ptr<PointsWithAttributes> PointsWithAttributes::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< TypeOfSelf >(
		new TypeOfSelf(name, parent)
		);

	// �e�̘A�����W�n�ɍ쐬�����I�u�W�F�N�g���q�Ƃ��ēo�^����
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summary>		���Ȍ`��̕`��
//	<Description>
//================================================================
void PointsWithAttributes::_drawShapeOfSelf()
{
	if (drawType == POLYGON)
	{
		glBegin(GL_POLYGON);
		glColor4fv(this->color.fv4);
	}
	else if (drawType == WIRE)
	{
		glBegin(GL_LINE_STRIP);
		glColor4fv(this->colorWire.fv4);
	}

	// �_�Q���Z�b�g����Ă���ꍇ
	if (_sPtr_points)
	{
		// �_�Q���v���b�g����
		for (auto itr = _sPtr_points->begin();
			itr != _sPtr_points->end();
			itr++)
		{
			glVertex3f(itr->x(), itr->y(), itr->z());
		}
	}
	glEnd();


	glBegin(GL_LINES);
	// �_�Q���Z�b�g����Ă��āA�����o�[�̕\�����L���ȏꍇ
	if(_sPtr_points 
	&& ( -1 != atrIdx_bar )							// �o�[�\���p�̑���idx���ݒ肳��Ă���
	&& ( _sPtr_attributes.size() != atrIdx_bar)		// �w�肳�ꂽ����idx�̑����|�C���^�����݂���
	&& ( _sPtr_attributes[ atrIdx_bar ] )			// �w�肳�ꂽ����idx�̑����|�C���^�̎w���悪NULL�łȂ�
	)
	{
		const auto  _sPtr_attribute = _sPtr_attributes[atrIdx_bar];
		// �_�Q���v���b�g����
		for (int i = 0;
			(	( i < _sPtr_points->size() )
			&&	( i < _sPtr_attribute->size() )
			);
			i++)
		{
			const auto pt  = &(*_sPtr_points)[i];
			const auto atr = (*_sPtr_attribute)[i];
			const auto barEnd = (*pt) + (atr*(atrBarDirec));

			// �_���瑮���l�ɉ����������̃o�[��`�悷��
			glVertex3f(pt->x(), pt->y(), pt->z());
			glVertex3f(barEnd.x(), barEnd.y(), barEnd.z());
		}
		std::cout << _sPtr_points->size() << ", " << _sPtr_attribute->size() << std::endl;
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
//	<Summary>		�R���X�g���N�^
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
//	<Summary>		�t�@�N�g���֐�
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
//	<Summary>		���Ȍ`��̕`��
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
		x - (x*rate), x + ((vpSizeX - x)*rate), 
		y - (y*rate), y + ((vpSizeY - y)*rate),
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

//================================================================
//
//	<Summary>		�����o�̃��x���S�Ăɑ΂��ăO���t�T�C�Y��ʒm����
//	<Description>	
//					
//================================================================
void LabelObj::SetVpSizeToChildrenLabel(
	std::shared_ptr<CoordChainObj> obj,
	float size_x,
	float size_y
)
{
	// �^����ꂽ�I�u�W�F�N�g��LabelObj�Ȃ�΁AvpSize�ɃO���t�T�C�Y��ݒ肷��
	if (auto sPtr_labelObj = std::dynamic_pointer_cast<LabelObj>(obj))
	{
		sPtr_labelObj->vpSizeX = size_x;
		sPtr_labelObj->vpSizeY = size_y;
	}

	// �S�Ă̎q�v�f��T�����A�ċA����
	for (auto child = obj->wPtr_chldrn.begin();
		child != obj->wPtr_chldrn.end();
		child++)
	{
		auto sPtr_child = child->lock();
		if (sPtr_child)
		{
			SetVpSizeToChildrenLabel(sPtr_child, size_x, size_y);
		}
	}
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
