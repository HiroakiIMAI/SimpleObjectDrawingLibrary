#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include <math.h>
#include <iostream>
#include <Windows.h>

namespace sodl = SmplObjDrwLib;

namespace app {
	// �O���[�o���ϐ�
	const int	WINDOW_SIZE_X = 1280;
	const int	WINDOW_SIZE_Y = 480;

	int mouse_x = 0;
	int mouse_y = 0;

	float ax_J1 =  0.0 * (M_PI / 180); // [rad]
	float ax_J2 = 45.0 * (M_PI / 180); // [rad]
	float ax_J3 = 90.0 * (M_PI / 180); // [rad]
	float ax_J4 =  0.0 * (M_PI / 180); // [rad]
	float ax_J5 = 45.0 * (M_PI / 180); // [rad]
	float ax_J6 =  0.0 * (M_PI / 180); // [rad]

	// Window�C���^���N�V�����֘A�R�[���o�b�N�֐�
	void onKeyboard(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);

	// ���̑��̃T�u�֐�
	std::string GetModulePath();	// ���s�t�@�C���̃p�X���擾

};


//================================================================
//
//	<Summary>		�A�v���P�[�V�����̃G���g���|�C���g
//	<Description>
//================================================================
int main(int argc, char ** argv)
{
	sodl::DrawingManager::initMngr(app::WINDOW_SIZE_X,app::WINDOW_SIZE_Y);

	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetMouseHover(app::onMouseHover);
	sodl::drwMngr->SetKeyboardFunc(app::onKeyboard);

	//////////////////////////////////////////////////////
	//
	// �r���[�|�[�g1�ƃJ����1�𒲐�����B(�f�t�H���g�̕`����[0]���B�e����)
	// 3D���f�������[�h���A�f�t�H���g�̕`����[0]�ɔz�u����B
	//
	//////////////////////////////////////////////////////
	//------------------------------------------------------
	// �r���[�|�[�g1
	//------------------------------------------------------
	{
		auto vp1 = sodl::drwMngr->viewPorts[0];
		vp1->setVpSize(
			0,						// left
			0,						// bottom
			app::WINDOW_SIZE_X / 2,	// width
			app::WINDOW_SIZE_Y		// height
		);
		auto cam1 = vp1->getCam();
		cam1->camPos = Eigen::Vector3f(400.f, -400.f, 400.f);
		cam1->camTgt = Eigen::Vector3f(0.f, 0.f, 0.f);
		cam1->zoomRatio = 0.8;
		vp1->attachCam(cam1);
	}

	//-----------------------------------------------------
	// ���[���h���W�n���_����A������J1~6���W�n�I�u�W�F�N�g���`
	//   J1~6���W�n��6�����{�b�g�A�[���̊e�֐߂̈ʒu�p����\������
	//-----------------------------------------------------
	// ���[���h���W���_���`
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);

	// J1(Joint1)���W�n�I�u�W�F�N�g���쐬�A���[���h���W�n�ɃA�^�b�`����
	auto J1_Crd = sodl::CoordChainObj::create("J1_Crd", World_Origin);
	// J1���W�n�̐e���W�n�ɑ΂���I�t�Z�b�g�ʂ�ݒ肷��
	J1_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 30.f);

	// J2���W�n�I�u�W�F�N�g���쐬�A�e(J1)���W�n�ɃA�^�b�`����
	auto J2_Crd = sodl::CoordChainObj::create("J2_Crd", J1_Crd);
	// J2���W�n�̐e���W�n�ɑ΂���I�t�Z�b�g�ʂ�ݒ肷��
	J2_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 230.f);

	// J3���W�n�I�u�W�F�N�g���쐬�A�e���W�n�ɃA�^�b�`����
	auto J3_Crd = sodl::CoordChainObj::create("J3_Crd", J2_Crd);
	// J3���W�n�̐e���W�n�ɑ΂���I�t�Z�b�g�ʂ�ݒ肷��
	J3_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 320.f);

	// J4���W�n�I�u�W�F�N�g���쐬�A�e���W�n�ɃA�^�b�`����
	auto J4_Crd = sodl::CoordChainObj::create("J4_Crd", J3_Crd);
	// J4���W�n�̐e���W�n�ɑ΂���I�t�Z�b�g�ʂ�ݒ肷��
	J4_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 200.f);

	// J5���W�n�I�u�W�F�N�g���쐬�A�e���W�n�ɃA�^�b�`����
	auto J5_Crd = sodl::CoordChainObj::create("J5_Crd", J4_Crd);
	// J5���W�n�̐e���W�n�ɑ΂���I�t�Z�b�g�ʂ�ݒ肷��
	J5_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 120.f);

	// J6���W�n�I�u�W�F�N�g���쐬
	auto J6_Crd = sodl::CoordChainObj::create("J6_Crd", J5_Crd);
	// J6���W�n�̐e���W�n�ɑ΂���I�t�Z�b�g�ʂ�ݒ肷��
	J6_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 50.f);


	{
		//-----------------------------------------------------
		// 3D���f���`��p�̃I�u�W�F�N�g���쐬����
		//   6�����{�b�g�A�[���̊e�֐߂�\������e���W�n��
		//   ���{�b�g�A�[���̊e�����N��3D���f�����A�^�b�`����B
		//-----------------------------------------------------
		std::string exePath = app::GetModulePath();

		// ���{�b�g����̃��f�������[�h�����`��I�u�W�F�N�g�𐶐����A���[���h���W�n���_�ɃA�^�b�`����
		auto RobotBase = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\BasePlate.stl", "RobotBase", World_Origin);

		// S1(sholder1)�̃��f�������[�h�����`��I�u�W�F�N�g�𐶐����AJ1���W�n�ɃA�^�b�`����
		auto RobotSholder1 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\S1.stl", "S1", J1_Crd);

		// S2�̃��f�������[�h�����`��I�u�W�F�N�g�𐶐����AJ2���W�n�ɃA�^�b�`����
		auto RobotSholder2 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\S2.stl", "S2", J2_Crd);
		// S2���f���̕\���ʒu�𒲐����邽�߂ɁA�e(J2)���W�n�ɑ΂�����W�n�I�t�Z�b�g��ݒ肷��
		RobotSholder2->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, -90.f);

		// E1(elbow1)�̃��f�������[�h�����`��I�u�W�F�N�g�𐶐����AJ3���W�n�ɃA�^�b�`����
		auto RobotElbow1 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\E1.stl", "E1", J3_Crd);
		// ���f���̕\���ʒu�𒲐����邽�߂ɁA�e���W�n�ɑ΂�����W�n�I�t�Z�b�g��ݒ肷��
		RobotElbow1->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, -70.f);

		// W1(Wrist1)�̃��f�������[�h�����`��I�u�W�F�N�g�𐶐����AJ4���W�n�ɃA�^�b�`����
		auto RobotWrist1 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\W1.stl", "W1", J4_Crd);
		// ���f���̕\���ʒu�𒲐����邽�߂ɁA�e���W�n�ɑ΂�����W�n�I�t�Z�b�g��ݒ肷��
		RobotWrist1->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);

		// W2(Wrist2)�̃��f�������[�h�����`��I�u�W�F�N�g�𐶐����AJ5���W�n�ɃA�^�b�`����
		auto RobotWrist2 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\W2.stl", "W2", J5_Crd);
		// ���f���̕\���ʒu�𒲐����邽�߂ɁA�e���W�n�ɑ΂�����W�n�I�t�Z�b�g��ݒ肷��
		RobotWrist2->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, -50.f);

		// W3(Wrist3)�̃��f�������[�h�����`��I�u�W�F�N�g�𐶐����AJ6���W�n�ɃA�^�b�`����
		auto RobotWrist3 = sodl::CoordChain3dMdl::create(exePath + "\\3dModel\\SimpleRobot6Ax\\W3.stl", "W3", J6_Crd);
		// ���f���̕\���ʒu�𒲐����邽�߂ɁA�e���W�n�ɑ΂�����W�n�I�t�Z�b�g��ݒ肷��
		RobotWrist3->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);


		//-----------------------------------------------------
		// ��`�����I�u�W�F�N�g��`��ΏۂƂ��ĕ`��}�l�[�W���ɓo�^
		//-----------------------------------------------------
		// ��`�������W�n�A����`��}�l�[�W���̎��f�t�H���g�̕`���ԂɃZ�b�g
		sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);
	}

	// ���n��O���t �A�N�Z�X�pshared_ptr��p��
	std::shared_ptr< sodl::TimeSeriesGraph > timeSerialGraph;
	//////////////////////////////////////////////////////
	//
	// �ǉ��̕`����[1]���쐬���A�r���[�|�[�g, �J������ݒ肷��B
	//
	// ���n��O���t(2����)���쐬���A�`����[1]�ɔz�u����B
	//
	//////////////////////////////////////////////////////
	{
		//----------------------------------------------------
		// �`���ԁA�r���[�|�[�g�A�J�����̒ǉ��Ɛݒ�
		//----------------------------------------------------

		//----------------------------------------------------
		// �O���t�p�ɕ`����[1]��ǉ�
		//----------------------------------------------------
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();

		//----------------------------------------------------
		// �r���[�|�[�g�쐬
		//----------------------------------------------------
		auto vpGrph = sodl::drwMngr->addViewPort("vpTimeSerialGraph");
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X / 2,		// left
			app::WINDOW_SIZE_Y / 2,		// bottom
			app::WINDOW_SIZE_X / 2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		//----------------------------------------------------
		// �J�����ݒ�
		//----------------------------------------------------
		auto camGrph = vpGrph->getCam();
		// �J�������`����[1]���B�e����悤�Ɋ֘A�t����
		camGrph->spaceAttached = spaceGrph;

		// �J������2D�O���t���[�h�ɐݒ肷��
		//   is2DGraph = true �ɐݒ肵���J�����́A�O���t���ʂ��Œ�I�ɎB�e����悤��
		//   vp->attachCam()�����Ƃ��A�����I�Ɉʒu�p���������������B
		//   ���[�U�R�[�h���ł��������J�����̈ʒu�p����ݒ肵�Ȃ��Ă悢�B
		camGrph->is2DGraphMode = true;

		// �r���[�|�[�g�ɐݒ���X�V�����J�������֘A�t����B
		vpGrph->attachCam(camGrph);

		//----------------------------------------------------
		// ���n��O���t(2����)�̍쐬�ƕ`����[1]�ւ̔z�u
		//----------------------------------------------------
		// ���n��O���t�̍쐬
		auto tsGrph = sodl::TimeSeriesGraph::create(
				"Ax_J1 Position[deg] + Noize",			// �I�u�W�F�N�g���� = �O���t�^�C�g��
				camGrph									// �֘A�t����J����(�J�������e�s��Ȃǂ̐ݒ肪�ς�ł��邱��)
			);
		// ���x���̐ݒ�
		tsGrph->xAxisLabel->text = "time";
		tsGrph->yAxisLabel->text = "position[deg]";

		// �`����[1]�ɃO���t��ǉ�
		sodl::drwMngr->AddObjTree_ToDrwSpace(tsGrph, 1);

		// �O���t�̓��e�����C�����[�v���ōX�V�������̂ŁA
		// {}�̊O�Ő錾����shared_ptr�ɍ쐬�����O���t�̃I�u�W�F�N�g��n���Ă����B
		timeSerialGraph = tsGrph;
	}


	// �U�z�}�O���t �A�N�Z�X�pshared_ptr��p��
	std::shared_ptr<sodl::GraphObj> scatterGraph_mouseDrag;
	//////////////////////////////////////////////////////
	//
	// �ǉ��̕`����[2]���쐬���A�r���[�|�[�g, �J������ݒ肷��B
	//
	// �U�z�}�O���t"�}�E�X�h���b�O�O��"���쐬���A�`����[2]�ɔz�u����B
	//
	//////////////////////////////////////////////////////
	{
		//----------------------------------------------------
		// �`���ԁA�r���[�|�[�g�A�J�����̒ǉ��Ɛݒ�
		//----------------------------------------------------

		// �O���t�p�ɕ`����[2]��ǉ�
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		// �r���[�|�[�g�쐬
		auto vpGrph = sodl::drwMngr->addViewPort("vpScatterGraph");
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X / 2,		// left
			0,							// bottom
			app::WINDOW_SIZE_Y / 2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		// �J�����ݒ�
		auto camGrph = vpGrph->getCam();
		camGrph->spaceAttached = spaceGrph;
		camGrph->is2DGraphMode = true;
		vpGrph->attachCam(camGrph);

		//----------------------------------------------------
		// �U�z�}�O���t(2����)�̍쐬�ƕ`����[2]�ւ̔z�u
		//----------------------------------------------------
		// �U�z�}�O���t�̍쐬
		auto scttrGrph = sodl::GraphObj::create(
			"Mouse Drag Trajectory",				// �I�u�W�F�N�g���� = �O���t�^�C�g��
			camGrph									// �֘A�t����J����(�J�������e�s��Ȃǂ̐ݒ肪�ς�ł��邱��)
		);
		// ���x���̐ݒ�
		scttrGrph->xAxisLabel->text = "X [pix]";
		scttrGrph->yAxisLabel->text = "Y [pix]";
		// �`����[2]�ɃO���t��ǉ�
		sodl::drwMngr->AddObjTree_ToDrwSpace(scttrGrph, 2);

		// �O���t�̓��e�����C�����[�v���ōX�V�������̂ŁA
		// {}�̊O�Ő錾����shared_ptr�ɍ쐬�����O���t�̃I�u�W�F�N�g��n���Ă����B
		scatterGraph_mouseDrag = scttrGrph;
	}


	// �U�z�}�O���t �A�N�Z�X�pshared_ptr��p��
	std::shared_ptr<sodl::GraphObj> scatterGraph_RobotHandXY;
	//////////////////////////////////////////////////////
	//
	// �ǉ��̕`����[3]���쐬���A�r���[�|�[�g, �J������ݒ肷��B
	//
	// �U�z�}�O���t"���{�b�g�n���hXY"���쐬���A�`����[3]�ɔz�u����B
	//
	//////////////////////////////////////////////////////
	{
		// �O���t�p�ɕ`����[3]��ǉ�
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		// �r���[�|�[�g�쐬
		auto vpGrph = sodl::drwMngr->addViewPort("vpRobotHandXY");
		vpGrph->setVpSize(
			(app::WINDOW_SIZE_X / 2) + (app::WINDOW_SIZE_Y / 2),		// left
			0,							// bottom
			app::WINDOW_SIZE_Y / 2,		// width
			app::WINDOW_SIZE_Y / 2		// height
		);
		// �J�����ݒ�
		auto camGrph = vpGrph->getCam();
		camGrph->spaceAttached = spaceGrph;
		camGrph->is2DGraphMode = true;
		vpGrph->attachCam(camGrph);

		//----------------------------------------------------
		// �U�z�}�O���t(2����)�̍쐬�ƕ`����[3]�ւ̔z�u
		//----------------------------------------------------
		// �U�z�}�O���t�̍쐬
		auto scttrGrph = sodl::GraphObj::create(
			"Robot Hand XY",						// �I�u�W�F�N�g���� = �O���t�^�C�g��
			camGrph									// �֘A�t����J����(�J�������e�s��Ȃǂ̐ݒ肪�ς�ł��邱��)
		);
		// ���x���̐ݒ�
		scttrGrph->xAxisLabel->text = "X [mm]";
		scttrGrph->yAxisLabel->text = "Y [mm]";
		// �`����[3]�ɃO���t��ǉ�
		sodl::drwMngr->AddObjTree_ToDrwSpace(scttrGrph, 3);

		// �O���t�̓��e�����C�����[�v���ōX�V�������̂ŁA
		// {}�̊O�Ő錾����shared_ptr�ɍ쐬�����O���t�̃I�u�W�F�N�g��n���Ă����B
		scatterGraph_RobotHandXY = scttrGrph;
	}


	//////////////////////////////////////////////////////
	//
	// ���������̎��{
	//

	int count = 0;
	while(1)
	{
		//-----------------------------------------------------
		// �L�[���͂ōX�V�����ϐ��l�ɂ���āA�`��I�u�W�F�N�g�̍��W�ϊ��s����X�V����
		//-----------------------------------------------------
		J1_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J1, UnitZ).matrix();
		J2_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J2, UnitX).matrix();
		J3_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J3, UnitX).matrix();
		J4_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J4, UnitZ).matrix();
		J5_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J5, UnitX).matrix();
		J6_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J6, UnitZ).matrix();

		//-----------------------------------------------------
		// J1�֐ߊp�x�{�����_���m�C�Y�����n��O���t�Ƀv���b�g
		//-----------------------------------------------------
		timeSerialGraph->AddData(Eigen::Vector3f(count, app::ax_J1*180.0/M_PI + ((rand()%100 - 50)/10.0), 0));

		//-----------------------------------------------------
		// ���{�b�g�n���h�ʒuX,Y���U�z�}�O���t�Ƀv���b�g
		//-----------------------------------------------------
		Eigen::Vector3f handPos = J6_Crd->GetTf_root2self().translation();
		scatterGraph_RobotHandXY->AddData( handPos );

		//-----------------------------------------------------
		// �h���b�O���̃}�E�X�ʒuX,Y���U�z�}�O���t�Ƀv���b�g
		//-----------------------------------------------------
		scatterGraph_mouseDrag->AddData(Eigen::Vector3f(app::mouse_x, app::mouse_y, 0));

		//-----------------------------------------------------
		// �`��}�l�[�W������`��X�V�����s����
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

		++count;
		Sleep(10);
	}

	return 0;
}


