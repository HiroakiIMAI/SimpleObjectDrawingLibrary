#define  _USE_MATH_DEFINES

#include "include/SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawLib.lib" )

#include <math.h>
#include <iostream>


#define			WINDOW_SIZE_X			(640 * 2)
#define			WINDOW_SIZE_Y			(480)

#define			VP_SIZE_X				(WINDOW_SIZE_X/2)
#define			VP_SIZE_Y				(WINDOW_SIZE_Y)



namespace sodl = SmplObjDrwLib;

namespace app {

	// �O���[�o���ϐ�
	int mouseBtnSt[3];
	int mouseU_prv = 0;
	int mouseV_prv = 0;

	float Axez_X = 10;
	float Axez_Y = -200;
	float Axez_Z = -300;
	float Axez_B = 0;
	float Axez_C = 0;

	// Window�C���^���N�V�����֘A�R�[���o�b�N�֐�
	void keyFunc(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);

	// ���̑��̃T�u�֐�
	std::string GetModulePath();	// ���s�t�@�C���̃p�X���擾

};


//================================================================
//
//	<Summry>		�A�v���P�[�V�����̃G���g���|�C���g
//	<Description>
//================================================================
int main(int argc, char ** argv) 
{
	sodl::DrawingManager::initMngr( &argc, argv, WINDOW_SIZE_X, WINDOW_SIZE_Y);
	
	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetPassiveMotionFunc(app::onMouseHover);
	sodl::drwMngr->SetKeyboardFunc(app::keyFunc);
	

	//////////////////////////////////////////////////////
	//
	// �r���[�|�[�g�ƃJ�����̐ݒ�
	//

	// �r���[�|�[�g1
	auto vp1 = sodl::drwMngr->viewPorts[0];
	vp1->setVpSize(
		0,			// left
		0,			// bottom
		VP_SIZE_X,	// width
		VP_SIZE_Y	// height
	);
	auto cam1 = vp1->getCam();
	cam1->camPos = Eigen::Vector3f(600.f, -2000.f, -0.f);
	cam1->camTgt = Eigen::Vector3f(300.f, -300.f, -300.f);
	cam1->zoomRatio = 0.5;
	vp1->attachCam(cam1);

	// �r���[�|�[�g2
	auto vp2 = sodl::drwMngr->addViewPort("vp2");
	vp2->setVpSize(
		VP_SIZE_X,	// left
		0,			// bottom
		VP_SIZE_X,	// width
		VP_SIZE_Y	// height
	);
	auto cam2 = vp2->camAttached;
	cam2->camPos = Eigen::Vector3f(300.f, -300.f, 5000.f);
	cam2->camTgt = Eigen::Vector3f(300.f, -300.f, -300.f);
	cam2->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);


	
 	// ���n��O���t�̍쐬
	auto timeSerialGraph = sodl::TimeSeriesGraph::create("TimeSerealGraph");
	timeSerialGraph->rangeMax.x() = 0;
	timeSerialGraph->rangeMin.x() = 0;
	timeSerialGraph->rangeMax.y() = 100;
	timeSerialGraph->rangeMin.y() = -100;
	{
		// �O���t�p�ɕ`���Ԃ�ǉ�
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		{
			// �r���[�|�[�g3
			auto vpGrph = sodl::drwMngr->addViewPort("vp3");
			vpGrph->spaceAttached = spaceGrph;
			vpGrph->setVpSize(
				VP_SIZE_X,		// left
				VP_SIZE_Y / 2,	// bottom
				VP_SIZE_X/2,		// width
				VP_SIZE_Y / 2		// height
			);
			auto camGrph = vpGrph->getCam();
			camGrph->camPos = Eigen::Vector3f(0.f, 0.f, 10.f);
			camGrph->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
			camGrph->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
			camGrph->SetPrjMtx_As2DView(100, 100);
		}
	}
	// �`���ԂɃO���t��ǉ�
	sodl::drwMngr->AddObjTree_ToDrwSpace(timeSerialGraph, 1);
	

	
	// �U�z�}�O���t�̍쐬
	auto scatterGraph = sodl::GraphObj::create("ScatterGraph");
	scatterGraph->rangeMax.x() =  100;
	scatterGraph->rangeMin.x() = -100;
	scatterGraph->rangeMax.y() =  100;
	scatterGraph->rangeMin.y() = -100;
	{
		// �O���t�p�ɕ`���Ԃ�ǉ�
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		{
			// �r���[�|�[�g4
			auto vpGrph = sodl::drwMngr->addViewPort("vp4");
			vpGrph->spaceAttached = spaceGrph;
			vpGrph->setVpSize(
				VP_SIZE_X,			// left
				0,					// bottom
				VP_SIZE_Y / 2,		// width
				VP_SIZE_Y / 2		// height
			);
			auto camGrph = vpGrph->getCam();
			camGrph->camPos = Eigen::Vector3f(0.f, 0.f, 10.f);
			camGrph->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
			camGrph->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
			camGrph->SetPrjMtx_As2DView(100, 100);
		}
	}
	// �`���ԂɃO���t��ǉ�
	sodl::drwMngr->AddObjTree_ToDrwSpace(scatterGraph, 2);
	


	//////////////////////////////////////////////////////
	//
	// �`��ΏۃI�u�W�F�N�g�̐���
	//

	//-----------------------------------------------------
	// �@�B���W���_���`
	//-----------------------------------------------------
	auto Mch_Origin = sodl::CoordChainObj::create("Mch_Origin");
	Mch_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f,0.f);

	//-----------------------------------------------------
	// �@�B���W���_����A������XYZ�����N���`
	//-----------------------------------------------------
	auto Y_Link = sodl::CoordChainObj::create("Y_Link", Mch_Origin);
	auto X_Link = sodl::CoordChainObj::create("X_Link", Y_Link );
	auto Z_Link = sodl::CoordChainObj::create("Z_Link", X_Link);

	//-----------------------------------------------------
	// �@�B���W���_����A������BC�����N���`
	//-----------------------------------------------------
	auto B_Link = sodl::CoordChainObj::create("B_Link", Mch_Origin);
	B_Link->CrdTrs.translation() = Eigen::Vector3f(300.f, 0.f, -300.f);
	auto C_Link = sodl::CoordChainObj::create("C_Link", B_Link);
	C_Link->CrdTrs.translation() = Eigen::Vector3f(0.f, -300.f, 0.f);	

	//-----------------------------------------------------
	// 3D���f���I�u�W�F�N�g�������N�ɃA�^�b�`����
	//-----------------------------------------------------
	// ���s�t�@�C���̃p�X���擾����(���f���t�@�C���̈ʒu�𑊑΃p�X�Ŏw�肷�邽��)
	std::string exePath = app::GetModulePath();

	auto Mch_OriginModel = sodl::CoordChain3dMdl::create( exePath+"3dModel\\Y_Rail.stl", "Y_Rail", Mch_Origin);
	Mch_OriginModel->CrdTrs.translation() = Eigen::Vector3f(-100.f,-800.f,-100.f);
	
	auto Y_LinkModel = sodl::CoordChain3dMdl::create( exePath + "3dModel\\X_Rail.stl", "X_Rail", Y_Link);
	Y_LinkModel->CrdTrs.translation() = Eigen::Vector3f(-100.f, 0.f, 0.f);

	auto X_LinkModel = sodl::CoordChain3dMdl::create( exePath + "3dModel\\Z_Rail.stl", "Z_Rail", X_Link);
	X_LinkModel->CrdTrs.translation() = Eigen::Vector3f(-50.f, -10.f, 0.f);

	auto Z_LinkModel = sodl::CoordChain3dMdl::create( exePath + "3dModel\\head_cone.stl", "head_cone", Z_Link);


	////////////////////////////////////////////////////// 
	//
	// ��`�����`��ΏۃI�u�W�F�N�g��`��}�l�[�W���ɓo�^
	//

	// ��`�������W�n�A����`���ԂɃZ�b�g
	sodl::drwMngr->AddObjTree_ToDrwSpace(Mch_Origin);

	int count = 0;
	while(1)
	{

		X_Link->CrdTrs.translation() = Eigen::Vector3f(app::Axez_X, 0, 0);
		Y_Link->CrdTrs.translation() = Eigen::Vector3f(0, app::Axez_Y, 0);
		Z_Link->CrdTrs.translation() = Eigen::Vector3f(0, 0, app::Axez_Z);

		B_Link->CrdTrs.linear() = Eigen::AngleAxisf(app::Axez_B, UnitY ).matrix();
		C_Link->CrdTrs.linear() = Eigen::AngleAxisf(app::Axez_C, UnitZ ).matrix();
		

		//---------------------------------
		// debug  
		
		timeSerialGraph->addData(Eigen::Vector3f(count, app::Axez_X + rand()%100 - 50, -3));

		// debug
		//---------------------------------

		sodl::drwMngr->drawUpdt();

		++count;
		Sleep(10);
	}

	return 0;
}


