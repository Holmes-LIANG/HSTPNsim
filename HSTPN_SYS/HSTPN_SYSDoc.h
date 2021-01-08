
// HSTPN_SYSDoc.h : CHSTPN_SYSDoc ��Ľӿ�
//



#pragma once
#include "HSTPN_Message.h"
#include <vector>
using namespace std;
#include "Place.h"
#include "Transition.h"
#include "HSTPN_SYSView.h"
#include "DlgSaveNote.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "HSTPN_SYSSim.h"
class CHSTPN_SYSSim;
class CHSTPN_SYSView;

#include "DlgModelCheck.h"
class CDlgModelCheck;

#include "CTRLShowMatrix.h"
class CCTRLShowMatrix;

// HSTPN��������--�����������ϡ���Ǩ���ϡ����򻡼���
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
class CHSTPN_SYSDoc : public CDocument
{
protected: // �������л�����
	//CHSTPN_SYSDoc();
	DECLARE_DYNCREATE(CHSTPN_SYSDoc)

// ����
public:
	CHSTPN_SYSDoc();
	//====================��Ա����=======================================================
	vector<CPlace*>           Vector_Place;         // ά��һ���������������е�CPlace����
	vector<CTransition*>      Vector_Trans;         // ά��һ���������������е�CTransition����
	CPtrArray                 arr_Arrow;            // �����ͷ����ļ���

	CWinThread              * m_simThread;          // �����߳� 
	CHSTPN_SYSSim           * m_hstpnSim;           // �����������
	HSTPN_DATA                m_hstpnData;          // petri���ṹ���� 
	CDlgModelCheck          * m_dlgModelCheck;      // ģ�ͼ��Ի���
	bool                      b_SimSuspend;         // ��ʾ��ǰִ�еķ����߳���û�б���ͣ
	CEvent                  * m_StopSimEvent;       // ֹͣ�̵߳��¼�ָ�� 
	CWinThread              * m_IncdMatThread;      // ������������߳�

	CRect                     rectofModel;          // �����ߴ�
	CRect                     rectofTrans;          // ��Ǩ�ߴ�
	CRect                     rectofOutputModel;    // the Size of OutputModel;

	vector<CCTRLShowMatrix*>  Vector_OutputModel;   // ά��һ���������������е�CCTRLShowMatrix����

	vector<IOValue*>          arryIOputDataG;       // ȫ�ֱ�����
	

	vector<CPlace*>       vec_Model_saveInitSet;    //�ֱ����ڱ����û��ĳ�ʼ���ã����û����������Petri�����á���ťʱ���档
	vector<CTransition*>  vec_Trans_saveInitSet;    // ���������ϣ����ǲ�ϣ������̬�仯����˲��������ĵ�ַ�����Ǳ��������
	vector<IOValue*>      vec_IOputDataG;


// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CHSTPN_SYSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	//====================��Ա����=======================================================
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();

	afx_msg void OnFileSceneOne();

	void           DeletePlcTrnsArcs(CHSTPN_SYSView *m_pView);
	void           WriteXMLFile(CString filePath_Name,TiXmlDocument *myDocument);
	TiXmlDocument* ReadXMLFile(CString filePath_Name,TiXmlDocument *myDocument,CHSTPN_SYSView *m_pView);
	TiXmlDocument* ReadXMLFile1(CString filePath_Name,TiXmlDocument *myDocument,CHSTPN_SYSView *m_pView);

	vector<CPlace*>::iterator      FindPlace( UINT id);
	vector<CTransition*>::iterator FindTrans(UINT id);

	afx_msg void OnDebugStart();
	afx_msg void OnModelcheck();
	afx_msg void OnSimThreadSuspend();
	afx_msg void OnSimThreadStop();
	afx_msg void OnIncdmatanaly();


	CString opening_filePath;  // ���ڱ��浱ǰ���򿪵��ļ���·��
	bool CreateMultipleDirectory(const CString szPath);
};
