

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "IRTDServer.h"

#define TYPE_FORMAT_STRING_SIZE   1121                              
#define PROC_FORMAT_STRING_SIZE   373                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _IRTDServer_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } IRTDServer_MIDL_TYPE_FORMAT_STRING;

typedef struct _IRTDServer_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } IRTDServer_MIDL_PROC_FORMAT_STRING;

typedef struct _IRTDServer_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } IRTDServer_MIDL_EXPR_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const IRTDServer_MIDL_TYPE_FORMAT_STRING IRTDServer__MIDL_TypeFormatString;
extern const IRTDServer_MIDL_PROC_FORMAT_STRING IRTDServer__MIDL_ProcFormatString;
extern const IRTDServer_MIDL_EXPR_FORMAT_STRING IRTDServer__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IRTDUpdateEvent_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IRTDUpdateEvent_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IRtdServer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IRtdServer_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const IRTDServer_MIDL_PROC_FORMAT_STRING IRTDServer__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure UpdateNotify */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 24 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_HeartbeatInterval */

/* 30 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 32 */	NdrFcLong( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0x8 ),	/* 8 */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	NdrFcShort( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x24 ),	/* 36 */
/* 44 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 46 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter plRetVal */

/* 54 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 56 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 58 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 60 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 62 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_HeartbeatInterval */

/* 66 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 68 */	NdrFcLong( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x9 ),	/* 9 */
/* 74 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 76 */	NdrFcShort( 0x8 ),	/* 8 */
/* 78 */	NdrFcShort( 0x8 ),	/* 8 */
/* 80 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 82 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x0 ),	/* 0 */
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter plRetVal */

/* 90 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 92 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 96 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 98 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Disconnect */

/* 102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0xa ),	/* 10 */
/* 110 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 116 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 118 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 128 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ServerStart */

/* 132 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x7 ),	/* 7 */
/* 140 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0x24 ),	/* 36 */
/* 146 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 148 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter CallbackObject */

/* 156 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 158 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 160 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter pfRes */

/* 162 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 164 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 170 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ConnectData */

/* 174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0x8 ),	/* 8 */
/* 182 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 184 */	NdrFcShort( 0x22 ),	/* 34 */
/* 186 */	NdrFcShort( 0x22 ),	/* 34 */
/* 188 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 190 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 192 */	NdrFcShort( 0xb5 ),	/* 181 */
/* 194 */	NdrFcShort( 0x1 ),	/* 1 */
/* 196 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter TopicID */

/* 198 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 200 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Strings */

/* 204 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 206 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 208 */	NdrFcShort( 0x426 ),	/* Type Offset=1062 */

	/* Parameter GetNewValues */

/* 210 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 212 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 214 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pvarOut */

/* 216 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 218 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 220 */	NdrFcShort( 0x43c ),	/* Type Offset=1084 */

	/* Return value */

/* 222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 224 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RefreshData */

/* 228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x9 ),	/* 9 */
/* 236 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 238 */	NdrFcShort( 0x1c ),	/* 28 */
/* 240 */	NdrFcShort( 0x24 ),	/* 36 */
/* 242 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 244 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 246 */	NdrFcShort( 0x1 ),	/* 1 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter TopicCount */

/* 252 */	NdrFcShort( 0x158 ),	/* Flags:  in, out, base type, simple ref, */
/* 254 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter parrayOut */

/* 258 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 260 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 262 */	NdrFcShort( 0x456 ),	/* Type Offset=1110 */

	/* Return value */

/* 264 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 266 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DisconnectData */

/* 270 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 272 */	NdrFcLong( 0x0 ),	/* 0 */
/* 276 */	NdrFcShort( 0xa ),	/* 10 */
/* 278 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 282 */	NdrFcShort( 0x8 ),	/* 8 */
/* 284 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 286 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 290 */	NdrFcShort( 0x0 ),	/* 0 */
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter TopicID */

/* 294 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 296 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 302 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Heartbeat */

/* 306 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 308 */	NdrFcLong( 0x0 ),	/* 0 */
/* 312 */	NdrFcShort( 0xb ),	/* 11 */
/* 314 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 318 */	NdrFcShort( 0x24 ),	/* 36 */
/* 320 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 322 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 328 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pfRes */

/* 330 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 332 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 338 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ServerTerminate */

