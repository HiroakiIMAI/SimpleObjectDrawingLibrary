#pragma once
#define  _USE_MATH_DEFINES
#include <math.h>

#include <memory>
#include <string>
#include <map>
#include <unordered_map>
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

	// std::shared_ptr< std::vector<T> > ÇÃíZèkå`
	template <class T>
	using sPtr_vector	= std::shared_ptr< std::vector<T> >;
	// std::shared_ptr< std::map<T> > ÇÃíZèkå`
	template <class T1, class T2 >
	using sPtr_map		= std::shared_ptr< std::map<T1, T2> >;

	const float CULLING_BACK = 50000.f;
	const float CULLING_FORE = 0.f;

	typedef int T_ID;

//	//------------------------------------------------------------
//	//	class ISodlListedItem
//	//------------------------------------------------------------
//	class ISodlListedItem
//		: public std::enable_shared_from_this<ISodlListedItem>
//	{
//	public:
//		static std::shared_ptr<ISodlListedItem> create(std::string name);
//
//		virtual ~ISodlListedItem();
//
//		int getSodlObjId();
//
//		std::string name = "default";
//
//		static std::list<std::shared_ptr<ISodlListedItem> > sodlObjInstanceList;
//
//
//	protected:
//		ISodlListedItem(std::string name);
//		void addSelfToList();
//
//	private:
//		int sodlObjId;
//	};


	//------------------------------------------------------------
	//	class ISodlObjBase
	//------------------------------------------------------------
	class ISodlObjBase
//		: public ISodlListedItem
	: public std::enable_shared_from_this<ISodlObjBase>
	{
	// static members
	private:
		static int globalInstanceIdCtr;
		static std::list<int> aliveInstanceList;

		static std::list<int> GetInstanceList();

	// instance members
	public:
		//static std::shared_ptr<T> create(std::string name);
		virtual ~ISodlObjBase();
		std::string name = "default";

	protected:
		ISodlObjBase( std::string name );

	private:
		std::list<T_ID>::iterator itr_selfInAliveList;
		T_ID instanceId;

	// accesser to prvate
	public:
		const T_ID& id_readOnly = instanceId;

	};

}



