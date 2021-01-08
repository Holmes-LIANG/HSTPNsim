
// MainFrm.h : CMainFrame 类的接口
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
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar        m_wndMenuBar;
	CMFCToolBar        m_wndToolBar;
	
	CMFCToolBarImages  m_UserImages;
	CSimToolBar        m_simToolBar;
public:
	CMFCStatusBar      m_wndStatusBar;
// 生成的消息映射函数
protected:
	afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void    OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void    OnApplicationLook(UINT id);
	afx_msg void    OnUpdateApplicationLook(CCmdUI* pCmdUI);
	virtual BOOL    OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	DECLARE_MESSAGE_MAP()

public:
	CSplitterWnd m_splitterWnd1;//拆分窗口

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
	// 显示仿真进度的进度条

	
	CProgressCtrl m_progress;// 显示仿真进度的进度条
//	afx_msg void OnPaint();
	afx_msg LRESULT OnUpdateProgess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowpramlist();

	CDlgModelParmList *  m_pParmListDlg;// 模型属性编辑对话框

	afx_msg void OnSimMove();
	//CWinThread * m_moveThread;//动画线程
};


