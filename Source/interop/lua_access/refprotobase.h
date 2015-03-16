

#ifndef _REF_PROTO_BASE_DEF__
#define _REF_PROTO_BASE_DEF__

#include "refproto.h"
#include <string>

class RefProtoBase: public LuaRefProto {
protected:
	RefProtoBase();
	RefProtoBase(const char *creator);
	virtual ~RefProtoBase();
	void copy(const RefProtoBase&);

public:
	virtual int tableRef()const override;
	virtual void createTableRef() override;

private:
	void setCreator(const char *creator);

private:
	int _ref;
	std::string _creator;
	RefProtoBase& operator=(const RefProtoBase&);
};



#endif