/* 342 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0xc ),	/* 12 */
/* 350 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 354 */	NdrFcShort( 0x8 ),	/* 8 */
/* 356 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 358 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 364 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 366 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 368 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const IRTDServer_MIDL_TYPE_FORMAT_STRING IRTDServer__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  8 */	NdrFcLong( 0xa43788c1 ),	/* -1539864383 */
/* 12 */	NdrFcShort( 0xd91b ),	/* -9957 */
/* 14 */	NdrFcShort( 0x11d3 ),	/* 4563 */
/* 16 */	0x8f,		/* 143 */
			0x39,		/* 57 */
/* 18 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 20 */	0x4f,		/* 79 */
			0x36,		/* 54 */
/* 22 */	0x51,		/* 81 */
			0xb8,		/* 184 */
/* 24 */	
			0x11, 0x0,	/* FC_RP */
/* 26 */	NdrFcShort( 0x40c ),	/* Offset= 1036 (1062) */
/* 28 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 30 */	NdrFcShort( 0x2 ),	/* Offset= 2 (32) */
/* 32 */	
			0x12, 0x0,	/* FC_UP */
/* 34 */	NdrFcShort( 0x3f2 ),	/* Offset= 1010 (1044) */
/* 36 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 38 */	NdrFcShort( 0x18 ),	/* 24 */
/* 40 */	NdrFcShort( 0xa ),	/* 10 */
/* 42 */	NdrFcLong( 0x8 ),	/* 8 */
/* 46 */	NdrFcShort( 0x70 ),	/* Offset= 112 (158) */
/* 48 */	NdrFcLong( 0xd ),	/* 13 */
/* 52 */	NdrFcShort( 0xa6 ),	/* Offset= 166 (218) */
/* 54 */	NdrFcLong( 0x9 ),	/* 9 */
/* 58 */	NdrFcShort( 0xd8 ),	/* Offset= 216 (274) */
/* 60 */	NdrFcLong( 0xc ),	/* 12 */
/* 64 */	NdrFcShort( 0x2d6 ),	/* Offset= 726 (790) */
/* 66 */	NdrFcLong( 0x24 ),	/* 36 */
/* 70 */	NdrFcShort( 0x300 ),	/* Offset= 768 (838) */
/* 72 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 76 */	NdrFcShort( 0x31c ),	/* Offset= 796 (872) */
/* 78 */	NdrFcLong( 0x10 ),	/* 16 */
/* 82 */	NdrFcShort( 0x336 ),	/* Offset= 822 (904) */
/* 84 */	NdrFcLong( 0x2 ),	/* 2 */
/* 88 */	NdrFcShort( 0x350 ),	/* Offset= 848 (936) */
/* 90 */	NdrFcLong( 0x3 ),	/* 3 */
/* 94 */	NdrFcShort( 0x36a ),	/* Offset= 874 (968) */
/* 96 */	NdrFcLong( 0x14 ),	/* 20 */
/* 100 */	NdrFcShort( 0x384 ),	/* Offset= 900 (1000) */
/* 102 */	NdrFcShort( 0xffff ),	/* Offset= -1 (101) */
/* 104 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 106 */	NdrFcShort( 0x2 ),	/* 2 */
/* 108 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 110 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 112 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 114 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 116 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (104) */
/* 122 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 124 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 126 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 128 */	NdrFcShort( 0x4 ),	/* 4 */
/* 130 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 132 */	NdrFcShort( 0x0 ),	/* 0 */
/* 134 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 136 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 138 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 140 */	NdrFcShort( 0x4 ),	/* 4 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0x1 ),	/* 1 */
/* 146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 150 */	0x12, 0x0,	/* FC_UP */
/* 152 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (116) */
/* 154 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 156 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 158 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 160 */	NdrFcShort( 0x8 ),	/* 8 */
/* 162 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 164 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 166 */	NdrFcShort( 0x4 ),	/* 4 */
/* 168 */	NdrFcShort( 0x4 ),	/* 4 */
/* 170 */	0x11, 0x0,	/* FC_RP */
/* 172 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (126) */
/* 174 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 176 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 178 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 180 */	NdrFcLong( 0x0 ),	/* 0 */
/* 184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 188 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 190 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 192 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 194 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 196 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 204 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 206 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 210 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 212 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 214 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (178) */
/* 216 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 218 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 220 */	NdrFcShort( 0x8 ),	/* 8 */
/* 222 */	NdrFcShort( 0x0 ),	/* 0 */
/* 224 */	NdrFcShort( 0x6 ),	/* Offset= 6 (230) */
/* 226 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 228 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 230 */	
			0x11, 0x0,	/* FC_RP */
