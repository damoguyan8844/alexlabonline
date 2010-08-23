/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Aug 24 00:49:52 2010
 */
/* Compiler settings for E:\AlexLabOnline\TTable\TTable.idl:
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

#ifndef __TTable_h__
#define __TTable_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITableColumns_FWD_DEFINED__
#define __ITableColumns_FWD_DEFINED__
typedef interface ITableColumns ITableColumns;
#endif 	/* __ITableColumns_FWD_DEFINED__ */


#ifndef __ITableRows_FWD_DEFINED__
#define __ITableRows_FWD_DEFINED__
typedef interface ITableRows ITableRows;
#endif 	/* __ITableRows_FWD_DEFINED__ */


#ifndef __ITableManager_FWD_DEFINED__
#define __ITableManager_FWD_DEFINED__
typedef interface ITableManager ITableManager;
#endif 	/* __ITableManager_FWD_DEFINED__ */


#ifndef __ITable_FWD_DEFINED__
#define __ITable_FWD_DEFINED__
typedef interface ITable ITable;
#endif 	/* __ITable_FWD_DEFINED__ */


#ifndef __ITableColumn_FWD_DEFINED__
#define __ITableColumn_FWD_DEFINED__
typedef interface ITableColumn ITableColumn;
#endif 	/* __ITableColumn_FWD_DEFINED__ */


#ifndef __ITableRow_FWD_DEFINED__
#define __ITableRow_FWD_DEFINED__
typedef interface ITableRow ITableRow;
#endif 	/* __ITableRow_FWD_DEFINED__ */


#ifndef __TableManager_FWD_DEFINED__
#define __TableManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class TableManager TableManager;
#else
typedef struct TableManager TableManager;
#endif /* __cplusplus */

#endif 	/* __TableManager_FWD_DEFINED__ */


#ifndef __Table_FWD_DEFINED__
#define __Table_FWD_DEFINED__

#ifdef __cplusplus
typedef class Table Table;
#else
typedef struct Table Table;
#endif /* __cplusplus */

#endif 	/* __Table_FWD_DEFINED__ */


#ifndef __TableColumn_FWD_DEFINED__
#define __TableColumn_FWD_DEFINED__

#ifdef __cplusplus
typedef class TableColumn TableColumn;
#else
typedef struct TableColumn TableColumn;
#endif /* __cplusplus */

#endif 	/* __TableColumn_FWD_DEFINED__ */


#ifndef __TableColumns_FWD_DEFINED__
#define __TableColumns_FWD_DEFINED__

#ifdef __cplusplus
typedef class TableColumns TableColumns;
#else
typedef struct TableColumns TableColumns;
#endif /* __cplusplus */

#endif 	/* __TableColumns_FWD_DEFINED__ */


#ifndef __TableRow_FWD_DEFINED__
#define __TableRow_FWD_DEFINED__

#ifdef __cplusplus
typedef class TableRow TableRow;
#else
typedef struct TableRow TableRow;
#endif /* __cplusplus */

#endif 	/* __TableRow_FWD_DEFINED__ */


#ifndef __TableRows_FWD_DEFINED__
#define __TableRows_FWD_DEFINED__

#ifdef __cplusplus
typedef class TableRows TableRows;
#else
typedef struct TableRows TableRows;
#endif /* __cplusplus */

#endif 	/* __TableRows_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __TTABLELib_LIBRARY_DEFINED__
#define __TTABLELib_LIBRARY_DEFINED__

/* library TTABLELib */
/* [helpstring][version][uuid] */ 



typedef /* [public][public][public][public][uuid] */ 
enum __MIDL___MIDL_itf_TTable_0000_0001
    {	DATA_INT	= 1,
	DATA_DOUBLE	= 2,
	DATA_STRING	= 3
    }	DATA_TYPE;


EXTERN_C const IID LIBID_TTABLELib;

#ifndef __ITableColumns_INTERFACE_DEFINED__
#define __ITableColumns_INTERFACE_DEFINED__

