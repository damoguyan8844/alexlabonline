/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 02 14:02:36 2004
 */
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IPersistVarXML_h__
#define __IPersistVarXML_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IPersistVarXML_FWD_DEFINED__
#define __IPersistVarXML_FWD_DEFINED__
typedef interface IPersistVarXML IPersistVarXML;
#endif 	/* __IPersistVarXML_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IPersistVarXML_INTERFACE_DEFINED__
#define __IPersistVarXML_INTERFACE_DEFINED__

/* interface IPersistVarXML */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IPersistVarXML;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B66873EC-BBFF-11D4-A802-112233445566")
    IPersistVarXML : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ElementName( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrElementName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadXML( 
            /* [in] */ VARIANT varSource,
            /* [defaultvalue][optional][in] */ IUnknown __RPC_FAR *pAttributes = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SaveXML( 
            /* [in] */ VARIANT varSource,
            /* [defaultvalue][optional][in] */ BOOL bSaveContent = 1L,
            /* [defaultvalue][optional][in] */ BSTR bstrElementName = L"") = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPersistVarXMLVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPersistVarXML __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPersistVarXML __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPersistVarXML __RPC_FAR * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ElementName )( 
            IPersistVarXML __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrElementName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadXML )( 
            IPersistVarXML __RPC_FAR * This,
            /* [in] */ VARIANT varSource,
            /* [defaultvalue][optional][in] */ IUnknown __RPC_FAR *pAttributes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveXML )( 
            IPersistVarXML __RPC_FAR * This,
            /* [in] */ VARIANT varSource,
            /* [defaultvalue][optional][in] */ BOOL bSaveContent,
            /* [defaultvalue][optional][in] */ BSTR bstrElementName);
        
        END_INTERFACE
    } IPersistVarXMLVtbl;

    interface IPersistVarXML
    {
        CONST_VTBL struct IPersistVarXMLVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPersistVarXML_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPersistVarXML_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPersistVarXML_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPersistVarXML_get_ElementName(This,bstrElementName)	\
    (This)->lpVtbl -> get_ElementName(This,bstrElementName)

#define IPersistVarXML_LoadXML(This,varSource,pAttributes)	\
    (This)->lpVtbl -> LoadXML(This,varSource,pAttributes)

#define IPersistVarXML_SaveXML(This,varSource,bSaveContent,bstrElementName)	\
    (This)->lpVtbl -> SaveXML(This,varSource,bSaveContent,bstrElementName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPersistVarXML_get_ElementName_Proxy( 
    IPersistVarXML __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrElementName);


void __RPC_STUB IPersistVarXML_get_ElementName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistVarXML_LoadXML_Proxy( 
    IPersistVarXML __RPC_FAR * This,
    /* [in] */ VARIANT varSource,
    /* [defaultvalue][optional][in] */ IUnknown __RPC_FAR *pAttributes);


void __RPC_STUB IPersistVarXML_LoadXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistVarXML_SaveXML_Proxy( 
    IPersistVarXML __RPC_FAR * This,
    /* [in] */ VARIANT varSource,
    /* [defaultvalue][optional][in] */ BOOL bSaveContent,
    /* [defaultvalue][optional][in] */ BSTR bstrElementName);


void __RPC_STUB IPersistVarXML_SaveXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPersistVarXML_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
