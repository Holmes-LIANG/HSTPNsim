
// HSTPN_SYSView.cpp : CHSTPN_SYSView 类的实现
//
#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HSTPN_SYS.h"
#endif

#include "HSTPN_SYSDoc.h"
#include "HSTPN_SYSView.h"

#include "DlgSaveNote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHSTPN_SYSView

IMPLEMENT_DYNCREATE(CHSTPN_SYSView, CScrollView)

BEGIN_MESSAGE_MAP(CHSTPN_SYSView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHSTPN_SYSView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//------------------------------
	ON_MESSAGE(WM_MESSAGE_CREATEMODEL2,OnCreateModel2)
	ON_MESSAGE(WM_MESSAGE_MOVEMODEL2,OnMoveModel2)
	ON_MESSAGE(WM_MESSAGE_DRAWLINEEND2,OnDrawLineend2)//

	ON_MESSAGE(WM_MESSAGE_GETTIMESTEP,OnGetTimeStep)

	ON_MESSAGE(WM_MESSAGE_MDDELETE2,OnMDdelete2)
	//------------------------------
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_SAVE_CURRENT_SET, &CHSTPN_SYSView::OnSaveCurrentPNSet)
	ON_COMMAND(ID_RESTORE_PNSET, &CHSTPN_SYSView::OnRestorePnset)
	ON_WM_DESTROY()

	//ON_COMMAND(ID_SHOWPRAMLIST, &CHSTPN_SYSView::OnShowpramlist)
END_MESSAGE_MAP()

// CHSTPN_SYSView 构造/析构

CHSTPN_SYSView::CHSTPN_SYSView()
{
	// TODO: 在此处添加构造代码
	u_whichSubscript   = 0;
	u_SubScriptofPlace = 0;
	u_SubScriptofTrans = 0;

	b_DrawLine = FALSE;

	m_bIsLButtonDown	  = FALSE;
	m_ptOffset.x		  = 0;
	m_ptOffset.y		  = 0;
	m_ptLBtnDown.x        = 0;
	m_ptLBtnDown.y        = 0;

	u_PlaceorTrans_Arrow  = 0;
	brush_Bk.CreateSolidBrush(RGB(255,255,255));

	d_TimeStep            = 0.1;

	myDocument            = NULL;

	bPNisChanged          = TRUE;

	m_pDoc                = NULL;

	rectofModel.left         = 0;
	rectofModel.top          = 0;
	rectofModel.right        = 35;
	rectofModel.bottom       = 35;

	rectofTrans.left         = 0;
	rectofTrans.top          = 0;
	rectofTrans.right        = 25;
	rectofTrans.bottom       = 25;

	rectofOutputModel.left   = 0;
	rectofOutputModel.top    = 0;
	rectofOutputModel.right  = 70;
	rectofOutputModel.bottom = 35;

	u_SubScriptofOutputModel = 0;

	d_SimTime                = DBL_MAX;
	d_IntervalTime           = 200;
	u_arrow_id               = 0;

	//m_pParmListDlg = NULL;
	if (!PathIsDirectory(_T("LuaFile")))//判断文件夹是否创建
	{
		::CreateDirectory(_T("LuaFile"), NULL);//创建一个文件夹
	}
}

CHSTPN_SYSView::~CHSTPN_SYSView()
{
	if(myDocument != NULL)
	{
		myDocument->Clear();// 释放子节点内存
	}
	delete myDocument; // 释放根节点本身

	// 释放输入/输出列表
	for(vector<CPlace*>::iterator iterP=m_pDoc->Vector_Place.begin();iterP!=m_pDoc->Vector_Place.end();iterP++)
	{
		for(vector<IOValue*>::iterator iter3=(*iterP)->arryIOputData.begin();iter3!=(*iterP)->arryIOputData.end();iter3++)
		{
			delete (*iter3);
		}
		(*iterP)->arryIOputData.clear();
		//(*iterP)->arryInputData.clear();//--5.27
		(*iterP)->arryInputofLua.clear();
		//(*iterP)->arryOutputData.clear();
		(*iterP)->arryOutputofLua.clear();

	}
	// 释放Vector_Place中指定元素，情况Vector_Place
	for(vector<CPlace*>::iterator iterP=m_pDoc->Vector_Place.begin();iterP!=m_pDoc->Vector_Place.end();iterP++)
	{
		delete (*iterP);
	}
	// 释放Vector_Trans中元素指向的内存，清空Vector_Trans
	for(vector<CTransition*>::iterator iterT=m_pDoc->Vector_Trans.begin();iterT!=m_pDoc->Vector_Trans.end();iterT++)
	{
		delete (*iterT);
	}
	// 释放arr_Arrow中元素指向的内存，清空arr_Arrow
	int n_SizeArcs = m_pDoc->arr_Arrow.GetSize();
	for(int i=0;i<n_SizeArcs;i++)
	{
		delete (CArrow*)m_pDoc->arr_Arrow.GetAt(i);
	}

	// release the vec_OutputData of OutputofModel
	for(vector<CCTRLShowMatrix*>::iterator iterO=m_pDoc->Vector_OutputModel.begin();iterO!=m_pDoc->Vector_OutputModel.end();iterO++)
	{
		for(vector<VEC_DOUBLE*>::iterator itervv=(*iterO)->vec_OutputData.begin();itervv!=(*iterO)->vec_OutputData.end();itervv++)
		{
			for(vector<double*>::iterator iterv=(*itervv)->begin();iterv!=(*itervv)->end();iterv++)
			{
				delete (*iterv);
			}
			delete (*itervv);
		}
	}
	// release the OutputofModel
	for(vector<CCTRLShowMatrix*>::iterator iterO=m_pDoc->Vector_OutputModel.begin();iterO!=m_pDoc->Vector_OutputModel.end();iterO++)
	{
		delete(*iterO);
	}
	// 释放vec_IOputDataG
	for(vector<IOValue*>::iterator iterg=m_pDoc->vec_IOputDataG.begin();iterg!=m_pDoc->vec_IOputDataG.end();iterg++)
	{
		delete (*iterg);
	}
	m_pDoc->vec_IOputDataG.clear();
	// 释放vec_Model_saveInitSet中的列表
	for(vector<CPlace*>::iterator iterP=m_pDoc->vec_Model_saveInitSet.begin();iterP!=m_pDoc->vec_Model_saveInitSet.end();iterP++)
	{
		for(vector<IOValue*>::iterator iterio=(*iterP)->arryIOputData.begin();iterio!=(*iterP)->arryIOputData.end();iterio++)
		{
			delete (*iterio);
		}
		(*iterP)->arryIOputData.clear();
		(*iterP)->arryInputofLua.clear();
		(*iterP)->arryOutputofLua.clear();
	}
	// 释放vec_Model_saveInitSet
	for(vector<CPlace*>::iterator iterP=m_pDoc->vec_Model_saveInitSet.begin();iterP!=m_pDoc->vec_Model_saveInitSet.end();iterP++)
	{
		delete (*iterP);
	}

	// 释放vec_Trans_saveInitSet
	for(vector<CTransition*>::iterator iterT=m_pDoc->vec_Trans_saveInitSet.begin();iterT!=m_pDoc->vec_Trans_saveInitSet.end();iterT++)
	{
		delete (*iterT);
	}


	//if(m_pParmListDlg != NULL)
	//	delete m_pParmListDlg;

}

BOOL CHSTPN_SYSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CHSTPN_SYSView 绘制

