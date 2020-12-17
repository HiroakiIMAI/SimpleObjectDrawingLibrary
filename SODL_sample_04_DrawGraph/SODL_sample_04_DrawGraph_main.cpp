#define  _USE_MATH_DEFINES

#include "include/SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawLib.lib" )

#include <math.h>
#include <iostream>




namespace sodl = SmplObjDrwLib;

namespace app {
	// �O���[�o���ϐ�
	const int	WINDOW_SIZE_X = 1280;
	const int	WINDOW_SIZE_Y = 480;

	int mouse_x = 0;
	int mouse_y = 0;

	float ax_X = 100; // [mm]
	float ax_Y = 100; // [mm]
	float ax_Z = 100; // [mm]

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
	sodl::DrawingManager::initMngr( &argc, argv, app::WINDOW_SIZE_X,app::WINDOW_SIZE_Y);
	
	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetPassiveMotionFunc(app::onMouseHover);
	sodl::drwMngr->SetKeyboardFunc(app::keyFunc);
	

	//////////////////////////////////////////////////////
	//
	// �r���[�|�[�g1�ƃJ����1�𒲐�����B(�f�t�H���g�̕`����[0]���B�e����)
	// 3D���f�������[�h���A�f�t�H���g�̕`����[0]�ɔz�u����B
	//
	//////////////////////////////////////////////////////
	// �r���[�|�[�g1
	auto vp1 = sodl::drwMngr->viewPorts[0];
	vp1->setVpSize(
		0,			// left
		0,			// bottom
		app::WINDOW_SIZE_X/2,	// width
		app::WINDOW_SIZE_Y	// height
	);
	auto cam1 = vp1->getCam();
	cam1->camPos = Eigen::Vector3f(400.f, -400.f, 400.f);
	cam1->camTgt = Eigen::Vector3f(100.f, 100.f, 100.f);
	cam1->zoomRatio = 0.8;
	vp1->attachCam(cam1);

