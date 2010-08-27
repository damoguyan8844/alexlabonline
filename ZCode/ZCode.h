/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Aug 27 17:39:43 2010
 */
/* Compiler settings for G:\THC\C0702\TCom2\ZCode\ZCode.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __ZCode_h__
#define __ZCode_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IUtility_FWD_DEFINED__
#define __IUtility_FWD_DEFINED__
typedef interface IUtility IUtility;
#endif 	/* __IUtility_FWD_DEFINED__ */


#ifndef __Utility_FWD_DEFINED__
#define __Utility_FWD_DEFINED__

#ifdef __cplusplus
typedef class Utility Utility;
#else
typedef struct Utility Utility;
#endif /* __cplusplus */

#endif 	/* __Utility_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __ZCODELib_LIBRARY_DEFINED__
#define __ZCODELib_LIBRARY_DEFINED__

/* library ZCODELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ZCODELib;

#ifndef __IUtility_INTERFACE_DEFINED__
#define __IUtility_INTERFACE_DEFINED__

/* interface IUtility */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUtility;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("02B8D2B1-CC2B-487B-84D0-59B0A8362E59")
    IUtility : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Base16Table( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Base32Table( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Base64Table( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CompressWithBase16( 
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CompressWithBase32( 
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CompressWithBase64( 
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecompressWithBase16( 
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecompressWithBase32( 
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecompressWithBase64( 
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CodeReplace( 
            /* [in] */ BSTR inTable,
            /* [in] */ BSTR toTable,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUtilityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IUtility __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IUtility __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IUtility __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IUtility __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IUtility __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IUtility __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IUtility __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Base16Table )( 
            IUtility __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Base32Table )( 
            IUtility __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Base64Table )( 
            IUtility __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompressWithBase16 )( 
            IUtility __RPC_FAR * This,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompressWithBase32 )( 
            IUtility __RPC_FAR * This,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompressWithBase64 )( 
            IUtility __RPC_FAR * This,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DecompressWithBase16 )( 
            IUtility __RPC_FAR * This,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DecompressWithBase32 )( 
            IUtility __RPC_FAR * This,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DecompressWithBase64 )( 
            IUtility __RPC_FAR * This,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CodeReplace )( 
            IUtility __RPC_FAR * This,
            /* [in] */ BSTR inTable,
            /* [in] */ BSTR toTable,
            /* [in] */ BSTR src,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IUtilityVtbl;

    interface IUtility
    {
        CONST_VTBL struct IUtilityVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUtility_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IUtility_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IUtility_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IUtility_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IUtility_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IUtility_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IUtility_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IUtility_get_Base16Table(This,pVal)	\
    (This)->lpVtbl -> get_Base16Table(This,pVal)

#define IUtility_get_Base32Table(This,pVal)	\
    (This)->lpVtbl -> get_Base32Table(This,pVal)

#define IUtility_get_Base64Table(This,pVal)	\
    (This)->lpVtbl -> get_Base64Table(This,pVal)

#define IUtility_CompressWithBase16(This,src,pVal)	\
    (This)->lpVtbl -> CompressWithBase16(This,src,pVal)

#define IUtility_CompressWithBase32(This,src,pVal)	\
    (This)->lpVtbl -> CompressWithBase32(This,src,pVal)

#define IUtility_CompressWithBase64(This,src,pVal)	\
    (This)->lpVtbl -> CompressWithBase64(This,src,pVal)

#define IUtility_DecompressWithBase16(This,src,pVal)	\
    (This)->lpVtbl -> DecompressWithBase16(This,src,pVal)

#define IUtility_DecompressWithBase32(This,src,pVal)	\
    (This)->lpVtbl -> DecompressWithBase32(This,src,pVal)

#define IUtility_DecompressWithBase64(This,src,pVal)	\
    (This)->lpVtbl -> DecompressWithBase64(This,src,pVal)

#define IUtility_CodeReplace(This,inTable,toTable,src,pVal)	\
    (This)->lpVtbl -> CodeReplace(This,inTable,toTable,src,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IUtility_get_Base16Table_Proxy( 
    IUtility __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_get_Base16Table_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IUtility_get_Base32Table_Proxy( 
    IUtility __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_get_Base32Table_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IUtility_get_Base64Table_Proxy( 
    IUtility __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_get_Base64Table_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUtility_CompressWithBase16_Proxy( 
    IUtility __RPC_FAR * This,
    /* [in] */ BSTR src,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_CompressWithBase16_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUtility_CompressWithBase32_Proxy( 
    IUtility __RPC_FAR * This,
    /* [in] */ BSTR src,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_CompressWithBase32_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUtility_CompressWithBase64_Proxy( 
    IUtility __RPC_FAR * This,
    /* [in] */ BSTR src,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_CompressWithBase64_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUtility_DecompressWithBase16_Proxy( 
    IUtility __RPC_FAR * This,
    /* [in] */ BSTR src,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_DecompressWithBase16_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUtility_DecompressWithBase32_Proxy( 
    IUtility __RPC_FAR * This,
    /* [in] */ BSTR src,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_DecompressWithBase32_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUtility_DecompressWithBase64_Proxy( 
    IUtility __RPC_FAR * This,
    /* [in] */ BSTR src,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_DecompressWithBase64_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUtility_CodeReplace_Proxy( 
    IUtility __RPC_FAR * This,
    /* [in] */ BSTR inTable,
    /* [in] */ BSTR toTable,
    /* [in] */ BSTR src,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IUtility_CodeReplace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IUtility_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Utility;

#ifdef __cplusplus

class DECLSPEC_UUID("774BD14F-D14D-4CF3-8243-19A189073929")
Utility;
#endif
#endif /* __ZCODELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
