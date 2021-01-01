#include "include/DrawingManager.h"
#include "include/DrawableObjectClass.h"
#include "include/glDrawExt.h"

#include <cstdio>
#include <iostream>
#include <vector>

#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#pragma comment( lib, "glew32.lib" )
#pragma comment( lib, "freeglut.lib" )

namespace SmplObjDrwLib {

	// extern ���Ȃ��ϐ�
	int mouseU_prv = 0;
	int mouseV_prv = 0;

	// extern �ŊO���t�@�C���Ɍ��J����ϐ�
	std::unique_ptr<DrawingManager> drwMngr;
	int mouseBtnSt[3];
	unsigned char key;
	
};



using namespace SmplObjDrwLib;

/////////////////////////////////////////////////////////////////////
//
//	CamClass
//

std::list<std::weak_ptr<CamClass> > CamClass::camInstanceList;

 // factory
std::shared_ptr<CamClass> CamClass::create(std::string name)
{
	auto ptr = std::shared_ptr<CamClass>(new CamClass(name));
//	ptr->addSelfToList();
	ptr->addSelfToCamList();
	return ptr;
}


// constructor
CamClass::CamClass(std::string name)
	: ISodlObjBase(name)
{
	//�@set default external cam param
	camPos = Eigen::Vector3f(1000.f, -1000.f, 1000.f);
	camTgt = Eigen::Vector3f(0.f, 0.f, -0.f);
	camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
}

void CamClass::addSelfToCamList()
{
	camInstanceList.push_back( 
		std::dynamic_pointer_cast<CamClass>(
			shared_from_this()
		)
	);
}

//================================================================
//
//	<Summary>		�J�����ɐ��ˉe�}�g���N�X��ݒ肷��
//	<Description>
//================================================================
void CamClass::SetPrjMtx_As2DView(
	float width,
	float height,
	float _near,
	float _far
)
{
	glPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0 , width, 0, height, _near, _far );
	glGetFloatv(GL_PROJECTION_MATRIX, m_prjection);

	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////
//
//	ViewPortClass
//

std::list<std::weak_ptr<ViewPortClass> > ViewPortClass::vpInstanceList;

//--------------------------------------------
// �t�@�N�g���֐�
//--------------------------------------------
std::shared_ptr<ViewPortClass> ViewPortClass::create(std::string name, int vpSizeX, int vpSizeY)
{
	auto ptr = std::shared_ptr<ViewPortClass>(new ViewPortClass(name, vpSizeX, vpSizeY));
	ptr->addSelfToVpList();

	// �쐬�����C���X�^���X���ŏ��̈�łȂ��ꍇ�A
	// �Ō�̍쐬�σC���X�^���X���Q�Ƃ��Ă���`��O���E���h��
	// �������̂��f�t�H���g�Q�ƂƂ���B
	auto itr_self = (--vpInstanceList.end());
	if (itr_self != vpInstanceList.begin())
	{
		ptr->spaceAttached = (--itr_self)->lock()->spaceAttached;
	}

	return ptr;
}

//--------------------------------------------
// �R���X�g���N�^
//--------------------------------------------
ViewPortClass::ViewPortClass(std::string name, int vpSizeX, int vpSizeY)
	:ISodlObjBase(name)
	,camAttached(NULL)
{
	vp_id = (int)vpInstanceList.size();
	//vpInstanceList.push_back( shared_from_this() );

	int num = vp_id + 1;

	width	= vpSizeX /num;
	height	= vpSizeY /num;
	left	= vpSizeX - width;
	bottom	= vpSizeY - height;

	attachCam( CamClass::create(DEFAULT_CAM) );
}

void ViewPortClass::addSelfToVpList()
{
	vpInstanceList.push_back(
		std::dynamic_pointer_cast<ViewPortClass>(
			shared_from_this()
			)
	);

}

void ViewPortClass::setVpSize(int left_, int bottom_, int width_, int height_)
{
	left	= left_;
	bottom	= bottom_;
	width	= width_;
	height	= height_;

	if (camAttached->isOrthoMode)
	{
		fitCamMatrixToOrthView();
	}
}

 std::shared_ptr<CamClass> ViewPortClass::getCam()
{
	 return camAttached;
}