/* interface ITableColumns */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITableColumns;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0D80605A-40EB-4957-8B07-04B3D48C75F8")
    ITableColumns : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ ITableColumn __RPC_FAR *inItem) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ long inIndex) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long inIndex,
            /* [retval][out] */ ITableColumn __RPC_FAR *__RPC_FAR *outChart) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveXML( 
            /* [retval][out] */ BSTR __RPC_FAR *xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            /* [in] */ BSTR xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadByTableName( 
            /* [in] */ BSTR tabName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITableColumnsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITableColumns __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITableColumns __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITableColumns __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            ITableColumns __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ ITableColumn __RPC_FAR *inItem);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ long inIndex);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ long inIndex,
            /* [retval][out] */ ITableColumn __RPC_FAR *__RPC_FAR *outChart);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            ITableColumns __RPC_FAR * This,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveXML )( 
            ITableColumns __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadXML )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ BSTR xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadByTableName )( 
            ITableColumns __RPC_FAR * This,
            /* [in] */ BSTR tabName);
        
        END_INTERFACE
    } ITableColumnsVtbl;

    interface ITableColumns
    {
        CONST_VTBL struct ITableColumnsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITableColumns_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITableColumns_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITableColumns_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITableColumns_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITableColumns_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITableColumns_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITableColumns_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITableColumns_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define ITableColumns_Add(This,inItem)	\
    (This)->lpVtbl -> Add(This,inItem)

#define ITableColumns_Remove(This,inIndex)	\
    (This)->lpVtbl -> Remove(This,inIndex)

#define ITableColumns_get_Item(This,inIndex,outChart)	\
    (This)->lpVtbl -> get_Item(This,inIndex,outChart)

#define ITableColumns_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define ITableColumns_SaveXML(This,xml)	\
    (This)->lpVtbl -> SaveXML(This,xml)

#define ITableColumns_LoadXML(This,xml)	\
    (This)->lpVtbl -> LoadXML(This,xml)

#define ITableColumns_LoadByTableName(This,tabName)	\
    (This)->lpVtbl -> LoadByTableName(This,tabName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITableColumns_get_Count_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITableColumns_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITableColumns_Add_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [in] */ ITableColumn __RPC_FAR *inItem);


void __RPC_STUB ITableColumns_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITableColumns_Remove_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [in] */ long inIndex);


void __RPC_STUB ITableColumns_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITableColumns_get_Item_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [in] */ long inIndex,
    /* [retval][out] */ ITableColumn __RPC_FAR *__RPC_FAR *outChart);


void __RPC_STUB ITableColumns_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITableColumns_get__NewEnum_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);


void __RPC_STUB ITableColumns_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumns_SaveXML_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *xml);


void __RPC_STUB ITableColumns_SaveXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumns_LoadXML_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [in] */ BSTR xml);


void __RPC_STUB ITableColumns_LoadXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumns_LoadByTableName_Proxy( 
    ITableColumns __RPC_FAR * This,
    /* [in] */ BSTR tabName);


void __RPC_STUB ITableColumns_LoadByTableName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITableColumns_INTERFACE_DEFINED__ */


#ifndef __ITableRows_INTERFACE_DEFINED__
#define __ITableRows_INTERFACE_DEFINED__

