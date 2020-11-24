#include <iostream>

#include "include/Drawable3dMdlClass.h"

#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#pragma comment( lib, "glew32.lib" )
#pragma comment( lib, "freeglut.lib" )

using namespace SmplObjDrwLib;

//--------------------------------------------
// コンストラクタ
//--------------------------------------------
Drawable3dMdlClass::Drawable3dMdlClass(std::string modelFilePath)
	: IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	if (1) // assimp
	{
		struct aiLogStream stream;

		/* get a handle to the predefined STDOUT log stream and attach
		it to the logging system. It remains active for all further
		calls to aiImportFile(Ex) and aiApplyPostProcessing. */
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
		aiAttachLogStream(&stream);

		/* ... same procedure, but this stream now writes the
		log messages to assimp_log.txt */
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
		aiAttachLogStream(&stream);

		/* the model name can be specified on the command line. If none
		is specified, we try to locate one of the more expressive test
		models from the repository (/models-nonbsd may be missing in
		some distributions so we need a fallback from /models!). */
		modelScene = aiImportFile(modelFilePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		if ( modelScene == NULL )
		{
			std::cout << "assimp: load model error" << std::endl;
		}

		isAssimpModel = true;

	}
}

//--------------------------------------------
// ファクトリ関数
//--------------------------------------------
std::shared_ptr<Drawable3dMdlClass> Drawable3dMdlClass::create
(
	std::string modelFilePath
)
{
	return std::shared_ptr< Drawable3dMdlClass >(
		new Drawable3dMdlClass(modelFilePath) 
	);
};


void Drawable3dMdlClass::draw()
{
	if (this->isAssimpModel)
	{
		asmpGlUtil::recursive_render(modelScene, modelScene->mRootNode);
	}
}


