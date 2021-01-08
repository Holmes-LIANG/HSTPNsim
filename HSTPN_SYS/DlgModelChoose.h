#pragma once
#include "afxcmn.h"

#include "HSTPN_Message.h"

// CDlgModelChoose 窗体视图

class CDlgModelChoose : public CFormView
{
	DECLARE_DYNCREATE(CDlgModelChoose)

protected:
	CDlgModelChoose();           // 动态创建所使用的受保护的构造函数
	virtual ~CDlgModelChoose();

public:
	enum { IDD = IDD_MODELCHOOSE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	// 模型选择树结构控件变量
	CTreeCtrl m_mdchoseTree;
	CImageList m_imageList;
    virtual void OnInitialUpdate();

	bool b_isInitTree; // 不知道为什么OnInitialUpdate()会被执行两次，树结构也会被初始化两次，用该变量控制其只初始化一次
	afx_msg void OnNMClickMdchosetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


