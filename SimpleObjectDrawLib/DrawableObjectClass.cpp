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
	else if(drawType == POINT)
	{
		glBegin(GL_POINTS);
		glColor4fv(this->color.fv4);
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
	_sPtr_ptVctrs = std::make_shared< std::deque< Eigen::Vector3f> >();
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

/** ***************************************************************
 * @brief 自己形状の描画
 *
 ******************************************************************/
void PointsWithAttributes::_drawShapeOfSelf()
{
	// 点群がセットされている場合
	if (_sPtr_points)
	{
		// デフォルト色の設定
		glColor4fv(this->color.fv4);

		GLfloat tick_bk;
		glGetFloatv( GL_LINE_WIDTH, &tick_bk );

		// 描画幅変更用のファンクタを用意する
		std::function<void(GLfloat)> Func_ChgTickness;
		if( drawType == POINT )
		{
			Func_ChgTickness = 	glPointSize;
		}
		else
		{
			Func_ChgTickness = 	glLineWidth;
		}
		Func_ChgTickness( this->pointTickness );

		// 描画タイプ別のglBegin引数を用意する
		GLenum glBeginArg;
		if (drawType == POLYGON)
		{
			glBeginArg = GL_POLYGON;
		}
		else if (drawType == WIRE)
		{
			glBeginArg = GL_LINE_STRIP;
		}
		else if (drawType == POINT)
		{
			glBeginArg = GL_POINTS;
		}
		else
		{
			glBeginArg = GL_LINE_STRIP;
		}

		//-------------------------------------------------------------------
		// 1. 座標点列の描画
		//-------------------------------------------------------------------
		// 点群サイズを取得
		int size_points = _sPtr_points->size();

		// 色指定の有無をチェック
		int size_colors = 0;
		auto sPtr_AtrColors = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_pointColor )
		&&	( _sPtr_attributes[atrIdx_pointColor] )
		)
		{
			sPtr_AtrColors = _sPtr_attributes[atrIdx_pointColor];
			size_colors = sPtr_AtrColors->data.size();
		}

		// 幅指定の有無をチェック
		int size_tickness = 0;
		auto sPtr_AtrTickness = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_pointTickness )
		&&	( _sPtr_attributes[atrIdx_pointTickness] )
		)
		{
			sPtr_AtrTickness = _sPtr_attributes[atrIdx_pointTickness];
			size_tickness = sPtr_AtrTickness->data.size();
		}

		// 描画開始
		glBegin( glBeginArg );

		// 点群を走査
		for(int i = 0; i < size_points; ++i)
		{
			//----------------------------------------
			// プロット色の設定
			//----------------------------------------
			// 色指定列のデータを使用する場合
			if(	( sPtr_AtrColors )	// 色指定列が有効な場合
			&&	( i < size_colors )	// 座標値に対応する色データがある場合
			)
			{
				ST_COLOR colorRgb;
				// 色指定列のデータからヒートマップカラーを取得する
				SmplObjDrwLib::GetNormalizedHeatColor(
					sPtr_AtrColors->data[i],
					sPtr_AtrColors->max,
					sPtr_AtrColors->min,
					colorRgb.fv4);

				// 色指定列のデータから生成したRGB値を使用する。
				glColor4fv(colorRgb.fv4);
			}
			// 色指定列のデータを使用しない場合
			else
			{
				// 点列クラスに設定された色を使用する。
				glColor4fv(this->colorWire.fv4);
			}


			//----------------------------------------
			// プロット幅の設定
			//----------------------------------------
			// 幅指定列のデータを使用する場合
			if(	( sPtr_AtrTickness )	// 色指定列が有効な場合
			&&	( i < size_tickness )	// 座標値に対応する色データがある場合
			)
			{
				// 描画モードが線である場合は、一度現在の点位置まで線を引き
				// 線を終了してから幅を変えて再度引き始めるために、
				// このタイミングでvertexを一つ撒く
				if( drawType != POINT )
				{
					// 終点をプロットする
					const auto& pt = (*_sPtr_points)[i];
					glVertex3f(pt.x(), pt.y(), pt.z());
				}

				//--------------------------------------------
				// 一度描画を終了してから幅を変えて描画再開する
				//--------------------------------------------
				// 描画終了
				glEnd();
				// 指定アトリビュート列のデータを幅として設定
				Func_ChgTickness( sPtr_AtrTickness->data[i] );

				// 描画再開
				glBegin( glBeginArg );
				// このブロックの外のglVertex3fで始点が描画される。
			}

			// 点群をプロットする
			const auto& pt = (*_sPtr_points)[i];
			glVertex3f(pt.x(), pt.y(), pt.z());
		}
		glEnd();



		//-------------------------------------------------------------------
		// 2. バーの描画
		//-------------------------------------------------------------------
		// デフォルト色の設定
		glColor4fv( this->color.fv4 );
		// デフォルト幅の設定
		glLineWidth( this->barWidth );

		// 色指定の有無をチェック
		int size_barColors = 0;
		auto sPtr_barColors = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_barColor )
		&&	( _sPtr_attributes[atrIdx_barColor] )
		)
		{
			sPtr_barColors = _sPtr_attributes[atrIdx_barColor];
			size_barColors = sPtr_barColors->data.size();
		}


		// 幅指定の有無をチェック
		int size_barWidth = 0;
		auto sPtr_barWidth = std::shared_ptr< SmplObjDrwLib::AttributeClass<float> >();
		if(	( ATRIDX_NONE != atrIdx_barWidth )
		&&	( _sPtr_attributes[atrIdx_barWidth] )
		)
		{
			sPtr_barWidth = _sPtr_attributes[atrIdx_barWidth];
			size_barWidth = sPtr_barWidth->data.size();
		}


		glBegin(GL_LINES);
		// 点群がセットされていて、属性バーの表示が有効な場合
		if(( ATRIDX_NONE != atrIdx_bar )							// バー表示用の属性idxが設定されている
		&& ( _sPtr_attributes.size() != atrIdx_bar)		// 指定された属性idxの属性ポインタ配列要素が存在する
		&& ( _sPtr_attributes[ atrIdx_bar ] )			// 指定された属性idxの属性ポインタの指す先がNULLでない
		)
		{
			const auto  sPtr_attribute = _sPtr_attributes[atrIdx_bar];
			// 点群をプロットする
			for (int i = 0;
				(	( i < _sPtr_points->size() )
				&&	( i < sPtr_attribute->data.size() )
				);
				i++)
			{
				//-------------------------------------------------------------------
				// 2.1 バー色の指定
				//-------------------------------------------------------------------
				if(	( sPtr_barColors )	// 色指定列が有効な場合
				&&	( i < size_barColors )	// 座標値に対応する色データがある場合
				)
				{
					// 色データを色相としてHSV→RGB変換
					ST_COLOR colorRgb;
					// 色指定列のデータからヒートマップカラーを取得する
					SmplObjDrwLib::GetNormalizedHeatColor(
						sPtr_barColors->data[i],
						sPtr_barColors->max,
						sPtr_barColors->min,
						colorRgb.fv4);
					glColor4fv(colorRgb.fv4);
				}

				//-------------------------------------------------------------------
				// 2.1 バー幅の指定
				//-------------------------------------------------------------------
				if(	( sPtr_barWidth )		// 幅指定列が有効な場合
				&&	( i < size_barWidth )	// 座標値に対応する幅データがある場合
				)
				{
					glEnd();
					float mx = MAX( sPtr_barWidth->max, 0.0001);
					glLineWidth( sPtr_barWidth->data[i] / sPtr_barWidth->max * 10.0);
					glBegin(GL_LINES);
				}

				// バーの方向を固定値で初期化する
				auto direc = atrBarDirec;
				// 方向ベクトル配列が設定されている場合は、配列の値を使用する
				if( _sPtr_ptVctrs->size() > i )
				{
					direc = (*_sPtr_ptVctrs)[i];
				}

				const auto  pt  = &(*_sPtr_points)[i];
				const auto& atr = sPtr_attribute->data[i];
				const auto  barEnd = (*pt) + ( atr * direc );

				// 点から属性値に応じた長さのバーを描画する
				glVertex3f(pt->x(), pt->y(), pt->z());
				glVertex3f(barEnd.x(), barEnd.y(), barEnd.z());
			}
		}
		glEnd();

		glLineWidth( tick_bk );
	}

}