void CHSTPN_SYSView::OnDraw(CDC* pDC)
{
	CHSTPN_SYSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_pDoc = GetDocument();
	// TODO: 在此处为本机数据添加绘制代码
	CPen pen1(PS_SOLID,1,RGB(0,0,240));
	pDC->SelectObject(&pen1);

	for(vector<CPlace*>::iterator iterp=m_pDoc->Vector_Place.begin();iterp!=m_pDoc->Vector_Place.end();iterp++)
	{
		pDC->TextOutW((*iterp)->Position.x,(*iterp)->Position.y-17,(*iterp)->m_caption);
	}
	for(vector<CTransition*>::iterator itert=m_pDoc->Vector_Trans.begin();itert!=m_pDoc->Vector_Trans.end();itert++)
	{
		pDC->TextOutW((*itert)->Position.x,(*itert)->Position.y-17,(*itert)->m_caption);
	}

	for(vector<CCTRLShowMatrix*>::iterator itero=m_pDoc->Vector_OutputModel.begin();itero!=m_pDoc->Vector_OutputModel.end();itero++)
	{
		pDC->TextOutW((*itero)->Position.x-rectofTrans.Width()/2,(*itero)->Position.y-17,(*itero)->m_caption);
	}
	// 获取视口原点，以解决绘图位置错位现象
	CPoint pt_ViewportOrg;// 定义一个视口的原点
	pt_ViewportOrg = pDC->GetViewportOrg();

	// 在每次重绘View视图时，重新绘制箭头
	int nCount = m_pDoc->arr_Arrow.GetSize();
	for(int i=0;i<nCount;i++)
	{
		CPoint pt_start;
		CPoint pt_end;
		vector<CPlace*>::iterator iterp;
		vector<CTransition*>::iterator itert;
		double theta1; 
		double theta2;
		CPoint ptcP;
		CPoint ptcT;
		switch(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->whichModel_Start)
		{
		case 1:// 开始模型为库所，结束模型为变迁时
			iterp = FindPlace(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofPlace);// 找到库所在vector_Place中的位置
			pt_StartDwLn[0].x = (LONG)((*iterp)->Position.x);// pt1 of P
			pt_StartDwLn[0].y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
			pt_StartDwLn[1].x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt2 of P
			pt_StartDwLn[1].y = (LONG)((*iterp)->Position.y + rectofModel.Height());
			pt_StartDwLn[2].x = (LONG)((*iterp)->Position.x + rectofModel.Width());// pt3 of P
			pt_StartDwLn[2].y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
			pt_StartDwLn[3].x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt4 of P
			pt_StartDwLn[3].y = (LONG)((*iterp)->Position.y);

			itert = FindTrans(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofTrans);// 找到变迁在Vector_Trans中的位置
			pt_EndDwLn[0].x = (LONG)((*itert)->Position.x);// pt1 of Trans
			pt_EndDwLn[0].y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
			pt_EndDwLn[1].x = (LONG)((*itert)->Position.x + rectofTrans.Width());// pt2 of T
			pt_EndDwLn[1].y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
			pt_EndDwLn[2].x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);// pt3 of T
			pt_EndDwLn[2].y = (LONG)((*itert)->Position.y + rectofTrans.Height());
			pt_EndDwLn[3].x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);// pt4 of T
			pt_EndDwLn[3].y = (LONG)((*itert)->Position.y);
			
			ptcP.x = (pt_StartDwLn[0].x + pt_StartDwLn[2].x) / 2.0;
			ptcP.y = (pt_StartDwLn[0].y + pt_StartDwLn[2].y) / 2.0;
			
			ptcT.x = (pt_EndDwLn[0].x + pt_EndDwLn[1].x) / 2.0;
			ptcT.y = (pt_EndDwLn[0].y + pt_EndDwLn[1].y) / 2.0;
			theta1 = atan2((double)(abs(ptcT.y-ptcP.y)),(double)(abs(ptcT.x-ptcP.x)));
			if(theta1<=3.1415926/4 && pt_StartDwLn[2].x<pt_EndDwLn[0].x)
			{
				pt_start.x = (LONG)((*iterp)->Position.x + rectofModel.Width()); // pt3 of Place
				pt_start.y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
				pt_end.x = (LONG)((*itert)->Position.x);                         // pt1 of Trans
				pt_end.y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
			}
			else
			{
				if(theta1>3.1415926/4 && pt_StartDwLn[1].y<pt_EndDwLn[3].y)
				{
					pt_start.x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt2 of P
					pt_start.y = (LONG)((*iterp)->Position.y + rectofModel.Height());
					pt_end.x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);// pt4 of T
					pt_end.y = (LONG)((*itert)->Position.y);
				}
				else
				{
					if(theta1<=3.1415926/4 && pt_StartDwLn[0].x>pt_EndDwLn[1].x)
					{
						pt_start.x = (LONG)((*iterp)->Position.x);// pt1 of P
						pt_start.y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
						pt_end.x = (LONG)((*itert)->Position.x + rectofTrans.Width());// pt2 of T
						pt_end.y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
					}
					else
					{
						pt_start.x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt4 of P
						pt_start.y = (LONG)((*iterp)->Position.y);
						pt_end.x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);// pt3 of T
						pt_end.y = (LONG)((*itert)->Position.y + rectofTrans.Height());
					}
				}
			}

			pDC->MoveTo(pt_start);
			pDC->LineTo(pt_end);
			DrawArrow(pt_start,pt_end);
			break;
		case 2:// 开始模型为变迁，结束模型为库所时
			itert = FindTrans(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofTrans);
			pt_StartDwLn[0].x = (LONG)((*itert)->Position.x);// pt1 of Trans
			pt_StartDwLn[0].y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
			pt_StartDwLn[1].x = (LONG)((*itert)->Position.x + rectofTrans.Width());// pt2 of T
			pt_StartDwLn[1].y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
			pt_StartDwLn[2].x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);// pt3 of T
			pt_StartDwLn[2].y = (LONG)((*itert)->Position.y + rectofTrans.Height());
			pt_StartDwLn[3].x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);// pt4 of T
			pt_StartDwLn[3].y = (LONG)((*itert)->Position.y);
			iterp = FindPlace(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofPlace);
			pt_EndDwLn[0].x = (LONG)((*iterp)->Position.x);// pt1 of P
			pt_EndDwLn[0].y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
			pt_EndDwLn[1].x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt2 of P
			pt_EndDwLn[1].y = (LONG)((*iterp)->Position.y + rectofModel.Height());
			pt_EndDwLn[2].x = (LONG)((*iterp)->Position.x + rectofModel.Width());// pt3 of P
			pt_EndDwLn[2].y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
			pt_EndDwLn[3].x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt4 of P
			pt_EndDwLn[3].y = (LONG)((*iterp)->Position.y);

			ptcP.x = (pt_StartDwLn[0].x + pt_StartDwLn[2].x) / 2.0;
			ptcP.y = (pt_StartDwLn[0].y + pt_StartDwLn[2].y) / 2.0;
			ptcT.x = (pt_EndDwLn[0].x + pt_EndDwLn[1].x) / 2.0;
			ptcT.y = (pt_EndDwLn[0].y + pt_EndDwLn[1].y) / 2.0;
			theta2 = atan2((double)(abs(ptcT.y-ptcP.y)),(double)(abs(ptcT.x-ptcP.x)));
			if(theta2<=3.1415926/4 && pt_StartDwLn[1].x<pt_EndDwLn[0].x)
			{
				pt_start.x = (LONG)((*itert)->Position.x + rectofTrans.Width());//pt2 of T
				pt_start.y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
				pt_end.x = (LONG)((*iterp)->Position.x);// pt1 of P
				pt_end.y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
			}
			else
			{
				if(theta2>3.1415926/4 && pt_StartDwLn[2].y<pt_EndDwLn[3].y)
				{
					pt_start.x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);//pt3 of T
					pt_start.y = (LONG)((*itert)->Position.y + rectofTrans.Height());
					pt_end.x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);// pt4 of P
					pt_end.y = (LONG)((*iterp)->Position.y);
				}
				else
				{
					if(theta2<=3.1415926/4 && pt_StartDwLn[0].x>pt_EndDwLn[2].x)
					{
						pt_start.x = (LONG)((*itert)->Position.x);// pt1 of T
						pt_start.y = (LONG)((*itert)->Position.y + rectofTrans.Height()/2.0);
						pt_end.x = (LONG)((*iterp)->Position.x + rectofModel.Width());// pt3 of P
						pt_end.y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
					}
					else
					{
						pt_start.x = (LONG)((*itert)->Position.x + rectofTrans.Width()/2.0);// pt4 of T
						pt_start.y = (LONG)((*itert)->Position.y);
						pt_end.x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);// pt2 of P
						pt_end.y = (LONG)((*iterp)->Position.y + rectofModel.Height());
					}
				}
			}
			pDC->MoveTo(pt_start);
			pDC->LineTo(pt_end);
			DrawArrow(pt_start,pt_end);
			break;
		default:
			break;
		}
		
	}


	ReleaseDC(pDC);
}


// CHSTPN_SYSView 打印


void CHSTPN_SYSView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHSTPN_SYSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHSTPN_SYSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHSTPN_SYSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CHSTPN_SYSView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
}

void CHSTPN_SYSView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHSTPN_SYSView 诊断

#ifdef _DEBUG
void CHSTPN_SYSView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CHSTPN_SYSView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CHSTPN_SYSDoc* CHSTPN_SYSView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHSTPN_SYSDoc)));
	return (CHSTPN_SYSDoc*)m_pDocument;
}
#endif //_DEBUG


// CHSTPN_SYSView 消息处理程序


void CHSTPN_SYSView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//====================================创建模型（库所或变迁）=========Begin=====
	// 当点击鼠标左键时，创建模型（库所或变迁）
	CreatePlaceorTrans(u_whichModel,point);
	u_whichModel = 0;
	//====================================创建模型（库所或变迁）=========End=======
	CScrollView::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////
