#pragma once

#include "HSTPN_SYSDoc.h"
class CHSTPN_SYSDoc;
#include "HSTPN_SYSSim.h"
#include "MainFrm.h"
class CMainFrame;

// CDlgModelCheck 对话框
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
	CDlgModelCheck(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModelCheck();

// 对话框数据
	enum { IDD = IDD_DLGMODELCHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

	CHSTPN_SYSSim * m_hstpnSim;// 接收hstpnSim对象
	LPVOID m_hstpnData; //接收库所、变迁、有向弧数据存储地址

	CWinThread         * m_simThread;          // 分析线程 
	ModelCheckData       m_modelcheckData;     // 传递给分析线程的数据
};
