#define  _USE_MATH_DEFINES

#include "SimpleObjectDrawingLibrary.h"
#include "VifClass_PlotCnfg.h"
#pragma comment( lib, "SimpleObjectDrawingLibrary.lib" )

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glut.h"
#include "ImGui/backends/imgui_impl_opengl2.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"



#include <math.h>
#include <iostream>
#include <fstream>

#include <Windows.h>

namespace sodl = SmplObjDrwLib;

namespace app {
	// �O���[�o���ϐ�
	const int	WINDOW_SIZE_X = 640*2;
	const int	WINDOW_SIZE_Y = 480*2;

	int mouse_x = 0;
	int mouse_y = 0;

	//-------------------------------------------------
	// CSV�t�@�C�����A�v���P�[�V�������W�b�N��ň������߂�I/F
	//-------------------------------------------------
	// �t�@�C�����̍\��
	typedef struct ST_PLOT_FILE_tp{
		std::string filePath;
		std::ifstream ifs;
		std::streampos seekPos = 0;
		int seekRow = 0;

		bool fg_xyz = false;
		bool fg_xyzijk = false;
		bool fg_xyzbc = false;
		bool fg_xyzac = false;
		bool fg_xyzabc = false;

		int cX = -1;
		int cY = -1;
		int cZ = -1;
		int cI = -1;
		int cJ = -1;
		int cK = -1;
	}ST_PLOT_FILE;

	// �t�@�C���֘A����I/F���[�g�\��
	typedef struct FILE_IF_tp{
		std::map< std::string, ST_PLOT_FILE > pltFiles;
	}FILE_IF;

	// �t�@�C���֘A����I/F���[�g�I�u�W�F�N�g
	FILE_IF csvIf;

	std::map< std::string, std::shared_ptr<sodl::GraphObj> > graphs2D;

	//-------------------------------------------------
	// UI�ƃA�v���P�[�V�������W�b�N��}���I/F�ϐ�
	//-------------------------------------------------
	appUi::VISUAL_IF visualIf;

	//-------------------------------------------------
	// Window�C���^���N�V�����֘A�R�[���o�b�N�֐�
	//-------------------------------------------------
	void onReshape(int u, int v);
	void onKeyboard(unsigned char key, int u, int v);
	void onKeyboardUp(unsigned char key, int u, int v);
	void onKeyboardSp(unsigned char key, int u, int v);
	void onKeyboardSpUp(unsigned char key, int u, int v);
	void onMouseBtn(int button, int state, int x, int y);
	void onMouseDrag(int u, int v);
	void onMouseHover(int u, int v);
	void onMouseWheel(int wheelNum, int dir, int u, int v);

	//-------------------------------------------------
	// UI���C�u����ImGui�𗘗p����A�v���P�[�V�����R�[�h(SODL�̃R�[���o�b�N�ɓo�^����)
	//-------------------------------------------------
	void PreDraw_ImGui();
	void PostDraw_ImGui();

	//-------------------------------------------------
	// ���̑��̃A�v���P�[�V�������W�b�N
	//-------------------------------------------------
	// �t�@�C����Ǐo���ăO���t�̃v���b�g�n��ɉ�����
//	void Func_ReadFile_ToPlotLine(
//			int pltLnIdx,
//			std::string pltLnName,
//			std::string filePath,
//			std::shared_ptr<sodl::GraphObj> grph ,
//			app::VISUAL_IF& vif
//			);

	void ReadCsvHeader_CreatePlotLine(
			std::string csvHeader,
			std::string pltLnName,
			app::ST_PLOT_FILE& plotFile,
			std::shared_ptr<sodl::GraphObj> grph,
			appUi::Vif_PlotCnfg& uidat
			);

	void ReadCsvData_ToPlotLine(
			std::string pltRowStr,
			std::string pltLnName,
			app::ST_PLOT_FILE& plotFile,
			std::shared_ptr<sodl::GraphObj> grph
			);

