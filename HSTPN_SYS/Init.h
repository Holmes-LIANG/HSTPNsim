#pragma once


class CHSTPN_SYSDoc;
// CInit �Ի���
class CInit : public CDialogEx
{
	DECLARE_DYNAMIC(CInit)

public:
	CInit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInit();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMoveButton();
	void InitDataToPDoc();

	CHSTPN_SYSDoc* p_Doc;
	CWinThread * m_moveThread;//�����߳�
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
