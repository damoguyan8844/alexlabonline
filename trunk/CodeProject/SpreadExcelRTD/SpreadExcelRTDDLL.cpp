/******************************************************************************
* $Id: SpreadExcelRTDDLL.cpp 17 2008-02-17 17:12:16Z jim.kleckner $
*
*	File:	SpreadExcelRTDDLL.cpp
*
*	Date:	February 5, 2001
*
*	Description:	This file contains the class factory and standard DLL 
*					functions for the SpreadExcelRTD COM object.  It is fairly
*					generic for use with other COM objects.
*
*	Modifications:
******************************************************************************/
#include "SpreadExcelRTDDLL.h"
#include "SpreadExcelRTDImpl.h"

#define RTDSERVERCLASSFACTORYTRACE(x) OutputDebugString(x)

//8D2EEA35-CBEB-49b1-8F3E-68C8F50F38D8
const CLSID CLSID_SpreadExcelRTD = {0x8D2EEA35,0xCBEB,0x49B1,{0x8F,0x3E,0x68,0xC8,
0xF5,0x0F,0x38,0xD8}};

SpreadExcelRTDClassFactory::SpreadExcelRTDClassFactory()
{
   m_refCount = 0;
}

SpreadExcelRTDClassFactory::~SpreadExcelRTDClassFactory()
{}

/******************************************************************************
*   IUnknown Interfaces -- All COM objects must implement, either directly or 
*   indirectly, the IUnknown interface.
******************************************************************************/

/******************************************************************************
*   QueryInterface -- Determines if this component supports the requested 
*   interface, places a pointer to that interface in ppvObj if it's available,
*   and returns S_OK.  If not, sets ppvObj to NULL and returns E_NOINTERFACE.
******************************************************************************/
STDMETHODIMP SpreadExcelRTDClassFactory::QueryInterface(REFIID riid, void ** ppvObj)
{
   //tracing purposes only
   RTDSERVERCLASSFACTORYTRACE("SpreadExcelRTDClassFactory::QueryInterface->");
   
   if (riid == IID_IUnknown){
      RTDSERVERCLASSFACTORYTRACE("IUnknown\n");
      *ppvObj = static_cast<IClassFactory*>(this);
   }
   
   else if (riid == IID_IClassFactory){
      RTDSERVERCLASSFACTORYTRACE("IDispatch\n");
      *ppvObj = static_cast<IClassFactory*>(this);
   }
   
   else{
      RTDSERVERCLASSFACTORYTRACE("Unsupported Interface\n");
      *ppvObj = NULL;
      return E_NOINTERFACE;
   }
   
   static_cast<IUnknown*>(*ppvObj)->AddRef();
   return S_OK;
}

/******************************************************************************
*   AddRef() -- In order to allow an object to delete itself when it is no 
*   longer needed, it is necessary to maintain a count of all references to 
*   this object.  When a new reference is created, this function increments
*   the count.
******************************************************************************/
STDMETHODIMP_(ULONG) SpreadExcelRTDClassFactory::AddRef()
{
   //tracing purposes only
   RTDSERVERCLASSFACTORYTRACE("SpreadExcelRTDClassFactory::AddRef\n");
   
   return ++m_refCount;
}

/******************************************************************************
*   Release() -- When a reference to this object is removed, this function 
*   decrements the reference count.  If the reference count is 0, then this 
*   function deletes this object and returns 0;
******************************************************************************/
STDMETHODIMP_(ULONG) SpreadExcelRTDClassFactory::Release()
{
   //tracing purposes only
   RTDSERVERCLASSFACTORYTRACE("SpreadExcelRTDClassFactory::Release\n");
   
   if (--m_refCount == 0)
   {
      delete this;
      return 0;
   }
   return m_refCount;
}


