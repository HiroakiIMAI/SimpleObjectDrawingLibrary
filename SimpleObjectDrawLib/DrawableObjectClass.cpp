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
	else if(drawType == POINT)
	{
		glBegin(GL_POINTS);
		glColor4fv(this->color.fv4);
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
	_sPtr_ptVctrs = std::make_shared< std::deque< Eigen::Vector3f> >();
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

/** ***************************************************************
 * @brief ���Ȍ`��̕`��
 *
 ******************************************************************/
void PointsWithAttributes::_drawShapeOfSelf()
{
	// �_�Q���Z�b�g����Ă���ꍇ
	if (_sPtr_points)
	{
		// �f�t�H���g�F�̐ݒ�
		glColor4fv(this->color.fv4);

		GLfloat tick_bk;
		glGetFloatv( GL_LINE_WIDTH, &tick_bk );

		// �`�敝�ύX�p�̃t�@���N�^��p�ӂ���
		std::function<void(GLfloat)> Func_ChgTickness;
		if( drawType == POINT )
		{
			Func_ChgTickness = 	glPointSize;
		}
		else
		{
			Func_ChgTickness = 	glLineWidth;
		}
		Func_ChgTickness( this->pointTickness );

		// �`��^�C�v�ʂ�glBegin������p�ӂ���
		GLenum glBeginArg;
		if (drawType == POLYGON)
		{
			glBeginArg = GL_POLYGON;
		}
		else if (drawType == WIRE)
		{
			glBeginArg = GL_LINE_STRIP;
		}
		else if (drawType == POINT)
		{
			glBeginArg = GL_POINTS;
		}
		else
		{
			glBeginArg = GL_LINE_STRIP;
		}

		//-------------------------------------------------------------------
		// 1. ���W�_��̕`��
		//-------------------------------------------------------------------
		// �_�Q�T�C�Y���擾
		int size_points = _sPtr_points->size();

		// �F�w��̗L�����`�F�b�N
		int size_colors = 0;
		auto sPtr_AtrColors = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_pointColor )
		&&	( _sPtr_attributes[atrIdx_pointColor] )
		)
		{
			sPtr_AtrColors = _sPtr_attributes[atrIdx_pointColor];
			size_colors = sPtr_AtrColors->data.size();
		}

		// ���w��̗L�����`�F�b�N
		int size_tickness = 0;
		auto sPtr_AtrTickness = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_pointTickness )
		&&	( _sPtr_attributes[atrIdx_pointTickness] )
		)
		{
			sPtr_AtrTickness = _sPtr_attributes[atrIdx_pointTickness];
			size_tickness = sPtr_AtrTickness->data.size();
		}

		// �`��J�n
		glBegin( glBeginArg );

		// �_�Q�𑖍�
		for(int i = 0; i < size_points; ++i)
		{
			//----------------------------------------
			// �v���b�g�F�̐ݒ�
			//----------------------------------------
			// �F�w���̃f�[�^���g�p����ꍇ
			if(	( sPtr_AtrColors )	// �F�w��񂪗L���ȏꍇ
			&&	( i < size_colors )	// ���W�l�ɑΉ�����F�f�[�^������ꍇ
			)
			{
				ST_COLOR colorRgb;
				// �F�w���̃f�[�^����q�[�g�}�b�v�J���[���擾����
				SmplObjDrwLib::GetNormalizedHeatColor(
					sPtr_AtrColors->data[i],
					sPtr_AtrColors->max,
					sPtr_AtrColors->min,
					colorRgb.fv4);

				// �F�w���̃f�[�^���琶������RGB�l���g�p����B
				glColor4fv(colorRgb.fv4);
			}
			// �F�w���̃f�[�^���g�p���Ȃ��ꍇ
			else
			{
				// �_��N���X�ɐݒ肳�ꂽ�F���g�p����B
				glColor4fv(this->colorWire.fv4);
			}


			//----------------------------------------
			// �v���b�g���̐ݒ�
			//----------------------------------------
			// ���w���̃f�[�^���g�p����ꍇ
			if(	( sPtr_AtrTickness )	// �F�w��񂪗L���ȏꍇ
			&&	( i < size_tickness )	// ���W�l�ɑΉ�����F�f�[�^������ꍇ
			)
			{
				// �`�惂�[�h�����ł���ꍇ�́A��x���݂̓_�ʒu�܂Ő�������
				// �����I�����Ă��畝��ς��čēx�����n�߂邽�߂ɁA
				// ���̃^�C�~���O��vertex����T��
				if( drawType != POINT )
				{
					// �I�_���v���b�g����
					const auto& pt = (*_sPtr_points)[i];
					glVertex3f(pt.x(), pt.y(), pt.z());
				}

				//--------------------------------------------
				// ��x�`����I�����Ă��畝��ς��ĕ`��ĊJ����
				//--------------------------------------------
				// �`��I��
				glEnd();
				// �w��A�g���r���[�g��̃f�[�^�𕝂Ƃ��Đݒ�
				Func_ChgTickness( sPtr_AtrTickness->data[i] );

				// �`��ĊJ
				glBegin( glBeginArg );
				// ���̃u���b�N�̊O��glVertex3f�Ŏn�_���`�悳���B
			}

			// �_�Q���v���b�g����
			const auto& pt = (*_sPtr_points)[i];
			glVertex3f(pt.x(), pt.y(), pt.z());
		}
		glEnd();



		//-------------------------------------------------------------------
		// 2. �o�[�̕`��
		//-------------------------------------------------------------------
		// �f�t�H���g�F�̐ݒ�
		glColor4fv( this->color.fv4 );
		// �f�t�H���g���̐ݒ�
		glLineWidth( this->barWidth );

		// �F�w��̗L�����`�F�b�N
		int size_barColors = 0;
		auto sPtr_barColors = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_barColor )
		&&	( _sPtr_attributes[atrIdx_barColor] )
		)
		{
			sPtr_barColors = _sPtr_attributes[atrIdx_barColor];
			size_barColors = sPtr_barColors->data.size();
		}


		// ���w��̗L�����`�F�b�N
		int size_barWidth = 0;
		auto sPtr_barWidth = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_barWidth )
		&&	( _sPtr_attributes[atrIdx_barWidth] )
		)
		{
			sPtr_barWidth = _sPtr_attributes[atrIdx_barWidth];
			size_barWidth = sPtr_barWidth->data.size();
		}


		glBegin(GL_LINES);
		// �_�Q���Z�b�g����Ă��āA�����o�[�̕\�����L���ȏꍇ
		if(( ATRIDX_NONE != atrIdx_bar )							// �o�[�\���p�̑���idx���ݒ肳��Ă���
		&& ( _sPtr_attributes.size() != atrIdx_bar)		// �w�肳�ꂽ����idx�̑����|�C���^�z��v�f�����݂���
		&& ( _sPtr_attributes[ atrIdx_bar ] )			// �w�肳�ꂽ����idx�̑����|�C���^�̎w���悪NULL�łȂ�
		)
		{
			const auto  sPtr_attribute = _sPtr_attributes[atrIdx_bar];
			// �_�Q���v���b�g����
			for (int i = 0;
				(	( i < _sPtr_points->size() )
				&&	( i < sPtr_attribute->data.size() )
				);
				i++)
			{
				//-------------------------------------------------------------------
				// 2.1 �o�[�F�̎w��
				//-------------------------------------------------------------------
				if(	( sPtr_barColors )	// �F�w��񂪗L���ȏꍇ
				&&	( i < size_barColors )	// ���W�l�ɑΉ�����F�f�[�^������ꍇ
				)
				{
					// �F�f�[�^��F���Ƃ���HSV��RGB�ϊ�
					ST_COLOR colorRgb;
					// �F�w���̃f�[�^����q�[�g�}�b�v�J���[���擾����
					SmplObjDrwLib::GetNormalizedHeatColor(
						sPtr_barColors->data[i],
						sPtr_barColors->max,
						sPtr_barColors->min,
						colorRgb.fv4);
					glColor4fv(colorRgb.fv4);
				}

				//-------------------------------------------------------------------
				// 2.1 �o�[���̎w��
				//-------------------------------------------------------------------
				if(	( sPtr_barWidth )		// ���w��񂪗L���ȏꍇ
				&&	( i < size_barWidth )	// ���W�l�ɑΉ����镝�f�[�^������ꍇ
				)
				{
					glEnd();
					float mx = MAX( sPtr_barWidth->max, 0.0001);
					glLineWidth( sPtr_barWidth->data[i] / sPtr_barWidth->max * 10.0);
					glBegin(GL_LINES);
				}

				// �o�[�̕������Œ�l�ŏ���������
				auto direc = atrBarDirec;
				// �����x�N�g���z�񂪐ݒ肳��Ă���ꍇ�́A�z��̒l���g�p����
				if( _sPtr_ptVctrs->size() > i )
				{
					direc = (*_sPtr_ptVctrs)[i];
				}

				const auto  pt  = &(*_sPtr_points)[i];
				const auto& atr = sPtr_attribute->data[i];
				const auto  barEnd = (*pt) + ( atr * direc );

				// �_���瑮���l�ɉ����������̃o�[��`�悷��
				glVertex3f(pt->x(), pt->y(), pt->z());
				glVertex3f(barEnd.x(), barEnd.y(), barEnd.z());
			}
		}
		glEnd();

		glLineWidth( tick_bk );
	}

}



