#pragma once

#include "HSTPN_Message.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "HSTPN_SYSDoc.h"
class CHSTPN_SYSDoc;
// CDlgModelChoose1 ������ͼ

class CDlgModelChoose1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgModelChoose1)

protected:
	CDlgModelChoose1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgModelChoose1();

public:
	enum { IDD = IDD_DLGMODELCHOOSE1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHSTPN_SYSDoc * m_pDoc;
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);


	afx_msg void OnPaint();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	CTreeCtrl m_HSTPNStrucTree;// HSTPN����ṹ��
	CImageList m_imageList;
	bool b_isInitTree;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnUpdateHSTPNTree2(WPARAM wParam, LPARAM lParam);
	HTREEITEM hRoot;                                                             // ���ĸ��ڵ�ľ�� 
};


