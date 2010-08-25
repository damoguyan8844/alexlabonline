#ifndef TTABLE_HELP_FUNCTION_
#define TTABLE_HELP_FUNCTION_

#include "TTableInc.h"
#include <string>
#include <functional>
#include <map>

inline std::string GetSQLiteTypeString(DATA_TYPE dataType)
{
	switch(dataType)
	{
		case DATA_INT: return " int ";
		case DATA_STRING: return " varchar(256) ";	
		case DATA_DOUBLE: return " number " ;
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

// struct AnyValue
// {
// 	DATA_TYPE data_Type;
// 	union {
// 	};
// };
#endif