namespace app {

	//================================================================
	//
	//	<Summary>		�L�[���쎞�̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onKeyboard(unsigned char key, int u, int v)
	{
		const float		KEY_MOT_UNIT	=	5.f;
		const float		CNV_DEG2RAD		=	(M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' �� ESC �� ASCII �R�[�h
			exit(0);
			break;

		case '1':
			ax_J1 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '2':
			ax_J2 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '3':
			ax_J3 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '4':
			ax_J4 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '5':
			ax_J5 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case '6':
			ax_J6 += KEY_MOT_UNIT * CNV_DEG2RAD;
			break;

		case 'q':
			ax_J1 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'w':
			ax_J2 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'e':
			ax_J3 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'r':
			ax_J4 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 't':
			ax_J5 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;
		case 'y':
			ax_J6 -= KEY_MOT_UNIT * CNV_DEG2RAD;
			break;

		default:
			break;
		}
	}


	//================================================================
	//
	//	<Summary>		GL�E�B���h�E��Ń}�E�X�{�^�����쎞�̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseBtn(int button, int state, int x, int y)
	{
	}

	//================================================================
	//
	//	<Summary>		GL�E�B���h�E��Ń}�E�X�h���b�O���̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseDrag(int u, int v)
	{
		mouse_x = u;
		mouse_y = WINDOW_SIZE_Y - v;
	}

	//================================================================
	//
	//	<Summary>		GL�E�B���h�E��Ń}�E�X�z�o�[���̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseHover(int u, int v)
	{
	}


	//================================================================
	//
	//	<Summary>		���s�t�@�C���̃p�X���擾
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
