#define  _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <Windows.h>

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )


// ���C�u�������̃G�C���A�X��ݒ肷��
namespace sodl = SmplObjDrwLib;

// �A�v���P�[�V�����̃O���[�o���V���{����app�l�[���X�y�[�X�Ɏ��߂�
namespace app {

	const int	WINDOW_SIZE_X = 640;
	const int	WINDOW_SIZE_Y = 480;

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
	sodl::DrawingManager::initMngr(app::WINDOW_SIZE_X, app::WINDOW_SIZE_Y);

	//--------------------------------------------------------------------------------------
	// �`��ΏۃI�u�W�F�N�g��create���A�`��}�l�[�W���̕`���ԂɃZ�b�g����
	//	Create�����I�u�W�F�N�g��shared_ptr�Ƃ��Ď擾�����B
	//	�`���ԂɃZ�b�g�����I�u�W�F�N�g�͕`��}�l�[�W������shared_ptr�Ƃ��ĕێ������̂ŁA
	//	Create���ɓ���ꂽshared_ptr�͕s�v�ɂȂ������_�Ŕj�����č\��Ȃ��B
	//--------------------------------------------------------------------------------------
	{
		//-----------------------------------------------------
		// ���[���h���W�n���_���`
		//-----------------------------------------------------
		// ���[���h���W�n���_��\���V���{���\���p�̃I�u�W�F�N�g�𐶐�����
		auto World_Origin = sodl::CoordChainObj::create("World_Origin");

		//-----------------------------------------------------
		// 3D���f���I�u�W�F�N�g�𐶐����A���W�n�I�u�W�F�N�g�ɃA�^�b�`����
		//-----------------------------------------------------
		// ���s�t�@�C���̃p�X���擾����(���f���t�@�C���̈ʒu�𑊑΃p�X�Ŏw�肷�邽��)
		std::string exePath = app::GetModulePath();
		// 3D���f���`��t�����W�n�I�u�W�F�N�g�𐶐����A�e���W�n�Ƃ��ă��[���h���W�n���w�肷��B
		auto model_Crd = sodl::CoordChain3dMdl::create(exePath + "3dModel\\teapot\\teapot.obj", "model_Crd", World_Origin);
		// �A�^�b�`�����e���W�n�Ƃ̑��Έʒu(���i)�𒲐�����
		model_Crd->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 20.f);
		// �A�^�b�`�����e���W�n�Ƃ̑��Έʒu(��])�𒲐�����
		//   Y+������������Ƃ������f�������[�h�����̂ŁA
		//   Z+��������ɂȂ�悤��X������90����]������B
		model_Crd->CrdTrs.linear() = Eigen::AngleAxisf(M_PI / 2.f, UnitX).matrix();

		//-----------------------------------------------------
		// ���������I�u�W�F�N�g��`��ΏۂƂ��ĕ`��}�l�[�W���ɓo�^
		//-----------------------------------------------------
		// ��`�������W�n�A����`��}�l�[�W���̎��`���ԂɃZ�b�g
		sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);
	}

	////////////////////////////////////////////////////// 
	//
	// ���C�����[�v
	//
	//////////////////////////////////////////////////////
	while(1)
	{
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
