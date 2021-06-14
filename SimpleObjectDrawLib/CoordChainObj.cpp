#include "include/DrawableObjectClass.h"
#include "include/glDrawExt.h"

#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#pragma comment( lib, "glew32.lib" )

using namespace SmplObjDrwLib;


/** ***************************************************************
 * @brief コンストラクタ
 *
 ******************************************************************/
CoordChainObj::CoordChainObj(std::string name, std::weak_ptr<CoordChainObj> parent)
	: ISodlObjBase(name)
	, IDrawableObjBase(name)
	, wPtr_prnt(parent)
	, wPtr_chldrn()
{
	CrdTrs.linear() = Eigen::Matrix3f::Identity();
	CrdTrs.translation() = Eigen::Vector3f(0, 0, 0);
}

/** ***************************************************************
 * @brief デストラクタ
 *
 ******************************************************************/
CoordChainObj::~CoordChainObj()
{

}


/** ***************************************************************
 * @brief ファクトリ関数
 *
 ******************************************************************/
std::shared_ptr<CoordChainObj> CoordChainObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< CoordChainObj >(
		new CoordChainObj(name, parent)
		);

	// 親の連鎖座標系に作成したオブジェクトを子として登録する
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

/** ***************************************************************
 * @brief 描画関数
 * <pre>
 * [機能概要]
 * 連鎖座標系を考慮して自己形状を描画する。
 *
 * [機能詳細]
 * SODLの描画マネージャによって DrawingManager::drawUpdt() の度にcallされる。
 * CoordChainObj とその派生クラスは、本関数の中で連鎖座標系における
 * 本オブジェクトの位置を再計算して、OpenGLのModel-Viewマトリックスを
 * 更新し、描画環境に反映させる。その後 CoordChainObj::_drawShapeOfSelf() によって
 * 親座標系の中における自己形状を描画する。
 *
 * 派生クラスは CoordChainObj::_drawShapeOfSelf() のみをoverrideして
 * draw()の座標変換のギミックを流用しつつ描画形状を派生させる。
 *
 * </pre>
 ******************************************************************/
void CoordChainObj::draw()
{
	if (!visible)
	{
		return;
	}

	GLfloat m[16];
	cvtMatrix_Eigen2GL(GetTf_root2self().matrix(), m);

	glPushMatrix();
	glMultMatrixf(m);

	// line parent to self
	if (visible_CoordChain)
	{
		auto prnt = this->wPtr_prnt.lock();
		if (NULL != prnt)
		{
			auto Tf_inv = CrdTrs.inverse();
			auto pos = Tf_inv.translation();

			float x = pos.x();
			float y = pos.y();
			float z = pos.z();

			glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(0, 0, 0);
			glVertex3f(pos.x(), pos.y(), pos.z());
			glEnd();
		}
	}

	// draw Shape of self
	_drawShapeOfSelf();

	glPopMatrix();

}


/** ***************************************************************
 * @brief 座標系連鎖の親との関係を削除する
 *
 ******************************************************************/
void CoordChainObj::DetachSelf_fromParentsCoordChain()
{
	auto sPtr_prnt = wPtr_prnt.lock();
	if (NULL != sPtr_prnt)
	{
		// 親の子供リストを探索
		for (auto itr = sPtr_prnt->wPtr_chldrn.begin();
			itr != sPtr_prnt->wPtr_chldrn.end();
			itr++
			)
		{
			// 自分自身を見つけた?
			if (itr->lock().get() == this)
			{
				// 自分をリストから削除してループを抜ける
				sPtr_prnt->wPtr_chldrn.erase(itr);
				break;
			}
		}
	}
}


/** ***************************************************************
 * @brief 座標系連鎖の親子関係に子を追加する
 *
 ******************************************************************/
void CoordChainObj::AttachSelf_toParentsCoordChain(std::weak_ptr<CoordChainObj> parent)
{
	auto prnt = parent.lock();
	if (prnt != NULL)
	{
		prnt->wPtr_chldrn.push_back(std::dynamic_pointer_cast<CoordChainObj>(shared_from_this()));
		this->wPtr_prnt = parent;
	}
}


/** ***************************************************************
 * @brief 自己形状の描画
 *
 ******************************************************************/
void CoordChainObj::_drawShapeOfSelf()
{
	drawCoordSymbol((unsigned char*)name.c_str(), 100.0);
}

//================================================================
//
//	<Summary>		GetTf_root2self()
//	<Description>
//================================================================
Eigen::Affine3f CoordChainObj::GetTf_root2self()
{
	auto prnt = wPtr_prnt.lock();
	if (prnt != NULL)
	{
		return (prnt->GetTf_root2self() * this->CrdTrs);
	}
	else
	{
		return this->CrdTrs;
	}
}

/** ***************************************************************
 * @brief 描画空間へのオブジェクトの追加
 *
 ******************************************************************/
void CoordChainObj::RemoveSelfRecursive_FromDrawingSpace()
{
	// 自身を描画空間から削除
	this->RemoveSelf_FromDrawingSpace();

	// 子への再帰処理
	for( auto	it = wPtr_chldrn.begin();
				it != wPtr_chldrn.end();
			  ++it)
	{
		auto sPtr_child = it->lock();
		sPtr_child->RemoveSelfRecursive_FromDrawingSpace();
	}
}




