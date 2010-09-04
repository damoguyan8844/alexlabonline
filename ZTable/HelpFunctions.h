#ifndef ZTable_HELP_FUNCTION_
#define ZTable_HELP_FUNCTION_

#include "ZTableInc.h"
#include <string>
#include <functional>
#include <map>

inline std::string GetSQLiteTypeString(DATA_TYPE dataType)
{
	switch(dataType)
	{
		case DATA_INT: return " int ";
		case DATA_STRING: return " varchar(2048) ";	
		case DATA_DOUBLE: return " number " ;
		case DATA_BIN: return " blob " ;
	}
	return "";
}

struct KeyComp: std::binary_function<std::string,std::string,bool>
{
	bool operator()(const std::string& k1, const std::string& k2) const
	{
		return stricmp(k1.c_str(),k2.c_str()) < 0;                                        
	}
};

struct Destroyer
{
	Destroyer(char * p) : _p(p)
	{}
	
	~Destroyer()
	{ if (_p) delete[] _p; }
	
	void DestroyNow() { delete[] _p; _p = 0; }
	
	char * _p;
};

// struct AnyValue
// {
// 	DATA_TYPE data_Type;
// 	union {
// 	};
// };
#endif