/* 232 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (196) */
/* 234 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 236 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 244 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 246 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 248 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 250 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 252 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 254 */	NdrFcShort( 0x0 ),	/* 0 */
/* 256 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 260 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 262 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 266 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 268 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 270 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (234) */
/* 272 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 274 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 276 */	NdrFcShort( 0x8 ),	/* 8 */
/* 278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 280 */	NdrFcShort( 0x6 ),	/* Offset= 6 (286) */
/* 282 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 284 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 286 */	
			0x11, 0x0,	/* FC_RP */
/* 288 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (252) */
/* 290 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 292 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 294 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 296 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 298 */	NdrFcShort( 0x2 ),	/* Offset= 2 (300) */
/* 300 */	NdrFcShort( 0x10 ),	/* 16 */
/* 302 */	NdrFcShort( 0x2f ),	/* 47 */
/* 304 */	NdrFcLong( 0x14 ),	/* 20 */
/* 308 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 310 */	NdrFcLong( 0x3 ),	/* 3 */
/* 314 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 316 */	NdrFcLong( 0x11 ),	/* 17 */
/* 320 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 322 */	NdrFcLong( 0x2 ),	/* 2 */
/* 326 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 328 */	NdrFcLong( 0x4 ),	/* 4 */
/* 332 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 334 */	NdrFcLong( 0x5 ),	/* 5 */
/* 338 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 340 */	NdrFcLong( 0xb ),	/* 11 */
/* 344 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 346 */	NdrFcLong( 0xa ),	/* 10 */
/* 350 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 352 */	NdrFcLong( 0x6 ),	/* 6 */
/* 356 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (588) */
/* 358 */	NdrFcLong( 0x7 ),	/* 7 */
/* 362 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 364 */	NdrFcLong( 0x8 ),	/* 8 */
/* 368 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (594) */
/* 370 */	NdrFcLong( 0xd ),	/* 13 */
/* 374 */	NdrFcShort( 0xff3c ),	/* Offset= -196 (178) */
/* 376 */	NdrFcLong( 0x9 ),	/* 9 */
/* 380 */	NdrFcShort( 0xff6e ),	/* Offset= -146 (234) */
/* 382 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 386 */	NdrFcShort( 0xd4 ),	/* Offset= 212 (598) */
/* 388 */	NdrFcLong( 0x24 ),	/* 36 */
/* 392 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (606) */
/* 394 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 398 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (606) */
/* 400 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 404 */	NdrFcShort( 0x100 ),	/* Offset= 256 (660) */
/* 406 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 410 */	NdrFcShort( 0xfe ),	/* Offset= 254 (664) */
/* 412 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 416 */	NdrFcShort( 0xfc ),	/* Offset= 252 (668) */
/* 418 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 422 */	NdrFcShort( 0xfa ),	/* Offset= 250 (672) */
/* 424 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 428 */	NdrFcShort( 0xf8 ),	/* Offset= 248 (676) */
/* 430 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 434 */	NdrFcShort( 0xf6 ),	/* Offset= 246 (680) */
/* 436 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 440 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (664) */
/* 442 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 446 */	NdrFcShort( 0xde ),	/* Offset= 222 (668) */
/* 448 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 452 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (684) */
/* 454 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 458 */	NdrFcShort( 0xde ),	/* Offset= 222 (680) */
/* 460 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 464 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (688) */
/* 466 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 470 */	NdrFcShort( 0xde ),	/* Offset= 222 (692) */
/* 472 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 476 */	NdrFcShort( 0xdc ),	/* Offset= 220 (696) */
/* 478 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 482 */	NdrFcShort( 0xda ),	/* Offset= 218 (700) */
/* 484 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 488 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (712) */
/* 490 */	NdrFcLong( 0x10 ),	/* 16 */
/* 494 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 496 */	NdrFcLong( 0x12 ),	/* 18 */
/* 500 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 502 */	NdrFcLong( 0x13 ),	/* 19 */
/* 506 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 508 */	NdrFcLong( 0x15 ),	/* 21 */
/* 512 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 514 */	NdrFcLong( 0x16 ),	/* 22 */
/* 518 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 520 */	NdrFcLong( 0x17 ),	/* 23 */
/* 524 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 526 */	NdrFcLong( 0xe ),	/* 14 */
/* 530 */	NdrFcShort( 0xbe ),	/* Offset= 190 (720) */
/* 532 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 536 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (730) */
/* 538 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 542 */	NdrFcShort( 0xc0 ),	/* Offset= 192 (734) */
/* 544 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 548 */	NdrFcShort( 0x74 ),	/* Offset= 116 (664) */
/* 550 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 554 */	NdrFcShort( 0x72 ),	/* Offset= 114 (668) */
/* 556 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 560 */	NdrFcShort( 0x70 ),	/* Offset= 112 (672) */
/* 562 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 566 */	NdrFcShort( 0x66 ),	/* Offset= 102 (668) */
/* 568 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 572 */	NdrFcShort( 0x60 ),	/* Offset= 96 (668) */
/* 574 */	NdrFcLong( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* Offset= 0 (578) */
/* 580 */	NdrFcLong( 0x1 ),	/* 1 */
/* 584 */	NdrFcShort( 0x0 ),	/* Offset= 0 (584) */
/* 586 */	NdrFcShort( 0xffff ),	/* Offset= -1 (585) */
/* 588 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 590 */	NdrFcShort( 0x8 ),	/* 8 */
/* 592 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 594 */	
			0x12, 0x0,	/* FC_UP */
/* 596 */	NdrFcShort( 0xfe20 ),	/* Offset= -480 (116) */
/* 598 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 600 */	NdrFcShort( 0x2 ),	/* Offset= 2 (602) */
/* 602 */	
			0x12, 0x0,	/* FC_UP */
/* 604 */	NdrFcShort( 0x1b8 ),	/* Offset= 440 (1044) */
/* 606 */	
			0x12, 0x0,	/* FC_UP */
/* 608 */	NdrFcShort( 0x20 ),	/* Offset= 32 (640) */
/* 610 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 612 */	NdrFcLong( 0x2f ),	/* 47 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 622 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 624 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 626 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 628 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 630 */	NdrFcShort( 0x1 ),	/* 1 */
/* 632 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 634 */	NdrFcShort( 0x4 ),	/* 4 */
/* 636 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 638 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 640 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 642 */	NdrFcShort( 0x10 ),	/* 16 */
/* 644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 646 */	NdrFcShort( 0xa ),	/* Offset= 10 (656) */
/* 648 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 650 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 652 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (610) */
/* 654 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 656 */	
			0x12, 0x0,	/* FC_UP */
/* 658 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (628) */
/* 660 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 662 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 664 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 666 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 668 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 670 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 672 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 674 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 676 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 678 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 680 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 682 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 684 */	
			0x12, 0x0,	/* FC_UP */
/* 686 */	NdrFcShort( 0xff9e ),	/* Offset= -98 (588) */
/* 688 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 690 */	NdrFcShort( 0xffa0 ),	/* Offset= -96 (594) */
/* 692 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 694 */	NdrFcShort( 0xfdfc ),	/* Offset= -516 (178) */
/* 696 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 698 */	NdrFcShort( 0xfe30 ),	/* Offset= -464 (234) */
/* 700 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 702 */	NdrFcShort( 0x2 ),	/* Offset= 2 (704) */
/* 704 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 706 */	NdrFcShort( 0x2 ),	/* Offset= 2 (708) */
/* 708 */	
			0x12, 0x0,	/* FC_UP */
/* 710 */	NdrFcShort( 0x14e ),	/* Offset= 334 (1044) */
/* 712 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 714 */	NdrFcShort( 0x2 ),	/* Offset= 2 (716) */
/* 716 */	
			0x12, 0x0,	/* FC_UP */
/* 718 */	NdrFcShort( 0x14 ),	/* Offset= 20 (738) */
/* 720 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 722 */	NdrFcShort( 0x10 ),	/* 16 */
/* 724 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 726 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 728 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 730 */	
			0x12, 0x0,	/* FC_UP */
/* 732 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (720) */
/* 734 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 736 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 738 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 740 */	NdrFcShort( 0x20 ),	/* 32 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x0 ),	/* Offset= 0 (744) */
/* 746 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 748 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 750 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 752 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 754 */	NdrFcShort( 0xfe30 ),	/* Offset= -464 (290) */
/* 756 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 758 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 760 */	NdrFcShort( 0x4 ),	/* 4 */
/* 762 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 768 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 770 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 772 */	NdrFcShort( 0x4 ),	/* 4 */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x1 ),	/* 1 */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	0x12, 0x0,	/* FC_UP */
/* 784 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (738) */
/* 786 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 788 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 790 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x6 ),	/* Offset= 6 (802) */
/* 798 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 800 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 802 */	
			0x11, 0x0,	/* FC_RP */
