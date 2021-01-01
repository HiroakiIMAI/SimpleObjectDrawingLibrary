#include "include/DrawableObjectClass.h"
#include "include/glDrawExt.h"


#include <glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#pragma comment( lib, "glew32.lib" )



using namespace SmplObjDrwLib;



/////////////////////////////////////////////////////////////////////
//
//	PlaneObj
//	平面
//

//================================================================
//
//	<Summary>		コンストラクタ
//	<Description>
//================================================================
PlaneObj::PlaneObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summary>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr<PlaneObj> PlaneObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< PlaneObj >(
		new PlaneObj(name, parent)
		);

	// 親の連鎖座標系に作成したオブジェクトを子として登録する
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summary>		自己形状の描画
//	<Description>
//================================================================
void PlaneObj::_drawShapeOfSelf()
{
	glBegin(GL_POLYGON);
	glColor4fv( this->color.fv4 );
	glVertex3f(0, 0, 0);
	glVertex3f(width, 0, 0);
	glVertex3f(width, height, 0);
	glVertex3f(0, height, 0);
	glEnd();

}




/////////////////////////////////////////////////////////////////////
//
//	Box
//	直方体
//

//================================================================
//
//	<Summary>		コンストラクタ
//	<Description>
//================================================================
BoxObj::BoxObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summary>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr<BoxObj> BoxObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< BoxObj >(
		new BoxObj(name, parent)
		);

	// 親の連鎖座標系に作成したオブジェクトを子として登録する
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summary>		自己形状の描画
//	<Description>
//================================================================
void BoxObj::_drawShapeOfSelf()
{
	if ((drawType == POLYGON)
	||	(drawType == WIRE_POLYGON)
	)
	{
		glColor4fv(this->color.fv4);
		Eigen::Vector3f& b = boxSize;

		//下底
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, 0);
		glVertex3f(0, b.y(), 0);
		glVertex3f(b.x(), b.y(), 0);
		glVertex3f(b.x(), 0, 0);
		glEnd();

		//上底
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, b.z());
		glVertex3f(b.x(), 0, b.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(0, b.y(), b.z());
		glEnd();

		//y0面
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, b.z());
		glVertex3f(0, 0, 0);
		glVertex3f(b.x(), 0, 0);
		glVertex3f(b.x(), 0, b.z());
		glEnd();

		//y+面
		glBegin(GL_POLYGON);
		glVertex3f(0, b.y(), b.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(b.x(), b.y(), 0);
		glVertex3f(0, b.y(), 0);
		glEnd();

		//x0面
		glBegin(GL_POLYGON);
		glVertex3f(0, 0, b.z());
		glVertex3f(0, b.y(), b.z());
		glVertex3f(0, b.y(), 0);
		glVertex3f(0, 0, 0);
		glEnd();

		//x+面
		glBegin(GL_POLYGON);
		glVertex3f(b.x(), 0, b.z());
		glVertex3f(b.x(), 0, 0);
		glVertex3f(b.x(), b.y(), 0);
		glVertex3f(b.x(), b.y(), b.z());
		glEnd();
	}

	if( (drawType == WIRE)
	||	(drawType == WIRE_POLYGON)
	)
	{
		glColor4fv(this->colorWire.fv4);
		drawBox(Eigen::Vector3f(0, 0, 0), boxSize);
	}

}


/////////////////////////////////////////////////////////////////////
//
//	Points
//	点列
//

//================================================================
//
//	<Summary>		コンストラクタ
//	<Description>
//================================================================
PointsObj::PointsObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summary>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr<PointsObj> PointsObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< PointsObj >(
		new PointsObj(name, parent)
		);

	// 親の連鎖座標系に作成したオブジェクトを子として登録する
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summary>		自己形状の描画
//	<Description>
//================================================================
void PointsObj::_drawShapeOfSelf()
{
	if(drawType == POLYGON)
	{
		glBegin(GL_POLYGON);
		glColor4fv(this->color.fv4);
	}
	else if(drawType == WIRE)
	{
		glBegin(GL_LINE_STRIP);
		glColor4fv(this->colorWire.fv4);
	}


	for (auto itr = points.begin();
		itr != points.end();
		itr++)
	{
		glVertex3f(itr->x(), itr->y(), itr->z());
	}

	glEnd();

}



/////////////////////////////////////////////////////////////////////
//
//	PointsWithAttibutes
//	属性データ付き点列
//

//================================================================
//
//	<Summary>		コンストラクタ
//	<Description>
//================================================================
PointsWithAttributes::PointsWithAttributes(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	visible_CoordChain = FALSE;
	_sPtr_points = std::make_shared< std::deque< Eigen::Vector3f> >();
}

