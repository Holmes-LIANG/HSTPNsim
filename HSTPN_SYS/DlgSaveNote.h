#pragma once


// CDlgSaveNote �Ի���

class CDlgSaveNote : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSaveNote)

public:
	CDlgSaveNote(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSaveNote();

// �Ի�������
	enum { IDD = IDD_DLG_SAVENOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnCancel();
};
