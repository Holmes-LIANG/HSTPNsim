#pragma once


class CHSTPN_SYSDoc;
// CInit 对话框
class CInit : public CDialogEx
{
	DECLARE_DYNAMIC(CInit)

public:
	CInit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInit();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMoveButton();
	void InitDataToPDoc();

	CHSTPN_SYSDoc* p_Doc;
	CWinThread * m_moveThread;//动画线程
	int m_editDZZ;
	int m_editGJJ;
	int m_editHZJ;
	int m_editBJXJD;
	int m_editDLBLJD;
	int m_editBJXDD;
	int m_editDLBLDD;
	int m_editYCDD;
	int m_editBJXLD;
	int m_editDLBLLD;
	int m_editYCLD;
};