/////////////////////////////////////////////////////////////////////
//
//	Label
//	�e�L�X�g���x��
//

/** ***************************************************************
 * @brief �R���X�g���N�^
 *
 ******************************************************************/
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

/** ***************************************************************
 * @brief �t�@�N�g���֐�
 *
 ******************************************************************/
std::shared_ptr<LabelObj> LabelObj::create(
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

/** ***************************************************************
 * @brief ���Ȍ`��̕`��
 *
 ******************************************************************/
void LabelObj::_drawShapeOfSelf()
{

	////////////////////////////////////////////////
	// �X�g���[�N�T�C�Y�����]�ɂȂ�悤��
	// �ꎞ�I��projectionMatrix���C������
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//----------------------------------------------
	// �J�����̃Y�[���ւ̑Ή�
	//----------------------------------------------
	// �������ň󎚂����ꍇ�̕����T�C�Y���擾����
	const float rawHgt = glutStrokeHeight(GLUT_STROKE_ROMAN);
	// �����T�C�Y�����p�̊g�k�{�����擾����
	const float rate = rawHgt / size;
	// �g�k�̒��S�ʒu���擾
	const float x = GetTf_root2self().translation().x();
	const float y = GetTf_root2self().translation().y();

	glOrtho(
		x - (x*rate), x + ((prjMtxRangeX - x)*rate),
		y - (y*rate), y + ((prjMtxRangeY - y)*rate),
		CULLING_FORE, CULLING_FORE);

	// name ���x���̕`��
	glColor4fv(color.fv4);			// �����F�̐ݒ�
//	glColor4fv(color4fv::WHITE);	// �����F�̐ݒ�
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
void LabelObj::SetPrjMtxSizeToChildrenLabel(
	std::shared_ptr<CoordChainObj> obj,
	float size_x,
	float size_y
)
{
	// �^����ꂽ�I�u�W�F�N�g��LabelObj�Ȃ�΁AprjMtxRange�ɃT�C�Y��ݒ肷��
	if (auto sPtr_labelObj = std::dynamic_pointer_cast<LabelObj>(obj))
	{
		sPtr_labelObj->prjMtxRangeX = size_x;
		sPtr_labelObj->prjMtxRangeY = size_y;
	}

	// �S�Ă̎q�v�f��T�����A�ċA����
	for (auto child = obj->wPtr_chldrn.begin();
		child != obj->wPtr_chldrn.end();
		child++)
	{
		auto sPtr_child = child->lock();
		if (sPtr_child)
		{
			SetPrjMtxSizeToChildrenLabel(sPtr_child, size_x, size_y);
		}
	}
}



/////////////////////////////////////////////////////////////////////
//
//	LabelSimple
//	�e�L�X�g���x��(�y�ʔ�)
//

/** ***************************************************************
 * @brief �R���X�g���N�^
 *
 ******************************************************************/
LabelSimple::LabelSimple(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: LabelObj(name, parent)
	, CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	copyColor4fv( color4fv::WHITE, color.fv4 );
	visible_CoordChain = FALSE;
}

/** ***************************************************************
 * @brief �t�@�N�g���֐�
 *
 ******************************************************************/
std::shared_ptr<LabelSimple> LabelSimple::create(
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


/** ***************************************************************
 * @brief ���Ȍ`��̕`��
 *
 ******************************************************************/
void LabelSimple::_drawShapeOfSelf()
{
	// name ���x���̕`��
	glColor3d(1.0, 1.0, 1.0);	// White
	glRasterPos3d(0, 0, -10.);		//0,0,0�ʒu���X�^�[�g�ʒu�ɂ���
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (unsigned char*)text.c_str() );

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
