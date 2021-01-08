#pragma once

#include "HSTPN_SYSDoc.h"
class CHSTPN_SYSDoc;
#include "HSTPN_SYSSim.h"
#include "MainFrm.h"
class CMainFrame;

// CDlgModelCheck �Ի���
#ifndef HSTPN_DATA_ST
#define HSTPN_DATA_ST

typedef struct __HSTPN_DATA
{
	vector<CPlace*>           vector_place;
	vector<CTransition*>      vector_trans;
	CPtrArray                 arr_arrow;
	CEvent                  * m_stopsimevent;
	double                    m_timestep;
	double                    m_simtime;
	double                    m_intervaltime;
	vector<CCTRLShowMatrix*>  vector_outputmodel;
	CMainFrame              * m_pmainframe;
	CHSTPN_SYSDoc           * m_pDoc;
}HSTPN_DATA;

typedef struct __ModelCheck_DATA
{
	LPVOID m_hstpnData;
	BOOL m_btnIsLtdNode;
	BOOL m_btnConflict;
	BOOL m_btnContact;
	BOOL m_btnDeadlock;
	BOOL m_btnTrap;
}ModelCheckData;

#endif

class CDlgModelCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModelCheck)

public:
	CDlgModelCheck(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModelCheck();

// �Ի�������
	enum { IDD = IDD_DLGMODELCHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStartModelCheck();
	BOOL m_btnIsLtdNode;
	BOOL m_btnConflict;
	BOOL m_btnContact;
	BOOL m_btnDeadlock;
	BOOL m_btnTrap;
	afx_msg void OnBnClickedIsltdnode();
	afx_msg void OnBnClickedConflict();
	afx_msg void OnBnClickedContact();
	afx_msg void OnBnClickedDeadlock();
	afx_msg void OnBnClickedTrap();
	virtual BOOL Create(LPCTSTR lpszTemplateName, CHSTPN_SYSSim * m_hstpnSim1, LPVOID m_hstpnData1,CWnd* pParentWnd = NULL);

	CHSTPN_SYSSim * m_hstpnSim;// ����hstpnSim����
	LPVOID m_hstpnData; //���տ�������Ǩ���������ݴ洢��ַ

	CWinThread         * m_simThread;          // �����߳� 
	ModelCheckData       m_modelcheckData;     // ���ݸ������̵߳�����
};
