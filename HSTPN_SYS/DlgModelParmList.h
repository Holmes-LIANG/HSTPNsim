#pragma once
#include "afxcmn.h"
#include "HSTPN_Message.h"
#include "Place.h"
#include "Transition.h"
#include "afxwin.h"
// CDlgModelParmList �Ի���

#include "HSTPN_SYSDoc.h"
class CHSTPN_SYSDoc;

class CDlgModelParmList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModelParmList)

public:
	CDlgModelParmList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModelParmList();

// �Ի�������
	enum { IDD = IDD_DLG_DATALIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	CTreeCtrl           m_treeListofModel;      // �����۳����еĿ�����Ŀ�����������еı����б�
	bool                b_isInitTree;
	HTREEITEM           hRoot;                  // ���ĸ��ڵ�ľ��

	CListCtrl           m_DataListAll;          // ��������������ȫ�ֵı���,ȫ�ֱ�����
	CImageList          m_imageList;
	CEdit               m_edtvaluename1;        // ȫ�ֱ����������ؼ�����
	CString             m_strvaluename1;        // ȫ�ֱ���������str
	CEdit               m_edtValueInt1;         // ȫ�ֱ����ĳ�ʼֵ�༭�����
	double              m_dblValueInt;          // ȫ�ֱ����ĳ�ʼֵ
	//vector<IOValue*>    arryIOputDataG;         // ȫ�ֱ�����
	int                 m_rowforDG;             // m_rowΪm_List_IOput���к�,ɾ������ʱ�õ�
    int                 m_columnforDG;          // m_columnΪm_List_IOput���к�,ɾ������ʱ�õ�
	int                 m_rowG;                 // m_rowΪm_List_IOput���к�
    int                 m_columnG;              // m_columnΪm_List_IOput���к�

	afx_msg void        OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT     OnUpdateTreeofModel(WPARAM wParam, LPARAM lParam);
	virtual BOOL        OnInitDialog();
	afx_msg void        OnBtnaAddValueG();          // ȫ�ֱ��£��������ӡ���ť
	afx_msg void        OnBtnDltValueG();           // ȫ�ֱ��£������ɾ������ť
	afx_msg void        OnClick_ListG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void        OnDblclk_ListG(NMHDR *pNMHDR, LRESULT *pResult);
	void                InitCtrl_GetIO(int rowofchart, int columnofchart, CRect rc);
	afx_msg void        OnGetValueNameG();
	afx_msg void        OnGetValueIntG();

	afx_msg void        OnNMClick_ChoseNode(NMHDR *pNMHDR, LRESULT *pResult);


	CStatic             m_staticG;              // ȫ�ֱ���������ֱ�ͷ
	CStatic             m_staticL;              // �ֲ�����������ֱ�ͷ
	CButton             m_btnAddG;              // ȫ�ֱ������µ���Ӱ�ť
	CButton             m_btnDeleteG;           // ȫ�ֱ������µ�ɾ����ť
	CButton             m_btnAddL;              // �ֲ��������µ���Ӱ�ť
	CButton             m_btnDeleteL;           // �ֲ��������µ�ɾ����ť

	CPlace *            p_Model;                // ��ʽ�ı����ӦModel�����ָ��

	afx_msg void        OnNMClick_ListLocal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void        OnNMDblclk_ListLocal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void        OnBtnaAddValueL();
	afx_msg void        OnBtnDltValueL();

	CListCtrl           m_ListLocal;            // �ֲ�������ؼ�
	CEdit               m_edtValueNameL;        // �ֲ��������оֲ��������༭��ؼ�
	CComboBox           m_cmbSquenceL;          // �ֲ�����������������б��ؼ�
	CComboBox           m_cmbIOtypeL;           // �ֲ���������IOTypeѡ�������б��ؼ�
	CComboBox           m_cmbIsQuoteG_L;        // �ֲ����������Ƿ�����ȫ�ֱ�������
	CComboBox           m_cmbGlobalNameL;       // �ֲ���������ȫ�ֱ���ѡ�������б�ؼ�
	CComboBox           m_cmbDataSourceL;       // �ֲ�������������Դ����ѡ�������б��ؼ�
	CComboBox           m_cmbSourceValueL;      // �ֲ�������������Դ����ѡ�������б��ؼ�
	CEdit               m_edtValueIntL;         // �ֲ��������б�����ʼֵ����༭��ؼ�
	
	int                 m_rowforDL;             // m_rowΪm_List_IOput���к�,ɾ������ʱ�õ�
    int                 m_columnforDL;          // m_columnΪm_List_IOput���к�,ɾ������ʱ�õ�
	int                 m_rowL;                 // m_rowΪm_List_IOput���к�
    int                 m_columnL;              // m_columnΪm_List_IOput���к�

	void                InitCtrl_GetIOL(int rowofchart, int columnofchart, CRect rc);
	
	void reShowListLocal();// ˢ����ʾ�ֲ�������
	
	afx_msg void OnBtnOK();
	afx_msg void OnGetVluName_L();
	afx_msg void OnGetSquence_L();
	afx_msg void OnGetIOtype_L();
	afx_msg void OnGetIsQuote_L();
	afx_msg void OnGetGlobalVlu_L();
	afx_msg void OnGetDataSource_L();
	afx_msg void OnGetDataSourceValue_L();
	afx_msg void OnGetValueInt_L();

	CHSTPN_SYSDoc * p_Doc;
	afx_msg void OnPaint();
	void reShowListGlobal();
};
