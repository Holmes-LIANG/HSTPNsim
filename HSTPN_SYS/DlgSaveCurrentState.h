#pragma once


// CDlgSaveCurrentState 对话框

class CDlgSaveCurrentState : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSaveCurrentState)

public:
	CDlgSaveCurrentState(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSaveCurrentState();

// 对话框数据
	enum { IDD = IDD_DLG_SAVECURRENTSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