/* interface ITableRows */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITableRows;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("73FE3D4F-25FA-42B2-93E1-904202055048")
    ITableRows : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ ITableRow __RPC_FAR *inItem) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ long inIndex) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long inIndex,
            /* [retval][out] */ ITableRow __RPC_FAR *__RPC_FAR *outChart) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveXML( 
            /* [retval][out] */ BSTR __RPC_FAR *xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            /* [in] */ BSTR xml) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITableRowsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITableRows __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITableRows __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITableRows __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            ITableRows __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ ITableRow __RPC_FAR *inItem);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ long inIndex);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ long inIndex,
            /* [retval][out] */ ITableRow __RPC_FAR *__RPC_FAR *outChart);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            ITableRows __RPC_FAR * This,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveXML )( 
            ITableRows __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadXML )( 
            ITableRows __RPC_FAR * This,
            /* [in] */ BSTR xml);
        
        END_INTERFACE
    } ITableRowsVtbl;

    interface ITableRows
    {
        CONST_VTBL struct ITableRowsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITableRows_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITableRows_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITableRows_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITableRows_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITableRows_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITableRows_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITableRows_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITableRows_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define ITableRows_Add(This,inItem)	\
    (This)->lpVtbl -> Add(This,inItem)

#define ITableRows_Remove(This,inIndex)	\
    (This)->lpVtbl -> Remove(This,inIndex)

#define ITableRows_get_Item(This,inIndex,outChart)	\
    (This)->lpVtbl -> get_Item(This,inIndex,outChart)

#define ITableRows_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define ITableRows_SaveXML(This,xml)	\
    (This)->lpVtbl -> SaveXML(This,xml)

#define ITableRows_LoadXML(This,xml)	\
    (This)->lpVtbl -> LoadXML(This,xml)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITableRows_get_Count_Proxy( 
    ITableRows __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITableRows_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITableRows_Add_Proxy( 
    ITableRows __RPC_FAR * This,
    /* [in] */ ITableRow __RPC_FAR *inItem);


void __RPC_STUB ITableRows_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITableRows_Remove_Proxy( 
    ITableRows __RPC_FAR * This,
    /* [in] */ long inIndex);


void __RPC_STUB ITableRows_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITableRows_get_Item_Proxy( 
    ITableRows __RPC_FAR * This,
    /* [in] */ long inIndex,
    /* [retval][out] */ ITableRow __RPC_FAR *__RPC_FAR *outChart);


void __RPC_STUB ITableRows_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITableRows_get__NewEnum_Proxy( 
    ITableRows __RPC_FAR * This,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);


void __RPC_STUB ITableRows_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableRows_SaveXML_Proxy( 
    ITableRows __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *xml);


void __RPC_STUB ITableRows_SaveXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableRows_LoadXML_Proxy( 
    ITableRows __RPC_FAR * This,
    /* [in] */ BSTR xml);


void __RPC_STUB ITableRows_LoadXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITableRows_INTERFACE_DEFINED__ */


#ifndef __ITableManager_INTERFACE_DEFINED__
#define __ITableManager_INTERFACE_DEFINED__

/* interface ITableManager */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITableManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C59A4E33-1A3E-4A3D-A748-2DCE4AD733EA")
    ITableManager : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DBFile( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DBFile( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveXML( 
            /* [retval][out] */ BSTR __RPC_FAR *xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            /* [in] */ BSTR xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddTable( 
            /* [in] */ BSTR tableName,
            /* [in] */ ITableColumns __RPC_FAR *pCols) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveTable( 
            /* [in] */ BSTR tableName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddTableColumn( 
            /* [in] */ BSTR tableName,
            /* [in] */ BSTR columnName,
            /* [in] */ DATA_TYPE Type) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveTableColumn( 
            /* [in] */ BSTR tableName,
            /* [in] */ BSTR columnName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadDataOnCondition( 
            /* [in] */ BSTR tableName,
            /* [in] */ BSTR whereState,
            /* [out] */ ITableRows __RPC_FAR *__RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITableManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITableManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITableManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITableManager __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DBFile )( 
            ITableManager __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DBFile )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveXML )( 
            ITableManager __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadXML )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ BSTR xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddTable )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ BSTR tableName,
            /* [in] */ ITableColumns __RPC_FAR *pCols);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveTable )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ BSTR tableName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddTableColumn )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ BSTR tableName,
            /* [in] */ BSTR columnName,
            /* [in] */ DATA_TYPE Type);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveTableColumn )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ BSTR tableName,
            /* [in] */ BSTR columnName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadDataOnCondition )( 
            ITableManager __RPC_FAR * This,
            /* [in] */ BSTR tableName,
            /* [in] */ BSTR whereState,
            /* [out] */ ITableRows __RPC_FAR *__RPC_FAR *pVal);
        
        END_INTERFACE
    } ITableManagerVtbl;

    interface ITableManager
    {
        CONST_VTBL struct ITableManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITableManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITableManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITableManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITableManager_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITableManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITableManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITableManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITableManager_get_DBFile(This,pVal)	\
    (This)->lpVtbl -> get_DBFile(This,pVal)

#define ITableManager_put_DBFile(This,newVal)	\
    (This)->lpVtbl -> put_DBFile(This,newVal)

#define ITableManager_SaveXML(This,xml)	\
    (This)->lpVtbl -> SaveXML(This,xml)

#define ITableManager_LoadXML(This,xml)	\
    (This)->lpVtbl -> LoadXML(This,xml)

#define ITableManager_AddTable(This,tableName,pCols)	\
    (This)->lpVtbl -> AddTable(This,tableName,pCols)

#define ITableManager_RemoveTable(This,tableName)	\
    (This)->lpVtbl -> RemoveTable(This,tableName)

#define ITableManager_AddTableColumn(This,tableName,columnName,Type)	\
    (This)->lpVtbl -> AddTableColumn(This,tableName,columnName,Type)

#define ITableManager_RemoveTableColumn(This,tableName,columnName)	\
    (This)->lpVtbl -> RemoveTableColumn(This,tableName,columnName)

#define ITableManager_LoadDataOnCondition(This,tableName,whereState,pVal)	\
    (This)->lpVtbl -> LoadDataOnCondition(This,tableName,whereState,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableManager_get_DBFile_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITableManager_get_DBFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableManager_put_DBFile_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITableManager_put_DBFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableManager_SaveXML_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *xml);


void __RPC_STUB ITableManager_SaveXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableManager_LoadXML_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [in] */ BSTR xml);