// 消息响应函数
// 接收来自MainFrame发生的消息WM_MESSAGE_CREATEMODEL2，
// 目的：接收参数u_whichModel，从而告诉View类那种类型的模型将被创建
///////////////////////////////////////////////////////////////////
LRESULT CHSTPN_SYSView::OnCreateModel2(WPARAM wParam, LPARAM lParam)
{
	u_whichModel = wParam;
	return 0;
}
//==================================================================说明==
// 消息响应函数
// 接收来自MainFrame发生的消息WM_MESSAGE_MOVEMODEL2，
// 目的：接收参数u_whichSubscript，从而告诉View类哪一个模型可以被拖动
//       同时，进行画线操作时，根据传递过来的值判断判断是哪两个模型被连接
//===================================================================
LRESULT CHSTPN_SYSView::OnMoveModel2(WPARAM wParam, LPARAM lParam)
{
	u_whichSubscript = wParam;
	u_whichKindModel = lParam;
	// 显示选中状态
	ShowFocus(u_whichKindModel,u_whichSubscript);
	//------画线-------------------------------------------begin1-----------------
	// 判断为画线状态时，根据开始模型的类型，获取模型的连接开始点
	if(u_whichModel==8)
	{
		b_DrawLine = TRUE;//表示画线
		if(u_whichKindModel==1)//开始模型为库所
		{
			vector<CPlace*>::iterator iterp = FindPlace(u_whichSubscript);
			pt_StartDwLn[0] = (*iterp)->pt1;
			pt_StartDwLn[1] = (*iterp)->pt2;
			pt_StartDwLn[2] = (*iterp)->pt3;
			pt_StartDwLn[3] = (*iterp)->pt4;
			ScreenToClient(&pt_StartDwLn[0]);
			ScreenToClient(&pt_StartDwLn[1]);
			ScreenToClient(&pt_StartDwLn[2]);
			ScreenToClient(&pt_StartDwLn[3]);
			u_subofModel_Arrow = u_whichSubscript;
			u_PlaceorTrans_Arrow = u_whichKindModel;
		}
		if(u_whichKindModel==2)//开始模型为变迁
		{
			vector<CTransition*>::iterator itert = FindTrans(u_whichSubscript);
			pt_StartDwLn[0] = (*itert)->pt1;
			pt_StartDwLn[1] = (*itert)->pt2;
			pt_StartDwLn[2] = 0;
			pt_StartDwLn[3] = 0;
			ScreenToClient(&pt_StartDwLn[0]);
			ScreenToClient(&pt_StartDwLn[1]);
			ScreenToClient(&pt_StartDwLn[2]);
			ScreenToClient(&pt_StartDwLn[3]);
			u_subofTrans_Arrow = u_whichSubscript;
			u_PlaceorTrans_Arrow = u_whichKindModel;
		}
	}
	else
	{ 
		b_DrawLine = FALSE;
	}
	//------画线-------------------------------------------end1-----------------
	//=====================================================================================Drag1==Begin==
	// 根据u_whichKindModel和u_whichSubscript两个参数，判断是哪个模型将被拖动
	// 调用DragBegin和DragEnter方法开始移动模型
	if(u_whichModel==0)
	{
		CRect rectPic;
		POINT ptPut = m_ptLBtnDown;
		CBitmap		bitmapTemp, *pOldBitmap;
		CBitmap bmp;
		CDC	*pDC;
		CDC	*pMemDC	= new CDC;
		vector<CPlace*>::iterator iterp;
		vector<CTransition*>::iterator itert;
		vector<CCTRLShowMatrix*>::iterator itero;
		switch(u_whichKindModel)
		{	
			case 1:
				iterp = FindPlace(u_whichSubscript);
				pDC	= (*iterp)->GetDC();
				
				(*iterp)->GetWindowRect(&rectPic);//获取模型在屏幕中的矩形位置
				(*iterp)->ScreenToClient(&rectPic);//将模型在屏幕中的矩形位置转换到客户区
				(*iterp)->MapWindowPoints(this,&rectPic);//将模型在客户区中的位置装换到View类
				
				m_ptOffset.x	= ptPut.x-rectPic.left;
				m_ptOffset.y	= ptPut.y-rectPic.top;

				break;
			case 2:
				itert = FindTrans(u_whichSubscript);
				pDC	= (*itert)->GetDC();	

				(*itert)->GetWindowRect(&rectPic);//获取模型在屏幕中的矩形位置
				(*itert)->ScreenToClient(&rectPic);//将模型在屏幕中的矩形位置转换到客户区
				(*itert)->MapWindowPoints(this,&rectPic);//将模型在客户区中的位置装换到View类
				
				m_ptOffset.x	= ptPut.x-rectPic.left;
				m_ptOffset.y	= ptPut.y-rectPic.top;
				break;
			case 3:
				itero = FindOuts(u_whichSubscript);
				pDC = (*itero)->GetDC();

				(*itero)->GetWindowRect(&rectPic);
				(*itero)->ScreenToClient(&rectPic);
				(*itero)->MapWindowPoints(this,&rectPic);
				
				m_ptOffset.x	= ptPut.x-rectPic.left;
				m_ptOffset.y	= ptPut.y-rectPic.top;
			default:
				break;
		}
		//创建位图内存
		bitmapTemp.CreateCompatibleBitmap(pDC, rectPic.Width(), rectPic.Height());
		pMemDC->CreateCompatibleDC(pDC);
		pOldBitmap	= pMemDC->SelectObject(&bitmapTemp);
		pMemDC->BitBlt(0, 0, rectPic.Width(), rectPic.Height(), pDC, 0, 0, SRCCOPY);
		pMemDC->SelectObject(pOldBitmap);
		delete	pMemDC;
		ReleaseDC(pDC);
		ReleaseDC(pMemDC);

		m_bIsLButtonDown	= TRUE;

//		m_ptOffset.x	= 0;//ptPut.x-rectPic.left;
//		m_ptOffset.y	= 0;//ptPut.y-rectPic.top;
		ClientToScreen(&ptPut);//得到相对于屏幕的坐标

		m_imgDrag.DeleteImageList();
		m_imgDrag.Create(rectPic.Width(), rectPic.Height(), ILC_COLOR32|ILC_MASK, 0, 1);
		m_imgDrag.Add(&bitmapTemp, RGB(1, 0, 0));//此处要设置成RGB(0,0,0)时，看不到拖动时的位图
		m_imgDrag.BeginDrag(0, m_ptOffset);//开始拖动图像。0：拖动图像基于零的索引；m_ptOffset:开始拖动处的光标。
		m_imgDrag.DragEnter(NULL, ptPut);//在拖动操作中封锁更新并在确定位置显示拖动图象。NULL：指向拥有拖动图象的窗口的指针。ptPut：显示拖动图象的位置，坐标相对于左上角窗口（而非客户域）。

		SetCapture();//使随后的所有鼠标输入都被发送到当前的CWnd对象，并不考虑光标的位置。
	}
	//=====================================================================================Drag1==End====
	
	
	return 0;
}

BOOL CHSTPN_SYSView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message ==	WM_LBUTTONDOWN)// 模型拖拽中用到的
	{
		m_ptLBtnDown = pMsg->pt;
		ScreenToClient(&m_ptLBtnDown);
	}
	return CScrollView::PreTranslateMessage(pMsg);
}


void CHSTPN_SYSView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CScrollView::OnRButtonDown(nFlags, point);
}

