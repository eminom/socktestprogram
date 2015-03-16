

#ifndef _REF_OBJECT_DEF__
#define _REF_OBJECT_DEF__

#include "refproto.h"
#include <string>

class RefObject: public LuaRefProto {
public:
	RefObject();
	RefObject(const char *name);
	virtual ~RefObject();
	void copy(const RefObject&);
	void loadRef(const char *name);
	void loadFromTop();			//Always pop the stack by 1.
	void loadFromFunc(const char *name);

public:
	virtual int tableRef()const override;
	virtual void createTableRef() override;

private:
	int _ref;
	RefObject& operator=(const RefObject&);
	RefObject(const RefObject&);
};



#endif