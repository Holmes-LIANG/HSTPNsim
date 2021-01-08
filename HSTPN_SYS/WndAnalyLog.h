#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
using namespace std;

// CWndAnalyLog 对话框

class CWndAnalyLog : public CDialogEx
{
	DECLARE_DYNAMIC(CWndAnalyLog)

public:
	CWndAnalyLog(CWnd* pParent = NULL);   // 标准构造函数
	CWndAnalyLog(vector<CString> IsltdnodeSet,UINT AnalysismodelID, CWnd* pParent = NULL);   // 自定义构造函数
	CWndAnalyLog(vector<CString> Incdncemat,int nSizep, int nSizet,UINT analysismodelID, CWnd* pParent = NULL);   // 自定义构造函数
	virtual ~CWndAnalyLog();

// 对话框数据
	enum { IDD = IDD_DLGANALYLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	vector<CString> IsLtdNodeSet;//孤立点检查结构保存集合
public:

	afx_msg void OnEnChangeShowanallylog();
	
	CEdit m_editShowLog;// 显示分析结构的edit框
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	int nSizeP;// 库所数量
	int nSizeT;// 变迁数量
	vector<CString> IncdnceMatStr;//用于存储传递给本窗口的关联矩阵
	UINT AnalysisModelID;//表示要显示的是哪个分析结果，当显示的结构为模型检查内容时为1，当显示为关联矩阵内容时为2
	
//	CListCtrl m_chartOfIncdnce; // 关联矩阵展示表格
};
