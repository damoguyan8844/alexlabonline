#pragma once
#include "resource.h"
#include "ResizableLib\ResizableDialog.h"
#include "SearchParams.h"
#include "TabWnd.h"

// CDlgMainTabSearch 对话框

class CDlgMainTabSearch : public CResizableDialog
{
	DECLARE_DYNAMIC(CDlgMainTabSearch)

public:
	CDlgMainTabSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMainTabSearch();

// 对话框数据
	enum { IDD = IDD_MAINTAB_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnNMActiveTabChanged(NMHDR* pNMHDR, LRESULT *pResult);
	afx_msg void OnNMTabDestroy(NMHDR* pNMHDR, LRESULT *pResult);
	afx_msg void OnNMTabCreate(NMHDR* pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnEMTabDestroy(WPARAM wParam, LPARAM lParam);

public:

	CTabWnd			m_tabWnd;
	DWORD           m_dwCounter;
	DWORD			m_dwTotalCount;
	CMap <int, int, SSearchParams*, SSearchParams*> m_SearchMap;

	void Localize();
	void ShowEmuleSearch();
	void OpenNewSearchResult(LPCTSTR lpszCaption,ESearchType * pSearchType = NULL);
	void CreateNewSearch(LPCTSTR lpszCaption,ESearchType * pSearchType = NULL);
	void UpdateSearchParam(int iIndex, SSearchParams *pSearchParams);
	void UpdateEMsClosableStatus();
	void OnMainTabChanged();
};
