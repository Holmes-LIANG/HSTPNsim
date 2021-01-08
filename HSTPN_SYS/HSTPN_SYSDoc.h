
// HSTPN_SYSDoc.h : CHSTPN_SYSDoc 类的接口
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

// HSTPN网络数据--包括库所集合、变迁集合、有向弧集合
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
protected: // 仅从序列化创建
	//CHSTPN_SYSDoc();
	DECLARE_DYNCREATE(CHSTPN_SYSDoc)

// 特性
public:
	CHSTPN_SYSDoc();
	//====================成员变量=======================================================
	vector<CPlace*>           Vector_Place;         // 维护一个向量，保存所有的CPlace对象
	vector<CTransition*>      Vector_Trans;         // 维护一个向量，保存所有的CTransition对象
	CPtrArray                 arr_Arrow;            // 保存箭头对象的集合

	CWinThread              * m_simThread;          // 仿真线程 
	CHSTPN_SYSSim           * m_hstpnSim;           // 仿真分析对象
	HSTPN_DATA                m_hstpnData;          // petri网结构数据 
	CDlgModelCheck          * m_dlgModelCheck;      // 模型检查对话框
	bool                      b_SimSuspend;         // 表示当前执行的仿真线程有没有被暂停
	CEvent                  * m_StopSimEvent;       // 停止线程的事件指针 
	CWinThread              * m_IncdMatThread;      // 关联矩阵求解线程

	CRect                     rectofModel;          // 库所尺寸
	CRect                     rectofTrans;          // 变迁尺寸
	CRect                     rectofOutputModel;    // the Size of OutputModel;

	vector<CCTRLShowMatrix*>  Vector_OutputModel;   // 维护一个向量，保存所有的CCTRLShowMatrix对象

	vector<IOValue*>          arryIOputDataG;       // 全局变量表
	

	vector<CPlace*>       vec_Model_saveInitSet;    //分别用于保存用户的初始设置，当用户点击“保存Petri网设置”按钮时保存。
	vector<CTransition*>  vec_Trans_saveInitSet;    // 这两个集合，我们不希望它动态变化，因此不保存对象的地址，而是保存对象本身
	vector<IOValue*>      vec_IOputDataG;


// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CHSTPN_SYSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	//====================成员函数=======================================================
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


	CString opening_filePath;  // 用于保存当前被打开的文件的路径
	bool CreateMultipleDirectory(const CString szPath);
};