//设置光标类型
BOOL CHSTPN_SYSView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(u_whichModel == 0 || u_whichModel == -1 || u_whichModel == -2)
	{
		SetCursor(LoadCursor(NULL,IDC_ARROW));
	}

	bool b_whichModelCreate = (u_whichModel ==1 || u_whichModel ==2 || u_whichModel ==3|| u_whichModel ==4 || 
		                       u_whichModel ==5|| u_whichModel ==6 || u_whichModel ==7 || u_whichModel == 8);
	if(b_whichModelCreate) 
	{
		SetCursor(LoadCursor(NULL,IDC_CROSS));
	}
	return TRUE;

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CHSTPN_SYSView::OnDrawLineend2(WPARAM wParam, LPARAM lParam)
{
	u_whichSubscript = wParam;
	u_whichKindModel = lParam;

	//=========================画线=================================================Begin2==
	if(b_DrawLine && u_whichModel == 8)//若处于画线状态，则获取结束点，并画线
	{
		CDC *dc = GetDC();

		CPoint pt_start;
		CPoint pt_end;
		if(u_whichKindModel==1)//结束模型为库所
		{
			if(u_PlaceorTrans_Arrow == 2)//如果开始模型为变迁
			{
				vector<CPlace*>::iterator iterp=FindPlace(u_whichSubscript);
				pt_EndDwLn[0] = (*iterp)->pt1;
				pt_EndDwLn[1] = (*iterp)->pt2;
				pt_EndDwLn[2] = (*iterp)->pt3;
				pt_EndDwLn[3] = (*iterp)->pt4;
				ScreenToClient(&pt_EndDwLn[0]);
				ScreenToClient(&pt_EndDwLn[1]);
				ScreenToClient(&pt_EndDwLn[2]);
				ScreenToClient(&pt_EndDwLn[3]);
				if(pt_StartDwLn[0].x < pt_EndDwLn[0].x)
				{
					pt_start = pt_StartDwLn[1];
					pt_end   = pt_EndDwLn[0];
				}
				else
				{
					pt_start = pt_StartDwLn[0];
					pt_end = pt_EndDwLn[2];
				}
				// 创建箭头对象，用于下面的保存-------------------------------------------------
				CArrow *m_arrow = new CArrow(u_whichSubscript,u_subofTrans_Arrow,2, pt_start, pt_end, u_arrow_id, false);
				// 保存箭头（画线对象）
				m_pDoc->arr_Arrow.Add(m_arrow);
				//------------------------------------------------------------------------------
				// 连线完成后，更新连接线两端模型的前置和后置-----------------------------------
				vector<CTransition*>::iterator itert=FindTrans(u_subofTrans_Arrow);
				(*itert)->arryTransPst.push_back(*iterp);
				(*iterp)->arryPlacePre.push_back(*itert);
				(*itert)->arryArrowPst.push_back(m_arrow);//变迁的后置有向弧
				(*iterp)->arryArrowPre.push_back(m_arrow);//库所的前置有向弧
				//------------------------------------------------------------------------------
				u_arrow_id += 1;
			}
			else
			{
				MessageBox(L"被连接模型应为变迁",L"错误",MB_ICONERROR);
			}		
		}
		if(u_whichKindModel==2)//结束模型为变迁
		{
			if(u_PlaceorTrans_Arrow == 1)//如果开始模型为库所
			{
				vector<CTransition*>::iterator itert=FindTrans(u_whichSubscript);
				pt_EndDwLn[0] = (*itert)->pt1;
				pt_EndDwLn[1] = (*itert)->pt2;
				pt_EndDwLn[2] = 0;
				pt_EndDwLn[3] = 0;
				ScreenToClient(&pt_EndDwLn[0]);
				ScreenToClient(&pt_EndDwLn[1]);
				ScreenToClient(&pt_EndDwLn[2]);
				ScreenToClient(&pt_EndDwLn[3]);
				if(pt_StartDwLn[0].x < pt_EndDwLn[0].x)
				{
					pt_start = pt_StartDwLn[2];
					pt_end   = pt_EndDwLn[0];
				}
				else
				{
					pt_start = pt_StartDwLn[0];
					pt_end = pt_EndDwLn[1];
				}
				// 创建箭头对象，用于下面的保存-------------------------------------------------
				vector<CPlace*>::iterator iterp = FindPlace(u_subofModel_Arrow);
				if(((*iterp)->PlaceStyle == STYLE_DISIC )||((*iterp)->PlaceStyle == STYLE_RAND))
				{
					CArrow *m_arrow = new CArrow(u_subofModel_Arrow,u_whichSubscript,1,pt_start,pt_end,u_arrow_id,true);
					// 保存箭头（画线对象）
					m_pDoc->arr_Arrow.Add(m_arrow);
					//------------------------------------------------------------------------------
					// 连线完成后，更新连接线两端模型的前置和后置----------------------------------
					(*iterp)->arryPlacePst.push_back(*itert);
					(*itert)->arryTransPre.push_back(*iterp);
					(*iterp)->arryArrowPst.push_back(m_arrow);//库所的后置有向弧
					(*itert)->arryArrowPre.push_back(m_arrow);//变迁的前置有向弧
					//------------------------------------------------------------------------------
				}
				else
				{
					CArrow *m_arrow = new CArrow(u_subofModel_Arrow,u_whichSubscript,1,pt_start,pt_end,u_arrow_id,false);
					// 保存箭头（画线对象）
				    m_pDoc->arr_Arrow.Add(m_arrow);
				    //------------------------------------------------------------------------------
				    // 连线完成后，更新连接线两端模型的前置和后置-----------------------------------
				
				    (*iterp)->arryPlacePst.push_back(*itert);
				    (*itert)->arryTransPre.push_back(*iterp);
				    (*iterp)->arryArrowPst.push_back(m_arrow);//库所的后置有向弧
				    (*itert)->arryArrowPre.push_back(m_arrow);//变迁的前置有向弧
				    //------------------------------------------------------------------------------
				}
				u_arrow_id += 1;
			}
			else
			{
				MessageBox(L"被连接模型应为库所",L"错误",MB_ICONERROR);
			}
		}

		dc->MoveTo(pt_start);
		dc->LineTo(pt_end);

		DrawArrow(pt_start,pt_end/*,dc*/);

		b_DrawLine = FALSE;//退出画线状态

		u_whichModel = 0;//同样是退出画线状态的作用
		u_PlaceorTrans_Arrow = 0;//标识（连接线开始模型的类型）恢复初始化状态

		this->Invalidate();
		this->UpdateWindow();

		bPNisChanged = TRUE;

		ReleaseDC(dc);
		
	}
	//==========================画线====================================================End2==
	return 0;
}
//=======================
// 在完成库所和变迁间的连线后，在连接线的结束点画上箭头
//=======================
void CHSTPN_SYSView::DrawArrow(CPoint pt_start, CPoint pt_end/*, CDC * pDC*/)
{
	//==============Arrow=========Begin==
	CClientDC pDC(this);
	CPen pen1(PS_SOLID,1,RGB(0,0,240));
	pDC.SelectObject(&pen1);
	CBrush brush1(RGB(0,0,128));
	pDC.SelectObject(&brush1);
	// 定义视口原点，解决绘图错位问题
	OnPrepareDC(&pDC);

	//箭头
	double arrowangle = 3.14159/10;//3.14159/6;//定义点头的角度
	int arrow_size = 10;//定义点头的大小
	double theta = atan2((double)(abs(pt_end.y-pt_start.y)),(double)(abs(pt_end.x-pt_start.x)));
	CPoint ptend1;
	CPoint ptend2;
	if(pt_start.x < pt_end.x)
	{			
		if(theta<=arrowangle)
		{
			if((pt_end.y-pt_start.y)<=0)
			{					
				ptend1.x = (LONG)(pt_end.x - arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend1.y = (LONG)(pt_end.y + arrow_size*cos(3.14159/2-theta-arrowangle));					
				ptend2.x = (LONG)(pt_end.x - arrow_size*cos(arrowangle-theta));
				ptend2.y = (LONG)(pt_end.y - arrow_size*sin(arrowangle-theta));
			}
			else
			{
				ptend1.x = (LONG)(pt_end.x - arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend1.y = (LONG)(pt_end.y - arrow_size*cos(3.14159/2-theta-arrowangle));
				ptend2.x = (LONG)(pt_end.x - arrow_size*cos(arrowangle-theta));
				ptend2.y = (LONG)(pt_end.y + arrow_size*sin(arrowangle-theta));
			}
		}
		else
		{
			if((pt_end.y-pt_start.y)>0)
			{
				ptend1.x = (LONG)(pt_end.x - arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend1.y = (LONG)(pt_end.y - arrow_size*cos(3.14159/2-theta-arrowangle));
				ptend2.x = (LONG)(pt_end.x - arrow_size*cos(theta-arrowangle));
				ptend2.y = (LONG)(pt_end.y - arrow_size*sin(theta-arrowangle));
			}
			else
			{
				ptend1.x = (LONG)(pt_end.x - arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend1.y = (LONG)(pt_end.y + arrow_size*cos(3.14159/2-theta-arrowangle));
				ptend2.x = (LONG)(pt_end.x - arrow_size*cos(theta-arrowangle));
				ptend2.y = (LONG)(pt_end.y + arrow_size*sin(theta-arrowangle));
			}
		}
	}
	else
	{
		if(theta<=arrowangle)
		{
			if(pt_end.y<=pt_start.y)
			{
				ptend1.x = (LONG)(pt_end.x + arrow_size*cos(arrowangle-theta));
				ptend1.y = (LONG)(pt_end.y - arrow_size*sin(arrowangle-theta));
				ptend2.x = (LONG)(pt_end.x + arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend2.y = (LONG)(pt_end.y + arrow_size*cos(3.14159/2-theta-arrowangle));
			}
			else
			{
				ptend1.x = (LONG)(pt_end.x + arrow_size*cos(arrowangle-theta));
				ptend1.y = (LONG)(pt_end.y + arrow_size*sin(arrowangle-theta));
				ptend2.x = (LONG)(pt_end.x + arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend2.y = (LONG)(pt_end.y - arrow_size*cos(3.14159/2-theta-arrowangle));
			}
		}
		else
		{
			if(pt_end.y<pt_start.y)
			{
				ptend1.x = (LONG)(pt_end.x + arrow_size*cos(theta-arrowangle));
				ptend1.y = (LONG)(pt_end.y + arrow_size*sin(theta-arrowangle));
				ptend2.x = (LONG)(pt_end.x + arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend2.y = (LONG)(pt_end.y + arrow_size*cos(3.14159/2-theta-arrowangle));
			}
			else
			{
				ptend1.x = (LONG)(pt_end.x + arrow_size*cos(theta-arrowangle));
				ptend1.y = (LONG)(pt_end.y - arrow_size*sin(theta-arrowangle));
				ptend2.x = (LONG)(pt_end.x + arrow_size*sin(3.14159/2-theta-arrowangle));
				ptend2.y = (LONG)(pt_end.y - arrow_size*cos(3.14159/2-theta-arrowangle));
			}
		}
	}
		pDC.MoveTo(pt_end);
		pDC.LineTo(ptend1);
		pDC.MoveTo(pt_end);
		pDC.LineTo(ptend2);

		pDC.MoveTo(ptend1);
		pDC.LineTo(ptend2);


		CPoint pt[3] = {pt_end,ptend1, ptend2};

		HRGN hThree = CreatePolygonRgn(pt,3,ALTERNATE);
		CRgn *rgnthree = CRgn::FromHandle(hThree);
		CBrush newbrush(RGB(0,0,0));
		CBrush *oldbrush;
		oldbrush = pDC.SelectObject(&newbrush);
		pDC.FillRgn(rgnthree,&newbrush);
		pDC.SelectObject(oldbrush);
		DeleteObject(hThree);
		//ReleaseDC(pDC);
		//==============Arrow=========End==
}


void CHSTPN_SYSView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//=====================================================================================Drag2==Begin====
	// 当鼠标移动时，调用DragMove方法使模型对应的位图移动
	if(u_whichModel==0)
	{
		if(m_bIsLButtonDown)
		{
			CRect		rtClient, rtPicture;

			m_ptMove	= point;

			vector<CPlace*>::iterator iterp;
			vector<CTransition*>::iterator itert;
			vector<CCTRLShowMatrix*>::iterator itero;
			switch(u_whichKindModel)
			{	
				case 1:
					iterp = FindPlace(u_whichSubscript);
					(*iterp)->GetWindowRect(rtPicture);
					break;
				case 2:
					itert = FindTrans(u_whichSubscript);
					(*itert)->GetWindowRect(rtPicture);
					break;
				case 3:
					itero = FindOuts(u_whichSubscript);
					(*itero)->GetWindowRect(rtPicture);
				default:
					break;
			}
			GetClientRect(rtClient);
			ClientToScreen(&rtClient);
			ClientToScreen(&m_ptMove);
			// 以下四句判断的目的是：防止图像被拖动到对话框以外的区域
			if(rtClient.left>m_ptMove.x-m_ptOffset.x)
				m_ptMove.x	= rtClient.left+m_ptOffset.x;
			if(rtClient.top>m_ptMove.y-m_ptOffset.y)
				m_ptMove.y	= rtClient.top+m_ptOffset.y;
			if(rtClient.right-rtPicture.Width()<m_ptMove.x-m_ptOffset.x)
				m_ptMove.x	= rtClient.right-rtPicture.Width()+m_ptOffset.x;
			if(rtClient.bottom-rtPicture.Height()<m_ptMove.y-m_ptOffset.y)
				m_ptMove.y	= rtClient.bottom-rtPicture.Height()+m_ptOffset.y;

			CImageList::DragMove(m_ptMove);
		}
	}
	//=====================================================================================Drag2==End====
	CScrollView::OnMouseMove(nFlags, point);
}


void CHSTPN_SYSView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 画线过程中，鼠标左键抬起，画线状态结束
	b_DrawLine = FALSE;
	// 创建模型过程中，鼠标左键抬起，创建状态结束
	u_whichModel = 0;
	//=====================================================================================Drag3==Begin====
	// 当鼠标左键抬起时，通过MoveWindow方法使模型是模型移动，并调用EndDrag方法结束模型的拖动
	if(m_bIsLButtonDown)//之前左键按下
	{
		CRect	rectPic;
		CWnd*	pPic;
		vector<CPlace*>::iterator iterp;
		vector<CTransition*>::iterator itert;
		vector<CCTRLShowMatrix*>::iterator itero;
		switch(u_whichKindModel)
		{	
			case 1:
				iterp = FindPlace(u_whichSubscript);//u_whichSubscript-库所的角标，在库所集中找到该库所
				pPic = (*iterp);
				break;
			case 2:
				itert = FindTrans(u_whichSubscript);
				pPic = (*itert);
				break;
			case 3:
				itero = FindOuts(u_whichSubscript);
				pPic = (*itero);
			default:
				break;
		}

		ScreenToClient(&m_ptMove);
		pPic->GetWindowRect(rectPic);
		pPic->MoveWindow(m_ptMove.x-m_ptOffset.x, m_ptMove.y-m_ptOffset.y, rectPic.Width(), rectPic.Height());
		m_bIsLButtonDown	= FALSE;

		//==================================5.10
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CPoint pt_ViewportOrg;
		pt_ViewportOrg = dc.GetViewportOrg();


		// 更新库所或变迁在视图类中的位置
		switch(u_whichKindModel)
		{	
			case 1:
				(*iterp)->Position.x = m_ptMove.x-m_ptOffset.x - pt_ViewportOrg.x;
				(*iterp)->Position.y = m_ptMove.y-m_ptOffset.y - pt_ViewportOrg.y;
				break;
			case 2:
				(*itert)->Position.x = m_ptMove.x-m_ptOffset.x - pt_ViewportOrg.x;
				(*itert)->Position.y = m_ptMove.y-m_ptOffset.y - pt_ViewportOrg.y;
				break;
			case 3:
				(*itero)->Position.x = m_ptMove.x-m_ptOffset.x - pt_ViewportOrg.x;
				(*itero)->Position.y = m_ptMove.y-m_ptOffset.y - pt_ViewportOrg.y;
				break;
			default:
				break;
		}

		CImageList::DragLeave(this);//解冻窗口并隐藏拖动图象，使窗口能被更新 
		CImageList::EndDrag();//结束一个拖动操作
		ReleaseCapture();//释放前边的SetCapture
		pPic->Invalidate();

		Invalidate();// 当发送一次使窗口无效的消息后，模型间连接线的连接点总是保持在被移动模型的上一个位置
		UpdateWindow();
		Invalidate();// 当手动改变窗口尺寸时，模型间的连接线恢复正常，因此在此处再发送一次窗口重绘消息，使模型间的连接线能够正常显示
		UpdateWindow();

		bPNisChanged = TRUE;// 此时模型被更改，需要弹出提醒对话框

		switch(u_whichKindModel)//当库所或变迁被拖动的时候，移动编辑框与有向弧中点保持一致
		{
		case 1:
			/*for(vector<CArrow*>::iterator itera= (*iterp)->arryArrowPre.begin(); itera!=(*iterp)->arryArrowPre.end(); itera++)
			{
			(*itera)->pt_end.x = (*iterp)->Position.x;
			(*itera)->pt_end.y = (*iterp)->Position.y;
			(*itera)->m_pEdit->MoveWindow(((*itera)->pt_start.x + (*itera)->pt_end.x)/2,((*itera)->pt_start.y + (*itera)->pt_end.y)/2,15,20);
			}*/
			for(vector<CArrow*>::iterator itera= (*iterp)->arryArrowPst.begin(); itera!=(*iterp)->arryArrowPst.end(); itera++)
			{
				if((*itera)->is_disic==true)
				{
					(*itera)->pt_start.x = (*iterp)->Position.x;
					(*itera)->pt_start.y = (*iterp)->Position.y;
					(*itera)->m_pEdit->MoveWindow(((*itera)->pt_start.x + (*itera)->pt_end.x)/2,((*itera)->pt_start.y + (*itera)->pt_end.y)/2,15,20);
				}
			}
			break;
		case 2:
			for(vector<CArrow*>::iterator itera= (*itert)->arryArrowPre.begin(); itera!=(*itert)->arryArrowPre.end(); itera++)
			{
				if((*itera)->is_disic==true)
				{
					(*itera)->pt_end.x = (*itert)->Position.x;
					(*itera)->pt_end.y = (*itert)->Position.y;
					(*itera)->m_pEdit->MoveWindow(((*itera)->pt_start.x + (*itera)->pt_end.x)/2,((*itera)->pt_start.y + (*itera)->pt_end.y)/2,15,20);
				}
			}
			/*for(vector<CArrow*>::iterator itera= (*itert)->arryArrowPst.begin(); itera!=(*itert)->arryArrowPst.end(); itera++)
			{
			(*itera)->pt_start.x = (*itert)->Position.x;
			(*itera)->pt_start.y = (*itert)->Position.y;
			(*itera)->m_pEdit->MoveWindow(((*itera)->pt_start.x + (*itera)->pt_end.x)/2,((*itera)->pt_start.y + (*itera)->pt_end.y)/2,15,20);
			}*/
			break;
		case 3:
			break;
		}
	}
	//=====================================================================================Drag3==End====
	CScrollView::OnLButtonUp(nFlags, point);
}

// 拖拽实现用到的函数
void CHSTPN_SYSView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CScrollView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	if(nState==WA_INACTIVE)
	{
		m_bIsLButtonDown	= FALSE;

		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();
	}
}


void CHSTPN_SYSView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Invalidate();
	UpdateWindow();
	Invalidate();
	UpdateWindow();
}


HBRUSH CHSTPN_SYSView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	pDC->SetBkMode(TRANSPARENT);
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;
	return brush_Bk;
}

// 获取步长和仿真时间
LRESULT CHSTPN_SYSView::OnGetTimeStep(WPARAM wParam, LPARAM lParam)
{
	CString str = ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_Edit_TimeStep->GetContentsAll(IDR_EDITTIMESTEP);
	char * str_step;
	USES_CONVERSION;
    //调用函数，T2A和W2A均支持ATL和MFC中的字符转换
    str_step = T2A(str);
	d_TimeStep = atof(str_step);

	CString str1 = ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_Edit_SimTime->GetContentsAll(IDR_EDITSIMTIME);
	if(str1 == "INF")
	{
		d_SimTime = DBL_MAX;
	}
	else
	{
		char * str_simtime;
		str_simtime = T2A(str1);
		d_SimTime = atof(str_simtime);
	}

	CString str2 = ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_Edit_IntervalTime->GetContentsAll(IDR_EDITINTERVALTIME);
	char * str_intervaltime;
	str_intervaltime = T2A(str2);
	d_IntervalTime = atof(str_intervaltime);

	
	this->Invalidate();
	this->UpdateWindow();

	return 0;
}


// 当用户点击“保存Petri网设置”按钮，响应该函数，保存当前Petri网的仿真设置
void CHSTPN_SYSView::OnSaveCurrentPNSet()
{
	// TODO: 在此添加命令处理程序代码
	CDlgSaveCurrentState m_dlgnotice;
	if(IDOK == m_dlgnotice.DoModal())
	{
		for(vector<IOValue*>::iterator iterg=m_pDoc->vec_IOputDataG.begin();iterg!=m_pDoc->vec_IOputDataG.end();iterg++)
		{
			delete (*iterg);
		}
		for(vector<CPlace*>::iterator iterP=m_pDoc->vec_Model_saveInitSet.begin();iterP!=m_pDoc->vec_Model_saveInitSet.end();iterP++)
		{
			for(vector<IOValue*>::iterator itero=(*iterP)->arryIOputData.begin();itero!=(*iterP)->arryIOputData.end();itero++)
			{
				delete (*itero);
			}
			delete (*iterP);
		}
		for(vector<CTransition*>::iterator iterT=m_pDoc->vec_Trans_saveInitSet.begin();iterT!=m_pDoc->vec_Trans_saveInitSet.end();iterT++)
		{
			delete (*iterT);
		}
		m_pDoc->vec_IOputDataG.clear();
		for(vector<IOValue*>::iterator iterg=m_pDoc->arryIOputDataG.begin();iterg!=m_pDoc->arryIOputDataG.end();iterg++)
		{
			IOValue* iovalue = new IOValue();
			iovalue->Value   = (*iterg)->Value;
			iovalue->InitValue = (*iterg)->InitValue;
			m_pDoc->vec_IOputDataG.push_back(iovalue);
		}
		m_pDoc->vec_Model_saveInitSet.clear();
		m_pDoc->vec_Trans_saveInitSet.clear();
		for(vector<CPlace*>::iterator iterP=m_pDoc->Vector_Place.begin();iterP!=m_pDoc->Vector_Place.end();iterP++)
		{
			CPlace * m_place = new CPlace();
			m_place->bActivation = (*iterP)->bActivation;
			m_place->bEnable     = false;
			m_place->bisAssigned = false;
			m_place->dProblty    = (*iterP)->dProblty;
			m_place->dTime       = (*iterP)->dTime;
			m_place->d_TimeTS    = (*iterP)->d_TimeTS;
			for(vector<IOValue*>::iterator itero=(*iterP)->arryIOputData.begin();itero!=(*iterP)->arryIOputData.end();itero++)
			{
				IOValue * iovalue = new IOValue();
				iovalue->Value = (*itero)->Value;
				iovalue->InitValue = (*itero)->InitValue;
				m_place->arryIOputData.push_back(iovalue);
			}
			m_pDoc->vec_Model_saveInitSet.push_back(m_place);
		}
		for(vector<CTransition*>::iterator iterT=m_pDoc->Vector_Trans.begin();iterT!=m_pDoc->Vector_Trans.end();iterT++)
		{
			CTransition * m_trans = new CTransition();
			m_trans->bFire = (*iterT)->bFire;
			m_pDoc->vec_Trans_saveInitSet.push_back(m_trans);
		}
	}

}

// 当用户点击“恢复Petri网设置”按钮，响应该函数，保存当前Petri网的仿真设置
void CHSTPN_SYSView::OnRestorePnset()
{
	// TODO: 在此添加命令处理程序代码
	CDlgResetState m_dlgnotice;
	if(IDOK == m_dlgnotice.DoModal())
	{
		// 当点击Reset按钮后，先将进度条设置回0状态
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.EnablePaneProgressBar(1,1,true);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.SetPaneProgress(1,0,true);
		int n_countG = 0;
		for(vector<IOValue*>::iterator iterg=m_pDoc->vec_IOputDataG.begin();iterg!=m_pDoc->vec_IOputDataG.end();iterg++)
		{
			m_pDoc->arryIOputDataG.at(n_countG)->Value = (*iterg)->InitValue;
			m_pDoc->arryIOputDataG.at(n_countG)->InitValue = (*iterg)->InitValue;
			n_countG++;
		}
		//然后进行参数的恢复
		int n_countP = 0; //定义两个计数器
		int n_countT = 0;
		for(vector<CPlace*>::iterator iterp = m_pDoc->vec_Model_saveInitSet.begin();iterp!=m_pDoc->vec_Model_saveInitSet.end();iterp++)
		{
			m_pDoc->Vector_Place.at(n_countP)->bActivation = (*iterp)->bActivation;
			m_pDoc->Vector_Place.at(n_countP)->bEnable     = false;
			m_pDoc->Vector_Place.at(n_countP)->bisAssigned = false;
			m_pDoc->Vector_Place.at(n_countP)->dProblty    = (*iterp)->dProblty;
			m_pDoc->Vector_Place.at(n_countP)->dTime       = (*iterp)->dTime;
			m_pDoc->Vector_Place.at(n_countP)->d_TimeTS    = (*iterp)->d_TimeTS;
			int n_o = 0;
			for(vector<IOValue*>::iterator itero=(*iterp)->arryIOputData.begin();itero!=(*iterp)->arryIOputData.end();itero++)
			{
				m_pDoc->Vector_Place.at(n_countP)->arryIOputData.at(n_o)->Value = (*itero)->InitValue;
				m_pDoc->Vector_Place.at(n_countP)->arryIOputData.at(n_o)->InitValue = (*itero)->InitValue;
				n_o++;
			}

			m_pDoc->Vector_Place.at(n_countP)->Invalidate();
			m_pDoc->Vector_Place.at(n_countP)->UpdateWindow();
			n_countP++;
		}
		for(vector<CTransition*>::iterator itert=m_pDoc->vec_Trans_saveInitSet.begin();itert!=m_pDoc->vec_Trans_saveInitSet.end();itert++)
		{
			m_pDoc->Vector_Trans.at(n_countT)->bFire = (*itert)->bFire;
			m_pDoc->Vector_Trans.at(n_countT)->Invalidate();
			m_pDoc->Vector_Trans.at(n_countT)->UpdateWindow();
			n_countT++;
		}
		for(vector<CCTRLShowMatrix*>::iterator iterO=m_pDoc->Vector_OutputModel.begin();iterO!=m_pDoc->Vector_OutputModel.end();iterO++)
		{
			for(vector<VEC_DOUBLE*>::iterator itervv=(*iterO)->vec_OutputData.begin();itervv!=(*iterO)->vec_OutputData.end();itervv++)
			{
				for(vector<double*>::iterator iterv=(*itervv)->begin();iterv!=(*itervv)->end();iterv++)
				{
					delete (*iterv);
				}
				(*itervv)->clear();
				//delete (*itervv);
			}
		}
	}
}

//根据条件创建模型（库所或变迁）
void CHSTPN_SYSView::CreatePlaceorTrans(int u_whichModel, CPoint point)
{
	//=====================================================================================================================Bigin==
	//------------------在鼠标点击View类时，如果可创建模型的标识u_whichModel==1,2,3,4,5,6,7--则在鼠标点击的位置创建一个模型1
	//-定义模型的大小

	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint pt_ViewportOrg;
	pt_ViewportOrg = dc.GetViewportOrg();// 获取设备坐标原点

	//-定义模型可以被创建的条件为 u_whichModel==1或2或3或4或5或6或7
	bool b_whichModelCreate = (u_whichModel==1 || u_whichModel==2 || u_whichModel==3 || u_whichModel==4 ||  
		                    u_whichModel==5 || u_whichModel==6 || u_whichModel==7 || u_whichModel == 9 || u_whichModel == 10
							|| u_whichModel==11||u_whichModel == 12);
	//-当满足创建条件时，在鼠标点击位置创建模型1
	if(b_whichModelCreate)
	{
		//--根据u_whichModel的值选择要创建的模型的类型1
		if(b_whichModelCreate && u_whichModel!=7 && u_whichModel!=9 && u_whichModel!=10 && u_whichModel != 12)
		{
			CPlace *m_Model = new CPlace();//在堆中为对象分配内存1
			switch(u_whichModel)
			{
			case 1:
				{
					m_Model->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,this,STYLE_DISCRETE);
					m_Model->m_thisModel = m_Model;
				}
				break;
			case 2:
				{
					m_Model->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,this,STYLE_CNTNUS);
					m_Model->m_thisModel = m_Model;
				}
				break;
			case 3:
				{
					m_Model->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,this,STYLE_RAND);
					m_Model->m_thisModel = m_Model;
				}
				break;
			case 4:
				{
					m_Model->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,this,STYLE_TIME);
					m_Model->m_thisModel = m_Model;
				}
				break;
			case 5:
				{
					m_Model->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,this,STYLE_TCTRL);
					m_Model->m_thisModel = m_Model;
				}
				break;
			case 6:
				{
					m_Model->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,this,STYLE_LCTRL);
					m_Model->m_thisModel = m_Model;
				}
				break;
			case 11:
				{
					m_Model->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,this,STYLE_DISIC);
					m_Model->m_thisModel = m_Model;
				}
				break;
			default:
				break;
			}
			// 通过MoveWindow方法，将模型移动到鼠标点击处1
			m_Model->MoveWindow(point.x-rectofModel.Width()/2,point.y-rectofModel.Height()/2,rectofModel.Width(),rectofModel.Height());
		//	m_Model->OnPaint();//能不能在此处画控件
			m_Model->ShowWindow(SW_SHOW);                                // 显示模型1???显示什么模型		
			m_Model->u_SubscriptofModel = u_SubScriptofPlace;            // 创建完成后，将下标赋值给模型1
			u_SubScriptofPlace          = u_SubScriptofPlace+1;          // 模型创建完成后，维护的模型变量数组的下标加1，防止模型变量的重复使用1
			//m_Model->Position.x = point.x-rectofModel.Width()/2;         // 记录模型在视图类中的位置1
			//m_Model->Position.y = point.y-rectofModel.Height()/2;
			//================================5.10
			m_Model->Position.x = point.x-rectofModel.Width()/2 - pt_ViewportOrg.x;
			m_Model->Position.y = point.y-rectofModel.Height()/2 - pt_ViewportOrg.y;
			CString str;
			str.Format(_T("%d"),m_Model->u_SubscriptofModel);
			m_Model->m_caption = _T("p")+str;
			// 显示模型的名字
			dc.TextOutW(m_Model->Position.x,m_Model->Position.y-17,m_Model->m_caption);

			m_pDoc->Vector_Place.push_back(m_Model);                             //保存模型

		}
		if(u_whichModel==7)
		{
			CTransition *m_Trans = new CTransition();
			m_Trans->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofTrans,this);

			// 通过MoveWindow方法，将模型移动到鼠标点击处
			m_Trans->MoveWindow(point.x-rectofTrans.Width()/2,point.y-rectofTrans.Height()/2,rectofTrans.Width(),rectofTrans.Height());
			m_Trans->ShowWindow(SW_SHOW);// 显示模型
			m_Trans->u_SubscriptofTrans = u_SubScriptofTrans;// 创建完成后，将下标赋值给模型
			u_SubScriptofTrans = u_SubScriptofTrans+1;// 模型创建完成后，维护的模型变量数组的下标加1，防止模型变量的重复使用
			//m_Trans->Position.x = point.x-rectofTrans.Width()/2;
			//m_Trans->Position.y = point.y-rectofTrans.Height()/2;
			//======================5.10
			m_Trans->Position.x = point.x-rectofTrans.Width()/2 - pt_ViewportOrg.x;
			m_Trans->Position.y = point.y-rectofTrans.Height()/2 - pt_ViewportOrg.y;

			CString str;
			str.Format(_T("%d"),m_Trans->u_SubscriptofTrans);
			m_Trans->m_caption = _T("t")+str;
			// 显示模型的名字
			dc.TextOutW(m_Trans->Position.x,m_Trans->Position.y-17,m_Trans->m_caption);

			m_pDoc->Vector_Trans.push_back(m_Trans);//保存模型
		}

		if(u_whichModel==9 || u_whichModel==10)//++++++++++++5.26
		{
			CCTRLShowMatrix * m_OutputModel = new CCTRLShowMatrix();
			if(u_whichModel==9)
			{
				m_OutputModel->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofOutputModel,this,OUT_SHOWMATRIX);
			}
			else
			{
				m_OutputModel->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofOutputModel,this,OUT_TOFILE);
			}

			m_OutputModel->MoveWindow(point.x-rectofOutputModel.Width()/2,point.y-rectofOutputModel.Height()/2,rectofOutputModel.Width(),rectofOutputModel.Height());
			m_OutputModel->ShowWindow(SW_SHOW);

			m_OutputModel->ID = u_SubScriptofOutputModel;
			u_SubScriptofOutputModel = u_SubScriptofOutputModel + 1;

			m_OutputModel->Position.x = point.x-rectofTrans.Width()/2 - pt_ViewportOrg.x;
			m_OutputModel->Position.y = point.y-rectofTrans.Height()/2 - pt_ViewportOrg.y;

			//CString str;
			//str.Format(_T("%d"),m_OutputModel->ID);
			m_OutputModel->m_caption = _T("From-px");//+str;
			dc.TextOutW(m_OutputModel->Position.x-rectofOutputModel.Width()/2,m_OutputModel->Position.y-23,m_OutputModel->m_caption);

			m_pDoc->Vector_OutputModel.push_back(m_OutputModel);
		}

		////创建封装模块
		//if (u_whichModel == 12)
		//{
		//	CPlace *m_Model = new CPlace();//在堆中为对象分配内存1
		//	m_Model->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY, rectofModel, this,STYLE_PACK);
		//	m_Model->m_thisModel = m_Model;
		//	// 通过MoveWindow方法，将模型移动到鼠标点击处1
		//	m_Model->MoveWindow(point.x - rectofModel.Width() / 2, point.y - rectofModel.Height() / 2, rectofModel.Width(), rectofModel.Height());
		//	//	m_Model->OnPaint();//能不能在此处画控件
		//	m_Model->ShowWindow(SW_SHOW);                                // 显示模型1???显示什么模型		
		//	m_Model->u_SubscriptofModel = u_SubScriptofPlace;            // 创建完成后，将下标赋值给模型1
		//	u_SubScriptofPlace = u_SubScriptofPlace + 1;          // 模型创建完成后，维护的模型变量数组的下标加1，防止模型变量的重复使用1
		//														  //m_Model->Position.x = point.x-rectofModel.Width()/2;         // 记录模型在视图类中的位置1
		//														  //m_Model->Position.y = point.y-rectofModel.Height()/2;
		//														  //================================5.10
		//	m_Model->Position.x = point.x - rectofModel.Width() / 2 - pt_ViewportOrg.x;
		//	m_Model->Position.y = point.y - rectofModel.Height() / 2 - pt_ViewportOrg.y;
		//	CString str;
		//	str.Format(_T("%d"), m_Model->u_SubscriptofModel);
		//	m_Model->m_caption = _T("p") + str;
		//	// 显示模型的名字
		//	dc.TextOutW(m_Model->Position.x, m_Model->Position.y - 17, m_Model->m_caption);

		//	m_pDoc->Vector_Place.push_back(m_Model);
		//}
		//

		bPNisChanged = TRUE;
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);// 目的发送至mainframe，再发送至dlgmodelchoose1，通知HSTPN结构树更新
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);
		/*if(m_pParmListDlg == NULL)// 发送消息值CDlgModelParmList类，控制树的更新
		{
			m_pParmListDlg = new CDlgModelParmList();
			m_pParmListDlg->Create(IDD_DLG_DATALIST,NULL);
		}
		else
		{
			m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);
		}*/
	}
	//===========================================================================================================================End======
}

