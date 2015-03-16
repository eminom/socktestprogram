

#include "lua_access.h"
#include "refprotobase.h"

RefProtoBase::RefProtoBase()
	:_ref(LUA_REFNIL) {
}

RefProtoBase::RefProtoBase(const char *creator)
	:_ref(LUA_REFNIL),
	_creator(creator) {
}

RefProtoBase::~RefProtoBase(){
	ljReleaseObj(_ref);
}

void RefProtoBase::copy(const RefProtoBase &rhs){
	ljReleaseObj(_ref);
	_ref = ljDuplicateObj(rhs._ref);
}

int RefProtoBase::tableRef()const {
	return _ref;
}

void RefProtoBase::createTableRef() {
	assert( _ref == LUA_REFNIL );
	assert( _creator.size() > 0);
	ljReleaseObj(_ref);
	FuncRef fr(_creator.c_str());
	_ref = fr.createObj();
	assert( _ref != LUA_REFNIL);
}

void RefProtoBase::setCreator(const char *creator) {
	_creator = creator;
}