#pragma once
#include "afxwin.h"

#include "DlgModelChoose1.h"
#include "DlgModelChoose.h"
class CDlgModelChoose1;

// CViewModelChoose ������ͼ

class CViewModelChoose : public CFormView
{
	DECLARE_DYNCREATE(CViewModelChoose)

protected:
	CViewModelChoose();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CDlgModelChoose  *m_dlgmodelchoose; // �Ի������;���û�ѡ�����Ԫ�ء����ͽṹ���������̶Ի���
	CDlgModelChoose1 *m_dlgmodelchoose1;// �Ի���������û�ѡ�����Ԫ��

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	// HSTPN����Ԫ��ѡ��Ի�����ʾ��ť����
//	CStatic btn_Dlgchos1_Fdmtlelemt;
	// HSTPN���ͽṹѡ��Ի���-ѡ��ť����
//	CStatic btn_Dlgchoose2_typclstruc;
	// ĳЩ����ģ��ѡ��Ի���-ѡ��ť����
//	CStatic btn_Dlgchoose3_phyclmodel;
	
};


