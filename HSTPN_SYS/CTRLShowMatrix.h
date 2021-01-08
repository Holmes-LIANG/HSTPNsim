#pragma once

#include "HSTPN_Message.h"
#include "DlgSMat_OutValue.h"
class CDlgSMat_OutValue;
#include "HSTPN_SYSView.h"

class CHSTPN_SYSView;


#include "DlgChseOVlueTFile.h"
class CDlgChseOVlueTFile;

class CCTRLShowMatrix : public CStatic
{
	DECLARE_DYNAMIC(CCTRLShowMatrix)

public:
	CCTRLShowMatrix();
	virtual ~CCTRLShowMatrix();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//=================��Ա����================================
	UINT                         ID;                // the ID of OutputModel
	CString                      m_caption;         // the name of OutputModel
	CPoint                       Position;          // the Position of OutputModel in View;
	enumOutputStyle              OutputStyle;       // the Style of OutputModel
	CDlgSMat_OutValue         *  m_pSMat_OutValue;  // the dialog to show OutValue
	vector<int>                  vec_OutValue;      // ���ڱ���ѡ�е����ֵ��arryIOputData�е�λ��,vec_OutValue�е�ֵΪ1���ʾ��Ӧλ���ϵ�arryIOputData��ֵ���趨Ϊ�����Ҫ����¼
	CString                      OutPName;          // ���ڱ���ѡ�е��������������
	vector<CString>              vec_OutValueName;  // ��vec_OutValue��Ԫ��ֵΪ1�ı�������������
	CHSTPN_SYSView            *  m_pParent;         // the pointor of Parent-class ,that is View-class
	int                          i_PposnOValue;     // ���ڱ���ѡ�еĿ�������vector_Places�е�λ��
	vector<VEC_DOUBLE*>          vec_OutputData;    // the output data of outputModel

	//CDlgChseOVlueTFile        * m_p_OVlueTFile; // the dialog to write the output data to file

	ofstream                   * m_pDatafile;      // ÿ��OUT_TOFILE���͵����ģ�鶼�����ļ�
	CString                      str_FilePath;     // �������ļ���·��

public:
	//================��Ա����=================================
	virtual BOOL    Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, enumOutputStyle outputstyle, UINT nID = 0xffff);

	virtual void    DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void    PreSubclassWindow();
	afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void    OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT UpdataList1(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMdDelete();
};


