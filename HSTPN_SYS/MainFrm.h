
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "HSTPN_SYSView.h"
#include "ViewModelChoose.h"

#include "SimToolBar.h"
#include "afxcmn.h"
#include "DlgModelParmList.h"
#include "DlgMove.h"

class CDlgModelParmList;
class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar        m_wndMenuBar;
	CMFCToolBar        m_wndToolBar;
	
	CMFCToolBarImages  m_UserImages;
	CSimToolBar        m_simToolBar;
public:
	CMFCStatusBar      m_wndStatusBar;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void    OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void    OnApplicationLook(UINT id);
	afx_msg void    OnUpdateApplicationLook(CCmdUI* pCmdUI);
	virtual BOOL    OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	DECLARE_MESSAGE_MAP()

public:
	CSplitterWnd m_splitterWnd1;//��ִ���

	afx_msg LRESULT OnCreateModel1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMoveModel1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawLineend1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDdelete1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateHSTPNTree1(WPARAM wParam, LPARAM lParam);

	CMFCToolBarEditBoxButton  *m_Text_TimeStep;
	CMFCToolBarEditBoxButton  *m_Edit_TimeStep;

	//afx_msg void OnClickEditTimeStep();
	afx_msg void OnGetTimeStep1();
	
	CMFCToolBarEditBoxButton  *m_Text_SimTime;
	CMFCToolBarEditBoxButton  *m_Edit_SimTime;

	//afx_msg void OnClickEditSimTime();
	afx_msg void OnGetSimTime1();

	CMFCToolBarEditBoxButton  *m_Text_IntervalTime;
	CMFCToolBarEditBoxButton  *m_Edit_IntervalTime;

	//afx_msg void OnClickEditIntervalTime();
	afx_msg void OnGetIntervalTime1();
	// ��ʾ������ȵĽ�����

	
	CProgressCtrl m_progress;// ��ʾ������ȵĽ�����
//	afx_msg void OnPaint();
	afx_msg LRESULT OnUpdateProgess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowpramlist();

	CDlgModelParmList *  m_pParmListDlg;// ģ�����Ա༭�Ի���

	afx_msg void OnSimMove();
	//CWinThread * m_moveThread;//�����߳�
};


