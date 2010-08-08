// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__057F4432_924A_40CC_8A09_9FD1FFFB8416__INCLUDED_)
#define AFX_STDAFX_H__057F4432_924A_40CC_8A09_9FD1FFFB8416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#include <afxmt.h>
#include	<math.h>
#include	<io.h>

#include "AfxCore.h"

#include	"SpPlat.h"
#include	"SpDefs.h"
#include	"SpAssert.h"
#include	"SpObject.h"
#include	"SpString.h"
#include	"SpTime.h"
#include	"SpFile.h"
#include	"SpArchive.h"
#include	"SpColl.h"
#include	"SpDebug.h"
#include	"SpMemory.h"
#include	"Strings.h"
#include "Strategy.h"

#include "CommFuns.h"
#include "Stock.h"
#include "Packets.h"
#include "Tongshi.h"
#include "StkReceiver.h"
#include "StockGraph.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_STDAFX_H__057F4432_924A_40CC_8A09_9FD1FFFB8416__INCLUDED_)
