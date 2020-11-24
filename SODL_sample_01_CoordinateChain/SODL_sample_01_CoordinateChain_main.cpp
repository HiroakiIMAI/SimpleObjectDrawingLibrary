#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "include/SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawLib.lib" )


// ���C�u�������̃G�C���A�X��ݒ肷��
namespace sodl = SmplObjDrwLib;

// �A�v���P�[�V�����̃O���[�o���V���{����app�l�[���X�y�[�X�Ɏ��߂�
namespace app {

	const int	WINDOW_SIZE_X = 640;
	const int	WINDOW_SIZE_Y = 480;
	float ax_J1 = 0; // [rad]
	float ax_J2 = 0; // [rad]
	float ax_J3 = 0; // [rad]
	float ax_J4 = 0; // [rad]

	void keyFunc(unsigned char key, int u, int v);
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
	// ���[���h���W�n���_����A������J1~4���W�n�I�u�W�F�N�g���`
	//-----------------------------------------------------
	// �@�B���W���_���`
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f,0.f);

	// J1~4���W�n���`
	auto J1_Crd = sodl::CoordChainObj::create("J1_Crd", World_Origin);
	J1_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 10.f);

	auto J2_Crd = sodl::CoordChainObj::create("J2_Crd", J1_Crd );
	J2_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 10.f);

	auto J3_Crd = sodl::CoordChainObj::create("J3_Crd", J2_Crd);
	J3_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 100.f);

	auto J4_Crd = sodl::CoordChainObj::create("J4_Crd", J3_Crd);
	J4_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 80.f);

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
		J1_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J1, UnitZ ).matrix();
		J2_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J2, UnitX ).matrix();
		J3_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J3, UnitX ).matrix();
		J4_Crd->CrdTrs.linear() = Eigen::AngleAxisf(app::ax_J4, UnitX ).matrix();

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
		const float		CNV_DEG2RAD		=	(M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' �� ESC �� ASCII �R�[�h
			exit(0);

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

		default:
			break;
		}
	}


};