namespace app {

	//================================================================
	//
	//	<Summry>		�L�[���쎞�̃R�[���o�b�N
	//	<Description>
	//================================================================
	void keyFunc(unsigned char key, int u, int v)
	{
		const float		KEY_MOT_UNIT	=	5.f;
		const float		CNV_DEG2RAD		=	(M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' �� ESC �� ASCII �R�[�h
			exit(0);

		case '1':
			Axez_X += KEY_MOT_UNIT;
			break;
		case '2':
			Axez_Y += KEY_MOT_UNIT;
			break;
		case '3':
			Axez_Z += KEY_MOT_UNIT;
			break;
		case '4':
			Axez_B += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '5':
			Axez_C += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;

		case 'q':
			Axez_X -= KEY_MOT_UNIT;
			break;
		case 'w':
			Axez_Y -= KEY_MOT_UNIT;
			break;
		case 'e':
			Axez_Z -= KEY_MOT_UNIT;
			break;
		case 'r':
			Axez_B -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 't':
			Axez_C -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;


		default:
			break;
		}
	}

	//================================================================
	//
	//	<Summry>		GL�E�B���h�E��Ń}�E�X�{�^�����쎞�̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseBtn(int button, int state, int x, int y)
	{
	}

	//================================================================
	//
	//	<Summry>		GL�E�B���h�E��Ń}�E�X�h���b�O���̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseDrag(int u, int v)
	{
	}

	//================================================================
	//
	//	<Summry>		GL�E�B���h�E��Ń}�E�X�z�o�[���̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseHover(int u, int v)
	{
	}


	//================================================================
	//
	//	<Summry>		���s�t�@�C���̃p�X���擾
	//	<Description>
	//================================================================
	std::string GetModulePath()
	{
		// ���s�t�@�C���̃p�X
		std::string modulePath = "";
		// �h���C�u���A�f�B���N�g�����A�t�@�C�����A�g���q
		char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

		// ���s�t�@�C���̃t�@�C���p�X���擾
		if (GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
		{
			// �t�@�C���p�X�𕪊�
			_splitpath_s(path, drive, dir, fname, ext);
			// �h���C�u�ƃf�B���N�g�������������Ď��s�t�@�C���p�X�Ƃ���
			modulePath = std::string(drive) + std::string(dir);
		}

		return modulePath;
	}

};
