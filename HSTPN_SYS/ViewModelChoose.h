#pragma once
#include "afxwin.h"

#include "DlgModelChoose1.h"
#include "DlgModelChoose.h"
class CDlgModelChoose1;

// CViewModelChoose 窗体视图

class CViewModelChoose : public CFormView
{
	DECLARE_DYNCREATE(CViewModelChoose)

protected:
	CViewModelChoose();           // 动态创建所使用的受保护的构造函数
	virtual ~CViewModelChoose();

public:
	enum { IDD = IDD_WNDMODELCHOOSE };
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

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CDlgModelChoose  *m_dlgmodelchoose; // 对话框对象;让用户选择基本元素、典型结构、连续过程对话框
	CDlgModelChoose1 *m_dlgmodelchoose1;// 对话框对象：让用户选择基本元素

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	// HSTPN基本元素选择对话框显示按钮变量
//	CStatic btn_Dlgchos1_Fdmtlelemt;
	// HSTPN典型结构选择对话框-选择按钮变量
//	CStatic btn_Dlgchoose2_typclstruc;
	// 某些物理模型选择对话框-选择按钮变量
//	CStatic btn_Dlgchoose3_phyclmodel;
	
};


