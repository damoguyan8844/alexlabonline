// WebBrowserEdit.h: interface for the CWebBrowserEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBBROWSEREDIT_H__C8FC4582_0669_4BED_86B8_7E039DD86E74__INCLUDED_)
#define AFX_WEBBROWSEREDIT_H__C8FC4582_0669_4BED_86B8_7E039DD86E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../WebBrowser2.H"
#include <mshtml.h>
#include <MsHtmcid.h>
#include <MsHtmHst.h>

/////////////////////////////////////////////////////////////////////////////
// CHtmlEditCtrl2 contol based on MFC70 CHtmlEditCtrlBase
/////////////////////////////////////////////////////////////////////////////

class CHtmlEditCtrl2  : public CWebBrowser2
{
public:
	CHtmlEditCtrl2();
	~CHtmlEditCtrl2();

	BOOL SetDesignMode(BOOL bMode);
	HRESULT ExecCommand(const GUID *pGuid, long cmdID, long cmdExecOpt, VARIANT* pInVar=NULL, VARIANT* pOutVar=NULL);
	HRESULT ExecCommand(long cmdID, long cmdExecOpt, VARIANT* pInVar=NULL, VARIANT* pOutVar=NULL);
	long QueryStatus(long cmdID);
	HRESULT GetEvent(IHTMLEventObj **ppEventObj);
	HRESULT GetEventSrcElement(IHTMLElement **ppSrcElement);
	HRESULT GetDocument(IHTMLDocument2** ppDoc);
	HRESULT NewDocument();
	HRESULT GetDocumentHTML(CString& szHTML, BOOL a_bClearDirtyFlag = FALSE);
	HRESULT SetDocumentHTML(LPCTSTR szHTML);
	HRESULT GetIsDirty();//returns S_OK when is dirty and S_FALSE when not dirty
	HRESULT GetDocumentTitle(CString& szTitle);
	HRESULT GetBlockFormatNames(CStringArray &sa);
	HRESULT SetForeColor(LPCTSTR szColor);
	HRESULT SetForeColor(int nColor);
	HRESULT GetForeColor(int &nColor);
	HRESULT GetBackColor(int& nColor);
	HRESULT SetBackColor(LPCTSTR szColor);
	HRESULT SetBackColor(int nColor);

	HRESULT SetDefaultComposeSettings(LPCTSTR szFontName=NULL,
									  unsigned short nFontSize=3,
									  COLORREF crFontColor=0xFF000000,
									  COLORREF crFontBgColor=0xFF000000,
									  bool bBold = false,
									  bool bItalic = false,
									  bool bUnderline = false);

	HRESULT GetBlockFormat(CString& strFormat);
	HRESULT SetBlockFormat(LPCTSTR szFormat);
	HRESULT GetFontFace(CString& strFace);
	HRESULT SetFontFace(LPCTSTR szFace);
	HRESULT IE50Paste(LPCTSTR szData);
	HRESULT GetBookMark(CString& strAnchor);
	HRESULT SetBookMark(LPCTSTR szAnchorName);
	HRESULT SetOverwriteMode(bool bMode);
	HRESULT Is1DElement(bool& bValue);
	HRESULT Is2DElement(bool& bValue);
	HRESULT GetFontSize(short& nSize);
	HRESULT SetFontSize(unsigned short size);
	HRESULT GetFrameZone(short& nZone);
	HRESULT SetCSSEditingLevel(short nLevel);
	HRESULT HyperLink(LPCTSTR szUrl = NULL);
	HRESULT Image(LPCTSTR szUrl = NULL);
	HRESULT OrderList(LPCTSTR szId = NULL);
	HRESULT UnorderList(LPCTSTR szId = NULL);

	HRESULT AddToGlyphTable(LPCTSTR szTag,
							LPCTSTR szImgUrl,
							unsigned short nTagType,
							unsigned short nAlignment,
							unsigned short nPosInfo,
							unsigned short nDirection,
							unsigned int nImgWidth,
							unsigned int nImgHeight);

