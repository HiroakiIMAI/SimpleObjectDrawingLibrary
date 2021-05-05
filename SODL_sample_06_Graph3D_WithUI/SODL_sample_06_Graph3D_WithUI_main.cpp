#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glut.h"
#include "ImGui/backends/imgui_impl_opengl2.h"

#include <math.h>
#include <iostream>
#include <fstream>

#include <Windows.h>

namespace sodl = SmplObjDrwLib;

namespace app {
	// �O���[�o���ϐ�
	const int	WINDOW_SIZE_X = 640*2;
	const int	WINDOW_SIZE_Y = 480*2;

	// UI�ƃA�v���P�[�V�������W�b�N��}���I/F�ϐ�
	int mouse_x = 0;
	int mouse_y = 0;
	bool Flg_UiWindowShow = true;
	class Vif_PlotCnfg
	{
	public:
		std::string pltLnName;

		// �`�F�b�N�{�b�N�X��ԕێ��pstatic�ϐ��錾
		bool fg_cBox_plotColorByAtr	= false;
		bool fg_cBox_plotWidthByAtr	= false;
		bool fg_cBox_barEnable		= false;
		bool fg_cBox_barLengAtr		= false;
		bool fg_cBox_barColorByAtr	= false;
		bool fg_cBox_barWidthByAtr	= false;

		int		idx_pltWdtAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_pltColorAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;

		bool	fg_BarEnable		= false;
		int		idx_barAtr			= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_barColorAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;
		int		idx_barWidthAtr		= sodl::PointsWithAttributes::ATRIDX_NONE;

		std::vector<std::string> vct_labelsOfAttribetes;

	public:

		auto Func_AtrCnfg( std::string label, bool& fgEnable, std::vector<std::string>& atrs, int& selectedIdx  )
		{
			// �`�F�b�N�{�b�N�X�\��
			ImGui::Checkbox( (label + " by attribute.").c_str(), &fgEnable);
			// �`�F�b�NON�̏ꍇ�A�R���{�{�b�N�X��\������
			if( fgEnable )
			{
				ImGui::SameLine();
				// �R���{�{�b�N�X�\���p�̉�����\���I�u�W�F�N�g��ʂɐݒ�
				ImGui::PushItemWidth(50); // [pix]

				// �R���{�{�b�N�X�\�����e���쐬
				std::vector< const char* > pList;
				// �A�g���r���[�g�������[�v
				for(int i = 0; i<atrs.size(); ++i)
				{
					pList.push_back( atrs[i].c_str() );
				}

				// �R���{�{�b�N�X�\��
				ImGui::Combo( label.c_str(), &selectedIdx, (char**)&pList[0], pList.size(), 10 );

				// �R���{�{�b�N�X�\���p�̉����̐ݒ��\���I�u�W�F�N�g��ʂɑ΂��ĉ�������
				ImGui::PopItemWidth();
			}
			// �`�F�b�N�{�b�N�XOFF�̏ꍇ
			else
			{
				// index�ɖ����l��ݒ肷��
				selectedIdx = sodl::PointsWithAttributes::ATRIDX_NONE;
			}
		}


		void Updt()
		{
			// �n�񖼂Ő܂��ݍ��ڂ��쐬
			if ( ImGui::CollapsingHeader( this->pltLnName.c_str() ) )
			{
				ImGui::Indent();

				// �v���b�g�F
				Func_AtrCnfg(pltLnName + " color ",	fg_cBox_plotColorByAtr,	vct_labelsOfAttribetes, idx_pltColorAtr	);
				// �v���b�g��
				Func_AtrCnfg(pltLnName + " width ",	fg_cBox_plotWidthByAtr,	vct_labelsOfAttribetes, idx_pltWdtAtr	);

				// �o�[�\���L���`�F�b�N�{�b�N�X
				ImGui::Checkbox( (pltLnName + " Enable bar for each plot.").c_str(), &fg_cBox_barEnable);
				// �o�[�\���L���̏ꍇ�A�o�[�\���̐ݒ荀�ڂ�`�悷��
				if( fg_cBox_barEnable )
				{
					ImGui::Indent();

					// �o�[����
					Func_AtrCnfg(pltLnName + " bar length  ",	fg_cBox_barLengAtr,		vct_labelsOfAttribetes, idx_barAtr		);
					// �o�[�F
					Func_AtrCnfg(pltLnName + " bar color  ",	fg_cBox_barColorByAtr,	vct_labelsOfAttribetes, idx_barColorAtr	);
					// �o�[��
					Func_AtrCnfg(pltLnName + " bar width ",		fg_cBox_barWidthByAtr,	vct_labelsOfAttribetes, idx_barWidthAtr	);

					ImGui::Unindent();
				}

				ImGui::Unindent();
			}
		}
	};

