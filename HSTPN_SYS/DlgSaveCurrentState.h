#pragma once


// CDlgSaveCurrentState �Ի���

class CDlgSaveCurrentState : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSaveCurrentState)

public:
	CDlgSaveCurrentState(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSaveCurrentState();

// �Ի�������
	enum { IDD = IDD_DLG_SAVECURRENTSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