	HRESULT EmptyGlyphTable();
	HRESULT Button(LPCTSTR szId = NULL);
	HRESULT CheckBox(LPCTSTR szId = NULL);
	HRESULT DropDownBox(LPCTSTR szId = NULL);
	HRESULT HorizontalLine(LPCTSTR szId = NULL);
	HRESULT Iframe(LPCTSTR szId = NULL);
	HRESULT InsFieldSet(LPCTSTR szId = NULL);
	HRESULT InsInputButton(LPCTSTR szId = NULL);
	HRESULT InsInputHidden(LPCTSTR szId = NULL);
	HRESULT InsInputImage(LPCTSTR szId = NULL);
	HRESULT InsInputPassword(LPCTSTR szId = NULL);
	HRESULT InsInputReset(LPCTSTR szId = NULL);
	HRESULT InsInputSubmit(LPCTSTR szId = NULL);
	HRESULT InsInputUpload(LPCTSTR szId = NULL);
	HRESULT ListBox(LPCTSTR szId = NULL);
	HRESULT Marquee(LPCTSTR szId = NULL);
	HRESULT Paragraph(LPCTSTR szId = NULL);
	HRESULT RadioButton(LPCTSTR szId = NULL);
	HRESULT SaveAs(LPCTSTR szPath = NULL);
	HRESULT TextArea(LPCTSTR szId = NULL);
	HRESULT TextBox(LPCTSTR szId = NULL);
	HRESULT GetAbsolutePosition(bool &bCurValue);
	HRESULT SetAbsolutePosition(bool bNewValue);
	HRESULT Set2DPosition(bool bNewValue);
	HRESULT SetAtomicSelection(bool bNewValue);
	HRESULT SetAutoURLDetectMode(bool bNewValue);
	HRESULT SetDisableEditFocusUI(bool bNewValue);
	HRESULT SetIE5PasteMode(bool bNewValue);
	HRESULT SetLiveResize(bool bNewValue);
	HRESULT SetMultiSelect(bool bNewValue);
	HRESULT SetOverrideCursor(bool bNewValue);
	HRESULT SetRespectVisInDesign(bool bNewValue);
	HRESULT GetShowAlignedSiteTags(bool &bCurValue);
	HRESULT SetShowAlignedSiteTags(bool bNewValue);
	HRESULT GetShowAllTags(bool &bCurValue);
	HRESULT SetShowAllTags(bool bNewValue);
	HRESULT GetShowAreaTags(bool &bCurValue);
	HRESULT SetShowAreaTags(bool bNewValue);
	HRESULT GetShowCommentTags(bool &bCurValue);
	HRESULT SetShowCommentTags(bool bNewValue);
	HRESULT GetShowMiscTags(bool &bCurValue);
	HRESULT SetShowMiscTags(bool bNewValue);
	HRESULT GetShowScriptTags(bool &bCurValue);
	HRESULT SetShowScriptTags(bool bNewValue);
	HRESULT GetShowStyleTags(bool &bCurValue);
	HRESULT SetShowStyleTags(bool bNewValue);
	HRESULT GetShowUnknownTags(bool &bCurValue);
	HRESULT SetShowUnknownTags(bool bNewValue);
	HRESULT GetShowBRTags(bool &bCurValue);
	HRESULT SetShowBRTags(bool bNewValue);
	HRESULT PrintDocument();
	HRESULT PrintDocument(LPCTSTR szPrintTemplate);
	HRESULT PrintDocument(bool bShowPrintDialog);
	HRESULT PrintPreview();
	HRESULT PrintPreview(LPCTSTR szPrintTemplate);
	HRESULT Bold();
	HRESULT Copy();
	HRESULT Cut();
	HRESULT Delete();
	HRESULT Indent();
	HRESULT Italic();
	HRESULT JustifyCenter();
	HRESULT JustifyLeft();
	HRESULT JustifyRight();
	HRESULT Outdent();
	HRESULT Paste();
	HRESULT RemoveFormat();
	HRESULT SelectAll();
	HRESULT Underline();
	HRESULT Unlink();
	HRESULT ClearSelection();
	HRESULT Font();
	HRESULT RefreshDocument();
	HRESULT UnBookmark();

	//IZ: Added by Irek Zielinski: //////////////////////////////////////////////////////
	BOOL IsBold();
	BOOL IsUnderline();
	BOOL IsStrikeOut();
	BOOL IsItalic();
	BOOL CanPaste();
	
	enum MoveType
	{
		moveToDocStart,
		moveToDocEnd,
		moveFromCurrentPos
	};

	HRESULT MoveCursorPosition(MoveType a_eType, int a_iCharacters = 0);
	HRESULT LineBreakNormal();
	BOOL	IsDesignMode();
	HRESULT StrikeOut();
	HRESULT GetURLsOfAllImages(CStringArray& a_arrImages);
	HRESULT ReplaceImageURL(const CString& a_sUrlToReplace, const CString& a_sUrlToReplaceWith);
	HRESULT GetDocumentBody(CString& a_sBody, BOOL a_bTextInsteadHTML);
	HRESULT Undo();
	HRESULT Redo();
	HRESULT Find();
	HRESULT SubScriptSelectedText();
	HRESULT SuperScriptSelectedText();
	HRESULT SetDocumentCharset(const CString& a_sCharsetEncoding);
	HRESULT GetDocumentCharset(CString& a_sCharsetEncoding);
	HRESULT ShowSource();
	HRESULT ShowIEOptionsDialog();