	typedef struct VISUAL_IF_tp{
		std::vector< Vif_PlotCnfg > pltLines;
	}VISUAL_IF;
	VISUAL_IF visualIf;

	// Window�C���^���N�V�����֘A�R�[���o�b�N�֐�
	void onReshape(int u, int v);
	void onKeyboard(unsigned char key, int u, int v);
	void onKeyboardUp(unsigned char key, int u, int v);
	void onKeyboardSp(unsigned char key, int u, int v);
	void onKeyboardSpUp(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);
	void onMouseWheel(int wheelNum, int dir, int u, int v);

	// UI���C�u����ImGui�𗘗p����A�v���P�[�V�����R�[�h(SODL�̃R�[���o�b�N�ɓo�^����)
	void PreDraw_ImGui();
	void PostDraw_ImGui();

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
	std::vector<std::string> vct_plotDataPath;
	//------------------------------------------------------
	// �R�}���h���C�������̏���
	//	�e�������v���b�g�f�[�^�t�@�C���ւ̃p�X�Ƃ݂Ȃ�
	//------------------------------------------------------
	if (argc > 2)
	{
		for (int i = 1; i < argc; ++i)
		{
			vct_plotDataPath.push_back(argv[i]);
			std::cout << vct_plotDataPath[i - 1] << std::endl; // debug out
		}
	}

	// �f�o�b�O����
	if (0 == vct_plotDataPath.size())
	{
		// �R�}���h���C���������w�肳��Ă��Ȃ��ꍇ�̓n�[�h�R�[�f�B���O�ŏ���̃t�@�C�����w�肷��B
		std::string exePath = app::GetModulePath();
		vct_plotDataPath.push_back(exePath + "plotData\\plotData.csv");
		vct_plotDataPath.push_back(exePath + "plotData\\plotData2.csv");
	}

	//------------------------------------------------------
	// SODL�̏�����
	//------------------------------------------------------
	sodl::DrawingManager::initMngr( app::WINDOW_SIZE_X,app::WINDOW_SIZE_Y);
	sodl::drwMngr->depthBufferIsEnable = true;

	sodl::drwMngr->SetReshapeFunc(app::onReshape);
	sodl::drwMngr->SetKeyboardFunc(app::onKeyboard);
	sodl::drwMngr->SetKeyboardUpFunc(app::onKeyboardUp);
	sodl::drwMngr->SetKeyboardSpFunc(app::onKeyboardSp);
	sodl::drwMngr->SetKeyboardSpUpFunc(app::onKeyboardSpUp);
	sodl::drwMngr->SetMouseFunc(app::onMouseBtn);
	sodl::drwMngr->SetMouseDrag(app::onMouseDrag);
	sodl::drwMngr->SetMouseHover(app::onMouseHover);
	sodl::drwMngr->SetMouseWheelFunc(app::onMouseWheel);
	// draw�̑O���ImGui�̂��߂̏������Ăяo���悤�ɓo�^����
	sodl::drwMngr->Func_PreDraw = app::PreDraw_ImGui;
	sodl::drwMngr->Func_PostDraw = app::PostDraw_ImGui;

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

