#pragma once
#define  _USE_MATH_DEFINES
#include <math.h>

#include <memory>
#include <string>
#include <list>
#include <vector>
#include <deque>
#include <algorithm>

#ifndef MAX
#define MAX( a, b )		(a>b)?a:b
#endif

#ifndef MIN
#define MIN( a, b )		(a<b)?a:b
#endif


namespace SmplObjDrwLib {

	const float CULLING_BACK = 50000.f;
	const float CULLING_FORE = 0.f;

	//------------------------------------------------------------
	//	class ISodlListedItem
	//------------------------------------------------------------
	class ISodlListedItem
		: public std::enable_shared_from_this<ISodlListedItem>
	{
	public:
		static std::shared_ptr<ISodlListedItem> create(std::string name);

		virtual ~ISodlListedItem();

		int getSodlObjId();

		std::string name = "default";

		static std::list<std::shared_ptr<ISodlListedItem> > sodlObjInstanceList;


	protected:
		ISodlListedItem(std::string name);
		void addSelfToList();

	private:
		int sodlObjId;
	};


	//------------------------------------------------------------
	//	class ISodlObjBase
	//------------------------------------------------------------
	class ISodlObjBase
//		: public ISodlListedItem
	: public std::enable_shared_from_this<ISodlObjBase>
	{
	public:
		//static std::shared_ptr<T> create(std::string name);
		virtual ~ISodlObjBase();
		std::string name = "default";

	protected:
		ISodlObjBase( std::string name );

	};






}



