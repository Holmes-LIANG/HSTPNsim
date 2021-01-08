#pragma once


// CDlgResetState 对话框

class CDlgResetState : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResetState)

public:
	CDlgResetState(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResetState();

// 对话框数据
	enum { IDD = IDD_DLG_RESETSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
