// IE2EM.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "IE2EM.h"

class CIE2EMModule : public CAtlDllModuleT< CIE2EMModule >
{
public :
	DECLARE_LIBID(LIBID_IE2EMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_IE2EM, "{BCEADC2F-0C89-43F5-8DE9-068B5BC265E2}")
};

CIE2EMModule _AtlModule;


// DLL ��ڵ�
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// ����ȷ�� DLL �Ƿ���� OLE ж��
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// ����һ���๤���Դ������������͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - ������ӵ�ϵͳע���
STDAPI DllRegisterServer(void)
{
    // ע��������Ϳ�����Ϳ��е����нӿ�
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