void CHSTPN_SYSView::OnDestroy()
{
	CScrollView::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	DeletePlcTrnsArcs();
}

// 该函数用于删除Vector_Place/Vector_Trans/arr_Arrow里的元素，并释放内存
void CHSTPN_SYSView::DeletePlcTrnsArcs()
{
	// 释放输入/输出列表
	for(vector<CPlace*>::iterator iterP=m_pDoc->Vector_Place.begin();iterP!=m_pDoc->Vector_Place.end();iterP++)
	{
		for(vector<IOValue*>::iterator iter3=(*iterP)->arryIOputData.begin();iter3!=(*iterP)->arryIOputData.end();iter3++)
		{
			delete (*iter3);
		}
		(*iterP)->arryIOputData.clear();
		//(*iterP)->arryInputData.clear();//--5.27
		(*iterP)->arryInputofLua.clear();
		//(*iterP)->arryOutputData.clear();
		(*iterP)->arryOutputofLua.clear();
	}

	// 释放Vector_Place中元素指向的内存，清空Vector_Place
	for(vector<CPlace*>::iterator iterP=m_pDoc->Vector_Place.begin();iterP!=m_pDoc->Vector_Place.end();iterP++)
	{
		(*iterP)->DestroyWindow();
		delete (*iterP);
	}
	m_pDoc->Vector_Place.clear();
	// 释放Vector_Trans中元素指向的内存，清空Vector_Trans
	for(vector<CTransition*>::iterator iterT=m_pDoc->Vector_Trans.begin();iterT!=m_pDoc->Vector_Trans.end();iterT++)
	{
		(*iterT)->DestroyWindow();
		delete (*iterT);
	}
	m_pDoc->Vector_Trans.clear();
	// 释放arr_Arrow中元素指向的内存，清空arr_Arrow
	int n_SizeArcs = m_pDoc->arr_Arrow.GetSize();
	for(int i=0;i<n_SizeArcs;i++)
	{
		delete (CArrow*)m_pDoc->arr_Arrow.GetAt(i);
	}
	m_pDoc->arr_Arrow.RemoveAll();
	// 将变量恢复初始化
	u_whichSubscript = 0;
	u_SubScriptofPlace = 0;  
	u_SubScriptofTrans = 0;
	b_DrawLine = FALSE;
	m_bIsLButtonDown	= FALSE;
	m_ptOffset.x		= 0;
	m_ptOffset.y		= 0;
	m_ptLBtnDown.x        = 0;
	m_ptLBtnDown.y        = 0;
	u_PlaceorTrans_Arrow = 0;
	myDocument=NULL;
	if(myDocument != NULL)
	{
		myDocument->Clear();// 释放子节点内存
	}
	delete myDocument; // 释放根节点本身
	bPNisChanged = TRUE;

}

