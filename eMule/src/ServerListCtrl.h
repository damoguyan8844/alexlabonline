#pragma once
#include "MuleListCtrl.h"

class CServerList; 
class CServer;
class CToolTipCtrlX;

class CServerListCtrl : public CMuleListCtrl 
{
	DECLARE_DYNAMIC(CServerListCtrl)
public:
	void	RefreshAllServer();//EastShare - added by AndCycle, IP to Country
	CServerListCtrl();
	virtual ~CServerListCtrl();

	bool	Init();
	bool	AddServer(const CServer* pServer, bool bAddToList = true);
	void	RemoveServer(const CServer* pServer);
	bool	AddServerMetToList(const CString& strFile);
	void	RefreshServer(const CServer* pServer);
	void	RemoveAllDeadServers();
	void	RemoveAllFilteredServers();
	void	Hide()		{ ShowWindow(SW_HIDE); }
	void	Visable()	{ ShowWindow(SW_SHOW); }
	void	Localize();
	void	ShowServerCount();
	bool	StaticServerFileAppend(CServer* pServer);
	bool	StaticServerFileRemove(CServer* pServer);

protected:
	CToolTipCtrlX*	m_tooltip;

	CString CreateSelectedServersURLs();
	void DeleteSelectedServers();

	void SetSelectedServersPriority(UINT uPriority);
	void SetAllIcons();
	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); //�� IP to Country �й�

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSysColorChange();
	afx_msg	void OnColumnClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNMDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
private:
	//Chocobo Start
	//server icon related
	//������ͼ�����,  added by Chocobo
 	CImageList imagelist;
	//Chocobo End
};
