#pragma once


// CDlgSaveNote 对话框

class CDlgSaveNote : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSaveNote)

public:
	CDlgSaveNote(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSaveNote();

// 对话框数据
	enum { IDD = IDD_DLG_SAVENOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnCancel();
};
