#pragma once

#include "HSTPN_SYSDoc.h"
#include "afxwin.h"
#include "afxcmn.h"
class CHSTPN_SYSDoc;
// CDlgChseOVlueTFile �Ի���

class CDlgChseOVlueTFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChseOVlueTFile)

public:
	CDlgChseOVlueTFile(CWnd* pParent = NULL);   // ��׼���캯��
	CDlgChseOVlueTFile(CHSTPN_SYSDoc* pdoc, CWnd* pParent = NULL);
	virtual ~CDlgChseOVlueTFile();

// �Ի�������
	enum { IDD = IDD_DLGTOFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
public:
	//----------��Ա����---------------
	CHSTPN_SYSDoc      * m_pDoc;
	CComboBox            m_cmbPlace;       // combox �ؼ������������ṩ�û�ѡ�������caption
	HTREEITEM            hRoot;            // ���ĸ��ڵ�ľ��
	vector<int>          vec_OutValue;     // ���ڱ���ѡ�е����ֵ��arryIOputData�е�λ��
	CString              OutPName;         // ���ڱ���ѡ�е��������������
	vector<int>          vec_subMap;       // ����Ҫ����ı�����arryIOputData�е�λ��
	int                  i_PposnOValue;    // ���ڱ���ѡ�еĿ�������vector_Places�е�λ��
	CTreeCtrl            m_treePOutValue;  // ������������������б�
	CEdit                m_edtFilePath;    // �༭���������ȡ����ʾҪ�����ļ����ļ�����·��

	CString               str_FilePath;     // �������ļ���·��


public:
	//----------��Ա����---------------

	
	
	virtual BOOL OnInitDialog();
	afx_msg void OnCmbSelchange_Chooseplace();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditFilepath();
	afx_msg void OnBnClicked_Chosefilepath();
	
	
};
