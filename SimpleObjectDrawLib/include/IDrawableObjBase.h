#pragma once
#include "ISodlObjBase.h"

namespace SmplObjDrwLib {

	enum DRAWTYPE{
		WIRE = 0,
		POLYGON,
		WIRE_POLYGON,
		POINT,
	};

	namespace color4fv {
		extern const float LLGRAY[];
		extern const float LGRAY[];
		extern const float GRAY[];
		extern const float DGRAY[];
		extern const float DDGRAY[];

		extern const float BLACK[];
		extern const float WHITE[];
		extern const float RED[];
		extern const float GREEN[];
		extern const float BLUE[];
	}

	extern void copyColor4fv(const float src[4], float* dst);
	extern void CvtHsv2Rgb(
		const float& h,
		const float& s,
		const float& v,
		const float& a,
		float rgba_fv4[4]
	);

	extern void GetNormalizedHeatColor(
		const float& dat,
		const float& max,
		const float& min,
		float rgba_fv4[4]
		);


	union ST_COLOR{
		float fv4[4] = { 1,1,1,1 };
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};

	class IDrawableObjBase
		: public virtual ISodlObjBase
	{
		//--------------------------------------------
		// コンストラクタ
		//--------------------------------------------
		// 継承クラス外からのインスタンス化を禁止する
		// →ファクトリの使用を強制し、shared_ptr以外に
		// 　インスタンスを保持させない。
	protected:
		IDrawableObjBase(std::string name);

		//--------------------------------------------
		// デストラクタ
		//--------------------------------------------
	public:

		~IDrawableObjBase();

		//--------------------------------------------
		// DrawableObjBaseの実装
		//--------------------------------------------
	public:
		virtual void draw() = 0;

		//--------------------------------------------
		// 以下、本クラス固有のメンバ
		//--------------------------------------------
	public:
		bool visible;

		ST_COLOR color;
		ST_COLOR colorWire;

		DRAWTYPE drawType = WIRE;

	};

	//----------------------------------------------------
	// 名前空間内で有効なエイリアスを定義する
	//----------------------------------------------------
	// std::shared_ptr< IDrawableObjBase > の短縮形
	using sPtr_IDrawableObjBase = std::shared_ptr< IDrawableObjBase > ;

	// std::shared_ptr< std::vector<T> > の短縮形
	template <class T>
	using sPtr_vector = std::shared_ptr< std::vector<T> > ;

};

