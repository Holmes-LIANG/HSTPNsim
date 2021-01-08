#pragma once

#include "HSTPN_SYSDoc.h"
#include "afxwin.h"
#include "afxcmn.h"
class CHSTPN_SYSDoc;
// CDlgChooseOValue �Ի���

class CDlgChooseOValue : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChooseOValue)

public:
	CDlgChooseOValue(CWnd* pParent = NULL);   // ��׼���캯��
	CDlgChooseOValue(CHSTPN_SYSDoc* pdoc, CWnd* pParent = NULL);
	virtual ~CDlgChooseOValue();

// �Ի�������
	enum { IDD = IDD_DLGCHOOSEVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------��Ա����----------------
	CHSTPN_SYSDoc     * m_pDoc;
	CComboBox           m_cmbPlace;   // combox �ؼ������������ṩ�û�ѡ�������caption
	HTREEITEM           hRoot;        // ���ĸ��ڵ�ľ��  
	vector<int>         vec_OutValue; // ���ڱ���ѡ�е����ֵ��arryIOputData�е�λ��
	CString             OutPName;     // ���ڱ���ѡ�е��������������
	vector<int>         vec_subMap;   // ����Ҫ����ı�����arryIOputData�е�λ��
	int                 i_PposnOValue; // ���ڱ���ѡ�еĿ�������vector_Places�е�λ��
public:
	//------------��Ա����---------------
	virtual BOOL OnInitDialog();
	
	CTreeCtrl m_treePOutValue;       // ������������������б�

	afx_msg void OnCmbSelchange_Chooseplace();
	afx_msg void OnChoosevalue_TreeClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