void __RPC_STUB ITableManager_LoadXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableManager_AddTable_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [in] */ BSTR tableName,
    /* [in] */ ITableColumns __RPC_FAR *pCols);


void __RPC_STUB ITableManager_AddTable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableManager_RemoveTable_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [in] */ BSTR tableName);


void __RPC_STUB ITableManager_RemoveTable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableManager_AddTableColumn_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [in] */ BSTR tableName,
    /* [in] */ BSTR columnName,
    /* [in] */ DATA_TYPE Type);


void __RPC_STUB ITableManager_AddTableColumn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableManager_RemoveTableColumn_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [in] */ BSTR tableName,
    /* [in] */ BSTR columnName);


void __RPC_STUB ITableManager_RemoveTableColumn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableManager_LoadDataOnCondition_Proxy( 
    ITableManager __RPC_FAR * This,
    /* [in] */ BSTR tableName,
    /* [in] */ BSTR whereState,
    /* [out] */ ITableRows __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ITableManager_LoadDataOnCondition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITableManager_INTERFACE_DEFINED__ */


#ifndef __ITable_INTERFACE_DEFINED__
#define __ITable_INTERFACE_DEFINED__

/* interface ITable */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITable;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FD8087CD-D239-431A-8C16-DF9C2F5E4AFD")
    ITable : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Columns( 
            /* [retval][out] */ ITableColumns __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Columns( 
            /* [in] */ ITableColumns __RPC_FAR *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveXML( 
            /* [retval][out] */ BSTR __RPC_FAR *xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            /* [in] */ BSTR xml) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITableVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITable __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITable __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITable __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITable __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITable __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITable __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITable __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Columns )( 
            ITable __RPC_FAR * This,
            /* [retval][out] */ ITableColumns __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Columns )( 
            ITable __RPC_FAR * This,
            /* [in] */ ITableColumns __RPC_FAR *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ITable __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            ITable __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveXML )( 
            ITable __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadXML )( 
            ITable __RPC_FAR * This,
            /* [in] */ BSTR xml);
        
        END_INTERFACE
    } ITableVtbl;

    interface ITable
    {
        CONST_VTBL struct ITableVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITable_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITable_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITable_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITable_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITable_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITable_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITable_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITable_get_Columns(This,pVal)	\
    (This)->lpVtbl -> get_Columns(This,pVal)

#define ITable_putref_Columns(This,newVal)	\
    (This)->lpVtbl -> putref_Columns(This,newVal)

#define ITable_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define ITable_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define ITable_SaveXML(This,xml)	\
    (This)->lpVtbl -> SaveXML(This,xml)

#define ITable_LoadXML(This,xml)	\
    (This)->lpVtbl -> LoadXML(This,xml)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITable_get_Columns_Proxy( 
    ITable __RPC_FAR * This,
    /* [retval][out] */ ITableColumns __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ITable_get_Columns_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propputref] */ HRESULT STDMETHODCALLTYPE ITable_putref_Columns_Proxy( 
    ITable __RPC_FAR * This,
    /* [in] */ ITableColumns __RPC_FAR *newVal);


void __RPC_STUB ITable_putref_Columns_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITable_get_Name_Proxy( 
    ITable __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITable_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITable_put_Name_Proxy( 
    ITable __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITable_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITable_SaveXML_Proxy( 
    ITable __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *xml);


void __RPC_STUB ITable_SaveXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITable_LoadXML_Proxy( 
    ITable __RPC_FAR * This,
    /* [in] */ BSTR xml);


void __RPC_STUB ITable_LoadXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITable_INTERFACE_DEFINED__ */


#ifndef __ITableColumn_INTERFACE_DEFINED__
#define __ITableColumn_INTERFACE_DEFINED__

/* interface ITableColumn */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITableColumn;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C7598C54-DFCD-48D5-A740-6F057534474B")
    ITableColumn : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ DATA_TYPE __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ DATA_TYPE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Index( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Index( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveXML( 
            /* [retval][out] */ BSTR __RPC_FAR *xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            /* [in] */ BSTR xml) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ID( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Update( 
            /* [defaultvalue][in] */ long TransactionLevel = 0) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR tableName,
            /* [retval][out] */ long __RPC_FAR *NewId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Delete( 
            /* [defaultvalue][in] */ long TransactionLevel = 0) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITableColumnVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITableColumn __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITableColumn __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITableColumn __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ITableColumn __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            ITableColumn __RPC_FAR * This,
            /* [retval][out] */ DATA_TYPE __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ DATA_TYPE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Index )( 
            ITableColumn __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Index )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveXML )( 
            ITableColumn __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadXML )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ BSTR xml);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            ITableColumn __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ID )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Update )( 
            ITableColumn __RPC_FAR * This,
            /* [defaultvalue][in] */ long TransactionLevel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            ITableColumn __RPC_FAR * This,
            /* [in] */ BSTR tableName,
            /* [retval][out] */ long __RPC_FAR *NewId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Delete )( 
            ITableColumn __RPC_FAR * This,
            /* [defaultvalue][in] */ long TransactionLevel);
        
        END_INTERFACE
    } ITableColumnVtbl;

    interface ITableColumn
    {
        CONST_VTBL struct ITableColumnVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITableColumn_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITableColumn_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITableColumn_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITableColumn_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITableColumn_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITableColumn_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITableColumn_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITableColumn_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define ITableColumn_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define ITableColumn_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define ITableColumn_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define ITableColumn_get_Index(This,pVal)	\
    (This)->lpVtbl -> get_Index(This,pVal)

#define ITableColumn_put_Index(This,newVal)	\
    (This)->lpVtbl -> put_Index(This,newVal)

#define ITableColumn_SaveXML(This,xml)	\
    (This)->lpVtbl -> SaveXML(This,xml)

#define ITableColumn_LoadXML(This,xml)	\
    (This)->lpVtbl -> LoadXML(This,xml)

#define ITableColumn_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define ITableColumn_put_ID(This,newVal)	\
    (This)->lpVtbl -> put_ID(This,newVal)

#define ITableColumn_Update(This,TransactionLevel)	\
    (This)->lpVtbl -> Update(This,TransactionLevel)

#define ITableColumn_Add(This,tableName,NewId)	\
    (This)->lpVtbl -> Add(This,tableName,NewId)

#define ITableColumn_Delete(This,TransactionLevel)	\
    (This)->lpVtbl -> Delete(This,TransactionLevel)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableColumn_get_Name_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITableColumn_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableColumn_put_Name_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITableColumn_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableColumn_get_Type_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [retval][out] */ DATA_TYPE __RPC_FAR *pVal);


void __RPC_STUB ITableColumn_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableColumn_put_Type_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [in] */ DATA_TYPE newVal);