// 定义一个CString转换为char*的函数；单独定义的原因：USES_CONVERSION宏会在堆上申请内存，而且不会释放；据说单独定义成函数可以解决该问题
char* CHSTPN_SYSView::CString2char(CString str)
{
	USES_CONVERSION;
	return T2A(str);
}

// 模型删除操作
LRESULT CHSTPN_SYSView::OnMDdelete2(WPARAM wParam, LPARAM lParam)
{
	UINT u_sub = wParam;// 模型ID
	UINT u_which = lParam; // 库所还是变迁

	if(u_which == 1)// 待删除模型为库所
	{
		vector<CPlace*>::iterator iterp = FindPlace(u_sub);// 找到库所在vector_Place中的位置
		// 更新前置变迁的后置库所集
		for(vector<CTransition*>::iterator iter1=(*iterp)->arryPlacePre.begin();iter1!=(*iterp)->arryPlacePre.end();iter1++)//遍历要删除库所的前置变迁集
		{
			for(vector<CPlace*>::iterator iter2=(*iter1)->arryTransPst.begin();iter2!=(*iter1)->arryTransPst.end();iter2++)// 遍历变迁的后置库所集合，找到待删除的库所，删除该元素
			{
				if((*iter2)->u_SubscriptofModel==u_sub)
				{
					(*iter1)->arryTransPst.erase(iter2);
					break;
				}
			}
		}
		// 更新后置变迁的前置库所集
		for(vector<CTransition*>::iterator iter3 = (*iterp)->arryPlacePst.begin();iter3!=(*iterp)->arryPlacePst.end();iter3++)//遍历要删除库所的后置变迁集
		{
			for(vector<CPlace*>::iterator iter4=(*iter3)->arryTransPre.begin();iter4!=(*iter3)->arryTransPre.end();iter4++)// 遍历每个后置变迁的前置库所集合，找到待删除库所，删除该元素
			{
				if((*iter4)->u_SubscriptofModel == u_sub)
				{
					(*iter3)->arryTransPre.erase(iter4);
					break;
				}
			}
		}
		// 更新有向弧集
		int nCountArc = m_pDoc->arr_Arrow.GetSize();
		for(int i= nCountArc-1;i>=0;i--)
		{
			if(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofPlace == u_sub)// 如果有向弧两端连接的模型中有要删除的库所模型，说明该有向弧也应该被删除
			{
				delete ((CArrow*)m_pDoc->arr_Arrow.GetAt(i));
				m_pDoc->arr_Arrow.RemoveAt(i);
			}
		}
		// 释放输入输出列表
		for(vector<IOValue*>::iterator iter3=(*iterp)->arryIOputData.begin();iter3!=(*iterp)->arryIOputData.end();iter3++)
		{
			delete (*iter3);
		}
		(*iterp)->arryIOputData.clear();
		(*iterp)->arryInputofLua.clear();
		(*iterp)->arryOutputofLua.clear();

		// 更新其他库所的输入输出列表，即，如果有库所的输入值是来自被删除库所，则应该将该值在输入输出列表中删除
		for(vector<CPlace*>::iterator iterp1=m_pDoc->Vector_Place.begin();iterp1!=m_pDoc->Vector_Place.end();iterp1++)
		{
			//--现在各个输入输出表中找到待删除值的位置
			vector<int> pos1;
			int count1 = 0;
			for(vector<IOValue*>::iterator iter1=(*iterp1)->arryIOputData.begin();iter1!=(*iterp1)->arryIOputData.end();iter1++)
			{
				if((*iter1)->IOType == "input" && (*iter1)->FromModel==(*iterp)->m_caption)// 如果该值为输入型，且数据源等于待删除库所的名字，那么记录该值的位置
				{
					pos1.push_back(count1);
				}
				count1++;
			}
			
			vector<int> pos4;
			int count4 = 0;
			for(vector<IOValue*>::iterator iter4=(*iterp1)->arryInputofLua.begin();iter4!=(*iterp1)->arryInputofLua.end();iter4++)
			{
				if((*iter4)->IOType == "input" && (*iter4)->FromModel==(*iterp)->m_caption)
				{
					pos4.push_back(count4);
				}
				count4++;
			}
			vector<int> pos5;
			int count5 = 0;
			for(vector<IOValue*>::iterator iter5=(*iterp1)->arryOutputofLua.begin();iter5!=(*iterp1)->arryOutputofLua.end();iter5++)
			{
				if((*iter5)->IOType == "input" && (*iter5)->FromModel==(*iterp)->m_caption)
				{
					pos5.push_back(count5);
				}
				count5++;
			}
			//--依次清除待删除值
			vector<IOValue*>::iterator iter11 = (*iterp1)->arryIOputData.begin();
			int nsize1 = pos1.size();
			for(int i=0;i<nsize1;i++)
			{
				iter11=iter11 + nsize1-1-i;
				delete (*iter11);
				(*iterp1)->arryIOputData.erase(iter11);
				iter11 = (*iterp1)->arryIOputData.begin();
			}
			vector<IOValue*>::iterator iter44 = (*iterp1)->arryInputofLua.begin();
			int nsize4 = pos4.size();
			for(int i=0;i<nsize4;i++)
			{
				iter44=iter44 + nsize4-1-i;
				(*iterp1)->arryInputofLua.erase(iter44);
				iter44 = (*iterp1)->arryInputofLua.begin();
			}
			vector<IOValue*>::iterator iter55 = (*iterp1)->arryOutputofLua.begin();
			int nsize5 = pos5.size();
			for(int i=0;i<nsize5;i++)
			{
				iter55=iter55 + nsize5-1-i;
				(*iterp1)->arryOutputofLua.erase(iter55);
				iter55 = (*iterp1)->arryOutputofLua.begin();
			}
		}

		// 销毁控件，删除模型，释放内存
		(*iterp)->DestroyWindow();
		delete (*iterp);
		m_pDoc->Vector_Place.erase(iterp);

	}

	if(u_which == 2)// 待删除模型为变迁
	{
		vector<CTransition*>::iterator itert = FindTrans(u_sub);// 找到变迁在Vector_Trans中的位置
		for(vector<CPlace*>::iterator iter1=(*itert)->arryTransPre.begin();iter1!=(*itert)->arryTransPre.end();iter1++)// 遍历要删除变迁的前置库所集
		{
			for(vector<CTransition*>::iterator iter2=(*iter1)->arryPlacePst.begin();iter2!=(*iter1)->arryPlacePst.end();iter2++)// 遍历每个前置库所的后置变迁集，找到待删除的变迁，删除该元素
			{
				if((*iter2)->u_SubscriptofTrans==u_sub)
				{
					(*iter1)->arryPlacePst.erase(iter2);
					break;
				}
			}
		}
		for(vector<CPlace*>::iterator iter3=(*itert)->arryTransPst.begin();iter3!=(*itert)->arryTransPst.end();iter3++)// 遍历要删除变迁的后置库所集
		{
			for(vector<CTransition*>::iterator iter4=(*iter3)->arryPlacePre.begin();iter4!=(*iter3)->arryPlacePre.end();iter4++)//遍历每个后置库所的前置变迁集，找到待删除的变迁，删除该元素
			{
				if((*iter4)->u_SubscriptofTrans==u_sub)
				{
					(*iter3)->arryPlacePre.erase(iter4);
					break;
				}
			}
		}
		// 更新有向弧集
		int nCountArc = m_pDoc->arr_Arrow.GetSize();
		for(int i= nCountArc-1;i>=0;i--)
		{
			if(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofTrans == u_sub)// 如果有向弧两端连接的模型中有要删除的库所模型，说明该有向弧也应该被删除
			{
				delete ((CArrow*)m_pDoc->arr_Arrow.GetAt(i));
				m_pDoc->arr_Arrow.RemoveAt(i);
			}
		}

		(*itert)->DestroyWindow();
		delete (*itert);
		m_pDoc->Vector_Trans.erase(itert);
	}

	if(u_which == 3)
	{
		vector<CCTRLShowMatrix*>::iterator iterO = FindOuts(u_sub);
		for(vector<VEC_DOUBLE*>::iterator itervv=(*iterO)->vec_OutputData.begin();itervv!=(*iterO)->vec_OutputData.end();itervv++)
		{
			for(vector<double*>::iterator iterv=(*itervv)->begin();iterv!=(*itervv)->end();iterv++)
			{
				delete (*iterv);
			}
			delete (*itervv);
		}
		(*iterO)->DestroyWindow();
		delete (*iterO);
		m_pDoc->Vector_OutputModel.erase(iterO);
	}

	this->bPNisChanged = true;
	this->Invalidate();
	this->UpdateWindow();
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);// 目的发送至mainframe，再发送至dlgmodelchoose1，通知HSTPN结构树更新
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);
	/*if(m_pParmListDlg == NULL)
	{
		m_pParmListDlg = new CDlgModelParmList();
		m_pParmListDlg->Create(IDD_DLG_DATALIST,NULL);
	}
	else
	{
		m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);
	}*/
	return 0;
}

