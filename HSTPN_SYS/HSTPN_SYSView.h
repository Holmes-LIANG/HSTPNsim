
// HSTPN_SYSView.h : CHSTPN_SYSView 类的接口
//

#pragma once

#include "HSTPN_SYSDoc.h"
#include "HSTPN_Message.h"
#include "Place.h"
#include "Transition.h"
#include <cmath>
#include "Arrow.h"
#include "MainFrm.h"
#include "CTRLShowMatrix.h"
class CCTRLShowMatrix;

#include "tinystr.h"
#include "tinyxml.h"
class CHSTPN_SYSDoc;
#include "DlgModelParmList.h"
#include "DlgSaveCurrentState.h"
#include "DlgResetState.h"


class CHSTPN_SYSView : public CScrollView//CView
{
protected: // 仅从序列化创建
	CHSTPN_SYSView();
	DECLARE_DYNCREATE(CHSTPN_SYSView)

// 特性
public:
	CHSTPN_SYSDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHSTPN_SYSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CHSTPN_SYSDoc *      m_pDoc;// 指向文档类的指针
	//===========================成员变量=================================
	int                  u_whichModel;          // 表示哪个模型将被创建,u_whichModel取值为1到8；当取值为1到6时，表明要创建的为库所；当值为7时，表明要创建的为变迁；当值为8时，表明处于画线状态
	int                  u_whichSubscript;
	int                  u_whichKindModel;      // 模型的类型，表示是库所还是变迁被创建

	UINT                 u_SubScriptofPlace;    // 维护一个无符号变量，表示Vector_Place中模型的位置（下标）；主要用于初始化CPlace模型中的u_SubScriptofPlace变量
	UINT                 u_SubScriptofTrans;    // 维护一个无符号变量，表示Vector_Trans中模型的位置（下标）；主要用于初始化CTransition模型中的u_SubScriptofTrans变量
	int                  u_arrow_id;            // 维护一个整形符号变量，表示有向弧唯一对应的的序号

	bool                 b_DrawLine;            // true表示在连线的状态，false表示不在连线的状态
	CPoint               pt_StartDwLn[4];       // 定义画线的开始点和结束点
	CPoint               pt_EndDwLn[4];

	UINT                 u_subofModel_Arrow;    // 保存画箭头开始模型的下标
	UINT                 u_subofTrans_Arrow;

	
	CImageList	         m_imgDrag;             // 利用CImageList实现模型的拖动
	BOOL		         m_bIsLButtonDown;      // 左键是否按下/是否为拖动模型状态的标志
	POINT		         m_ptOffset;
	POINT		         m_ptMove;
	POINT                m_ptLBtnDown;

	UINT                 u_PlaceorTrans_Arrow;  // 表示的开始模型的类型，是库所（值为1）还是变迁（值为2）

	CBrush               brush_Bk;              // 模型的背景色（设置成白色）

	double               d_TimeStep;            // 接收用户设置的仿真步长

//	vector<CPlace*>       vec_Model_saveInitSet; //分别用于保存用户的初始设置，当用户点击“保存Petri网设置”按钮时保存。
//	vector<CTransition*>  vec_Trans_saveInitSet; // 这两个集合，我们不希望它动态变化，因此不保存对象的地址，而是保存对象本身
//	vector<IOValue*>      vec_IOputDataG;

	TiXmlDocument      * myDocument;            // 待保存文档对象（仿真文件）

	bool                 bPNisChanged;           // 判断当模型保存后，是否被再次更改，如果视图内的内容没有更改，就不会弹出提醒对话框
	
	CRect                rectofModel;            // 库所尺寸
	CRect                rectofTrans;            // 变迁尺寸
	CRect                rectofOutputModel;      // the Size of OutputModel;

	UINT                 u_SubScriptofOutputModel;  // the subscript of OutputModel

	double               d_SimTime;                 // 接收用户设置的仿真时间
	double               d_IntervalTime;            // 接收用户设置的token流动的间隔时间

	//CDlgModelParmList *  m_pParmListDlg;
public:
	//====================成员函数===========================================
	afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnCreateModel2(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMoveModel2(WPARAM wParam, LPARAM lParam);
	virtual BOOL    PreTranslateMessage(MSG* pMsg);
	afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void    OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL    OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnDrawLineend2(WPARAM wParam, LPARAM lParam);
	afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void    OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void    OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void            DrawArrow(CPoint pt_start, CPoint pt_end);
	afx_msg LRESULT OnGetTimeStep(WPARAM wParam, LPARAM lParam);// 获取用户设置的时间步长
	afx_msg void    OnSaveCurrentPNSet();
	afx_msg void    OnRestorePnset();
	void            CreatePlaceorTrans(int u_whichModel, CPoint point);
	void            DeletePlcTrnsArcs();
	afx_msg void    OnDestroy();
	char*           CString2char(CString str);
	afx_msg LRESULT OnMDdelete2(WPARAM wParam, LPARAM lParam);

	vector<CPlace*>::iterator           FindPlace(UINT id);
	vector<CTransition*>::iterator      FindTrans(UINT id);
	vector<CCTRLShowMatrix*>::iterator  FindOuts(UINT id);

	void            ShowFocus(UINT u_whichKindModel,UINT u_whichSubscript);
	virtual void    OnInitialUpdate();
	//afx_msg void OnShowpramlist();
};

#ifndef _DEBUG  // HSTPN_SYSView.cpp 中的调试版本
inline CHSTPN_SYSDoc* CHSTPN_SYSView::GetDocument() const
   { return reinterpret_cast<CHSTPN_SYSDoc*>(m_pDocument); }
#endif