void __RPC_STUB ITableColumn_put_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableColumn_get_Index_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITableColumn_get_Index_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableColumn_put_Index_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ITableColumn_put_Index_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumn_SaveXML_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *xml);


void __RPC_STUB ITableColumn_SaveXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumn_LoadXML_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [in] */ BSTR xml);


void __RPC_STUB ITableColumn_LoadXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableColumn_get_ID_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITableColumn_get_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableColumn_put_ID_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ITableColumn_put_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumn_Update_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [defaultvalue][in] */ long TransactionLevel);


void __RPC_STUB ITableColumn_Update_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumn_Add_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [in] */ BSTR tableName,
    /* [retval][out] */ long __RPC_FAR *NewId);


void __RPC_STUB ITableColumn_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableColumn_Delete_Proxy( 
    ITableColumn __RPC_FAR * This,
    /* [defaultvalue][in] */ long TransactionLevel);


void __RPC_STUB ITableColumn_Delete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITableColumn_INTERFACE_DEFINED__ */


#ifndef __ITableRow_INTERFACE_DEFINED__
#define __ITableRow_INTERFACE_DEFINED__

/* interface ITableRow */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITableRow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("235A716F-DA1B-4B46-AD85-98132DB9B708")
    ITableRow : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IntField( 
            /* [in] */ BSTR columnName,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IntField( 
            /* [in] */ BSTR columnName,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_StrField( 
            /* [in] */ BSTR columnName,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_StrField( 
            /* [in] */ BSTR columnName,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DblField( 
            /* [in] */ BSTR columnName,
            /* [retval][out] */ double __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DblField( 
            /* [in] */ BSTR columnName,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DateField( 
            /* [in] */ BSTR columnName,
            /* [retval][out] */ DATE __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DateField( 
            /* [in] */ BSTR columnName,
            /* [in] */ DATE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ID( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            ITableColumns __RPC_FAR *pCols,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            ITableColumns __RPC_FAR *pCols,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReBuildValue( 
            ITableColumns __RPC_FAR *pCols) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveXML( 
            /* [retval][out] */ BSTR __RPC_FAR *xml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            /* [in] */ BSTR xml) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITableRowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITableRow __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITableRow __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITableRow __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IntField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IntField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StrField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StrField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DblField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [retval][out] */ double __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DblField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DateField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [retval][out] */ DATE __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DateField )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR columnName,
            /* [in] */ DATE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ID )( 
            ITableRow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ID )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Value )( 
            ITableRow __RPC_FAR * This,
            ITableColumns __RPC_FAR *pCols,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Value )( 
            ITableRow __RPC_FAR * This,
            ITableColumns __RPC_FAR *pCols,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReBuildValue )( 
            ITableRow __RPC_FAR * This,
            ITableColumns __RPC_FAR *pCols);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveXML )( 
            ITableRow __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *xml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadXML )( 
            ITableRow __RPC_FAR * This,
            /* [in] */ BSTR xml);
        
        END_INTERFACE
    } ITableRowVtbl;

    interface ITableRow
    {
        CONST_VTBL struct ITableRowVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITableRow_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITableRow_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITableRow_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITableRow_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITableRow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITableRow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITableRow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITableRow_get_IntField(This,columnName,pVal)	\
    (This)->lpVtbl -> get_IntField(This,columnName,pVal)

#define ITableRow_put_IntField(This,columnName,newVal)	\
    (This)->lpVtbl -> put_IntField(This,columnName,newVal)

#define ITableRow_get_StrField(This,columnName,pVal)	\
    (This)->lpVtbl -> get_StrField(This,columnName,pVal)

#define ITableRow_put_StrField(This,columnName,newVal)	\
    (This)->lpVtbl -> put_StrField(This,columnName,newVal)

#define ITableRow_get_DblField(This,columnName,pVal)	\
    (This)->lpVtbl -> get_DblField(This,columnName,pVal)

#define ITableRow_put_DblField(This,columnName,newVal)	\
    (This)->lpVtbl -> put_DblField(This,columnName,newVal)

#define ITableRow_get_DateField(This,columnName,pVal)	\
    (This)->lpVtbl -> get_DateField(This,columnName,pVal)

#define ITableRow_put_DateField(This,columnName,newVal)	\
    (This)->lpVtbl -> put_DateField(This,columnName,newVal)

#define ITableRow_get_ID(This,pVal)	\
    (This)->lpVtbl -> get_ID(This,pVal)

#define ITableRow_put_ID(This,newVal)	\
    (This)->lpVtbl -> put_ID(This,newVal)

#define ITableRow_get_Value(This,pCols,pVal)	\
    (This)->lpVtbl -> get_Value(This,pCols,pVal)

#define ITableRow_put_Value(This,pCols,newVal)	\
    (This)->lpVtbl -> put_Value(This,pCols,newVal)

#define ITableRow_ReBuildValue(This,pCols)	\
    (This)->lpVtbl -> ReBuildValue(This,pCols)

#define ITableRow_SaveXML(This,xml)	\
    (This)->lpVtbl -> SaveXML(This,xml)

#define ITableRow_LoadXML(This,xml)	\
    (This)->lpVtbl -> LoadXML(This,xml)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableRow_get_IntField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITableRow_get_IntField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableRow_put_IntField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [in] */ long newVal);


void __RPC_STUB ITableRow_put_IntField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableRow_get_StrField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITableRow_get_StrField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableRow_put_StrField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITableRow_put_StrField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableRow_get_DblField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [retval][out] */ double __RPC_FAR *pVal);


void __RPC_STUB ITableRow_get_DblField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableRow_put_DblField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [in] */ double newVal);


void __RPC_STUB ITableRow_put_DblField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableRow_get_DateField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [retval][out] */ DATE __RPC_FAR *pVal);


void __RPC_STUB ITableRow_get_DateField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableRow_put_DateField_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR columnName,
    /* [in] */ DATE newVal);


