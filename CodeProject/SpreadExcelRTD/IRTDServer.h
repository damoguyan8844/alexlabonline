

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Jul 14 16:41:57 2009
 */
/* Compiler settings for .\IRTDServer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __IRTDServer_h__
#define __IRTDServer_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IRTDUpdateEvent_FWD_DEFINED__
#define __IRTDUpdateEvent_FWD_DEFINED__
typedef interface IRTDUpdateEvent IRTDUpdateEvent;
#endif 	/* __IRTDUpdateEvent_FWD_DEFINED__ */


#ifndef __IRtdServer_FWD_DEFINED__
#define __IRtdServer_FWD_DEFINED__
typedef interface IRtdServer IRtdServer;
#endif 	/* __IRtdServer_FWD_DEFINED__ */


#ifndef __IRtdServer_FWD_DEFINED__
#define __IRtdServer_FWD_DEFINED__
typedef interface IRtdServer IRtdServer;
#endif 	/* __IRtdServer_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "objidl.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IRTDUpdateEvent_INTERFACE_DEFINED__
#define __IRTDUpdateEvent_INTERFACE_DEFINED__

/* interface IRTDUpdateEvent */
/* [object][oleautomation][dual][helpcontext][uuid] */ 


EXTERN_C const IID IID_IRTDUpdateEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A43788C1-D91B-11D3-8F39-00C04F3651B8")
    IRTDUpdateEvent : public IDispatch
    {
    public:
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE UpdateNotify( void) = 0;
        
        virtual /* [helpcontext][propget][id] */ HRESULT STDMETHODCALLTYPE get_HeartbeatInterval( 
            /* [retval][out] */ long *plRetVal) = 0;
        
        virtual /* [helpcontext][propput][id] */ HRESULT STDMETHODCALLTYPE put_HeartbeatInterval( 
            /* [in] */ long plRetVal) = 0;
        
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRTDUpdateEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRTDUpdateEvent * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRTDUpdateEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRTDUpdateEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRTDUpdateEvent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRTDUpdateEvent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRTDUpdateEvent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRTDUpdateEvent * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *UpdateNotify )( 
            IRTDUpdateEvent * This);
        
        /* [helpcontext][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HeartbeatInterval )( 
            IRTDUpdateEvent * This,
            /* [retval][out] */ long *plRetVal);
        
        /* [helpcontext][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HeartbeatInterval )( 
            IRTDUpdateEvent * This,
            /* [in] */ long plRetVal);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            IRTDUpdateEvent * This);
        
        END_INTERFACE
    } IRTDUpdateEventVtbl;

    interface IRTDUpdateEvent
    {
        CONST_VTBL struct IRTDUpdateEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRTDUpdateEvent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRTDUpdateEvent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRTDUpdateEvent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRTDUpdateEvent_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRTDUpdateEvent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRTDUpdateEvent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRTDUpdateEvent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRTDUpdateEvent_UpdateNotify(This)	\
    ( (This)->lpVtbl -> UpdateNotify(This) ) 

#define IRTDUpdateEvent_get_HeartbeatInterval(This,plRetVal)	\
    ( (This)->lpVtbl -> get_HeartbeatInterval(This,plRetVal) ) 

#define IRTDUpdateEvent_put_HeartbeatInterval(This,plRetVal)	\
    ( (This)->lpVtbl -> put_HeartbeatInterval(This,plRetVal) ) 

#define IRTDUpdateEvent_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRTDUpdateEvent_INTERFACE_DEFINED__ */


#ifndef __IRtdServer_INTERFACE_DEFINED__
#define __IRtdServer_INTERFACE_DEFINED__

/* interface IRtdServer */
/* [object][oleautomation][dual][helpcontext][uuid] */ 


EXTERN_C const IID IID_IRtdServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EC0E6191-DB51-11D3-8F3E-00C04F3651B8")
    IRtdServer : public IDispatch
    {
    public:
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE ServerStart( 
            /* [in] */ IRTDUpdateEvent *CallbackObject,
            /* [retval][out] */ long *pfRes) = 0;
        
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE ConnectData( 
            /* [in] */ long TopicID,
            /* [in] */ SAFEARRAY * *Strings,
            /* [out][in] */ VARIANT_BOOL *GetNewValues,
            /* [retval][out] */ VARIANT *pvarOut) = 0;
        
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE RefreshData( 
            /* [out][in] */ long *TopicCount,
            /* [retval][out] */ SAFEARRAY * *parrayOut) = 0;
        
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE DisconnectData( 
            /* [in] */ long TopicID) = 0;
        
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE Heartbeat( 
            /* [retval][out] */ long *pfRes) = 0;
        
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE ServerTerminate( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRtdServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRtdServer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRtdServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRtdServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRtdServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRtdServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRtdServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRtdServer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *ServerStart )( 
            IRtdServer * This,
            /* [in] */ IRTDUpdateEvent *CallbackObject,
            /* [retval][out] */ long *pfRes);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectData )( 
            IRtdServer * This,
            /* [in] */ long TopicID,
            /* [in] */ SAFEARRAY * *Strings,
            /* [out][in] */ VARIANT_BOOL *GetNewValues,
            /* [retval][out] */ VARIANT *pvarOut);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *RefreshData )( 
            IRtdServer * This,
            /* [out][in] */ long *TopicCount,
            /* [retval][out] */ SAFEARRAY * *parrayOut);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *DisconnectData )( 
            IRtdServer * This,
            /* [in] */ long TopicID);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *Heartbeat )( 
            IRtdServer * This,
            /* [retval][out] */ long *pfRes);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *ServerTerminate )( 
            IRtdServer * This);
        
        END_INTERFACE
    } IRtdServerVtbl;

    interface IRtdServer
    {
        CONST_VTBL struct IRtdServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRtdServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRtdServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRtdServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRtdServer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRtdServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRtdServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRtdServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRtdServer_ServerStart(This,CallbackObject,pfRes)	\
    ( (This)->lpVtbl -> ServerStart(This,CallbackObject,pfRes) ) 

#define IRtdServer_ConnectData(This,TopicID,Strings,GetNewValues,pvarOut)	\
    ( (This)->lpVtbl -> ConnectData(This,TopicID,Strings,GetNewValues,pvarOut) ) 

#define IRtdServer_RefreshData(This,TopicCount,parrayOut)	\
    ( (This)->lpVtbl -> RefreshData(This,TopicCount,parrayOut) ) 

#define IRtdServer_DisconnectData(This,TopicID)	\
    ( (This)->lpVtbl -> DisconnectData(This,TopicID) ) 

#define IRtdServer_Heartbeat(This,pfRes)	\
    ( (This)->lpVtbl -> Heartbeat(This,pfRes) ) 

#define IRtdServer_ServerTerminate(This)	\
    ( (This)->lpVtbl -> ServerTerminate(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRtdServer_INTERFACE_DEFINED__ */



#ifndef __RTDServerLib_LIBRARY_DEFINED__
#define __RTDServerLib_LIBRARY_DEFINED__

/* library RTDServerLib */
/* [version][uuid] */ 




EXTERN_C const IID LIBID_RTDServerLib;
#endif /* __RTDServerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


