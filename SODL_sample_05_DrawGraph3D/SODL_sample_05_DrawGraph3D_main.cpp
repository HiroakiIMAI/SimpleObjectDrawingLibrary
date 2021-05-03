#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include <math.h>
#include <iostream>
#include <fstream>

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
	void keyFunc(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);

	// ���̑��̃T�u�֐�
	std::string GetModulePath();							// ���s�t�@�C���̃p�X���擾

	// �t�@�C�����J���čs���Ƃ�std::string�ɋl�߂�
	void ReadFile_IntoVctStr_PerLine(
		std::string path ,
		std::vector<std::string>& vctStr
	);
	// �����񕪊��֐�
	void SplitString(
		std::string str,
		std::string delim,
		std::vector<std::string>& vctStr
	);

};



//================================================================
//
//	<Summary>		�A�v���P�[�V�����̃G���g���|�C���g
//	<Description>
//================================================================
int main(int argc, char ** argv)
{
	sodl::DrawingManager::initMngr( &argc, argv, app::WINDOW_SIZE_X,app::WINDOW_SIZE_Y);
	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetPassiveMotionFunc(app::onMouseHover);
	sodl::drwMngr->SetKeyboardFunc(app::keyFunc);

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
	// ���[���h���W�n���_����A��������W�n�I�u�W�F�N�g���`
	//-----------------------------------------------------
	// ���[���h���W���_���`
	auto World_Origin = sodl::CoordChainObj::create("World_Origin");
	World_Origin->CrdTrs.translation() = Eigen::Vector3f(0.f, 0.f, 0.f);

	auto Crd1 = sodl::CoordChainObj::create("Crd1", World_Origin);
	Crd1->CrdTrs.translation() = Eigen::Vector3f(10.f, 10.f, 0.f);

	//-----------------------------------------------------
	// 3D�O���t�I�u�W�F�N�g�𐶐�����
	//-----------------------------------------------------
	auto graph3D = sodl::GraphObj3D::create("graph3D", Eigen::Vector3f(200,200,200), Crd1);
	{
		//-----------------------------------------------------
		// 1. �v���b�g�f�[�^��csv�t�@�C������ǂݏo���Čn��1�Ƃ��ăO���t�ɃZ�b�g
		//-----------------------------------------------------
		// 1.1 �v���b�g�f�[�^��csv�t�@�C������ǂݏo��
		//-----------------------------------------------------
		// �t�@�C�����J���čs���Ƃ�string�ɋl�߂�
		std::vector<std::string> plotDatStr;
		std::string exePath = app::GetModulePath();
		app::ReadFile_IntoVctStr_PerLine(exePath + "plotData\\plotData.csv", plotDatStr);

		for (int row = 1; row < plotDatStr.size(); ++row) // 1�s�ڂ̓w�b�_�Ȃ̂œǂݔ�΂�
		{
			auto str = plotDatStr[row];
			// 1�s�f�[�^��','��؂��token�ɕ�������
			std::vector<std::string> tokens;
			app::SplitString(str, ",", tokens);

			// 1�s���̃g�[�N������������
			std::vector<float> dataOfLine;
			for (int col = 0; col < tokens.size(); ++col)
			{
				float fDat = 0;
				// �g�[�N���𐔒l�f�[�^�ɕϊ�����
				try
				{
					fDat = std::stof(tokens[col]);
				}
				// ���l�ϊ��Ɏ��s�����ꍇ�͏����l��0���̗p�����
				catch( std::exception& e )
				{
				}
				// �g�[�N����ϊ����ē���ꂽ���l�f�[�^��z��ɋl�߂�
				dataOfLine.push_back( fDat );

				// 6��ڈȍ~���A�g���r���[�g�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// �ŏ��̍s�����̏���
					if( 1 == row )
					{
						// �A�g���r���[�g���ǉ�����
						graph3D->AddAtr();
					}

					// �A�g���r���[�g�f�[�^���O���t�I�u�W�F�N�g�ɃZ�b�g����
					graph3D->AddAtrData( col - atrTopCol, fDat );
				}
			}

			//-----------------------------------------------------
			// 1.2 �ǂݏo�����f�[�^���n��1�̃v���b�g�Ώۍ��W�Ƃ��ăZ�b�g����
			//-----------------------------------------------------
			// 0�`2��ڃf�[�^���v���b�g���W�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
			graph3D->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]) );
			// 3�`5��ڂ�����x�N�g���Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
			graph3D->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]) );
		}

		//-----------------------------------------------------
		// 1.3 �n��1�̃O���t�\���ɒǉ�����A�g���r���[�g�f�[�^��ݒ肷��
		//-----------------------------------------------------
		//graph3D->CnfgAtrDisp_BarIdx(1);
		//graph3D->CnfgAtrDisp_BarColorIdx(1);
		graph3D->CnfgAtrDisp_PtColorIdx(2);
		graph3D->CnfgAtrDisp_PtLnWdtIdx(0);
		graph3D->SetPlotLineDrawType( sodl::DRAWTYPE::POINT );
		graph3D->SetPlotLineWidth(10);

	}

	{
		//-----------------------------------------------------
		// 2. �v���b�g�f�[�^2��csv�t�@�C������ǂݏo���Čn��2�Ƃ��ăO���t�ɃZ�b�g
		//-----------------------------------------------------
		// �O���t�Ƀv���b�g����n���ǉ�����
		int pltIdx = graph3D->AddPlotLine();
		//-----------------------------------------------------
		// 2.1 �v���b�g�f�[�^2��csv�t�@�C������ǂݏo��
		//-----------------------------------------------------
		// �t�@�C�����J���čs���Ƃ�string�ɋl�߂�
		std::vector<std::string> plotDatStr;
		std::string exePath = app::GetModulePath();
		app::ReadFile_IntoVctStr_PerLine(exePath + "plotData\\plotData2.csv", plotDatStr);

		for (int row = 1; row < plotDatStr.size(); ++row) // 1�s�ڂ̓w�b�_�Ȃ̂œǂݔ�΂�
		{
			auto str = plotDatStr[row];
			// 1�s�f�[�^��','��؂��token�ɕ�������
			std::vector<std::string> tokens;
			app::SplitString(str, ",", tokens);

			// 1�s���̃g�[�N������������
			std::vector<float> dataOfLine;
			for (int col = 0; col < tokens.size(); ++col)
			{
				float fDat = 0;
				// �g�[�N���𐔒l�f�[�^�ɕϊ�����
				try
				{
					fDat = std::stof(tokens[col]);
				}
				// ���l�ϊ��Ɏ��s�����ꍇ�͏����l��0���̗p�����
				catch( std::exception& e )
				{
				}
				// �g�[�N����ϊ����ē���ꂽ���l�f�[�^��z��ɋl�߂�
				dataOfLine.push_back( fDat );

				// 6��ڈȍ~���A�g���r���[�g�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// �ŏ��̍s�����̏���
					if( 1 == row )
					{
						// �A�g���r���[�g���ǉ�����
						graph3D->AddAtr(pltIdx);
					}

					// �A�g���r���[�g�f�[�^���O���t�I�u�W�F�N�g�ɃZ�b�g����
					graph3D->AddAtrData( col - atrTopCol, fDat, pltIdx );
				}
			}

			//-----------------------------------------------------
			// 2.2 �ǂݏo�����f�[�^���n��1�̃v���b�g�Ώۍ��W�Ƃ��ăZ�b�g����
			//-----------------------------------------------------
			// 0�`2��ڃf�[�^���v���b�g���W�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
			graph3D->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]), pltIdx );
			// 3�`5��ڂ�����x�N�g���Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
			graph3D->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]), pltIdx );
		}

		//-----------------------------------------------------
		// 2.3 �n��1�̃O���t�\���ɒǉ�����A�g���r���[�g�f�[�^��ݒ肷��
		//-----------------------------------------------------
		graph3D->CnfgAtrDisp_BarIdx(1, pltIdx);
		//graph3D->CnfgAtrDisp_BarColorIdx(1, pltIdx);
		//graph3D->CnfgAtrDisp_PtColorIdx(2, pltIdx);
	}


	{
		//-----------------------------------------------------
		// 2. �v���b�g�f�[�^2��csv�t�@�C������ǂݏo���Čn��2�Ƃ��ăO���t�ɃZ�b�g
		//-----------------------------------------------------
		// �O���t�Ƀv���b�g����n���ǉ�����
		int pltIdx = graph3D->AddPlotLine();
		//-----------------------------------------------------
		// 2.1 �v���b�g�f�[�^2��csv�t�@�C������ǂݏo��
		//-----------------------------------------------------
		// �t�@�C�����J���čs���Ƃ�string�ɋl�߂�
		std::vector<std::string> plotDatStr;
		std::string exePath = app::GetModulePath();
		app::ReadFile_IntoVctStr_PerLine(exePath + "plotData\\plotData2.csv", plotDatStr);

		for (int row = 1; row < plotDatStr.size(); ++row) // 1�s�ڂ̓w�b�_�Ȃ̂œǂݔ�΂�
		{
			auto str = plotDatStr[row];
			// 1�s�f�[�^��','��؂��token�ɕ�������
			std::vector<std::string> tokens;
			app::SplitString(str, ",", tokens);

			// 1�s���̃g�[�N������������
			std::vector<float> dataOfLine;
			for (int col = 0; col < tokens.size(); ++col)
			{
				float fDat = 0;
				// �g�[�N���𐔒l�f�[�^�ɕϊ�����
				try
				{
					fDat = std::stof(tokens[col]);
				}
				// ���l�ϊ��Ɏ��s�����ꍇ�͏����l��0���̗p�����
				catch( std::exception& e )
				{
				}
				// �g�[�N����ϊ����ē���ꂽ���l�f�[�^��z��ɋl�߂�
				dataOfLine.push_back( fDat );

				// 6��ڈȍ~���A�g���r���[�g�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// �ŏ��̍s�����̏���
					if( 1 == row )
					{
						// �A�g���r���[�g���ǉ�����
						graph3D->AddAtr(pltIdx);
					}

					// �A�g���r���[�g�f�[�^���O���t�I�u�W�F�N�g�ɃZ�b�g����
					graph3D->AddAtrData( col - atrTopCol, fDat, pltIdx );
				}
			}

			//-----------------------------------------------------
			// 2.2 �ǂݏo�����f�[�^���n��1�̃v���b�g�Ώۍ��W�Ƃ��ăZ�b�g����
			//-----------------------------------------------------
			// 0�`2��ڃf�[�^���v���b�g���W�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
			graph3D->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]), pltIdx );
			// 3�`5��ڂ�����x�N�g���Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
			graph3D->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]), pltIdx );
		}

		//-----------------------------------------------------
		// 2.3 �n��1�̃O���t�\���ɒǉ�����A�g���r���[�g�f�[�^��ݒ肷��
		//-----------------------------------------------------
		graph3D->SetPlotLineWidth(5, pltIdx);
		graph3D->SetPlotLineOffset( Eigen::Vector3f(10,0,0) , pltIdx );
		graph3D->CnfgAtrDisp_BarIdx(1, pltIdx);

		//graph3D->CnfgAtrDisp_BarColorIdx(1, pltIdx);
		//graph3D->CnfgAtrDisp_PtColorIdx(2, pltIdx);
	}