/////////////////////////////////////////////////////////////////////
//
//	Label
//	テキストラベル
//

/** ***************************************************************
 * @brief コンストラクタ
 *
 ******************************************************************/
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

/** ***************************************************************
 * @brief ファクトリ関数
 *
 ******************************************************************/
std::shared_ptr<LabelObj> LabelObj::create(
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

/** ***************************************************************
 * @brief 自己形状の描画
 *
 ******************************************************************/
void LabelObj::_drawShapeOfSelf()
{

	////////////////////////////////////////////////
	// ストロークサイズが所望になるように
	// 一時的にprojectionMatrixを修正する
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//----------------------------------------------
	// カメラのズームへの対応
	//----------------------------------------------
	// 無調整で印字した場合の文字サイズを取得する
	const float rawHgt = glutStrokeHeight(GLUT_STROKE_ROMAN);
	// 文字サイズ調整用の拡縮倍率を取得する
	const float rate = rawHgt / size;
	// 拡縮の中心位置を取得
	const float x = GetTf_root2self().translation().x();
	const float y = GetTf_root2self().translation().y();

	glOrtho(
		x - (x*rate), x + ((prjMtxRangeX - x)*rate),
		y - (y*rate), y + ((prjMtxRangeY - y)*rate),
		CULLING_FORE, CULLING_FORE);

	// name ラベルの描画
	glColor4fv(color.fv4);			// 文字色の設定
//	glColor4fv(color4fv::WHITE);	// 文字色の設定
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
void LabelObj::SetPrjMtxSizeToChildrenLabel(
	std::shared_ptr<CoordChainObj> obj,
	float size_x,
	float size_y
)
{
	// 与えられたオブジェクトがLabelObjならば、prjMtxRangeにサイズを設定する
	if (auto sPtr_labelObj = std::dynamic_pointer_cast<LabelObj>(obj))
	{
		sPtr_labelObj->prjMtxRangeX = size_x;
		sPtr_labelObj->prjMtxRangeY = size_y;
	}

	// 全ての子要素を探索し、再帰する
	for (auto child = obj->wPtr_chldrn.begin();
		child != obj->wPtr_chldrn.end();
		child++)
	{
		auto sPtr_child = child->lock();
		if (sPtr_child)
		{
			SetPrjMtxSizeToChildrenLabel(sPtr_child, size_x, size_y);
		}
	}
}



/////////////////////////////////////////////////////////////////////
//
//	LabelSimple
//	テキストラベル(軽量版)
//

/** ***************************************************************
 * @brief コンストラクタ
 *
 ******************************************************************/
LabelSimple::LabelSimple(
	std::string name,
	std::weak_ptr<CoordChainObj> parent
)
	: LabelObj(name, parent)
	, CoordChainObj(name, parent)
	, IDrawableObjBase(name)
	, ISodlObjBase(name)
{
	copyColor4fv( color4fv::WHITE, color.fv4 );
	visible_CoordChain = FALSE;
}

/** ***************************************************************
 * @brief ファクトリ関数
 *
 ******************************************************************/
std::shared_ptr<LabelSimple> LabelSimple::create(
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


/** ***************************************************************
 * @brief 自己形状の描画
 *
 ******************************************************************/
void LabelSimple::_drawShapeOfSelf()
{
	// name ラベルの描画
	glColor3d(1.0, 1.0, 1.0);	// White
	glRasterPos3d(0, 0, -10.);		//0,0,0位置をスタート位置にする
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (unsigned char*)text.c_str() );

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