/* 804 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (758) */
/* 806 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 808 */	NdrFcShort( 0x4 ),	/* 4 */
/* 810 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 814 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 816 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 818 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x1 ),	/* 1 */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 830 */	0x12, 0x0,	/* FC_UP */
/* 832 */	NdrFcShort( 0xff40 ),	/* Offset= -192 (640) */
/* 834 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 836 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 838 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 840 */	NdrFcShort( 0x8 ),	/* 8 */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	NdrFcShort( 0x6 ),	/* Offset= 6 (850) */
/* 846 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 848 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 850 */	
			0x11, 0x0,	/* FC_RP */
/* 852 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (806) */
/* 854 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 856 */	NdrFcShort( 0x8 ),	/* 8 */
/* 858 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 860 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 862 */	NdrFcShort( 0x10 ),	/* 16 */
/* 864 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 866 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 868 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (854) */
			0x5b,		/* FC_END */
/* 872 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 874 */	NdrFcShort( 0x18 ),	/* 24 */
/* 876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 878 */	NdrFcShort( 0xa ),	/* Offset= 10 (888) */
/* 880 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 882 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 884 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (860) */
/* 886 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 888 */	
			0x11, 0x0,	/* FC_RP */
/* 890 */	NdrFcShort( 0xfd4a ),	/* Offset= -694 (196) */
/* 892 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 894 */	NdrFcShort( 0x1 ),	/* 1 */
/* 896 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 900 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 902 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 904 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 906 */	NdrFcShort( 0x8 ),	/* 8 */
/* 908 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 910 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 912 */	NdrFcShort( 0x4 ),	/* 4 */
/* 914 */	NdrFcShort( 0x4 ),	/* 4 */
/* 916 */	0x12, 0x0,	/* FC_UP */
/* 918 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (892) */
/* 920 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 922 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 924 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 926 */	NdrFcShort( 0x2 ),	/* 2 */
/* 928 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 932 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 934 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 936 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 938 */	NdrFcShort( 0x8 ),	/* 8 */
/* 940 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 942 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 944 */	NdrFcShort( 0x4 ),	/* 4 */
/* 946 */	NdrFcShort( 0x4 ),	/* 4 */
/* 948 */	0x12, 0x0,	/* FC_UP */
/* 950 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (924) */
/* 952 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 954 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 956 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 958 */	NdrFcShort( 0x4 ),	/* 4 */
/* 960 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 966 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 968 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 970 */	NdrFcShort( 0x8 ),	/* 8 */
/* 972 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 974 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 976 */	NdrFcShort( 0x4 ),	/* 4 */
/* 978 */	NdrFcShort( 0x4 ),	/* 4 */
/* 980 */	0x12, 0x0,	/* FC_UP */
/* 982 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (956) */
/* 984 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 986 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 988 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 990 */	NdrFcShort( 0x8 ),	/* 8 */
/* 992 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */
/* 996 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 998 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1000 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1002 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1004 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1006 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1008 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1010 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1012 */	0x12, 0x0,	/* FC_UP */
/* 1014 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (988) */
/* 1016 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1018 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1020 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1022 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1024 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1026 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1028 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1030 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1032 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1034 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1036 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1038 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1040 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1020) */
/* 1042 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1044 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1046 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1048 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1028) */
/* 1050 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1050) */
/* 1052 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1054 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1056 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1058 */	NdrFcShort( 0xfc02 ),	/* Offset= -1022 (36) */
/* 1060 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1062 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1064 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1066 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1068 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1070 */	NdrFcShort( 0xfbee ),	/* Offset= -1042 (28) */
/* 1072 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 1074 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1076 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1078 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1084) */
/* 1080 */	
			0x13, 0x0,	/* FC_OP */