//	auto Crd2 = sodl::CoordChainObj::create("Crd2", World_Origin);
//	Crd2->CrdTrs.translation() = Eigen::Vector3f(200.f, 100.f, 0.f);
//
//	auto graph3D2 = sodl::GraphObj3D::create( "graph3D", Eigen::Vector3f(100, 100, 100), Crd2 );
//	graph3D2->addData(Eigen::Vector3f(0, 0, 0));
//	graph3D2->addData(Eigen::Vector3f(100, 100, 100));


	sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);


	//////////////////////////////////////////////////////
	//
	// ���������̎��{
	//

	int count = 0;
	//while( count < 100 )
	while( 1 )
	{
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
	void keyFunc(unsigned char key, int u, int v)
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

	/** **************************************************************
	*
	* @brief			�t�@�C�����J���čs���Ƃ�std::string�ɋl�߂�
	* <pre></pre>
	******************************************************************/
	void ReadFile_IntoVctStr_PerLine(	// �t�@�C�����J���čs���Ƃ�std::string�ɋl�߂�
		std::string path,
		std::vector<std::string>& vctStr
	)
	{
		std::ifstream ifs(path);
		if (ifs.is_open())
		{
			std::string line;
			while ( std::getline(ifs, line) )
			{
				vctStr.push_back(line);
			}
		}
	}

	/** **************************************************************
	*
	* @brief			�����񕪊�����
	* <pre>
	* string ���f���~�^�ŋ�؂��� vector �ɋl�߂�
	* </pre>
	******************************************************************/
	void SplitString(
		std::string str,
		std::string delim,
		std::vector<std::string>& vctStr
	)
	{
		vctStr.clear();
		while (1)
		{
			// ����������ɑ΂��ăf���~�^������
			auto pos = str.find(delim);
			// �f���~�^���݂���Ȃ���Ό��݂̕�������o�͔z��ɒǉ����ă��[�v�𔲂���B
			if (std::string::npos == pos)
			{
				vctStr.push_back(str);
				break;
			}

			// �f���~�^�܂ł�؂�o���ďo�͔z��ɒǉ�
			vctStr.push_back( str.substr(0, pos) );

			// �f���~�^�ȍ~�̕�����ň�����������X�V
			str = str.substr(pos + 1, std::string::npos);

		}
	}

};


