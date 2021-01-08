#pragma once
#include "afxcmn.h"

#include "HSTPN_Message.h"

// CDlgModelChoose ������ͼ

class CDlgModelChoose : public CFormView
{
	DECLARE_DYNCREATE(CDlgModelChoose)

protected:
	CDlgModelChoose();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	// ģ��ѡ�����ṹ�ؼ�����
	CTreeCtrl m_mdchoseTree;
	CImageList m_imageList;
    virtual void OnInitialUpdate();

	bool b_isInitTree; // ��֪��ΪʲôOnInitialUpdate()�ᱻִ�����Σ����ṹҲ�ᱻ��ʼ�����Σ��øñ���������ֻ��ʼ��һ��
	afx_msg void OnNMClickMdchosetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


