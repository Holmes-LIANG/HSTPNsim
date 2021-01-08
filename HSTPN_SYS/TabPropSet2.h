#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "HSTPN_Message.h"
//��������ͷ�ļ���std�����ռ�
#include <vector>
using namespace std;

// CTabPropSet2 �Ի���
#include "Place.h"
#include "HSTPN_SYSDoc.h"
class CTabPropSet2 : public CDialog
{
	DECLARE_DYNAMIC(CTabPropSet2)

public:
	CTabPropSet2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPropSet2();

// �Ի�������
	enum { IDD = IDD_PROPSET_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHSTPN_SYSDoc *m_pDoc;
	//===========��Ա����=====================================================
	enumPlaceStyle      PlaceStyle; // ��ǰ����������

	CListCtrl           m_List_IOput;           // ģ�͵��������ֵ�б�ؼ�����
	CEdit               m_edit_getIOName;       // ����/��������Ļ�ȡ�༭��Ŀؼ�����
	CComboBox           m_cmb_getIOType;        // ����/������������ͻ�ȡ��Combo��Ŀؼ�����
	CComboBox           m_cmb_IorO;             // ���ñ����ǡ����롱���ǡ���������������õ�Combo��Ŀؼ�����
	CComboBox           m_cmb_getIOPort;        // ��ȡ����/���ֵ�Ĵ���˿ں�
	CComboBox           m_cmb_getIOFrom;        // ��ȡ����/���ֵ�Ǵ��ĸ�ģ�ʹ��ݹ�����Comob��Ŀؼ�����
	CComboBox           m_cmb_getIOFromport;    // ��ȡ���ݹ���ģ�͵Ĵ��ݶ˿ڵ�Comob��Ŀؼ�����

	int                 m_row;                  // m_rowΪm_List_IOput���к�
    int                 m_column;               // m_columnΪm_List_IOput���к�

	int                 m_rowforD;              // m_rowΪm_List_IOput���к�,ɾ������ʱ�õ�
    int                 m_columnforD;           // m_columnΪm_List_IOput���к�,ɾ������ʱ�õ�

	vector<IOValue*>    arryIOputData;          // ģ������/���ֵ�ļ���

	CString             str_IOFromcaption;      // ģ�����������Դģ�͵�caption
	CString             m_editvalue_IOname;     // �������������name--�ؼ�����

	CPlace *            p_Model;                // ��ʽ�ı����ӦModel�����ָ��

public:
	//============��Ա����==========================================================
	virtual BOOL Create(UINT nIDTemplate, enumPlaceStyle placestyle, CPlace* pModel,  CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();

	afx_msg void OnDblclk_ListIoput(NMHDR *pNMHDR, LRESULT *pResult); // Double Clicked -- to get the row and column
	afx_msg void OnClick_ListIoput(NMHDR *pNMHDR, LRESULT *pResult);  // Single Clicked -- to set the name of value
	afx_msg void OnCmbCSelchange_Getiotype();                         // to get the type of value;
	afx_msg void OnCmbSelchange_Getioro();                            // to get the In or Out of value
	afx_msg void OnCmbSelchange_Getioport();
	afx_msg void OnCmbSelchange_Getiofrom();
	afx_msg void OnCmbSelchng_Getiofromport();

	
	afx_msg void OnPaint();

	void         InitCtrl_GetIO(int rowofchart, int columnofchart, CRect rc); //��ؿؼ��ĳ�ʼ��

	void         SetCtrlStatus(UINT status);
	afx_msg void OnBtnaAddValue();
	afx_msg void OnBtnDltValue();
	afx_msg void OnLvnItemchangedListIoput(NMHDR *pNMHDR, LRESULT *pResult);
};