// 查找库所
vector<CPlace*>::iterator CHSTPN_SYSView::FindPlace( UINT id)
{
	vector<CPlace*>::iterator iter = m_pDoc->Vector_Place.begin(); // 定义一个迭代器
	int nSize = m_pDoc->Vector_Place.size();                       // 获取vector_place的大小
	nSize = (int)floor(nSize/2.0);                              // 定义标志j
	int j = nSize;
	iter = iter + j;                                       // 将迭代器推进到中间位置

	do{
		if(id >= (*iter)->u_SubscriptofModel)                  //要寻找的元素在中间元素的右边
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter + j;
			}
			else
			{
				for(;iter!=m_pDoc->Vector_Place.end();iter++)
				{
					if((*iter)->u_SubscriptofModel == id)
						return iter;
				}
			}
		}
		else                                                   //要寻找的元素在中间元素的左边
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter - j;
			}
			else
			{
				for(;iter!=m_pDoc->Vector_Place.begin();iter--)
				{
					if((*iter)->u_SubscriptofModel == id)
						return iter;
				}
				// 如果倒序变量没有找到，就只剩第一个了，因为第一个没有被遍历到
				vector<CPlace*>::iterator iter1 = m_pDoc->Vector_Place.begin();
				if((*iter1)->u_SubscriptofModel == id)
				{
					return iter1;
				}
			}

		}
	}while(id != (*iter)->u_SubscriptofModel);

	return iter;
}

