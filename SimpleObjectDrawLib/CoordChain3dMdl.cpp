#include "include/CoordChain3dMdl.h"
#include "include/glDrawExt.h"

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
CoordChain3dMdl::CoordChain3dMdl(
	std::string modelFilePath,
	std::string coordName,
	std::weak_ptr<CoordChainObj> parent
)
	: Drawable3dMdlClass(modelFilePath)
	, CoordChainObj(coordName, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	;
}

//--------------------------------------------
// ファクトリ関数
//--------------------------------------------
std::shared_ptr< CoordChain3dMdl > CoordChain3dMdl::create
(
	std::string modelFilePath,
	std::string coordName,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< CoordChain3dMdl >(
		new CoordChain3dMdl(modelFilePath, coordName, parent)
	);

	auto prnt = parent.lock();
	if (prnt != NULL)
	{
		prnt->wPtr_chldrn.push_back(ptr);
	}

	return ptr;
}

void CoordChain3dMdl::draw()
{
	if (!visible)
	{
		return;
	}

	Eigen::Matrix4f mtx = GetTf_root2self().matrix();

	GLfloat m[] = {
		mtx(0,0),mtx(1,0),mtx(2,0),mtx(3,0) ,
		mtx(0,1),mtx(1,1),mtx(2,1),mtx(3,1) ,
		mtx(0,2),mtx(1,2),mtx(2,2),mtx(3,2) ,
		mtx(0,3),mtx(1,3),mtx(2,3),mtx(3,3) ,
	};

	glPushMatrix();
	glMultMatrixf(m);

	asmpGlUtil::recursive_render(modelScene, modelScene->mRootNode);
	
	//	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	//	print_glMatlix(m);

	glPopMatrix();

}