	HRESULT GetBodyBackgroundImage(CString& a_sImage);
	HRESULT SetBodyBackgroundImage(const CString& a_sImage);
	HRESULT GetBodyProperties(CString& a_sTag);
	HRESULT GetBodyBackgroundColor(CString& a_sColor);
	HRESULT GetBodyTextColor(CString& a_sColor);
	HRESULT GetBodyBackgroundCSSText(CString& a_sStyleText);
	HRESULT PasteHTMLAtCurrentSelection(const CString& a_sHTMLText, BOOL a_bSetCursorAtBeginingOfInsertedText);

	IHTMLDocument2*		m_pHtmlDoc;
protected:
	HRESULT				m_hResult;
	IDispatch*			m_pDispatch;
	IOleCommandTarget*	m_pCmdTarg;
	
	HRESULT GetBodyColorParameters(CString& a_sColor, BOOL a_bTextColor);
	BOOL    QueryDocumentState(BSTR a_bstrItemToQuery);
	BOOL    PrepareInterfaces();

	HRESULT CopyEx(BOOL a_bCutText);
	static HRESULT PutHtmlAndTextToClipboard(const CString& a_sHtmlText, const CString& a_sPlainText);
	//IZ: End of: Added by Irek Zielinski: /////////////////////////////////////////////

protected:
	// Implementation

	HRESULT ExecHelperNN(UINT nID,
						 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
						 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperSetVal(UINT nCmdID, 
							 LPCTSTR szID=NULL,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperSetVal(UINT nCmdID,
							 bool bValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperSetVal(UINT nCmdID,
							 short nNewVal,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperSetVal(UINT nCmdID,
							 int nNewVal,
							 long nMinSupportLevel = OLECMDF_SUPPORTED|OLECMDF_ENABLED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
							 bool &bValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
							 short &nValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
							 int &nValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
							 CString& strValue,
							 long nMinSupportLevel = OLECMDF_SUPPORTED,
							 long nExecOpt = OLECMDEXECOPT_DODEFAULT);


//IZ conversion code taken from: http://www.flounder.com/
public:
	static void BSTR2CString(BSTR b, CString& s)
	{
		s = _T("");

		if(b == NULL)
			return; // empty for NULL BSTR

	#ifdef UNICODE
		s = b;
	#else
		LPSTR p = s.GetBuffer(SysStringLen(b) + 1); 
		::WideCharToMultiByte(CP_ACP,            // ANSI Code Page
			0,                 // no flags
			b,                 // source widechar string
			-1,                // assume NUL-terminated
			p,                 // target buffer
			SysStringLen(b)+1, // target buffer length
			NULL,              // use system default char
			NULL);             // don't care if default used
		s.ReleaseBuffer();
	#endif
	}
protected:

	// CStreamOnCString
	// This is a helper class used by the CHtmlEditCtrlBase class to stream
	// HTML into and out of the web browser control.

	/*
	IZ:
	This clas is taken from MFC70 and changed to compile under VC++ 6.0
	All changes are marked by IZ tags.
	UNICODE support is not solved
	*/

	class CStreamOnCString : public IStream
	{
	public:
		CStreamOnCString();
		CStreamOnCString(LPCTSTR szData);

		BOOL CopyData(CString& target);
		BOOL SetData(LPCTSTR szData);
		STDMETHOD(QueryInterface)(REFIID iid, void **ppUnk);
		ULONG STDMETHODCALLTYPE AddRef( void) ;
		ULONG STDMETHODCALLTYPE Release( void );
		STDMETHOD(Read)(void *pv, ULONG cb, ULONG *pcbRead);
		STDMETHOD(Write)(const void *pv, ULONG cb, ULONG *pcbWritten);
		STDMETHOD(Seek)(LARGE_INTEGER , DWORD , ULARGE_INTEGER *);
		STDMETHOD(SetSize)(ULARGE_INTEGER );
		STDMETHOD(CopyTo)(IStream *, ULARGE_INTEGER , ULARGE_INTEGER *,ULARGE_INTEGER *);
		STDMETHOD(Commit)(DWORD );
		STDMETHOD(Revert)( void );
		STDMETHOD(LockRegion)(ULARGE_INTEGER , ULARGE_INTEGER , DWORD );
		STDMETHOD(UnlockRegion)(ULARGE_INTEGER , ULARGE_INTEGER , DWORD );
		STDMETHOD(Stat)(STATSTG *, DWORD );
		STDMETHOD(Clone)(IStream **);

	protected:
		CString m_strStream;
		CString m_strAnsi; //IZ replaced CStringA with CString (not sure if it will work in unicode right)
		UINT m_current_index;

		//IZ: added to simulate Append method of CString from MFC70
		void AppendToCString(CString& a_sString, LPCTSTR a_pstrToAdd, UINT a_uiLength);

	};//CStreamOnCString

}; //CHtmlEditCtrl2





#endif // !defined(AFX_WEBBROWSEREDIT_H__C8FC4582_0669_4BED_86B8_7E039DD86E74__INCLUDED_)