	//-----------------------------------------------------
	// ���[���h���W�n���_����A������J1~4���W�n�I�u�W�F�N�g���`
	//-----------------------------------------------------
	// ���[���h���W�n���_���`
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);

	// J1~4���W�n���`
	auto X_Link = sodl::CoordChainObj::create("X_Link", World_Origin);
	auto Y_Link = sodl::CoordChainObj::create("Y_Link", X_Link);
	auto Z_Link = sodl::CoordChainObj::create("Z_Link", Y_Link);

	//-----------------------------------------------------
	// 3D���f���I�u�W�F�N�g�����W�n�I�u�W�F�N�g�ɃA�^�b�`����
	//-----------------------------------------------------
	// ���s�t�@�C���̃p�X���擾����(���f���t�@�C���̈ʒu�𑊑΃p�X�Ŏw�肷�邽��)
	std::string exePath = app::GetModulePath();
	// 3D���f���`��t�����W�n�I�u�W�F�N�g�𐶐�
	auto model_Crd = sodl::CoordChain3dMdl::create(exePath + "3dModel\\miku.obj", "model_Crd", Z_Link);
	// �A�^�b�`����Z_Link�Ƃ̑��Έʒu�𒲐�����
	model_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 10.f);

	//-----------------------------------------------------
	// ��`�����I�u�W�F�N�g��`��ΏۂƂ��ĕ`��}�l�[�W���ɓo�^
	//-----------------------------------------------------
	// ��`�������W�n�A����`��}�l�[�W���̎��f�t�H���g�̕`���ԂɃZ�b�g
	sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);



	//////////////////////////////////////////////////////
	//
	// �ǉ��̕`����[1]���쐬���A
	// �r���[�|�[�g, �J������ݒ肷��B
	//
	// ���n��O���t(2����)���쐬���A
	// �ǉ��̕`����[1]�ɔz�u����B
	//
	//////////////////////////////////////////////////////
	{
		// �O���t�p�ɕ`����[1]��ǉ�
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();

		// �r���[�|�[�g
		auto vpGrph = sodl::drwMngr->addViewPort("vpTimeSerialGraph");
		vpGrph->spaceAttached = spaceGrph;
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X/2,		// left
			app::WINDOW_SIZE_Y / 2,	// bottom
			app::WINDOW_SIZE_X/2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		// �J����
		auto camGrph = vpGrph->getCam();
		camGrph->camPos = Eigen::Vector3f(0.f, 0.f, 10.f);
		camGrph->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
		camGrph->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
		camGrph->SetPrjMtx_As2DView(200, 100);
	}
	// ���n��O���t�̍쐬
	auto timeSerialGraph
		= sodl::TimeSeriesGraph::create(
			"TimeSerealGraph",
			std::weak_ptr<sodl::CoordChainObj>(),
			200,
			100
		);
	timeSerialGraph->rangeMax.x() = 0;
	timeSerialGraph->rangeMin.x() = 0;
	timeSerialGraph->rangeMax.y() = 100;
	timeSerialGraph->rangeMin.y() = -100;
	// �`����[1]�ɃO���t��ǉ�
	sodl::drwMngr->AddObjTree_ToDrwSpace(timeSerialGraph, 1);
	

	//////////////////////////////////////////////////////
	//
	// �ǉ��̕`����[2]���쐬���A
	// �r���[�|�[�g, �J������ݒ肷��B
	//
	// �U�z�}�O���t(2����)���쐬���A
	// �ǉ��̕`����[2]�ɔz�u����B
	//
	//////////////////////////////////////////////////////
	{
		// �O���t�p�ɕ`����[2]��ǉ�
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		// �r���[�|�[�g
		auto vpGrph = sodl::drwMngr->addViewPort("vpScatterGraph");
		vpGrph->spaceAttached = spaceGrph;
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X / 2,		// left
			0,							// bottom
			app::WINDOW_SIZE_Y / 2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		// �J����
		auto camGrph = vpGrph->getCam();
		camGrph->camPos = Eigen::Vector3f(0.f, 0.f, 10.f);
		camGrph->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
		camGrph->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
		camGrph->SetPrjMtx_As2DView(100, 100);
	}
	// �U�z�}�O���t�̍쐬
	auto scatterGraph = sodl::GraphObj::create("ScatterGraph");
	scatterGraph->rangeMax.x() = 100;
	scatterGraph->rangeMin.x() = -100;
	scatterGraph->rangeMax.y() = 100;
	scatterGraph->rangeMin.y() = -100;
	// �`���ԂɃO���t��ǉ�
	sodl::drwMngr->AddObjTree_ToDrwSpace(scatterGraph, 2);
		

	//////////////////////////////////////////////////////
	//
	// ���������̎��{
	//

	int count = 0;
	while(1)
	{
		// ���f�����W���X�V
		X_Link->CrdTrs.translation() = Eigen::Vector3f(app::ax_X, 0, 0);
		Y_Link->CrdTrs.translation() = Eigen::Vector3f(0, app::ax_Y, 0);
		Z_Link->CrdTrs.translation() = Eigen::Vector3f(0, 0, app::ax_Z);
		
		// ���f��X���W�{�����_���m�C�Y�����n��O���t�Ƀv���b�g
		timeSerialGraph->addData(Eigen::Vector3f(count, app::ax_X + rand()%100 - 50, -3));

		// �h���b�O���̃}�E�X�ʒuX,Y���U�z�}�O���t�Ƀv���b�g
		scatterGraph->addData(Eigen::Vector3f(app::mouse_x, app::mouse_y, -3));

		// �`��X�V
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
			ax_X += KEY_MOT_UNIT;
			break;
		case '2':
			ax_Y += KEY_MOT_UNIT;
			break;
		case '3':
			ax_Z += KEY_MOT_UNIT;
			break;

		case 'q':
			ax_X -= KEY_MOT_UNIT;
			break;
		case 'w':
			ax_Y -= KEY_MOT_UNIT;
			break;
		case 'e':
			ax_Z -= KEY_MOT_UNIT;
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
		mouse_x = u;
		mouse_y = WINDOW_SIZE_Y - v;
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