	void Func_ReadFile_ToPlotLine(
			std::string pltLnName,
			std::shared_ptr<sodl::GraphObj> grph,
			app::ST_PLOT_FILE& plotFile,
			appUi::Vif_PlotCnfg& uidat
			);

	std::shared_ptr<sodl::GraphObj> Add2DPlot( std::string name );

	//-------------------------------------------------
	// ���̑��̃T�u�֐�
	//-------------------------------------------------
	// ���s�t�@�C���̃p�X���擾
	std::string GetModulePath();

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
//		std::string exePath = app::GetModulePath();
//		vct_plotDataPath.push_back(exePath + "plotData\\plotData.csv");
//		vct_plotDataPath.push_back(exePath + "plotData\\plotData2.csv");
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
			app::WINDOW_SIZE_Y 		// height
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

	//-----------------------------------------------------
	// 3D�O���t�I�u�W�F�N�g�𐶐�����
	//-----------------------------------------------------
	auto graph3D = sodl::GraphObj3D::create("graph3D", Eigen::Vector3f(200,200,200), World_Origin);
	// �R�}���h���C�������Ŏw�肳�ꂽ�t�@�C�������ɓǂ݂����ăO���t�̃v���b�g�n��Ƃ���
	for( int i=0; i<vct_plotDataPath.size(); ++i )
	{
		// i�Ԗڂ̃v���b�g�n���ǉ�
		std::string pltName = "p" + std::to_string(i);
		graph3D->AddPlotLine( pltName );
		app::csvIf.pltFiles.	emplace( std::make_pair( pltName, app::ST_PLOT_FILE() ) );
		app::visualIf.pltLines.	emplace( std::make_pair( pltName,appUi::Vif_PlotCnfg() ) );

		// i�Ԗڂ̃t�@�C�����I�[�v��
		app::csvIf.pltFiles[pltName].ifs.open( vct_plotDataPath[i] );
		app::csvIf.pltFiles[pltName].filePath = vct_plotDataPath[i];

		// i�Ԗڂ̌n��Ƀt�@�C������ǂݏo�����f�[�^�𗬂�����
		app::Func_ReadFile_ToPlotLine(
			pltName,graph3D,
			app::csvIf.pltFiles[pltName],
			app::visualIf.pltLines[pltName]
		);
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

	//------------------------------------------------------
	// UI���C�u���� ImGuiFileDialog�̏�����
	//------------------------------------------------------
	ImGuiFileDialog::Instance()->SetExtentionInfos(".csv", ImVec4(1.0f, 1.0f, 0.0f, 0.9f));



	//////////////////////////////////////////////////////
	//
	// ���������̎��{
	//

	int count = 0;
	while( 1 )
	{
		//-----------------------------------------------------
		// �������s�A�v���P�[�V�������W�b�N
		//-----------------------------------------------------
		// UI����̓��͌��ʂ�`����e�ɔ��f������

		//-------------------------------------------------------------------------------------------------
		// C1 �v���b�g�n��̒ǉ�
		//-------------------------------------------------------------------------------------------------
		// �ǉ��t�@�C�����̂�UI�ɂ���ăZ�b�g����Ă���ꍇ
		if( 0 != app::visualIf.newFilePath.size() )
		{
			std::string pltName = app::visualIf.newPlotLineName;
			// �v���b�g�n���ǉ�
			graph3D->AddPlotLine(app::visualIf.newPlotLineName);
			app::csvIf.pltFiles.	emplace( std::make_pair( pltName, app::ST_PLOT_FILE() ) );
			app::visualIf.pltLines.	emplace( std::make_pair( pltName, appUi::Vif_PlotCnfg() ) );

			// �t�@�C�����I�[�v��
			app::csvIf.pltFiles[pltName].ifs.open( app::visualIf.newFilePath );
			app::csvIf.pltFiles[pltName].filePath = app::visualIf.newFilePath;

			// �n��Ƀt�@�C������ǂݏo�����f�[�^�𗬂�����
			app::Func_ReadFile_ToPlotLine(
				pltName,graph3D,
				app::csvIf.pltFiles[pltName],
				app::visualIf.pltLines[pltName]
			);

			// �ǉ��t�@�C�����̂��N���A����
			app::visualIf.newFilePath.clear();
			app::visualIf.newPlotLineName.clear();
		}

		//-------------------------------------------------------------------------------------------------
		// C2 2D�v���b�g�̒ǉ�
		//-------------------------------------------------------------------------------------------------
		// �v���b�g�n�񃋁[�v
		for(auto	vifPltLineMapItm  = app::visualIf.pltLines.begin();
					vifPltLineMapItm != app::visualIf.pltLines.end();
				  ++vifPltLineMapItm)
		{
			auto& vifPltLine = vifPltLineMapItm->second;
			auto& atrSlct = vifPltLine.ST_atrSlctWindowCtl;

			// �V�K2D�v���b�g�����v������Ă���ꍇ
			if( "" != atrSlct.selectedAtrName )
			{
				std::string structredName = vifPltLine.pltLnName + "." + atrSlct.selectedAtrName;

				// 2D�v���b�g�̎��̂𐶐����ăA�v���P�[�V�������ʂ�2D�v���b�g�ێ��p���X�g�ɉ�����
				auto grph2D = app::Add2DPlot( structredName );

				// UI�őI�����ꂽ3D�v���b�g�̃A�g���r���[�g���
				// ��������2D�v���b�g�Ƀv���b�g�f�[�^�Ƃ��ė�������
				auto attributes = graph3D->GetAttributes_Shared( vifPltLine.pltLnName );
				auto atr = attributes[atrSlct.selectedAtrIdx];
				for( int i=0; i<atr->data.size(); ++i )
				{
					// X:�f�[�^index
					// Y:�A�g���r���[�g�l
					// Z:�Œ�l
					grph2D->AddData( Eigen::Vector3f( i, atr->data[i], 10) );
				}

				// ��������2D�v���b�g�ɑΉ����鑀��p��UI�𐶐�����
				appUi::Vif_PlotCnfg2D plt2DVif;
				plt2DVif.plt2dName = structredName;
				vifPltLine.grps2DIf.insert( std::make_pair( structredName, plt2DVif ) );

				atrSlct.selectedAtrName = "";
			}
		}

		//-------------------------------------------------------------------------------------------------
		// C3 �v���b�g�n��̕\���ݒ���O���t�I�u�W�F�N�g�ɔ��f����
		//-------------------------------------------------------------------------------------------------
		// �v���b�g�n�񃋁[�v
		for(auto	vifPltLineMapItm  = app::visualIf.pltLines.begin();
					vifPltLineMapItm != app::visualIf.pltLines.end();
				  ++vifPltLineMapItm)
		{
			auto& vifPltLine = vifPltLineMapItm->second;
			graph3D->SetBarEnable				( vifPltLine.fg_cBox_barEnable, vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_PtColorIdx		(vifPltLine.idx_pltColorAtr,	vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_PtLnWdtIdx		(vifPltLine.idx_pltWdtAtr,		vifPltLine.pltLnName);

			graph3D->CnfgAtrDisp_BarIdx			(vifPltLine.idx_barAtr,			vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_BarColorIdx	(vifPltLine.idx_barColorAtr,	vifPltLine.pltLnName);
			graph3D->CnfgAtrDisp_BarWidthIdx	(vifPltLine.idx_barWidthAtr,	vifPltLine.pltLnName);

			if( vifPltLine.fg_cBox_updateCyclic )
			{
				// �t�@�C���Ǐo�����J��Ԃ����s����
				app::Func_ReadFile_ToPlotLine( vifPltLine.pltLnName , graph3D, app::csvIf.pltFiles[ vifPltLine.pltLnName ], vifPltLine );
			}

			//-----------------------------------------------------
			// 2D�v���b�g�ւ̐ݒ�X�V
			//-----------------------------------------------------
			for( auto	itr_plt2dIf  = vifPltLine.grps2DIf.begin();
						itr_plt2dIf != vifPltLine.grps2DIf.end();
					  ++itr_plt2dIf )
			{
				auto& plt2dIf =  itr_plt2dIf->second;

				// �Ώۂ�2D�O���t���擾�E���݃`�F�b�N
				auto itr_grph2D = app::graphs2D.find( plt2dIf.plt2dName );
				if( app::graphs2D.end() != itr_grph2D )
				{
					auto& grph2D = itr_grph2D->second;

					// �O���t�̃v���b�g���C���̃f�[�^���擾
					auto p_line = grph2D->GetCpyPltLn();
					const auto sz = p_line->size();

					// �����ŗ^�����͈͂ɂ�����y���W�̍ő�/�ŏ����擾����B
					auto func_GetYMax = [&p_line](
						const int idxMin,
						const int idxMax,
						float& yMin,
						float& yMax
						)
					{
						yMax = -999999;
						yMin = +999999;
						for( int ln_idx  = idxMin;
								 ln_idx <= idxMax;
							  ++ ln_idx )
						{
							const float y = (*p_line)[ln_idx].y();
							// �ő�l�ێ�
							if( y > yMax )
							{
								yMax = y;
							}
							// �ŏ��l�ێ�
							if( y < yMin )
							{
								yMin = y;
							}
						}
					};

					// �T�u�J�[�\���ɂ��X�P�[�����O�L����Ԃ��O��l�ƈقȂ�ꍇ
					if( plt2dIf.fg_cBox_scalefitToSubCursols
					!=  plt2dIf.fg_cBox_scalefitToSubCursols_prv
					)
					{
						// �T�u�J�[�\���ɂ��X�P�[�����O�L���̏ꍇ
						if( plt2dIf.fg_cBox_scalefitToSubCursols )
						{
							// �J�[�\���̑召�֌W�𐳋K��
							const int lnIdx_min = MIN( app::visualIf.subCursol_idx[0], app::visualIf.subCursol_idx[1] );
							const int lnIdx_max = MAX( app::visualIf.subCursol_idx[0], app::visualIf.subCursol_idx[1] );

							// �J�[�\�����v���b�g���C����ɂ��邱�Ƃ��`�F�b�N
							if(	( sz > lnIdx_min )
							&&	( sz > lnIdx_max )
							)
							{
								// �J�[�\����̃v���b�g�_���擾
								const auto p1 = (*grph2D->GetCpyPltLn())[ lnIdx_min ];
								const auto p2 = (*grph2D->GetCpyPltLn())[ lnIdx_max ];

								// �J�[�\���Ԃ�y���W�̍ő�/�ŏ��l���擾
								float yMax;
								float yMin;
								func_GetYMax( lnIdx_min, lnIdx_max, yMin, yMax );

								// 2D�O���t�̕`��͈͂��Đݒ�
								grph2D->drwRngMin.x() = lnIdx_min;
								grph2D->drwRngMax.x() = lnIdx_max;
								grph2D->drwRngMin.y() = yMin;
								grph2D->drwRngMax.y() = yMax;
							}
						}
						// �T�u�J�[�\���ɂ��X�P�[�����O�����̏ꍇ
						else
						{
							// 2D�O���t�̕`��͈͂��Đݒ�
							grph2D->drwRngMin.x() = grph2D->datRngMin.x();
							grph2D->drwRngMax.x() = grph2D->datRngMax.x();
							grph2D->drwRngMin.y() = grph2D->datRngMin.y();
							grph2D->drwRngMax.y() = grph2D->datRngMax.y();
						}
					}
					// �T�u�J�[�\���ɂ��X�P�[�����O�L����� �O��l�X�V
					plt2dIf.fg_cBox_scalefitToSubCursols_prv = plt2dIf.fg_cBox_scalefitToSubCursols;
				}
			}


			//-----------------------------------------------------
			// �v���b�g�n��̍폜
			//-----------------------------------------------------
			if( vifPltLine.fg_delete )
			{
				// �`��I�u�W�F�N�g�폜
				graph3D->DeletePlotLine(vifPltLine.pltLnName);
				// visualIf�̍폜
				app::visualIf.pltLines.erase(vifPltLine.pltLnName);

				// for���̌J��Ԃ������ŋ�̃C�e���[�^��++���Ȃ��悤�Ƀ��[�v�𔲂���
				// �����ł��Ȃ��������̃��[�v�́A���߂Ď��̕\�������ɏ�������B
				break;
			}
		}

		//-------------------------------------------------------------------------------------------------
		// C4 �J�[�\���֘A
		//-------------------------------------------------------------------------------------------------
		if( auto cursoledLine = graph3D->GetCpyPltLn( app::visualIf.cursol_pltLnName ) )
		{
			app::visualIf.cursol_idxMax = cursoledLine->size();
		}
		else
		{
			app::visualIf.cursol_idxMax = 0;
		}
		graph3D->PutCursolToLine( app::visualIf.cursol_pltLnName,		sodl::GraphObj::CURSOL_SEL::MAIN );
		graph3D->SetCursolViible( app::visualIf.cursol_enable,			sodl::GraphObj::CURSOL_SEL::MAIN );
		graph3D->UpdtCursol		( app::visualIf.cursol_idx,				sodl::GraphObj::CURSOL_SEL::MAIN );

		bool subCursolEnable_1 = ( app::visualIf.cursol_enable && app::visualIf.subCursol_enable[0] );
		graph3D->PutCursolToLine( app::visualIf.cursol_pltLnName,		sodl::GraphObj::CURSOL_SEL::SUB1 );
		graph3D->SetCursolViible( subCursolEnable_1,					sodl::GraphObj::CURSOL_SEL::SUB1 );
		graph3D->UpdtCursol		( app::visualIf.subCursol_idx[0],		sodl::GraphObj::CURSOL_SEL::SUB1 );

		bool subCursolEnable_2 = ( app::visualIf.cursol_enable && app::visualIf.subCursol_enable[1] );
		graph3D->SetCursolViible( subCursolEnable_2,					sodl::GraphObj::CURSOL_SEL::SUB2 );
		graph3D->PutCursolToLine( app::visualIf.cursol_pltLnName,		sodl::GraphObj::CURSOL_SEL::SUB2 );
		graph3D->UpdtCursol		( app::visualIf.subCursol_idx[1],		sodl::GraphObj::CURSOL_SEL::SUB2 );

		// 2D�v���b�g�̃J�[�\����`�悷��
		for( auto grph2Ditr = app::graphs2D.begin();
				  grph2Ditr != app::graphs2D.end();
				++grph2Ditr )
		{
			auto grph2D = grph2Ditr->second;
			grph2D->SetCursolViible	( app::visualIf.cursol_enable,			sodl::GraphObj::CURSOL_SEL::MAIN );
			grph2D->UpdtCursol		( app::visualIf.cursol_idx,				sodl::GraphObj::CURSOL_SEL::MAIN );

			grph2D->SetCursolViible	( subCursolEnable_1,					sodl::GraphObj::CURSOL_SEL::SUB1 );
			grph2D->UpdtCursol		( app::visualIf.subCursol_idx[0],		sodl::GraphObj::CURSOL_SEL::SUB1 );

			grph2D->SetCursolViible	( subCursolEnable_2,					sodl::GraphObj::CURSOL_SEL::SUB2 );
			grph2D->UpdtCursol		( app::visualIf.subCursol_idx[1],		sodl::GraphObj::CURSOL_SEL::SUB2 );
		}

		//-----------------------------------------------------
		// �`��}�l�[�W������`��X�V�����s����
		//-----------------------------------------------------
		sodl::drwMngr->drawUpdt();

		++count;
		Sleep(30);
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
	 * @brief	ini�t�@�C������f�t�H���g�v���b�g��̃w�b�_���擾
	 * 			GetDefaultPlotHeaderNameFromIni()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void GetDefaultPlotHeaderNameFromIni(
		std::string 				iniFilePath,
		std::vector<std::string>&	headers
		)
	{
		char buf[100];
		if( 6 <= headers.size() )
		{
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "X", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[0] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "Y", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[1] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "Z", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[2] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "I", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[3] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "J", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[4] = buf;
			GetPrivateProfileStringA( "DEFAULT_3D_PLOT_COL", "K", "NoKey", buf, 100, iniFilePath.c_str() );
			headers[5] = buf;
		}
	}


	/** ***************************************************************
	 *
	 * @brief	csv�̃w�b�_�s���󂯂āA�O���t�v���b�g�̌n����\�z����
	 * 			Func_ReadFile_ToPlotLine()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void ReadCsvHeader_CreatePlotLine(
			std::string csvHeader,
			std::string pltLnName,
			app::ST_PLOT_FILE& plotFile,
			std::shared_ptr<sodl::GraphObj> grph,
			appUi::Vif_PlotCnfg& uidat
			)
	{
		uidat.pltLnName = pltLnName;


		// �w�b�_�s�f�[�^��','��؂��token�ɕ�������
		std::vector<std::string> tokens;
		app::SplitString(csvHeader, ",", tokens);

		// 1�s���̃g�[�N�������[�v��������
		std::vector<float> dataOfLine;
		for (int col = 0; col < tokens.size(); ++col)
		{
			// �O���t�̃A�g���r���[�g�z��ɃA�g���r���[�g���ǉ�����B
			grph->AddAtr(tokens[col], pltLnName );

			// �A�g���r���[�g����UI�\���p�ɕێ�����
			uidat.vct_labelsOfAttribetes.emplace_back( tokens[col] );
		}

		//-------------------------------------------------------------------
		// ini�t�@�C������f�t�H���g�v���b�g���̗񖼏̂��擾����
		//-------------------------------------------------------------------
		std::vector< std::string > vct_defaultPltColNms(6);
		GetDefaultPlotHeaderNameFromIni( ".\\plot.ini", vct_defaultPltColNms );

		//-------------------------------------------------------------------
		// 1.2.1 �񖼏̂�����̕��тɈ�v����Ƃ��A���ʏ��������{���邽�߂̃t���O�𗧂Ă�
		//-------------------------------------------------------------------
		// ini�t�@�C���ݒ�̃f�t�H���g�v���b�g��XYZ���������݂��邩�`�F�b�N����
		for (int col = 0; col < tokens.size(); ++col)
		{
			if( std::string::npos != tokens[col].find( vct_defaultPltColNms[0] ) )
			{
				plotFile.cX = col;
			}
			if( std::string::npos != tokens[col].find( vct_defaultPltColNms[1] ) )
			{
				plotFile.cY = col;
			}
			if( std::string::npos != tokens[col].find( vct_defaultPltColNms[2] ) )
			{
				plotFile.cZ = col;
			}
		}
		if( ( -1 != plotFile.cX )
		&&	( -1 != plotFile.cY )
		&&	( -1 != plotFile.cZ )
		)
		{
			// XYZ�񑶍݃t���OON
			plotFile.fg_xyz = true;

			// ini�t�@�C���ݒ�̃f�t�H���g�v���b�g��IJK���������݂��邩�`�F�b�N����
			for (int col = 0; col < tokens.size(); ++col)
			{
				if( std::string::npos != tokens[col].find( vct_defaultPltColNms[3] ) )
				{
					plotFile.cI = col;
				}
				if( std::string::npos != tokens[col].find( vct_defaultPltColNms[4] ) )
				{
					plotFile.cJ = col;
				}
				if( std::string::npos != tokens[col].find( vct_defaultPltColNms[5] ) )
				{
					plotFile.cK = col;
				}
			}
			if( ( -1 != plotFile.cI )
			&&	( -1 != plotFile.cJ )
			&&	( -1 != plotFile.cK )
			)
			{
				// XYZIJK�񑶍݃t���OON
				plotFile.fg_xyzijk = true;
				uidat.fg_cBox_barEnable = true;
			}
		}
		// �f�t�H���g�v���b�g��XYZ���������݂��邩�`�F�b�N����
		else
		{
			// ��̐擪�� x, y, z �ł���ꍇ
			if( ( tokens.size() >= 3 )
			&&	(	("x" == tokens[0] )
				||  ("X" == tokens[0] )
				)
			&&  (	("y" == tokens[1] )
				||  ("Y" == tokens[1] )
				)
			&&  (	("z" == tokens[2] )
				||  ("Z" == tokens[2] )
				)
			)
			{
				// �擪�񂩂珇�� x, y, z �̖��̂�csv�ɋL�ڂ���Ă���t���O ON
				plotFile.fg_xyz = true;
				plotFile.cX = 0;
				plotFile.cY = 1;
				plotFile.cZ = 2;

				// �㑱�̗� i, j, k �ł���ꍇ�A���������x�N�g���Ƃ��Ĉ���
				if( ( tokens.size() >= 6 )
				&&	(	("i" == tokens[3] )
					||  ("I" == tokens[3] )
					)
				&&  (	("j" == tokens[4] )
					||  ("J" == tokens[4] )
					)
				&&  (	("k" == tokens[5] )
					||  ("K" == tokens[5] )
					)
				)
				{
					// x, y, z �㑱���� i, j, k �̖��̂�csv�ɋL�ڂ���Ă���t���O ON
					plotFile.fg_xyzijk = true;
					uidat.fg_cBox_barEnable = true;
					plotFile.cI = 3;
					plotFile.cJ = 4;
					plotFile.cK = 5;
				}
			}
		}
	}



	/** ***************************************************************
	 *
	 * @brief	csv�̍s�x�N�g�����󂯂āA�O���t�v���b�g�̍s�ɉ�����
	 * 			Func_ReadFile_ToPlotLine()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void ReadCsvData_ToPlotLine(
		std::string pltRowStr,
		std::string pltLnName,
		app::ST_PLOT_FILE& plotFile,
		std::shared_ptr<sodl::GraphObj> grph
		)
	{
		// 1�s�f�[�^��','��؂��token�ɕ�������
		std::vector<std::string> tokens;
		app::SplitString(pltRowStr, ",", tokens);

		//-------------------------------------------------------------------
		// 1.2 1�s�ڂ̓w�b�_�Ȃ̂œ��ʏ��������{����
		//-------------------------------------------------------------------
		if( plotFile.seekRow == 0 )
		{

		}
		//-------------------------------------------------------------------
		// 1.3 �w�b�_�s�ȍ~�̃f�[�^����������
		//-------------------------------------------------------------------
		else
		{
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

				// �A�g���r���[�g�f�[�^���O���t�I�u�W�F�N�g�ɃZ�b�g����
				grph->AddAtrData( col, fDat , pltLnName);
			}

			//-----------------------------------------------------
			// 1.4 �ǂݏo�����f�[�^���n��1�̃v���b�g�Ώۍ��W�Ƃ��ăZ�b�g����
			//-----------------------------------------------------
			// �擪�񂩂珇�� x, y, z �̖��̂�csv�ɋL�ڂ���Ă���ꍇ
			if( plotFile.fg_xyz )
			{
				// 0�`2��ڃf�[�^���v���b�g���W x, y, z �Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
				grph->AddData( Eigen::Vector3f(dataOfLine[plotFile.cX], dataOfLine[plotFile.cY], dataOfLine[plotFile.cZ]), pltLnName);

				// x, y, z �㑱���� i, j, k �̖��̂�csv�ɋL�ڂ���Ă���ꍇ
				if( plotFile.fg_xyzijk )
				{
					// 3�`5��ڂ�����x�N�g���Ƃ��ăO���t�I�u�W�F�N�g�ɃZ�b�g����
					grph->AddPtVct( Eigen::Vector3f(dataOfLine[plotFile.cI], dataOfLine[plotFile.cJ], dataOfLine[plotFile.cK]), pltLnName);
				}

			}
			// �擪�񂩂珇�� x, y, z �̖��̂�csv�ɋL�ڂ���Ă��Ȃ��ꍇ
			else
			{
				// 0��ڃf�[�^��y���W�Ƃ��ăv���b�g(n, y, 0)���O���t�I�u�W�F�N�g�ɃZ�b�g����
				grph->AddData( Eigen::Vector3f( plotFile.seekRow, dataOfLine[0], 0 ), pltLnName);
			}
		}
	}



	/** ***************************************************************
	 *
	 * @brief	�t�@�C����Ǐo���ăO���t�̃v���b�g�n��ɉ�����
	 * 			Func_ReadFile_ToPlotLine()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void Func_ReadFile_ToPlotLine(
			std::string pltLnName,
			std::shared_ptr<sodl::GraphObj> grph,
			app::ST_PLOT_FILE& plotFile,
			appUi::Vif_PlotCnfg& uidat
			)
	{

		//-----------------------------------------------------
		// 1. �v���b�g�f�[�^��csv�t�@�C������ǂݏo���Čn��1�Ƃ��ăO���t�ɃZ�b�g
		//-----------------------------------------------------
		// EOF�r�b�g�����N���A���Ă���
		plotFile.ifs.clear();
		if( 0 != plotFile.seekPos  )
		{
			plotFile.ifs.seekg( plotFile.seekPos );
		}
		std::string line;
		while( std::getline( plotFile.ifs, line ) )
		{
			if( 0 == plotFile.seekRow )
			{
				ReadCsvHeader_CreatePlotLine(
						line,
						pltLnName,
						plotFile,
						grph,
						uidat
						);
			}
			else
			{
				ReadCsvData_ToPlotLine(
						line,
						pltLnName,
						plotFile,
						grph
						);
			}

			++plotFile.seekRow;									// �Ǐo���s���̃J�E���g
			plotFile.seekPos = plotFile.ifs.tellg();			// ���̓Ǐo���ʒu���������Ă���
		}
	};


	/** ***************************************************************
	 *
	 * @brief	2D�v���b�g��ǉ�����
	 * 			Add2DPlot()
	 * <pre>
	 * 2D�v���b�g�̎��̂𐶐����A
	 * �A�v���P�[�V�����X�R�[�v��2D�v���b�g�ێ��p�\���̂ɕێ�����B
	 * �����I�ɍ폜���Ȃ�����A�A�v���P�[�V�����̏I���܂ŕێ������B
	 * </pre>
	 ******************************************************************/
	std::shared_ptr<sodl::GraphObj> Add2DPlot( std::string name )
	{
		//----------------------------------------------------
		// �`���ԁA�r���[�|�[�g�A�J�����̒ǉ��Ɛݒ�
		//----------------------------------------------------

		// �O���t�p�ɕ`���Ԃ�ǉ�
		int idx_ds = sodl::drwMngr->drawingSpace->size();
		auto spaceGrph = sodl::drwMngr->addDrawingSpace();
		// �r���[�|�[�g�쐬
		const int hgt = 200;
		auto vpGrph = sodl::drwMngr->addViewPort("vp" + name);
		vpGrph->setVpSize(
			app::WINDOW_SIZE_X / 2,		// left
			hgt * (idx_ds-1),			// bottom
			app::WINDOW_SIZE_X / 2,		// width
			hgt							// height
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
		auto grph = sodl::GraphObj::create(
			name,									// �I�u�W�F�N�g���� = �O���t�^�C�g��
			camGrph									// �֘A�t����J����(�J�������e�s��Ȃǂ̐ݒ肪�ς�ł��邱��)
		);
		// ���x���̐ݒ�
		grph->title->text = name;
		grph->xAxisLabel->text = "plot data idx";
		grph->yAxisLabel->text = name;
		// �`����[2]�ɃO���t��ǉ�
		sodl::drwMngr->AddObjTree_ToDrwSpace(grph, idx_ds);

		// �쐬�����O���t���A�v���P�[�V������2D�O���t�z��ɕێ�����
		app::graphs2D.insert( std::make_pair( name, grph ) );

		return grph;
	}



	/** ***************************************************************
	 *
	 * @brief	ImGui�̃��C���`�揈��
	 * 			my_display_code()
	 * <pre>
	 * </pre>
	 ******************************************************************/
	void ImGui_DrawAndOperate()
	{
		visualIf.drawVif();
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

	    ImGui_DrawAndOperate();

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