void ViewPortClass::attachCam(std::shared_ptr<CamClass> cam)
{
	camAttached = cam;

	if (camAttached->isOrthoMode)
	{
		fitCamMatrixToOrthView();
	}
}

void ViewPortClass::fitCamMatrixToOrthView()
{
	widthHalfZoomed  = (width  >> 1) / camAttached->zoomRatio;
	heightHalfZoomed = (height >> 1) / camAttached->zoomRatio;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(
		-widthHalfZoomed, 
		 widthHalfZoomed,
		-heightHalfZoomed,
		 heightHalfZoomed,
		0,
		50000.f
	);
	glGetFloatv(GL_PROJECTION_MATRIX, camAttached->m_prjection);
}


/////////////////////////////////////////////////////////////////////
//
//	DrawingManager
//

//================================================================
//
//	<Summary>		�R���X�g���N�^
//	<Description>
//================================================================
DrawingManager::DrawingManager(int* argc, char** argv, int windowSizeX, int windowSizeY, std::string windowTitle) :
	_windowSizeX(windowSizeX),
	_windowSizeY(windowSizeY)
{
	// init Glut
	glutInit(argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(windowSizeX, windowSizeY);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutCreateWindow( windowTitle.c_str() );

	// init GLEW
	glewInit();

	glEnableClientState(GL_VERTEX_ARRAY);

	// �R�[���o�b�N�֐��̐ݒ�
	// ���ӁFdisplayfunc�̃R�[���o�b�N�֐���o�^���Ă����Ȃ��ƁA
	// glutMainLoopEvent()���R�[�������Ƃ��ɃG���[���������ăv���O�������I�����Ă��܂��B
	glutDisplayFunc( OnDispFunc );
	glutReshapeFunc( OnReshapeFunc);
	glutKeyboardFunc( OnKeyboardFunc );
	glutMouseFunc( OnMouseBtn );
	glutMotionFunc( OnMouseDrag );
	glutPassiveMotionFunc( OnMouseHover );
	glutMouseWheelFunc( OnMouseWheel);

	// �`���ԃx�N�^������������
	drawingSpace = sPtr_vector< sPtr_vector< sPtr_IDrawableObjBase > >(
		new std::vector< sPtr_vector< sPtr_IDrawableObjBase > >
	);
	// �f�t�H���g�`���Ԃ��쐬���ăx�N�^�Ƀv�b�V������
	drawingSpace->push_back(sPtr_vector< sPtr_IDrawableObjBase >(
		new std::vector< sPtr_IDrawableObjBase >)
	);

	//--------------------------------------------------------
	// init ViewPortData

	auto vpDiag = addViewPort(DIAG_VIEW);
	vpDiag->spaceAttached = (*drawingSpace)[0];

	// init ViewPortData
	//--------------------------------------------------------

	//�u�����h�̗L����
	glEnable(GL_BLEND);
	// �A���t�@�`�����l�����g�����J���[�u�����h��I��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glEnable(GL_LIGHTING);				// ���C�e�B���O�̗L����
//	glEnable(GL_COLOR_MATERIAL);		// ���C�e�B���O�L������glColor3d()�̗L����

}


//================================================================
//
//	<Summary>		�t�@�N�g���֐�(�V���O���g���p�^�[��)
//	<Description>
//================================================================
DrawingManager::TypeOfSelf* DrawingManager::initMngr(int* argc, char** argv, int windowSizeX, int windowSizeY, std::string windowTitle)
{
	static auto ptr = new TypeOfSelf( argc, argv , windowSizeX, windowSizeY, windowTitle);
	drwMngr.reset(ptr);

	return  ptr;

}


//================================================================
//
//	<Summary>		�`��֐�
//	<Description>
//================================================================
void DrawingManager::draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto itr = viewPorts.begin();
		itr != viewPorts.end();
		itr++)
	{
		auto vp = *itr;

		//------------------------
		// �r���[�|�[�g�̔z�u
		//------------------------
		glViewport(vp->left, vp->bottom, vp->width, vp->height);

		//------------------------
		// �������e�̐ݒ�(�J�����̓����p���ݒ�)
		//------------------------
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(vp->camAttached->m_prjection);

		//------------------------
		// �J�����̔z�u(�J�����̊O���p���ݒ�)
		//------------------------
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// �J������z�u����O��viewPort�ɉ���`��
		int _w = vp->widthHalfZoomed;
		int _h = vp->heightHalfZoomed;
		glColor3d(1.0, 1.0, 1.1); // white
		glRecti(-_w, -_h, _w, _w);
		glColor3d(0.0, 0.0, 0.1); // navy
		glRecti(-_w+1, -_h+1, _w-1, _w-1);
	

		// �J�����̔z�u
		gluLookAt(
			vp->camAttached->camPos.x(),		vp->camAttached->camPos.y(),		vp->camAttached->camPos.z(),	// camPos
			vp->camAttached->camTgt.x(),		vp->camAttached->camTgt.y(),		vp->camAttached->camTgt.z(),	// camTargetCentor
			vp->camAttached->camUpVec.x(),		vp->camAttached->camUpVec.y(),		vp->camAttached->camUpVec.z()	// upVec
		);

		//------------------------
		// �`��ΏۃI�u�W�F�N�g��`�悷��
		//------------------------

		// �`��Ώۂ̃O���E���h���L���ɐݒ肳��Ă��邩���`�F�b�N����
		// (�|�C���^�Q�Ɛ悪���ݒ�or�j������Ă����Ȃ���?)
		auto grnd = vp->spaceAttached.lock();
		if( NULL != grnd )
		{
			// �O���E���h�ɔz�u���ꂽ�I�u�W�F�N�g�����ɕ`�悷��
			for (auto obj = grnd->begin();
				obj != grnd->end();
				obj++)
			{
				(*obj)->draw();
			}
		}
	}

	glutSwapBuffers();

}