/******* IClassFactory Methods *******/
/******************************************************************************
*	CreateInstance() -- This method attempts to create an instance of SpreadExcelRTD
*	and returns it to the caller.  It maintains a count of the number of
*	created objects.
******************************************************************************/
STDMETHODIMP SpreadExcelRTDClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, 
                                                      REFIID riid, 
                                                      LPVOID *ppvObj)
{
   //tracing purposes only
   RTDSERVERCLASSFACTORYTRACE("SpreadExcelRTDClassFactory::CreateInstance\n");

   HRESULT hr;
   SpreadExcelRTD* pObj;
   
   *ppvObj = NULL;
   hr = ResultFromScode(E_OUTOFMEMORY);

   //It's illegal to ask for anything but IUnknown when aggregating
   if ((pUnkOuter != NULL) && (riid != IID_IUnknown))
      return E_INVALIDARG;
   
   //Create a new instance of SpreadExcelRTD
   pObj = new SpreadExcelRTD(pUnkOuter);
   
   if (pObj == NULL)
      return hr;
   
   //Return the resulting object
   hr = pObj->NonDelegatingQueryInterface(riid, ppvObj);

   if (FAILED(hr))
      delete pObj;
   
   return hr;
}

/******************************************************************************
*	LockServer() -- This method maintains a count of the current locks on this
*	DLL.  The count is used to determine if the DLL can be unloaded, or if
*	clients are still using it.
******************************************************************************/
STDMETHODIMP SpreadExcelRTDClassFactory::LockServer(BOOL fLock)
{
   //tracing purposes only
   RTDSERVERCLASSFACTORYTRACE("SpreadExcelRTDClassFactory::LockServer\n");

   if (fLock)
      InterlockedIncrement( &g_cLock );
   else
      InterlockedDecrement( &g_cLock );
   return NOERROR;
}

/******* Exported DLL functions *******/
/******************************************************************************
*  g_RegTable -- This N*3 array contains the keys, value names, and values that
*  are associated with this dll in the registry.
******************************************************************************/
const char *g_RegTable[][3] = {
   //format is {key, value name, value }
   {"SpreadExcelRTD.RTDFunctions", 0, "SpreadExcelRTD.RTDFunctions"},
   {"SpreadExcelRTD.RTDFunctions\\CLSID", 0, "{8D2EEA35-CBEB-49b1-8F3E-68C8F50F38D8}"},
   
   {"CLSID\\{8D2EEA35-CBEB-49b1-8F3E-68C8F50F38D8}", 0, "SpreadExcelRTD.RTDFunctions"},
   {"CLSID\\{8D2EEA35-CBEB-49b1-8F3E-68C8F50F38D8}\\InprocServer32", 0, 
      (const char*)-1},
   {"CLSID\\{8D2EEA35-CBEB-49b1-8F3E-68C8F50F38D8}\\ProgId", 0, 
   "SpreadExcelRTD.RTDFunctions"},
   {"CLSID\\{8D2EEA35-CBEB-49b1-8F3E-68C8F50F38D8}\\TypeLib", 0, 
      "{0DD8CA71-1832-406a-BCFF-192089D7109A}"},
   
   //	copied this from Kruglinski with my uuids and names.  
   //Just marks where the typelib is
   {"TypeLib\\{0DD8CA71-1832-406a-BCFF-192089D7109A}", 0, "SpreadExcelRTD.RTDFunctions"},
   {"TypeLib\\{0DD8CA71-1832-406a-BCFF-192089D7109A}\\1.0", 0, 
   "SpreadExcelRTD.RTDFunctions"},
   {"TypeLib\\{0DD8CA71-1832-406a-BCFF-192089D7109A}\\1.0\\0", 0, "win32"},
   {"TypeLib\\{0DD8CA71-1832-406a-BCFF-192089D7109A}\\1.0\\0\\Win32", 0, 
      (const char*)-1},
   {"TypeLib\\{0DD8CA71-1832-406a-BCFF-192089D7109A}\\1.0\\FLAGS", 0, "0"},
};

