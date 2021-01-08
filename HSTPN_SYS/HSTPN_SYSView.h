
// HSTPN_SYSView.h : CHSTPN_SYSView ��Ľӿ�
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
protected: // �������л�����
	CHSTPN_SYSView();
	DECLARE_DYNCREATE(CHSTPN_SYSView)

// ����
public:
	CHSTPN_SYSDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CHSTPN_SYSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CHSTPN_SYSDoc *      m_pDoc;// ָ���ĵ����ָ��
	//===========================��Ա����=================================
	int                  u_whichModel;          // ��ʾ�ĸ�ģ�ͽ�������,u_whichModelȡֵΪ1��8����ȡֵΪ1��6ʱ������Ҫ������Ϊ��������ֵΪ7ʱ������Ҫ������Ϊ��Ǩ����ֵΪ8ʱ���������ڻ���״̬
	int                  u_whichSubscript;
	int                  u_whichKindModel;      // ģ�͵����ͣ���ʾ�ǿ������Ǳ�Ǩ������

	UINT                 u_SubScriptofPlace;    // ά��һ���޷��ű�������ʾVector_Place��ģ�͵�λ�ã��±꣩����Ҫ���ڳ�ʼ��CPlaceģ���е�u_SubScriptofPlace����
	UINT                 u_SubScriptofTrans;    // ά��һ���޷��ű�������ʾVector_Trans��ģ�͵�λ�ã��±꣩����Ҫ���ڳ�ʼ��CTransitionģ���е�u_SubScriptofTrans����
	int                  u_arrow_id;            // ά��һ�����η��ű�������ʾ����Ψһ��Ӧ�ĵ����

	bool                 b_DrawLine;            // true��ʾ�����ߵ�״̬��false��ʾ�������ߵ�״̬
	CPoint               pt_StartDwLn[4];       // ���廭�ߵĿ�ʼ��ͽ�����
	CPoint               pt_EndDwLn[4];

	UINT                 u_subofModel_Arrow;    // ���滭��ͷ��ʼģ�͵��±�
	UINT                 u_subofTrans_Arrow;

	
	CImageList	         m_imgDrag;             // ����CImageListʵ��ģ�͵��϶�
	BOOL		         m_bIsLButtonDown;      // ����Ƿ���/�Ƿ�Ϊ�϶�ģ��״̬�ı�־
	POINT		         m_ptOffset;
	POINT		         m_ptMove;
	POINT                m_ptLBtnDown;

	UINT                 u_PlaceorTrans_Arrow;  // ��ʾ�Ŀ�ʼģ�͵����ͣ��ǿ�����ֵΪ1�����Ǳ�Ǩ��ֵΪ2��

	CBrush               brush_Bk;              // ģ�͵ı���ɫ�����óɰ�ɫ��

	double               d_TimeStep;            // �����û����õķ��沽��

//	vector<CPlace*>       vec_Model_saveInitSet; //�ֱ����ڱ����û��ĳ�ʼ���ã����û����������Petri�����á���ťʱ���档
//	vector<CTransition*>  vec_Trans_saveInitSet; // ���������ϣ����ǲ�ϣ������̬�仯����˲��������ĵ�ַ�����Ǳ��������
//	vector<IOValue*>      vec_IOputDataG;

	TiXmlDocument      * myDocument;            // �������ĵ����󣨷����ļ���

	bool                 bPNisChanged;           // �жϵ�ģ�ͱ�����Ƿ��ٴθ��ģ������ͼ�ڵ�����û�и��ģ��Ͳ��ᵯ�����ѶԻ���
	
	CRect                rectofModel;            // �����ߴ�
	CRect                rectofTrans;            // ��Ǩ�ߴ�
	CRect                rectofOutputModel;      // the Size of OutputModel;

	UINT                 u_SubScriptofOutputModel;  // the subscript of OutputModel

	double               d_SimTime;                 // �����û����õķ���ʱ��
	double               d_IntervalTime;            // �����û����õ�token�����ļ��ʱ��

	//CDlgModelParmList *  m_pParmListDlg;
public:
	//====================��Ա����===========================================
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
	afx_msg LRESULT OnGetTimeStep(WPARAM wParam, LPARAM lParam);// ��ȡ�û����õ�ʱ�䲽��
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

#ifndef _DEBUG  // HSTPN_SYSView.cpp �еĵ��԰汾
inline CHSTPN_SYSDoc* CHSTPN_SYSView::GetDocument() const
   { return reinterpret_cast<CHSTPN_SYSDoc*>(m_pDocument); }
#endif

