// Arrow.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "Arrow.h"


// CArrow
int CArrow::u_arrow_id;

IMPLEMENT_DYNAMIC(CArrow, CWnd)

CArrow::CArrow()
{
	//n=0;
}
//======================================
// 带参数的构造函数
// 在View类中利用该函数，构造箭头对象，并保存到CBoArray集合中，编译保存箭头对象
//======================================
CArrow::CArrow(UINT u_SubScriptofPlace,UINT u_SubScriptofTrans, UINT whichModel_Start, CPoint pt_start, CPoint pt_end, int u_arrow_id, bool is_disic)
{
//	this->PnPlace = PnPlace;
//	this->PnTrans = PnTrans;
	bool m_EditCreated = true;
	this->u_SubScriptofPlace = u_SubScriptofPlace;
	this->u_SubScriptofTrans = u_SubScriptofTrans;
	this->whichModel_Start = whichModel_Start;
	this->pt_start = pt_start;
	this->pt_end = pt_end;
	this->u_arrow_id = u_arrow_id;
	this->is_disic = is_disic;
	float x1 = pt_start.x;
	float y1 = pt_start.y;
	float x2 = pt_end.x;
	float y2 = pt_end.y;
	float x3 = (x1+x2)/2;
	float y3 = (y1+y2)/2;
	if(is_disic)
	{
		m_EditCreated = false;
	}
	else
	{
		m_pEdit = NULL;
	}
	if(!m_EditCreated)
	{
		m_pEdit = new CArrowEdit();//CEdit();
		m_pEdit->Create(WS_CHILD|WS_BORDER|WS_VISIBLE,CRect(x3,y3,x3+15,y3+20),((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)),u_arrow_id,this);
		m_EditCreated = true;
	}
}

CArrow::~CArrow()
{
	if(m_pEdit!=NULL)
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
	}
}


BEGIN_MESSAGE_MAP(CArrow, CWnd)
END_MESSAGE_MAP()

