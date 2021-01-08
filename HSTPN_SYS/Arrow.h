#pragma once


// CArrow
#include "Place.h"
#include "Transition.h"
#include "ArrowEdit.h"
class CArrowEdit;

//向量容器头文件及std命名空间
#include <vector>
using namespace std;
class CArrow : public CWnd
{
	DECLARE_DYNAMIC(CArrow)

public:
	CArrow();
	CArrow(UINT u_SubScriptofPlace,UINT u_SubScriptofTrans, UINT whichModel_Start,CPoint pt_start, CPoint pt_end, int u_arrow_id, bool is_disic);
	virtual ~CArrow();
public:
//	CPlace PnPlace;
//	CTransition PnTrans;
	UINT u_SubScriptofPlace;
	UINT u_SubScriptofTrans;
	UINT whichModel_Start;
	CArrowEdit *m_pEdit;
	CPoint pt_start;
	CPoint pt_end;
	static int u_arrow_id;
	int num;
	bool is_disic;
public:
//	void DrawARC(vector<CPlace*> vector_place,vector<CTransition*>vector_trans,CDC * dc);
//	void DrawArrow(CPoint pt_start, CPoint pt_end, CDC * pDC);
	afx_msg LRESULT OnMoveArrow(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};


