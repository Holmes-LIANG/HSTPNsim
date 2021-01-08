#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
using namespace std;

// CWndAnalyLog �Ի���

class CWndAnalyLog : public CDialogEx
{
	DECLARE_DYNAMIC(CWndAnalyLog)

public:
	CWndAnalyLog(CWnd* pParent = NULL);   // ��׼���캯��
	CWndAnalyLog(vector<CString> IsltdnodeSet,UINT AnalysismodelID, CWnd* pParent = NULL);   // �Զ��幹�캯��
	CWndAnalyLog(vector<CString> Incdncemat,int nSizep, int nSizet,UINT analysismodelID, CWnd* pParent = NULL);   // �Զ��幹�캯��
	virtual ~CWndAnalyLog();

// �Ի�������
	enum { IDD = IDD_DLGANALYLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	vector<CString> IsLtdNodeSet;//��������ṹ���漯��
public:

	afx_msg void OnEnChangeShowanallylog();
	
	CEdit m_editShowLog;// ��ʾ�����ṹ��edit��
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	int nSizeP;// ��������
	int nSizeT;// ��Ǩ����
	vector<CString> IncdnceMatStr;//���ڴ洢���ݸ������ڵĹ�������
	UINT AnalysisModelID;//��ʾҪ��ʾ�����ĸ��������������ʾ�ĽṹΪģ�ͼ������ʱΪ1������ʾΪ������������ʱΪ2
	
//	CListCtrl m_chartOfIncdnce; // ��������չʾ���
};
