#include "include/glDrawExt.h"

#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

// ���W�n�V���{���̕`��
void SmplObjDrwLib::drawCoordSymbol(const unsigned char* name, float size)
{
	// ���W�n�V���{���`��̕`��
	glBegin(GL_LINES);
	glColor3d(1.0, 0.0, 0.0);	// Red
	glVertex3f(0, 0, 0);		// X Axis
	glVertex3f(size, 0, 0);		// X Axis

	glColor3d(0.0, 1.0, 0.0);	// Green
	glVertex3f(0, 0, 0);		// Y Axis
	glVertex3f(0, size, 0);		// Y Axis

	glColor3d(0.0, 0.0, 1.0);	// Blue
	glVertex3f(0, 0, 0);		// Y Axis
	glVertex3f(0, 0, size);		// Z Axis
	glEnd();


	// name ���x���̕`��
	glColor3d(1.0, 1.0, 1.0);	// White
	glRasterPos3d(0, 0, -10.);		//0,0,0�ʒu���X�^�[�g�ʒu�ɂ���
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, name);

}


//������
void SmplObjDrwLib::drawBox(Eigen::Vector3f origin, Eigen::Vector3f length)
{
	//����
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0);
	glVertex3f(length.x(), 0, 0);
	glVertex3f(length.x(), length.y(), 0);
	glVertex3f(0, length.y(), 0);
	glEnd();

	//���
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, length.z());
	glVertex3f(length.x(), 0, length.z());
	glVertex3f(length.x(), length.y(), length.z());
	glVertex3f(0, length.y(), length.z());
	glEnd();

	//��
	glBegin(GL_LINES);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, length.z());

	glVertex3f(length.x(), 0, 0);
	glVertex3f(length.x(), 0, length.z());

	glVertex3f(length.x(), length.y(), 0);
	glVertex3f(length.x(), length.y(), length.z());

	glVertex3f(0, length.y(), 0);
	glVertex3f(0, length.y(), length.z());

	glEnd();

}

void SmplObjDrwLib::cvtMatrix_Eigen2GL(const Eigen::Matrix4f &mtx, GLfloat m[16])
{
	m[0] = mtx(0, 0);
	m[1] = mtx(1, 0);
	m[2] = mtx(2, 0);
	m[3] = mtx(3, 0);

	m[4] = mtx(0, 1);
	m[5] = mtx(1, 1);
	m[6] = mtx(2, 1);
	m[7] = mtx(3, 1);

	m[8] = mtx(0, 2);
	m[9] = mtx(1, 2);
	m[10] = mtx(2, 2);
	m[11] = mtx(3, 2);

	m[12] = mtx(0, 3);
	m[13] = mtx(1, 3);
	m[14] = mtx(2, 3);
	m[15] = mtx(3, 3);
}