	auto Func_ReadFile_ToPlotLine = [](
			int pltLnIdx,
			std::string pltLnName,
			std::string filePath,
			std::shared_ptr<sodl::GraphObj> grph ,
			app::VISUAL_IF& vif
			)
	{
		app::Vif_PlotCnfg uidat;
		uidat.pltLnName = pltLnName;

		//-----------------------------------------------------
		// 1. �v���b�g�f�[�^��csv�t�@�C������ǂݏo���Čn��1�Ƃ��ăO���t�ɃZ�b�g
		//-----------------------------------------------------
		// 1.1 �v���b�g�f�[�^��csv�t�@�C������ǂݏo��
		//-----------------------------------------------------
		// �t�@�C�����J���čs���Ƃ�string�ɋl�߂�
		std::vector<std::string> plotDatStr;
		app::ReadFile_IntoVctStr_PerLine( filePath, plotDatStr);

		for (int row = 0; row < plotDatStr.size(); ++row) // 1�s�ڂ̓w�b�_�Ȃ̂œǂݔ�΂�
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

				// 1�s�ڂ̓w�b�_�Ȃ̂Ŕ�΂�
				if( row != 0 )
				{
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
				}

				// 6��ڈȍ~���A�g���r���[�g�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
				const int atrTopCol = 6;
				if(atrTopCol <= col )
				{
					// �ŏ��̍s�����̏���
					if( 0 == row )
					{
						// �A�g���r���[�g���ǉ�����
						grph->AddAtr(pltLnIdx);

						// �A�g���r���[�g����UI�\���p�ɕێ�����
						uidat.vct_labelsOfAttribetes.push_back( tokens[col] );
					}

					// �A�g���r���[�g�f�[�^���O���t�I�u�W�F�N�g�ɃZ�b�g����
					grph->AddAtrData( col - atrTopCol, fDat , pltLnIdx);
				}
			}

			//-----------------------------------------------------
			// 1.2 �ǂݏo�����f�[�^���n��1�̃v���b�g�Ώۍ��W�Ƃ��ăZ�b�g����
			//-----------------------------------------------------
			if( dataOfLine.size() > 6 )
			{
				// 0�`2��ڃf�[�^���v���b�g���W�Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
				grph->AddData( Eigen::Vector3f(dataOfLine[0], dataOfLine[1], dataOfLine[2]), pltLnIdx );
				// 3�`5��ڂ�����x�N�g���Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
				grph->AddPtVct( Eigen::Vector3f(dataOfLine[3], dataOfLine[4], dataOfLine[5]), pltLnIdx );
			}
		}

		vif.pltLines.push_back( uidat );
	};

	// �R�}���h���C�������Ŏw�肳�ꂽ�t�@�C�������ɓǂ݂����ăO���t�̃v���b�g�n��Ƃ���
	for( int i=0; i<vct_plotDataPath.size(); ++i )
	{
		if( i!=0 )
		{
			// �v���b�g�n���ǉ�
			graph3D->AddPlotLine();
		}

		// i �Ԗڂ̌n��Ƀt�@�C������ǂݏo�����f�[�^�𗬂�����
		Func_ReadFile_ToPlotLine( i,"plot"+std::to_string(i) , vct_plotDataPath[i],	graph3D, app::visualIf );
	}


	// ���������`��ΏۃI�u�W�F�N�g�c���[���f�t�H���g�̕`���Ԃɔz�u����
	sodl::drwMngr->AddObjTree_ToDrwSpace(World_Origin);

	//------------------------------------------------------
	// UI���C�u���� ImGui�̏�����
	//------------------------------------------------------
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	// ImGui�̃R�[���o�b�N�֐���GLUT�ɓo�^�����Ȃ��B
	// �����SODL�̃R�[���o�b�N�̒���ImGui�̃R�[���o�b�N���Ăяo���B
	//ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	// ImGui�ɉ�ʃT�C�Y��ʒm
	{
		auto& io = ImGui::GetIO();
		io.DisplaySize.x = sodl::drwMngr->viewPorts[0]->width;
		io.DisplaySize.y = sodl::drwMngr->viewPorts[0]->height;
	}


	//////////////////////////////////////////////////////
	//
	// ���������̎��{
	//

	int count = 0;
	//while( count < 100 )
	while( 1 )
	{
		//-----------------------------------------------------
		// �������s �A�v���P�[�V�������W�b�N
		//-----------------------------------------------------
		for( int i=0; i < app::visualIf.pltLines.size(); ++i )
		{
			graph3D->CnfgAtrDisp_PtColorIdx		( app::visualIf.pltLines[i].idx_pltColorAtr,	i);
			graph3D->CnfgAtrDisp_PtLnWdtIdx		( app::visualIf.pltLines[i].idx_pltWdtAtr,		i);

			graph3D->CnfgAtrDisp_BarIdx			( app::visualIf.pltLines[i].idx_barAtr,			i);
			graph3D->CnfgAtrDisp_BarColorIdx	( app::visualIf.pltLines[i].idx_barColorAtr,	i);
			graph3D->CnfgAtrDisp_BarWidthIdx	( app::visualIf.pltLines[i].idx_barWidthAtr,	i);
		}

		//-----------------------------------------------------
		// �`��}�l�[�W������`��X�V�����s����
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

		++count;
		Sleep(10);
	}

	//------------------------------------------------------
	// UI���C�u���� ImGui�̏I������
	//------------------------------------------------------
    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

	return 0;
}


