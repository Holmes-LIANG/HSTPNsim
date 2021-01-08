#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

// CWndShowMatrix �Ի���

class CWndShowMatrix : public CDialogEx
{
	DECLARE_DYNAMIC(CWndShowMatrix)

public:
	CWndShowMatrix(CWnd* pParent = NULL);   // ��׼���캯��
	CWndShowMatrix(vector<CString> Incdncemat,int nSizep, int nSizet,UINT analysismodelID, CWnd* pParent = NULL);   // redefine the function
	virtual ~CWndShowMatrix();

// �Ի�������
	enum { IDD = IDD_WNDSHOWMATRIX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����չʾ���
	CListCtrl m_Chart_Matrix;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int nSizeP;// ��������
	int nSizeT;// ��Ǩ����
	vector<CString> IncdnceMatStr;//���ڴ洢���ݸ������ڵĹ�������
	UINT AnalysisModelID;//��ʾҪ��ʾ�����ĸ��������������ʾ�ĽṹΪģ�ͼ������ʱΪ1������ʾΪ������������ʱΪ2
};