void __RPC_STUB ITableRow_put_DateField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableRow_get_ID_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITableRow_get_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableRow_put_ID_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ITableRow_put_ID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITableRow_get_Value_Proxy( 
    ITableRow __RPC_FAR * This,
    ITableColumns __RPC_FAR *pCols,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ITableRow_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITableRow_put_Value_Proxy( 
    ITableRow __RPC_FAR * This,
    ITableColumns __RPC_FAR *pCols,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITableRow_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableRow_ReBuildValue_Proxy( 
    ITableRow __RPC_FAR * This,
    ITableColumns __RPC_FAR *pCols);


void __RPC_STUB ITableRow_ReBuildValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableRow_SaveXML_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *xml);


void __RPC_STUB ITableRow_SaveXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITableRow_LoadXML_Proxy( 
    ITableRow __RPC_FAR * This,
    /* [in] */ BSTR xml);


void __RPC_STUB ITableRow_LoadXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITableRow_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TableManager;

#ifdef __cplusplus

class DECLSPEC_UUID("C92F93F5-7D8A-46F4-9960-8B11A30742A9")
TableManager;
#endif

EXTERN_C const CLSID CLSID_Table;

#ifdef __cplusplus

class DECLSPEC_UUID("E7EE0D3E-384E-4A5E-B223-1D8A81B65EAA")
Table;
#endif

EXTERN_C const CLSID CLSID_TableColumn;

#ifdef __cplusplus

class DECLSPEC_UUID("A534BCE0-8C70-46FA-A6ED-F044640E6B6A")
TableColumn;
#endif

EXTERN_C const CLSID CLSID_TableColumns;

#ifdef __cplusplus

class DECLSPEC_UUID("E1F8986D-B1D7-4D9B-80B7-15195579EA07")
TableColumns;
#endif

EXTERN_C const CLSID CLSID_TableRow;

#ifdef __cplusplus

class DECLSPEC_UUID("657747F5-AE21-44C5-B3DD-1241B51F5B1B")
TableRow;
#endif

EXTERN_C const CLSID CLSID_TableRows;

#ifdef __cplusplus

class DECLSPEC_UUID("B170F253-D70F-4AD6-8AA6-FE826FCE5296")
TableRows;
#endif
#endif /* __TTABLELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