// 查找变迁
vector<CTransition*>::iterator CHSTPN_SYSView::FindTrans(UINT id)
{
	vector<CTransition*>::iterator iter = m_pDoc->Vector_Trans.begin(); // 定义一个迭代器
	int nSize = m_pDoc->Vector_Trans.size();                       // 获取vector_Trans的大小
	nSize = (int)floor(nSize/2.0);                              // 定义标志j
	int j = nSize;
	iter = iter + j;                                       // 将迭代器推进到中间位置

	do{
		if(id >= (*iter)->u_SubscriptofTrans)                  //要寻找的元素在中间元素的右边
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter + j;
			}
			else
			{
				for(;iter!=m_pDoc->Vector_Trans.end();iter++)
				{
					if((*iter)->u_SubscriptofTrans == id)
						return iter;
				}
			}
		}
		else                                                   //要寻找的元素在中间元素的左边
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter - j;
			}
			else
			{
				for(;iter!=m_pDoc->Vector_Trans.begin();iter--)
				{
					if((*iter)->u_SubscriptofTrans == id)
						return iter;
				}
				// 如果倒序变量没有找到，就只剩第一个了，因为第一个没有被遍历到
				vector<CTransition*>::iterator iter1 = m_pDoc->Vector_Trans.begin();
				if((*iter1)->u_SubscriptofTrans == id)
				{
					return iter1;
				}
			}

		}
	}while(id != (*iter)->u_SubscriptofTrans);

	return iter;
}

void CHSTPN_SYSView::ShowFocus(UINT u_whichKindModel,UINT u_whichSubscript)
{
	// 将控件显示为选择状态
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint pt_ViewportOrg;
	pt_ViewportOrg = dc.GetViewportOrg();

	if(u_whichKindModel==1)
	{
		vector<CPlace*>::iterator iterp = FindPlace(u_whichSubscript);
		(*iterp)->b_ModelFocus = TRUE;
		CRect rectofp((*iterp)->Position.x-5+pt_ViewportOrg.x,(*iterp)->Position.y-5+pt_ViewportOrg.y,(*iterp)->Position.x-5+rectofModel.Width()+10+pt_ViewportOrg.x,(*iterp)->Position.y-5+rectofModel.Height()+10+pt_ViewportOrg.y);
		CClientDC dc(this);

		CPen pen1(PS_DOT,1,RGB(0,168,255));
		dc.SelectObject(&pen1);

		dc.MoveTo(rectofp.left+3,rectofp.top+3);
		dc.LineTo(rectofp.right-3,rectofp.top+3);
		dc.LineTo(rectofp.right-3,rectofp.bottom-3);
		dc.LineTo(rectofp.left+3,rectofp.bottom-3);
		dc.LineTo(rectofp.left+3,rectofp.top+3);

		CPen pen2(PS_SOLID,1,RGB(40,146,201));
		dc.SelectObject(&pen2);
		dc.MoveTo(rectofp.left,rectofp.top+5);
		dc.LineTo(rectofp.left,rectofp.top);
		dc.LineTo(rectofp.left+5,rectofp.top);

		dc.MoveTo(rectofp.right-5,rectofp.top);
		dc.LineTo(rectofp.right,rectofp.top);
		dc.LineTo(rectofp.right,rectofp.top+5);

		dc.MoveTo(rectofp.right,rectofp.bottom-5);
		dc.LineTo(rectofp.right,rectofp.bottom);
		dc.LineTo(rectofp.right-5,rectofp.bottom);

		dc.MoveTo(rectofp.left+5,rectofp.bottom);
		dc.LineTo(rectofp.left,rectofp.bottom);
		dc.LineTo(rectofp.left,rectofp.bottom-5);
	}
	if(u_whichKindModel==2)
	{
		vector<CTransition*>::iterator itert = FindTrans(u_whichSubscript);

		CRect rectofp((*itert)->Position.x-7+pt_ViewportOrg.x,(*itert)->Position.y-7+pt_ViewportOrg.y,(*itert)->Position.x-10+rectofTrans.Width()+15+pt_ViewportOrg.x,(*itert)->Position.y-10+rectofTrans.Height()+15+pt_ViewportOrg.y);
		CClientDC dc(this);
		CPen pen1(PS_DOT,1,RGB(0,168,255));
		dc.SelectObject(&pen1);

		dc.MoveTo(rectofp.left+3,rectofp.top+3);
		dc.LineTo(rectofp.right-3,rectofp.top+3);
		dc.LineTo(rectofp.right-3,rectofp.bottom-3);
		dc.LineTo(rectofp.left+3,rectofp.bottom-3);
		dc.LineTo(rectofp.left+3,rectofp.top+3);

		CPen pen2(PS_SOLID,1,RGB(40,146,201));
		dc.SelectObject(&pen2);
		dc.MoveTo(rectofp.left,rectofp.top+5);
		dc.LineTo(rectofp.left,rectofp.top);
		dc.LineTo(rectofp.left+5,rectofp.top);

		dc.MoveTo(rectofp.right-5,rectofp.top);
		dc.LineTo(rectofp.right,rectofp.top);
		dc.LineTo(rectofp.right,rectofp.top+5);

		dc.MoveTo(rectofp.right,rectofp.bottom-5);
		dc.LineTo(rectofp.right,rectofp.bottom);
		dc.LineTo(rectofp.right-5,rectofp.bottom);

		dc.MoveTo(rectofp.left+5,rectofp.bottom);
		dc.LineTo(rectofp.left,rectofp.bottom);
		dc.LineTo(rectofp.left,rectofp.bottom-5);

	}
}

void CHSTPN_SYSView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	SetScrollSizes(MM_TEXT,CSize(5000,6000));

}
// 查找输出模块
vector<CCTRLShowMatrix*>::iterator  CHSTPN_SYSView::FindOuts(UINT id)
{
	vector<CCTRLShowMatrix*>::iterator iter = m_pDoc->Vector_OutputModel.begin(); // 定义一个迭代器
	int nSize = m_pDoc->Vector_OutputModel.size();                                // 获取Vector_OutputModel的大小
	nSize = (int)floor(nSize/2.0);                                                // 定义标志j
	int j = nSize;
	iter = iter + j;                                                              // 将迭代器推进到中间位置

	do{
		if(id >= (*iter)->ID)                  //要寻找的元素在中间元素的右边
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter + j;
			}
			else
			{
				for(;iter!=m_pDoc->Vector_OutputModel.end();iter++)
				{
					if((*iter)->ID == id)
						return iter;
				}
			}
		}
		else                                                   //要寻找的元素在中间元素的左边
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter - j;
			}
			else
			{
				for(;iter!=m_pDoc->Vector_OutputModel.begin();iter--)
				{
					if((*iter)->ID == id)
						return iter;
				}
				// 如果倒序变量没有找到，就只剩第一个了，因为第一个没有被遍历到
				vector<CCTRLShowMatrix*>::iterator iter1 = m_pDoc->Vector_OutputModel.begin();
				if((*iter1)->ID == id)
				{
					return iter1;
				}
			}

		}
	}while(id != (*iter)->ID);

	return iter;
}

/*// 弹出输入输出列表（模型属性编辑框）
void CHSTPN_SYSView::OnShowpramlist()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pParmListDlg==NULL)
	{
		m_pParmListDlg = new CDlgModelParmList();
		m_pParmListDlg->Create(IDD_DLG_DATALIST,NULL);
	}
	m_pParmListDlg->ShowWindow(SW_SHOW);

}*/