/******************************************************************************
*  DLLRegisterServer -- This method is the exported method that is used by
*  COM to self-register this component.  It removes the need for a .reg file.
*  ( Taken from Don Box's _Essential COM_ pg. 110-112)
******************************************************************************/
STDAPI DllRegisterServer(void)
{
   HRESULT hr = S_OK;
   
   //look up server's file name
   char szFileName[255];
   HMODULE dllModule = GetModuleHandle("SpreadExcelRTD.dll");
   GetModuleFileName(dllModule, szFileName, 255);
   
   //the typelib should be in the same directory
   char szTypeLibName[255];
   char* pszFileName = NULL;
   memset( szTypeLibName, '\0', 255);
   lstrcpy( szTypeLibName, szFileName );
   pszFileName = strstr( szTypeLibName, "SpreadExcelRTD.dll");

   //register entries from the table
   int nEntries = sizeof(g_RegTable)/sizeof(*g_RegTable);
   for (int i = 0; SUCCEEDED(hr) && i < nEntries; i++)
   {
      const char *pszName = g_RegTable[i][0];
      const char *pszValueName = g_RegTable[i][1];
      const char *pszValue = g_RegTable[i][2];
      
      //Map rogue values to module file name
      if (pszValue == (const char*)-1)
         pszValue = szFileName;
      
      //Create the key
      HKEY hkey;
      long err = RegCreateKeyA( HKEY_CLASSES_ROOT, pszName, &hkey);
      
      //Set the value
      if (err == ERROR_SUCCESS){
         err = RegSetValueExA( hkey, pszValueName, 0, REG_SZ, 
            (const BYTE*)pszValue, (strlen(pszValue) + 1));
         RegCloseKey(hkey);
      }
      
      //if cannot add key or value, back out and fail
      if (err != ERROR_SUCCESS){
         DllUnregisterServer();
         hr = SELFREG_E_CLASS;
      }
   }
   return hr;
}

/******************************************************************************
*  DllUnregisterServer -- This method is the exported method that is used by 
*  COM to remove the keys added to the registry by DllRegisterServer.  It
*  is essentially for housekeeping.
*  (Taken from Don Box, _Essential COM_ pg 112)
******************************************************************************/
STDAPI DllUnregisterServer(void)
{
   HRESULT hr = S_OK;
   
   int nEntries = sizeof(g_RegTable)/sizeof(*g_RegTable);
   
   for (int i = nEntries - 1; i >= 0; i--){
      const char * pszKeyName = g_RegTable[i][0];
      
      long err = RegDeleteKeyA(HKEY_CLASSES_ROOT, pszKeyName);
      if (err != ERROR_SUCCESS)
         hr = S_FALSE;
   }
   return hr;
}

/******************************************************************************
*	DllGetClassObject() -- This method is the exported method that clients use
*	to create objects in the DLL.  It uses class factories to generate the
*	desired object and returns it to the caller.  The caller must call Release()
*	on the object when they're through with it.
******************************************************************************/
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR * ppvObj)
{
   //tracing purposes only
   RTDSERVERCLASSFACTORYTRACE("DLLGetClassObject");

   //Make sure the requested class is supported by this server
   if (!IsEqualCLSID(rclsid, CLSID_SpreadExcelRTD))
      return ResultFromScode(E_FAIL);
   
   //Make sure the requested interface is supported
   if ((!IsEqualCLSID(riid, IID_IUnknown)) && (!IsEqualCLSID(riid, 
      IID_IClassFactory)))
      return ResultFromScode(E_NOINTERFACE);
   
   //Create the class factory
   *ppvObj = (LPVOID) new SpreadExcelRTDClassFactory();
   
   //error checking
   if (*ppvObj == NULL)
      return ResultFromScode(E_OUTOFMEMORY);
   
   //Addref the Class Factory
   ((LPUNKNOWN)*ppvObj)->AddRef();
   
   return NOERROR;
}

/******************************************************************************
*	DllCanUnloadNow() -- This method checks to see if it's alright to unload 
*	the dll by determining if there are currently any locks on the dll.
******************************************************************************/
STDAPI DllCanUnloadNow()
{
   if ((g_cLock == 0) && (g_cOb == 0))
      return S_OK;
   else
      return S_FALSE;
}
