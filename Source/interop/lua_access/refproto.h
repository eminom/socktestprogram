

#ifndef _REF_PROTOCOL__
#define _REF_PROTOCOL__

#include <cstdlib>
#include <string>

class LuaRefProto
{
public:
	virtual int tableRef()const = 0;
	virtual void createTableRef() = 0;

public:	//Common codes
	void loadWith(int ref);
	void load();

public: //Base codes
	int getInt(const char *name);
	bool getBoolean(const char *name);
	std::string getString(const char *name);
	float getFloat(const char *name);

	
	void runVoidFormat(const char *name, bool *result, const char *format, ...);
	
public:	//OK.  Here is the execVoid version
	void execVoid(const char *name, const char *format = "", ...);
	int execInt(const char *name, const char *format = "", ...);
	std::string execString(const char *name, const char *format = "", ...);
	bool execBoolean(const char *name, const char *format="",...);
	float execFloat(const char *name, const char *format="",...);

private:
	void getVoidFormat(const char *name, bool *result, const char *format, va_list args);
	int getIntFormat(const char *name, bool *result, const char *format, va_list args);
	std::string getStringFormat(const char *name, bool *result, const char *format, va_list args);
	bool getBooleanFormat(const char *name, bool *result, const char *format, va_list args);
	float getFloatFormat(const char *name, bool *result, const char *format, va_list args);

public:
	int mix(int ref, const char *name);
	void fireOn(const char *event);
};


#endif