namespace app {

	void onReshape(int u, int v)
	{
		ImGui_ImplGLUT_ReshapeFunc(u, v);
	}


	//================================================================
	//
	//	<Summary>		�L�[���쎞�̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onKeyboard(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_KeyboardFunc( key, u, v );

		const float		KEY_MOT_UNIT	=	5.f;
		const float		CNV_DEG2RAD		=	(M_PI / 180.f);

		switch (key) {
		case '\033':	// '\033' �� ESC �� ASCII �R�[�h
			exit(0);
			break;

		default:
			break;
		}
	}

	void onKeyboardUp(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_KeyboardUpFunc( key, u, v);
	}

	void onKeyboardSp(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_SpecialFunc( key, u, v);
	}

	void onKeyboardSpUp(unsigned char key, int u, int v)
	{
		ImGui_ImplGLUT_SpecialUpFunc( key, u, v);
	}


	//================================================================
	//
	//	<Summary>		GL�E�B���h�E��Ń}�E�X�{�^�����쎞�̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseBtn(int button, int state, int x, int y)
	{
		ImGui_ImplGLUT_MouseFunc( button, state, x, y );
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

		// ImGui�̃R�[���o�b�N���R�[������
		ImGui_ImplGLUT_MotionFunc(u,v);
	}

	//================================================================
	//
	//	<Summary>		GL�E�B���h�E��Ń}�E�X�z�o�[���̃R�[���o�b�N
	//	<Description>
	//================================================================
	void onMouseHover(int u, int v)
	{
		// ImGui�̃R�[���o�b�N���R�[������
		ImGui_ImplGLUT_MotionFunc(u,v);
	}

	void onMouseWheel(int wheelNum, int dir, int u, int v)
	{
		ImGui_ImplGLUT_MouseWheelFunc( wheelNum, dir, u, v );
	}


	/** ***************************************************************
	 *
	 * @brief	ImGui�̃��C���`�揈��
	 * 			my_display_code()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void my_display_code()
	{
		// �A�g���r���[�g���v���b�g�ɂǂ̂悤�ɔ��f���邩��ݒ肷��
		{
			// ImGui�E�B���h�E�J�n
			ImGui::Begin("Plot Configuration");

			// �v���b�g�n�񐔕����[�v
			for (int pltLnIdx = 0; pltLnIdx < visualIf.pltLines.size(); ++pltLnIdx)
			{
				auto& vmifPltLn = visualIf.pltLines[pltLnIdx];
				vmifPltLn.Updt();
			}

			// ImGui�E�B���h�E�I��
			ImGui::End();
		}
	}


	/** ***************************************************************
	 *
	 * @brief	SODL�`��O ImGui����
	 * 			PreDraw_ImGui()
	 * <pre>
	 * UI���C�u����ImGui�̕`��O�����ƃ��C���`�揈�������{����
	 * </pre>
	 ******************************************************************/
	void PreDraw_ImGui()
	{
	    // Start the Dear ImGui frame
	    ImGui_ImplOpenGL2_NewFrame();
	    ImGui_ImplGLUT_NewFrame();

	    my_display_code();

	    // Rendering
	    ImGui::Render();
	    ImGuiIO& io = ImGui::GetIO();

	}

	/** ***************************************************************
	 *
	 * @brief	SODL�`��� ImGui����
	 * 			PostDraw_ImGui()
	 * <pre>
	 * UI���C�u����ImGui�̕`��㏈�������{����
	 * </pre>
	 ******************************************************************/
	void PostDraw_ImGui()
	{
	    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
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


