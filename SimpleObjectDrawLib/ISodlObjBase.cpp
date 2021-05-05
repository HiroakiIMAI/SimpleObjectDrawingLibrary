#include "include/ISodlObjBase.h"
#include <iostream>


using namespace SmplObjDrwLib;

/////////////////////////////////////////////////////////////////////
//
//	class ISodlListedItem
//

// instantiation of static member
//std::list<std::shared_ptr<ISodlListedItem> > ISodlListedItem::sodlObjInstanceList;
//
//std::shared_ptr<ISodlListedItem> ISodlListedItem::create(std::string name)
//{
//	auto ptr = std::shared_ptr<ISodlListedItem>(new ISodlListedItem(name));
//	ptr->addSelfToList();
//	return ptr;
//}
//
//// constructor
//
//ISodlListedItem::ISodlListedItem(std::string name) :
//	name(name)
//{
//}
//
//// destructor
//ISodlListedItem::~ISodlListedItem()
//{
//};
//
//void ISodlListedItem::addSelfToList()
//{
//	sodlObjInstanceList.push_back(shared_from_this());
//}
//
//
//int ISodlListedItem::getSodlObjId()
//{
//	return sodlObjId;
//}


/////////////////////////////////////////////////////////////////////
//
//	class ISodlObjBase
//

/*
// factory
template<class T>
std::shared_ptr<T> ISodlObjBase<T>::create(std::string name)
{
class T2 :public T
{
public:
void addSelfToList_ext() { addSelfToList(); }
T2(std::string name) :T(name) {  }
};
std::shared_ptr<T2> T2ptr = std::shared_ptr<T2>(new T2(name));
T2ptr->addSelfToList_ext();

std::shared_ptr<T> Tptr = std::dynamic_pointer_cast<ISodlListedItem>(T2ptr);
return  Tptr;
}
*/

// static methods
std::list<int> ISodlObjBase::GetInstanceList()
{
	return aliveInstanceList;
}


// constructor
ISodlObjBase::ISodlObjBase(std::string name)
//	: ISodlListedItem(name)
	: name(name)
{
	this->instanceId = ++ISodlObjBase::globalInstanceIdCtr;
	ISodlObjBase::aliveInstanceList.push_back( this->instanceId );
	this->itr_selfInAliveList = --ISodlObjBase::aliveInstanceList.end();
}

// destructor
ISodlObjBase::~ISodlObjBase()
{
	ISodlObjBase::aliveInstanceList.erase( this->itr_selfInAliveList );
}

// instantiate satic member
int				ISodlObjBase::globalInstanceIdCtr = 0;
std::list<int>	ISodlObjBase::aliveInstanceList;

