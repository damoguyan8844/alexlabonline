#ifndef TTABLE_HELP_FUNCTION_
#define TTABLE_HELP_FUNCTION_

#include "TTableInc.h"
#include <string>

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

// struct AnyValue
// {
// 	DATA_TYPE data_Type;
// 	union {
// 	};
// };
#endif