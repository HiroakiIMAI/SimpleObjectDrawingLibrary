#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "include/SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawLib.lib" )


// ���C�u�������̃G�C���A�X��ݒ肷��
namespace sodl = SmplObjDrwLib;

// �A�v���P�[�V�����̃O���[�o���V���{����app�l�[���X�y�[�X�Ɏ��߂�
namespace app {

	const int	WINDOW_SIZE_X = 1280;
	const int	WINDOW_SIZE_Y = 960;
	float ax_X = 100; // [mm]
	float ax_Y = 100; // [mm]
	float ax_Z = 100; // [mm]

	void keyFunc(unsigned char key, int u, int v);

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
	//////////////////////////////////////////////////////
	//
	// �A�v���P�[�V�����̏�����
	//
	//////////////////////////////////////////////////////

	//-----------------------------------------------------
	// ���C�u�����̕`��}�l�[�W��������������
	//-----------------------------------------------------
	// ���̎��_�Ń��C�u�����ɂ����OpenGL�R���e�L�X�g���쐬����A
	// OpenGL�̃E�B���h�E���\�������
	sodl::DrawingManager::initMngr( &argc, argv, app::WINDOW_SIZE_X, app::WINDOW_SIZE_Y);

	// �`��}�l�[�W���ɃR�[���o�b�N�֐���ݒ肷��
	sodl::drwMngr->SetKeyboardFunc(app::keyFunc);


	//-----------------------------------------------------
	// �r���[�|�[�g�ƃJ�����̐ݒ�
	//-----------------------------------------------------
	// �r���[�|�[�g1
	//-----------------------------------------------------
	//	�f�t�H���g�̃r���[�|�[�g�͕`��}�l�[�W���̏��������ɃC���X�^���X������A
	//	viewPorts[0]��std::shared_ptr�ŕێ�����Ă���B
	//	������ꎞ�ϐ��Ɏ󂯎���ăT�C�Y��֘A�t����ꂽ�J�����ݒ�𑀍삷��

	// �r���[�|�[�g1�ւ�shared_ptr�擾
	auto vp1 = sodl::drwMngr->viewPorts[0];

	// �r���[�|�[�g1�̃T�C�Y��ݒ肷��
	//	�E�B���h�E��4������������̈�Ƀr���[�|�[�g�𒣂�
	//	(OpenGL�摜���W�n�ɏ]���̂ŁA�E�B���h�E�������_�A�������Y+�A�E������X+)
	vp1->setVpSize(
		0,								// left
		app::WINDOW_SIZE_Y / 2,			// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// �r���[�|�[�g1�Ɋ֘A�t����ꂽ�J�����ւ�shared_ptr���擾����
	auto cam1 = vp1->getCam();

	// �J�����̈ʒu��ݒ肷��
	cam1->camPos = Eigen::Vector3f(600.f, -2000.f, 300.f);
	// �J�����̎B�e�Ώۈʒu��ݒ肷��
	cam1->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	// �J�����̃Y�[���䗦��ݒ肷��
	cam1->zoomRatio = 0.8;
	vp1->attachCam(cam1);

	//-----------------------------------------------------
	// �r���[�|�[�g2
	//-----------------------------------------------------
	// �r���[�|�[�g��ǉ�����B
	//	�ǉ����ꂽ�r���[�|�[�g�C���X�^���X��
	//	drwMngr->viewPorts[]�ɂ�shared_ptr����ĕێ������
	auto vp2 = sodl::drwMngr->addViewPort("vp2");

	// �E�B���h�E��4���������E��̈�Ƀr���[�|�[�g�𒣂�
	vp2->setVpSize(
		app::WINDOW_SIZE_X / 2,			// left
		app::WINDOW_SIZE_Y / 2,			// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// �r���[�|�[�g2�Ɋ֘A�t����ꂽ�J�����ւ�shared_ptr���擾����
	auto cam2 = vp2->getCam();
	// ��ʐ}���B�e����
	cam2->camPos = Eigen::Vector3f(0.f, -0.f, 5000.f);
	cam2->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	cam2->camUpVec = Eigen::Vector3f(0.f, 1.f, 0.f);
	vp2->attachCam(cam2);

	//-----------------------------------------------------
	// �r���[�|�[�g3
	//-----------------------------------------------------
	// �r���[�|�[�g��ǉ�����B
	auto vp3 = sodl::drwMngr->addViewPort("vp3");

	// �E�B���h�E��4�������������̈�Ƀr���[�|�[�g�𒣂�
	vp3->setVpSize(
		0,								// left
		0,								// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// �r���[�|�[�g2�Ɋ֘A�t����ꂽ�J�����ւ�shared_ptr���擾����
	auto cam3 = vp3->getCam();
	// ���ʐ}���B�e����
	cam3->camPos = Eigen::Vector3f(-1000.f, 0.f, 0.f);
	cam3->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	cam3->camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
	vp3->attachCam(cam3);

	//-----------------------------------------------------
	// �r���[�|�[�g4
	//-----------------------------------------------------
	// �r���[�|�[�g��ǉ�����B
	auto vp4 = sodl::drwMngr->addViewPort("vp4");

	// �E�B���h�E��4���������E���̈�Ƀr���[�|�[�g�𒣂�
	vp4->setVpSize(
		app::WINDOW_SIZE_X / 2, 		// left
		0,								// bottom
		app::WINDOW_SIZE_X / 2,			// width
		app::WINDOW_SIZE_Y / 2			// height
	);

	// �r���[�|�[�g2�Ɋ֘A�t����ꂽ�J�����ւ�shared_ptr���擾����
	auto cam4 = vp4->getCam();
	// ���ʐ}���B�e����
	cam4->camPos = Eigen::Vector3f(0.f, -1000.f, 0.f);
	cam4->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
	cam4->camUpVec = Eigen::Vector3f(0.f, 0.f, 1.f);
	vp4->attachCam(cam4);


	//-----------------------------------------------------
	// ���[���h���W�n���_����A������J1~4���W�n�I�u�W�F�N�g���`
	//-----------------------------------------------------
	// ���[���h���W�n���_���`
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f,0.f);

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
	auto model_Crd = sodl::CoordChain3dMdl::create(exePath+"3dModel\\miku.obj", "model_Crd", Z_Link);
	// �A�^�b�`����Z_Link�Ƃ̑��Έʒu�𒲐�����
	model_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 10.f);

	//-----------------------------------------------------
	// ��`�����I�u�W�F�N�g��`��ΏۂƂ��ĕ`��}�l�[�W���ɓo�^
	//-----------------------------------------------------
	// ��`�������W�n�A����`��}�l�[�W���̎��`���ԂɃZ�b�g
	sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);



	////////////////////////////////////////////////////// 
	//
	// ���C�����[�v
	//
	//////////////////////////////////////////////////////
	while(1)
	{
		//-----------------------------------------------------
		// �L�[���͂ōX�V�����ϐ��l�ɂ���āA�`��I�u�W�F�N�g�̍��W�ϊ��s����X�V����
		//-----------------------------------------------------
		X_Link->CrdTrs.translation() = Eigen::Vector3f(app::ax_X, 0, 0);
		Y_Link->CrdTrs.translation() = Eigen::Vector3f(0, app::ax_Y, 0);
		Z_Link->CrdTrs.translation() = Eigen::Vector3f(0, 0, app::ax_Z);

		//-----------------------------------------------------
		// �`��}�l�[�W������`��X�V�����s����
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

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
		const float		CNV_DEG2RAD		=	((float)M_PI / 180.f);

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
