#pragma once
#include "HSTPN_Message.h"
#include <vector>
using namespace std;
#include "DlgChooseOValue.h"
#include "HSTPN_SYSDoc.h"
#include "afxcmn.h"
class CHSTPN_SYSDoc;

class CCTRLShowMatrix;

#include "CSVStream.h"

// CDlgSMat_OutValue �Ի���

class CDlgSMat_OutValue : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSMat_OutValue)

public:
	CDlgSMat_OutValue(CWnd* pParent = NULL);   // ��׼���캯��
	CDlgSMat_OutValue(CHSTPN_SYSDoc* m_pdoc, CWnd* pParent = NULL);
	virtual ~CDlgSMat_OutValue();

// �Ի�������
	enum { IDD = IDD_DLGSHOWMATRIX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//-----------��Ա����-------------------
	CHSTPN_SYSDoc       * m_pDoc;
	vector<int>           vec_OutValue;     // ���ڱ���ѡ�е����ֵ��arryIOputData�е�λ��,vec_OutValue�е�ֵΪ1���ʾ��Ӧλ���ϵ�arryIOputData��ֵ���趨Ϊ�����Ҫ����¼
	CString               OutPName;         // ���ڱ���ѡ�е��������������
	CListCtrl             m_Chart_SMatrix;  // ����չʾ���������ListControl
	vector<CString>       vec_OutValueName; // ��vec_OutValue��Ԫ��ֵΪ1�ı�������������
	CCTRLShowMatrix     * p_Parent;         // ����Ի���ĸ����ڵ�ָ��
	int                   i_PposnOValue;    // ���ڱ���ѡ�еĿ�������vector_Places�е�λ��
	vector<VEC_DOUBLE*>   vec_OutputData;   // the output data of outputModel
	vector<list<string> > vec_listOputDat;  // ��vec_OutputData�е�����ת����string����
public:
	afx_msg void OnSmatSet();
	virtual BOOL Create(UINT nIDTemplate,CHSTPN_SYSDoc* m_pdoc, CWnd* pParentWnd = NULL);
	
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void SetChartHead(CString Pname, vector<int> vec_outvalue);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSavemat();
};
