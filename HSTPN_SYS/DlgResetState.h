#pragma once


// CDlgResetState �Ի���

class CDlgResetState : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResetState)

public:
	CDlgResetState(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResetState();

// �Ի�������
	enum { IDD = IDD_DLG_RESETSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
