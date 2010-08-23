// TTable.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f TTableps.mk in the project directory.

#include "TTableInc.h"
#include "resource.h"
#include <initguid.h>
#include "TTable.h"
#include <sys/stat.h>
#include <log4cxx/appenderskeleton.h>
#include <log4cxx/defaultcategoryfactory.h>
#include <log4cxx/helpers/properties.h>
#include <log4cxx/propertyconfigurator.h>

#include "TTable_i.c"
#include "TableManager.h"
#include "Table.h"
#include "TableColumn.h"
#include "TableColumns.h"
#include "Log4cxxHelper.h"
#include <string>
#include "TableRow.h"
#include "TableRows.h"

CComModule _Module;
std::string gStrLogFile="zsoftlog.properties";

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_TableManager, CTableManager)
OBJECT_ENTRY(CLSID_Table, CTable)
OBJECT_ENTRY(CLSID_TableColumn, CTableColumn)
OBJECT_ENTRY(CLSID_TableColumns, CTableColumns)
OBJECT_ENTRY(CLSID_TableRow, CTableRow)
OBJECT_ENTRY(CLSID_TableRows, CTableRows)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_TTABLELib);
        DisableThreadLibraryCalls(hInstance);

		Log4cxxHelper::configure("TTable", 2000);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
		if(log4cxx::PropertyConfigurator::watchThread_ != 0)
		{
			::TerminateThread(log4cxx::PropertyConfigurator::watchThread_,0);
		}
		_Module.Term();
	}
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