//================================================================
//
//	<Summary>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr<PointsWithAttributes> PointsWithAttributes::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< TypeOfSelf >(
		new TypeOfSelf(name, parent)
		);

	// 親の連鎖座標系に作成したオブジェクトを子として登録する
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summary>		自己形状の描画
//	<Description>
//================================================================
void PointsWithAttributes::_drawShapeOfSelf()
{
	if (drawType == POLYGON)
	{
		glBegin(GL_POLYGON);
		glColor4fv(this->color.fv4);
	}
	else if (drawType == WIRE)
	{
		glBegin(GL_LINE_STRIP);
		glColor4fv(this->colorWire.fv4);
	}

	// 点群がセットされている場合
	if (_sPtr_points)
	{
		// 点群をプロットする
		for (auto itr = _sPtr_points->begin();
			itr != _sPtr_points->end();
			itr++)
		{
			glVertex3f(itr->x(), itr->y(), itr->z());
		}
	}
	glEnd();


	glBegin(GL_LINES);
	// 点群がセットされていて、属性バーの表示が有効な場合
	if(_sPtr_points 
	&& ( -1 != atrIdx_bar )							// バー表示用の属性idxが設定されている
	&& ( _sPtr_attributes.size() != atrIdx_bar)		// 指定された属性idxの属性ポインタが存在する
	&& ( _sPtr_attributes[ atrIdx_bar ] )			// 指定された属性idxの属性ポインタの指す先がNULLでない
	)
	{
		const auto  _sPtr_attribute = _sPtr_attributes[atrIdx_bar];
		// 点群をプロットする
		for (int i = 0;
			(	( i < _sPtr_points->size() )
			&&	( i < _sPtr_attribute->size() )
			);
			i++)
		{
			const auto pt  = &(*_sPtr_points)[i];
			const auto atr = (*_sPtr_attribute)[i];
			const auto barEnd = (*pt) + (atr*(atrBarDirec));

			// 点から属性値に応じた長さのバーを描画する
			glVertex3f(pt->x(), pt->y(), pt->z());
			glVertex3f(barEnd.x(), barEnd.y(), barEnd.z());
		}
		std::cout << _sPtr_points->size() << ", " << _sPtr_attribute->size() << std::endl;
	}
	glEnd();



}



/////////////////////////////////////////////////////////////////////
//
//	Label
//	テキストラベル
//

//================================================================
//
//	<Summary>		コンストラクタ
//	<Description>
//================================================================
LabelObj::LabelObj(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	copyColor4fv( color4fv::BLACK, color.fv4 );
	visible_CoordChain = FALSE;
}

//================================================================
//
//	<Summary>		ファクトリ関数
//	<Description>
//================================================================
std::shared_ptr<LabelObj> LabelObj::create(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
{
	auto ptr = std::shared_ptr< LabelObj >(
		new LabelObj(name, parent)
		);

	// 親の連鎖座標系に作成したオブジェクトを子として登録する
	ptr->AttachSelf_toParentsCoordChain(parent);

	return  ptr;
}

//================================================================
//
//	<Summary>		自己形状の描画
//	<Description>
//================================================================
void LabelObj::_drawShapeOfSelf()
{
	
	////////////////////////////////////////////////
	// ストロークサイズが所望になるように
	// 一時的にprojectionMatrixを修正する
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// 無調整で印字した場合の文字サイズを取得する
	float rawHgt = glutStrokeHeight(GLUT_STROKE_ROMAN);

	// 文字サイズ調整用の拡縮倍率を取得する
	float rate = rawHgt / size;
	// 拡縮の中心位置を取得
	float x = GetTf_root2self().translation().x();
	float y = GetTf_root2self().translation().y();	
	glOrtho(
		x - (x*rate), x + ((vpSizeX - x)*rate), 
		y - (y*rate), y + ((vpSizeY - y)*rate),
		-1000, 1000);
	
	

	// name ラベルの描画
	glColor4fv(color.fv4);			// 文字色の設定
	glRasterPos3d(0, 0, 0.);		// 0,0,0 位置をスタート位置にする

	////////////////////////////////////////////////
	// 文字列レンダリングの内部でカーソルを進めるために
	// ModelViewMatrixに復帰する
	glMatrixMode(GL_MODELVIEW);

	// テキストアラインメント
	float rawLength = glutStrokeLengthf(GLUT_STROKE_ROMAN, (unsigned char*)text.c_str());
	float offset = 0;
	switch ( align )
	{
	case LabelAlign::RIGHT:
		offset = -rawLength;
		break;
	case LabelAlign::CENTER:
		offset = -rawLength / 2;
		break;
	case LabelAlign::LEFT:
	default:
		break;
	}
	glTranslatef(offset, 0, 0);

	// 文字列レンダリング
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(text.c_str()) );

	////////////////////////////////////////////////
	// projectionMatrixを復元する
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	////////////////////////////////////////////////
	// ModelViewMatrixに復帰する
	glMatrixMode(GL_MODELVIEW);

}

//================================================================
//
//	<Summary>		メンバのラベル全てに対してグラフサイズを通知する
//	<Description>	
//					
//================================================================
void LabelObj::SetVpSizeToChildrenLabel(
	std::shared_ptr<CoordChainObj> obj,
	float size_x,
	float size_y
)
{
	// 与えられたオブジェクトがLabelObjならば、vpSizeにグラフサイズを設定する
	if (auto sPtr_labelObj = std::dynamic_pointer_cast<LabelObj>(obj))
	{
		sPtr_labelObj->vpSizeX = size_x;
		sPtr_labelObj->vpSizeY = size_y;
	}

	// 全ての子要素を探索し、再帰する
	for (auto child = obj->wPtr_chldrn.begin();
		child != obj->wPtr_chldrn.end();
		child++)
	{
		auto sPtr_child = child->lock();
		if (sPtr_child)
		{
			SetVpSizeToChildrenLabel(sPtr_child, size_x, size_y);
		}
	}
}

////////////////
//debug func
/*
void print_glMatlix(GLfloat* m)
{
printf("%f, %f, %f, %f\n", m[0], m[4], m[8], m[12]);
printf("%f, %f, %f, %f\n", m[1], m[5], m[9], m[13]);
printf("%f, %f, %f, %f\n", m[2], m[6], m[10], m[14]);
printf("%f, %f, %f, %f\n", m[3], m[7], m[11], m[15]);
printf("\n");
}
*/
