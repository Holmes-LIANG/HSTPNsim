#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

// CWndShowMatrix 对话框

class CWndShowMatrix : public CDialogEx
{
	DECLARE_DYNAMIC(CWndShowMatrix)

public:
	CWndShowMatrix(CWnd* pParent = NULL);   // 标准构造函数
	CWndShowMatrix(vector<CString> Incdncemat,int nSizep, int nSizet,UINT analysismodelID, CWnd* pParent = NULL);   // redefine the function
	virtual ~CWndShowMatrix();

// 对话框数据
	enum { IDD = IDD_WNDSHOWMATRIX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 矩阵展示表格
	CListCtrl m_Chart_Matrix;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int nSizeP;// 库所数量
	int nSizeT;// 变迁数量
	vector<CString> IncdnceMatStr;//用于存储传递给本窗口的关联矩阵
	UINT AnalysisModelID;//表示要显示的是哪个分析结果，当显示的结构为模型检查内容时为1，当显示为关联矩阵内容时为2
};