//================================================================
//
//	<Summary>		�`��X�V
//	<Description>
//================================================================
void DrawingManager::drawUpdt(void)
{
	draw();
	glutMainLoopEvent(); // glutMainLoop() �����ȓ��e�����{���Ă����
}

//================================================================
//
//	<Summary>		�J�����ړ�
//	<Description>
//================================================================
void DrawingManager::mvCam( Eigen::Vector3f mv )
{
	viewPorts[0]->camAttached->camPos += mv;
}

//================================================================
//
//	<Summary>		�r���[�|�[�g�̒ǉ�
//	<Description>
//================================================================
std::shared_ptr<ViewPortClass> DrawingManager::addViewPort(std::string name, int vpSizeX, int vpSizeY)
{
	// ������vpSize���ȗ����ꂽ�ꍇ�́AwindowSize���g�p����
	(vpSizeX == -1) ? vpSizeX = _windowSizeX : vpSizeX = vpSizeX;
	(vpSizeY == -1) ? vpSizeY = _windowSizeY : vpSizeY = vpSizeY;

	auto vp = ViewPortClass::create(name, vpSizeX, vpSizeY);
	viewPorts.push_back(vp);
	return vp;
}


//================================================================
//
//	<Summary>		�`���Ԃ̒ǉ�
//	<Description>
//================================================================
sPtr_vector< sPtr_IDrawableObjBase > DrawingManager::addDrawingSpace()
{
	drawingSpace->emplace_back(
		sPtr_vector< sPtr_IDrawableObjBase >(
			new std::vector< sPtr_IDrawableObjBase >()
		) 
	);
	return drawingSpace->back();
}

//================================================================
//
//	<Summary>		�`���Ԃւ̃I�u�W�F�N�g�̒ǉ�
//	<Description>
//================================================================
void DrawingManager::AddObj_ToDrwSpace(sPtr_IDrawableObjBase obj, int numGround)
{
	if (numGround < drawingSpace->size())
	{
		(*drawingSpace)[numGround]->push_back(obj);
	}
}


//================================================================
//
//	<Summary>		�`���Ԃւ̃I�u�W�F�N�g�c���[�̒ǉ�
//	<Description>
//================================================================
void DrawingManager::AddObjTree_ToDrwSpace(std::shared_ptr<CoordChainObj> obj, int numGround)
{
	if (numGround < drawingSpace->size())
	{
		// �󂯎����obj��`���Ԃɒǉ�����
		(*drawingSpace)[numGround]->push_back(obj);
		std::cout << obj->name << std::endl;

		// �q�v�f�ɑ΂��čċA����
		for (auto itr = obj->wPtr_chldrn.begin();
			itr != obj->wPtr_chldrn.end();
			itr++)
		{
			auto sPtr_child =  itr->lock();
			AddObjTree_ToDrwSpace( sPtr_child, numGround);
		}
	}
}