/* 1082 */	NdrFcShort( 0xfea8 ),	/* Offset= -344 (738) */
/* 1084 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1086 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1088 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1080) */
/* 1094 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 1096 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1098 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1100 */	NdrFcShort( 0xa ),	/* Offset= 10 (1110) */
/* 1102 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1104 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1106) */
/* 1106 */	
			0x13, 0x0,	/* FC_OP */
/* 1108 */	NdrFcShort( 0xffc0 ),	/* Offset= -64 (1044) */
/* 1110 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1114 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1118 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (1102) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            LPSAFEARRAY_UserSize
            ,LPSAFEARRAY_UserMarshal
            ,LPSAFEARRAY_UserUnmarshal
            ,LPSAFEARRAY_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IRTDUpdateEvent, ver. 0.0,
   GUID={0xA43788C1,0xD91B,0x11D3,{0x8F,0x39,0x00,0xC0,0x4F,0x36,0x51,0xB8}} */

#pragma code_seg(".orpc")
static const unsigned short IRTDUpdateEvent_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    30,
    66,
    102
    };

static const MIDL_STUBLESS_PROXY_INFO IRTDUpdateEvent_ProxyInfo =
    {
    &Object_StubDesc,
    IRTDServer__MIDL_ProcFormatString.Format,
    &IRTDUpdateEvent_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IRTDUpdateEvent_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    IRTDServer__MIDL_ProcFormatString.Format,
    &IRTDUpdateEvent_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _IRTDUpdateEventProxyVtbl = 
{
    &IRTDUpdateEvent_ProxyInfo,
    &IID_IRTDUpdateEvent,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IRTDUpdateEvent::UpdateNotify */ ,
    (void *) (INT_PTR) -1 /* IRTDUpdateEvent::get_HeartbeatInterval */ ,
    (void *) (INT_PTR) -1 /* IRTDUpdateEvent::put_HeartbeatInterval */ ,
    (void *) (INT_PTR) -1 /* IRTDUpdateEvent::Disconnect */
};


static const PRPC_STUB_FUNCTION IRTDUpdateEvent_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IRTDUpdateEventStubVtbl =
{
    &IID_IRTDUpdateEvent,
    &IRTDUpdateEvent_ServerInfo,
    11,
    &IRTDUpdateEvent_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IRtdServer, ver. 0.0,
   GUID={0xEC0E6191,0xDB51,0x11D3,{0x8F,0x3E,0x00,0xC0,0x4F,0x36,0x51,0xB8}} */

#pragma code_seg(".orpc")
static const unsigned short IRtdServer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    132,
    174,
    228,
    270,
    306,
    342
    };

static const MIDL_STUBLESS_PROXY_INFO IRtdServer_ProxyInfo =
    {
    &Object_StubDesc,
    IRTDServer__MIDL_ProcFormatString.Format,
    &IRtdServer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IRtdServer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    IRTDServer__MIDL_ProcFormatString.Format,
    &IRtdServer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IRtdServerProxyVtbl = 
{
    &IRtdServer_ProxyInfo,
    &IID_IRtdServer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IRtdServer::ServerStart */ ,
    (void *) (INT_PTR) -1 /* IRtdServer::ConnectData */ ,
    (void *) (INT_PTR) -1 /* IRtdServer::RefreshData */ ,
    (void *) (INT_PTR) -1 /* IRtdServer::DisconnectData */ ,
    (void *) (INT_PTR) -1 /* IRtdServer::Heartbeat */ ,
    (void *) (INT_PTR) -1 /* IRtdServer::ServerTerminate */
};


static const PRPC_STUB_FUNCTION IRtdServer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IRtdServerStubVtbl =
{
    &IID_IRtdServer,
    &IRtdServer_ServerInfo,
    13,
    &IRtdServer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    IRTDServer__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x70001f4, /* MIDL Version 7.0.500 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * _IRTDServer_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IRtdServerProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IRTDUpdateEventProxyVtbl,
    0
};

const CInterfaceStubVtbl * _IRTDServer_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IRtdServerStubVtbl,
    ( CInterfaceStubVtbl *) &_IRTDUpdateEventStubVtbl,
    0
};

PCInterfaceName const _IRTDServer_InterfaceNamesList[] = 
{
    "IRtdServer",
    "IRTDUpdateEvent",
    0
};

const IID *  _IRTDServer_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _IRTDServer_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _IRTDServer, pIID, n)

int __stdcall _IRTDServer_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _IRTDServer, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _IRTDServer, 2, *pIndex )
    
}

const ExtendedProxyFileInfo IRTDServer_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _IRTDServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _IRTDServer_StubVtblList,
    (const PCInterfaceName * ) & _IRTDServer_InterfaceNamesList,
    (const IID ** ) & _IRTDServer_BaseIIDList,
    & _IRTDServer_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