void DrawingManager::OnDispFunc()
{
	;
}

void DrawingManager::OnReshapeFunc(int x, int y)
{
	;
}


void DrawingManager::OnKeyboardFunc(unsigned char key, int u, int v)
{
	SmplObjDrwLib::key = key;

	//-------------------------------------
	// ���C�u�������[�U�p�R�[���o�b�N
	//-------------------------------------
	if (drwMngr->usrKeyboardFunc)
	{
		(*usrKeyboardFunc)(key, u, v);
	}
}

void DrawingManager::OnMouseBtn(int button, int state, int u, int v)
{
	mouseBtnSt[button] = state;

	//-------------------------------------
	// ���C�u�������[�U�p�R�[���o�b�N
	//-------------------------------------
	if (usrMouseBtnFunc)
	{
		(*usrMouseBtnFunc)(button, state, u, v);
	}
}


void DrawingManager::OnMouseDrag(int u, int v)
{
	const int du = u - mouseU_prv;
	const int dv = v - mouseV_prv;

	Eigen::Vector3f mv;

	if (mouseBtnSt[MOUSE_LEFT_BUTTON] == MOUSE_DOWN)
	{
		Eigen::Vector3f* cPos = &SmplObjDrwLib::drwMngr->viewPorts[0]->camAttached->camPos;
		Eigen::Vector3f* tPos = &SmplObjDrwLib::drwMngr->viewPorts[0]->camAttached->camTgt;
		Eigen::Vector3f dir_p2t = *cPos - *tPos;
		dir_p2t.normalize();
		Eigen::Vector3f uDir3D = dir_p2t.cross(UnitZ);
		Eigen::Vector3f vDir3D = dir_p2t.cross(uDir3D);

		*cPos += 2 * ((uDir3D * du) + (-vDir3D * dv));
	}

	mouseU_prv = u;
	mouseV_prv = v;

	//-------------------------------------
	// ���C�u�������[�U�p�R�[���o�b�N
	//-------------------------------------
	if (usrMouseDragFunc)
	{
		(*usrMouseDragFunc)(u, v);
	}
}

void DrawingManager::OnMouseHover(int u, int v)
{
	mouseU_prv = u;
	mouseV_prv = v;

	//-------------------------------------
	// ���C�u�������[�U�p�R�[���o�b�N
	//-------------------------------------
	if (usrMouseHoverFunc)
	{
		(*usrMouseHoverFunc)(u, v);
	}
}

void DrawingManager::OnMouseWheel(int wheelNum, int dir, int u, int v)
{
	// �f�t�H���g����:
	//  �f�t�H���gvp �̃f�t�H���gcam �ɑ΂��ăY�[���C���A�Y�[���A�E�g�����{����
	auto dfltCam = drwMngr->viewPorts[0]->getCam();

	// ����] �� �g��
	if (dir > 0)
	{
		dfltCam->zoomRatio *= 1.1;
	}
	// ����] �� �k��
	else
	{
		dfltCam->zoomRatio *= 0.9;
	}

	drwMngr->viewPorts[0]->attachCam(dfltCam);
}

void DrawingManager::SetKeyboardFunc(void(*func)(unsigned char key, int u, int v))
{
	usrKeyboardFunc = func;
}

void DrawingManager::SetMouseFunc(void(*func)(int button, int state, int u, int v))
{
	usrMouseBtnFunc=func;
}

void DrawingManager::SetMouseDrag(void(*func)(int u, int v))
{
	usrMouseDragFunc=func;
}

void DrawingManager::SetPassiveMotionFunc(void(*func)(int u, int v))
{
	usrMouseHoverFunc=func;
}

void (*DrawingManager::usrKeyboardFunc)(unsigned char key, int u, int v) = NULL;
void (*DrawingManager::usrMouseBtnFunc)(int button, int state, int u, int v) = NULL;
void (*DrawingManager::usrMouseDragFunc)(int u, int v) = NULL;
void (*DrawingManager::usrMouseHoverFunc)(int u, int v) = NULL;


//-- �ȉ����� ----------------------------------
//		glGetFloatv(GL_MODELVIEW_MATRIX, m);
