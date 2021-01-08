
// HSTPN_SYSView.cpp : CHSTPN_SYSView ���ʵ��
//
#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CHSTPN_SYSView ����/����

CHSTPN_SYSView::CHSTPN_SYSView()
{
	// TODO: �ڴ˴���ӹ������
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
	if (!PathIsDirectory(_T("LuaFile")))//�ж��ļ����Ƿ񴴽�
	{
		::CreateDirectory(_T("LuaFile"), NULL);//����һ���ļ���
	}
}

CHSTPN_SYSView::~CHSTPN_SYSView()
{
	if(myDocument != NULL)
	{
		myDocument->Clear();// �ͷ��ӽڵ��ڴ�
	}
	delete myDocument; // �ͷŸ��ڵ㱾��

	// �ͷ�����/����б�
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
	// �ͷ�Vector_Place��ָ��Ԫ�أ����Vector_Place
	for(vector<CPlace*>::iterator iterP=m_pDoc->Vector_Place.begin();iterP!=m_pDoc->Vector_Place.end();iterP++)
	{
		delete (*iterP);
	}
	// �ͷ�Vector_Trans��Ԫ��ָ����ڴ棬���Vector_Trans
	for(vector<CTransition*>::iterator iterT=m_pDoc->Vector_Trans.begin();iterT!=m_pDoc->Vector_Trans.end();iterT++)
	{
		delete (*iterT);
	}
	// �ͷ�arr_Arrow��Ԫ��ָ����ڴ棬���arr_Arrow
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
	// �ͷ�vec_IOputDataG
	for(vector<IOValue*>::iterator iterg=m_pDoc->vec_IOputDataG.begin();iterg!=m_pDoc->vec_IOputDataG.end();iterg++)
	{
		delete (*iterg);
	}
	m_pDoc->vec_IOputDataG.clear();
	// �ͷ�vec_Model_saveInitSet�е��б�
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
	// �ͷ�vec_Model_saveInitSet
	for(vector<CPlace*>::iterator iterP=m_pDoc->vec_Model_saveInitSet.begin();iterP!=m_pDoc->vec_Model_saveInitSet.end();iterP++)
	{
		delete (*iterP);
	}

	// �ͷ�vec_Trans_saveInitSet
	for(vector<CTransition*>::iterator iterT=m_pDoc->vec_Trans_saveInitSet.begin();iterT!=m_pDoc->vec_Trans_saveInitSet.end();iterT++)
	{
		delete (*iterT);
	}


	//if(m_pParmListDlg != NULL)
	//	delete m_pParmListDlg;

}

BOOL CHSTPN_SYSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CHSTPN_SYSView ����

void CHSTPN_SYSView::OnDraw(CDC* pDC)
{
	CHSTPN_SYSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_pDoc = GetDocument();
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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
	// ��ȡ�ӿ�ԭ�㣬�Խ����ͼλ�ô�λ����
	CPoint pt_ViewportOrg;// ����һ���ӿڵ�ԭ��
	pt_ViewportOrg = pDC->GetViewportOrg();

	// ��ÿ���ػ�View��ͼʱ�����»��Ƽ�ͷ
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
		case 1:// ��ʼģ��Ϊ����������ģ��Ϊ��Ǩʱ
			iterp = FindPlace(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofPlace);// �ҵ�������vector_Place�е�λ��
			pt_StartDwLn[0].x = (LONG)((*iterp)->Position.x);// pt1 of P
			pt_StartDwLn[0].y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
			pt_StartDwLn[1].x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt2 of P
			pt_StartDwLn[1].y = (LONG)((*iterp)->Position.y + rectofModel.Height());
			pt_StartDwLn[2].x = (LONG)((*iterp)->Position.x + rectofModel.Width());// pt3 of P
			pt_StartDwLn[2].y = (LONG)((*iterp)->Position.y + rectofModel.Height()/2.0);
			pt_StartDwLn[3].x = (LONG)((*iterp)->Position.x + rectofModel.Width()/2.0);//pt4 of P
			pt_StartDwLn[3].y = (LONG)((*iterp)->Position.y);

			itert = FindTrans(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofTrans);// �ҵ���Ǩ��Vector_Trans�е�λ��
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
		case 2:// ��ʼģ��Ϊ��Ǩ������ģ��Ϊ����ʱ
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


// CHSTPN_SYSView ��ӡ


void CHSTPN_SYSView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHSTPN_SYSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CHSTPN_SYSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CHSTPN_SYSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CHSTPN_SYSView ���

#ifdef _DEBUG
void CHSTPN_SYSView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CHSTPN_SYSView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CHSTPN_SYSDoc* CHSTPN_SYSView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHSTPN_SYSDoc)));
	return (CHSTPN_SYSDoc*)m_pDocument;
}
#endif //_DEBUG


// CHSTPN_SYSView ��Ϣ�������


void CHSTPN_SYSView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//====================================����ģ�ͣ��������Ǩ��=========Begin=====
	// �����������ʱ������ģ�ͣ��������Ǩ��
	CreatePlaceorTrans(u_whichModel,point);
	u_whichModel = 0;
	//====================================����ģ�ͣ��������Ǩ��=========End=======
	CScrollView::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////
// ��Ϣ��Ӧ����
// ��������MainFrame��������ϢWM_MESSAGE_CREATEMODEL2��
// Ŀ�ģ����ղ���u_whichModel���Ӷ�����View���������͵�ģ�ͽ�������
///////////////////////////////////////////////////////////////////
LRESULT CHSTPN_SYSView::OnCreateModel2(WPARAM wParam, LPARAM lParam)
{
	u_whichModel = wParam;
	return 0;
}
//==================================================================˵��==
// ��Ϣ��Ӧ����
// ��������MainFrame��������ϢWM_MESSAGE_MOVEMODEL2��
// Ŀ�ģ����ղ���u_whichSubscript���Ӷ�����View����һ��ģ�Ϳ��Ա��϶�
//       ͬʱ�����л��߲���ʱ�����ݴ��ݹ�����ֵ�ж��ж���������ģ�ͱ�����
//===================================================================
LRESULT CHSTPN_SYSView::OnMoveModel2(WPARAM wParam, LPARAM lParam)
{
	u_whichSubscript = wParam;
	u_whichKindModel = lParam;
	// ��ʾѡ��״̬
	ShowFocus(u_whichKindModel,u_whichSubscript);
	//------����-------------------------------------------begin1-----------------
	// �ж�Ϊ����״̬ʱ�����ݿ�ʼģ�͵����ͣ���ȡģ�͵����ӿ�ʼ��
	if(u_whichModel==8)
	{
		b_DrawLine = TRUE;//��ʾ����
		if(u_whichKindModel==1)//��ʼģ��Ϊ����
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
		if(u_whichKindModel==2)//��ʼģ��Ϊ��Ǩ
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
	//------����-------------------------------------------end1-----------------
	//=====================================================================================Drag1==Begin==
	// ����u_whichKindModel��u_whichSubscript�����������ж����ĸ�ģ�ͽ����϶�
	// ����DragBegin��DragEnter������ʼ�ƶ�ģ��
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
				
				(*iterp)->GetWindowRect(&rectPic);//��ȡģ������Ļ�еľ���λ��
				(*iterp)->ScreenToClient(&rectPic);//��ģ������Ļ�еľ���λ��ת�����ͻ���
				(*iterp)->MapWindowPoints(this,&rectPic);//��ģ���ڿͻ����е�λ��װ����View��
				
				m_ptOffset.x	= ptPut.x-rectPic.left;
				m_ptOffset.y	= ptPut.y-rectPic.top;

				break;
			case 2:
				itert = FindTrans(u_whichSubscript);
				pDC	= (*itert)->GetDC();	

				(*itert)->GetWindowRect(&rectPic);//��ȡģ������Ļ�еľ���λ��
				(*itert)->ScreenToClient(&rectPic);//��ģ������Ļ�еľ���λ��ת�����ͻ���
				(*itert)->MapWindowPoints(this,&rectPic);//��ģ���ڿͻ����е�λ��װ����View��
				
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
		//����λͼ�ڴ�
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
		ClientToScreen(&ptPut);//�õ��������Ļ������

		m_imgDrag.DeleteImageList();
		m_imgDrag.Create(rectPic.Width(), rectPic.Height(), ILC_COLOR32|ILC_MASK, 0, 1);
		m_imgDrag.Add(&bitmapTemp, RGB(1, 0, 0));//�˴�Ҫ���ó�RGB(0,0,0)ʱ���������϶�ʱ��λͼ
		m_imgDrag.BeginDrag(0, m_ptOffset);//��ʼ�϶�ͼ��0���϶�ͼ��������������m_ptOffset:��ʼ�϶����Ĺ�ꡣ
		m_imgDrag.DragEnter(NULL, ptPut);//���϶������з������²���ȷ��λ����ʾ�϶�ͼ��NULL��ָ��ӵ���϶�ͼ��Ĵ��ڵ�ָ�롣ptPut����ʾ�϶�ͼ���λ�ã�������������ϽǴ��ڣ����ǿͻ��򣩡�

		SetCapture();//ʹ��������������붼�����͵���ǰ��CWnd���󣬲������ǹ���λ�á�
	}
	//=====================================================================================Drag1==End====
	
	
	return 0;
}

BOOL CHSTPN_SYSView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message ==	WM_LBUTTONDOWN)// ģ����ק���õ���
	{
		m_ptLBtnDown = pMsg->pt;
		ScreenToClient(&m_ptLBtnDown);
	}
	return CScrollView::PreTranslateMessage(pMsg);
}


void CHSTPN_SYSView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CScrollView::OnRButtonDown(nFlags, point);
}

//���ù������
BOOL CHSTPN_SYSView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	//=========================����=================================================Begin2==
	if(b_DrawLine && u_whichModel == 8)//�����ڻ���״̬�����ȡ�����㣬������
	{
		CDC *dc = GetDC();

		CPoint pt_start;
		CPoint pt_end;
		if(u_whichKindModel==1)//����ģ��Ϊ����
		{
			if(u_PlaceorTrans_Arrow == 2)//�����ʼģ��Ϊ��Ǩ
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
				// ������ͷ������������ı���-------------------------------------------------
				CArrow *m_arrow = new CArrow(u_whichSubscript,u_subofTrans_Arrow,2, pt_start, pt_end, u_arrow_id, false);
				// �����ͷ�����߶���
				m_pDoc->arr_Arrow.Add(m_arrow);
				//------------------------------------------------------------------------------
				// ������ɺ󣬸�������������ģ�͵�ǰ�úͺ���-----------------------------------
				vector<CTransition*>::iterator itert=FindTrans(u_subofTrans_Arrow);
				(*itert)->arryTransPst.push_back(*iterp);
				(*iterp)->arryPlacePre.push_back(*itert);
				(*itert)->arryArrowPst.push_back(m_arrow);//��Ǩ�ĺ�������
				(*iterp)->arryArrowPre.push_back(m_arrow);//������ǰ������
				//------------------------------------------------------------------------------
				u_arrow_id += 1;
			}
			else
			{
				MessageBox(L"������ģ��ӦΪ��Ǩ",L"����",MB_ICONERROR);
			}		
		}
		if(u_whichKindModel==2)//����ģ��Ϊ��Ǩ
		{
			if(u_PlaceorTrans_Arrow == 1)//�����ʼģ��Ϊ����
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
				// ������ͷ������������ı���-------------------------------------------------
				vector<CPlace*>::iterator iterp = FindPlace(u_subofModel_Arrow);
				if(((*iterp)->PlaceStyle == STYLE_DISIC )||((*iterp)->PlaceStyle == STYLE_RAND))
				{
					CArrow *m_arrow = new CArrow(u_subofModel_Arrow,u_whichSubscript,1,pt_start,pt_end,u_arrow_id,true);
					// �����ͷ�����߶���
					m_pDoc->arr_Arrow.Add(m_arrow);
					//------------------------------------------------------------------------------
					// ������ɺ󣬸�������������ģ�͵�ǰ�úͺ���----------------------------------
					(*iterp)->arryPlacePst.push_back(*itert);
					(*itert)->arryTransPre.push_back(*iterp);
					(*iterp)->arryArrowPst.push_back(m_arrow);//�����ĺ�������
					(*itert)->arryArrowPre.push_back(m_arrow);//��Ǩ��ǰ������
					//------------------------------------------------------------------------------
				}
				else
				{
					CArrow *m_arrow = new CArrow(u_subofModel_Arrow,u_whichSubscript,1,pt_start,pt_end,u_arrow_id,false);
					// �����ͷ�����߶���
				    m_pDoc->arr_Arrow.Add(m_arrow);
				    //------------------------------------------------------------------------------
				    // ������ɺ󣬸�������������ģ�͵�ǰ�úͺ���-----------------------------------
				
				    (*iterp)->arryPlacePst.push_back(*itert);
				    (*itert)->arryTransPre.push_back(*iterp);
				    (*iterp)->arryArrowPst.push_back(m_arrow);//�����ĺ�������
				    (*itert)->arryArrowPre.push_back(m_arrow);//��Ǩ��ǰ������
				    //------------------------------------------------------------------------------
				}
				u_arrow_id += 1;
			}
			else
			{
				MessageBox(L"������ģ��ӦΪ����",L"����",MB_ICONERROR);
			}
		}

		dc->MoveTo(pt_start);
		dc->LineTo(pt_end);

		DrawArrow(pt_start,pt_end/*,dc*/);

		b_DrawLine = FALSE;//�˳�����״̬

		u_whichModel = 0;//ͬ�����˳�����״̬������
		u_PlaceorTrans_Arrow = 0;//��ʶ�������߿�ʼģ�͵����ͣ��ָ���ʼ��״̬

		this->Invalidate();
		this->UpdateWindow();

		bPNisChanged = TRUE;

		ReleaseDC(dc);
		
	}
	//==========================����====================================================End2==
	return 0;
}
//=======================
// ����ɿ����ͱ�Ǩ������ߺ��������ߵĽ����㻭�ϼ�ͷ
//=======================
void CHSTPN_SYSView::DrawArrow(CPoint pt_start, CPoint pt_end/*, CDC * pDC*/)
{
	//==============Arrow=========Begin==
	CClientDC pDC(this);
	CPen pen1(PS_SOLID,1,RGB(0,0,240));
	pDC.SelectObject(&pen1);
	CBrush brush1(RGB(0,0,128));
	pDC.SelectObject(&brush1);
	// �����ӿ�ԭ�㣬�����ͼ��λ����
	OnPrepareDC(&pDC);

	//��ͷ
	double arrowangle = 3.14159/10;//3.14159/6;//�����ͷ�ĽǶ�
	int arrow_size = 10;//�����ͷ�Ĵ�С
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//=====================================================================================Drag2==Begin====
	// ������ƶ�ʱ������DragMove����ʹģ�Ͷ�Ӧ��λͼ�ƶ�
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
			// �����ľ��жϵ�Ŀ���ǣ���ֹͼ���϶����Ի������������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ���߹����У�������̧�𣬻���״̬����
	b_DrawLine = FALSE;
	// ����ģ�͹����У�������̧�𣬴���״̬����
	u_whichModel = 0;
	//=====================================================================================Drag3==Begin====
	// ��������̧��ʱ��ͨ��MoveWindow����ʹģ����ģ���ƶ���������EndDrag��������ģ�͵��϶�
	if(m_bIsLButtonDown)//֮ǰ�������
	{
		CRect	rectPic;
		CWnd*	pPic;
		vector<CPlace*>::iterator iterp;
		vector<CTransition*>::iterator itert;
		vector<CCTRLShowMatrix*>::iterator itero;
		switch(u_whichKindModel)
		{	
			case 1:
				iterp = FindPlace(u_whichSubscript);//u_whichSubscript-�����ĽǱ꣬�ڿ��������ҵ��ÿ���
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


		// ���¿������Ǩ����ͼ���е�λ��
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

		CImageList::DragLeave(this);//�ⶳ���ڲ������϶�ͼ��ʹ�����ܱ����� 
		CImageList::EndDrag();//����һ���϶�����
		ReleaseCapture();//�ͷ�ǰ�ߵ�SetCapture
		pPic->Invalidate();

		Invalidate();// ������һ��ʹ������Ч����Ϣ��ģ�ͼ������ߵ����ӵ����Ǳ����ڱ��ƶ�ģ�͵���һ��λ��
		UpdateWindow();
		Invalidate();// ���ֶ��ı䴰�ڳߴ�ʱ��ģ�ͼ�������߻ָ�����������ڴ˴��ٷ���һ�δ����ػ���Ϣ��ʹģ�ͼ���������ܹ�������ʾ
		UpdateWindow();

		bPNisChanged = TRUE;// ��ʱģ�ͱ����ģ���Ҫ�������ѶԻ���

		switch(u_whichKindModel)//���������Ǩ���϶���ʱ���ƶ��༭���������е㱣��һ��
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

// ��קʵ���õ��ĺ���
void CHSTPN_SYSView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CScrollView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
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

	// TODO: �ڴ˴������Ϣ����������
	Invalidate();
	UpdateWindow();
	Invalidate();
	UpdateWindow();
}


HBRUSH CHSTPN_SYSView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	pDC->SetBkMode(TRANSPARENT);
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//return hbr;
	return brush_Bk;
}

// ��ȡ�����ͷ���ʱ��
LRESULT CHSTPN_SYSView::OnGetTimeStep(WPARAM wParam, LPARAM lParam)
{
	CString str = ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_Edit_TimeStep->GetContentsAll(IDR_EDITTIMESTEP);
	char * str_step;
	USES_CONVERSION;
    //���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
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


// ���û����������Petri�����á���ť����Ӧ�ú��������浱ǰPetri���ķ�������
void CHSTPN_SYSView::OnSaveCurrentPNSet()
{
	// TODO: �ڴ���������������
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

// ���û�������ָ�Petri�����á���ť����Ӧ�ú��������浱ǰPetri���ķ�������
void CHSTPN_SYSView::OnRestorePnset()
{
	// TODO: �ڴ���������������
	CDlgResetState m_dlgnotice;
	if(IDOK == m_dlgnotice.DoModal())
	{
		// �����Reset��ť���Ƚ����������û�0״̬
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.EnablePaneProgressBar(1,1,true);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.SetPaneProgress(1,0,true);
		int n_countG = 0;
		for(vector<IOValue*>::iterator iterg=m_pDoc->vec_IOputDataG.begin();iterg!=m_pDoc->vec_IOputDataG.end();iterg++)
		{
			m_pDoc->arryIOputDataG.at(n_countG)->Value = (*iterg)->InitValue;
			m_pDoc->arryIOputDataG.at(n_countG)->InitValue = (*iterg)->InitValue;
			n_countG++;
		}
		//Ȼ����в����Ļָ�
		int n_countP = 0; //��������������
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

//������������ģ�ͣ��������Ǩ��
void CHSTPN_SYSView::CreatePlaceorTrans(int u_whichModel, CPoint point)
{
	//=====================================================================================================================Bigin==
	//------------------�������View��ʱ������ɴ���ģ�͵ı�ʶu_whichModel==1,2,3,4,5,6,7--�����������λ�ô���һ��ģ��1
	//-����ģ�͵Ĵ�С

	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint pt_ViewportOrg;
	pt_ViewportOrg = dc.GetViewportOrg();// ��ȡ�豸����ԭ��

	//-����ģ�Ϳ��Ա�����������Ϊ u_whichModel==1��2��3��4��5��6��7
	bool b_whichModelCreate = (u_whichModel==1 || u_whichModel==2 || u_whichModel==3 || u_whichModel==4 ||  
		                    u_whichModel==5 || u_whichModel==6 || u_whichModel==7 || u_whichModel == 9 || u_whichModel == 10
							|| u_whichModel==11||u_whichModel == 12);
	//-�����㴴������ʱ���������λ�ô���ģ��1
	if(b_whichModelCreate)
	{
		//--����u_whichModel��ֵѡ��Ҫ������ģ�͵�����1
		if(b_whichModelCreate && u_whichModel!=7 && u_whichModel!=9 && u_whichModel!=10 && u_whichModel != 12)
		{
			CPlace *m_Model = new CPlace();//�ڶ���Ϊ��������ڴ�1
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
			// ͨ��MoveWindow��������ģ���ƶ����������1
			m_Model->MoveWindow(point.x-rectofModel.Width()/2,point.y-rectofModel.Height()/2,rectofModel.Width(),rectofModel.Height());
		//	m_Model->OnPaint();//�ܲ����ڴ˴����ؼ�
			m_Model->ShowWindow(SW_SHOW);                                // ��ʾģ��1???��ʾʲôģ��		
			m_Model->u_SubscriptofModel = u_SubScriptofPlace;            // ������ɺ󣬽��±긳ֵ��ģ��1
			u_SubScriptofPlace          = u_SubScriptofPlace+1;          // ģ�ʹ�����ɺ�ά����ģ�ͱ���������±��1����ֹģ�ͱ������ظ�ʹ��1
			//m_Model->Position.x = point.x-rectofModel.Width()/2;         // ��¼ģ������ͼ���е�λ��1
			//m_Model->Position.y = point.y-rectofModel.Height()/2;
			//================================5.10
			m_Model->Position.x = point.x-rectofModel.Width()/2 - pt_ViewportOrg.x;
			m_Model->Position.y = point.y-rectofModel.Height()/2 - pt_ViewportOrg.y;
			CString str;
			str.Format(_T("%d"),m_Model->u_SubscriptofModel);
			m_Model->m_caption = _T("p")+str;
			// ��ʾģ�͵�����
			dc.TextOutW(m_Model->Position.x,m_Model->Position.y-17,m_Model->m_caption);

			m_pDoc->Vector_Place.push_back(m_Model);                             //����ģ��

		}
		if(u_whichModel==7)
		{
			CTransition *m_Trans = new CTransition();
			m_Trans->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofTrans,this);

			// ͨ��MoveWindow��������ģ���ƶ����������
			m_Trans->MoveWindow(point.x-rectofTrans.Width()/2,point.y-rectofTrans.Height()/2,rectofTrans.Width(),rectofTrans.Height());
			m_Trans->ShowWindow(SW_SHOW);// ��ʾģ��
			m_Trans->u_SubscriptofTrans = u_SubScriptofTrans;// ������ɺ󣬽��±긳ֵ��ģ��
			u_SubScriptofTrans = u_SubScriptofTrans+1;// ģ�ʹ�����ɺ�ά����ģ�ͱ���������±��1����ֹģ�ͱ������ظ�ʹ��
			//m_Trans->Position.x = point.x-rectofTrans.Width()/2;
			//m_Trans->Position.y = point.y-rectofTrans.Height()/2;
			//======================5.10
			m_Trans->Position.x = point.x-rectofTrans.Width()/2 - pt_ViewportOrg.x;
			m_Trans->Position.y = point.y-rectofTrans.Height()/2 - pt_ViewportOrg.y;

			CString str;
			str.Format(_T("%d"),m_Trans->u_SubscriptofTrans);
			m_Trans->m_caption = _T("t")+str;
			// ��ʾģ�͵�����
			dc.TextOutW(m_Trans->Position.x,m_Trans->Position.y-17,m_Trans->m_caption);

			m_pDoc->Vector_Trans.push_back(m_Trans);//����ģ��
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

		////������װģ��
		//if (u_whichModel == 12)
		//{
		//	CPlace *m_Model = new CPlace();//�ڶ���Ϊ��������ڴ�1
		//	m_Model->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY, rectofModel, this,STYLE_PACK);
		//	m_Model->m_thisModel = m_Model;
		//	// ͨ��MoveWindow��������ģ���ƶ����������1
		//	m_Model->MoveWindow(point.x - rectofModel.Width() / 2, point.y - rectofModel.Height() / 2, rectofModel.Width(), rectofModel.Height());
		//	//	m_Model->OnPaint();//�ܲ����ڴ˴����ؼ�
		//	m_Model->ShowWindow(SW_SHOW);                                // ��ʾģ��1???��ʾʲôģ��		
		//	m_Model->u_SubscriptofModel = u_SubScriptofPlace;            // ������ɺ󣬽��±긳ֵ��ģ��1
		//	u_SubScriptofPlace = u_SubScriptofPlace + 1;          // ģ�ʹ�����ɺ�ά����ģ�ͱ���������±��1����ֹģ�ͱ������ظ�ʹ��1
		//														  //m_Model->Position.x = point.x-rectofModel.Width()/2;         // ��¼ģ������ͼ���е�λ��1
		//														  //m_Model->Position.y = point.y-rectofModel.Height()/2;
		//														  //================================5.10
		//	m_Model->Position.x = point.x - rectofModel.Width() / 2 - pt_ViewportOrg.x;
		//	m_Model->Position.y = point.y - rectofModel.Height() / 2 - pt_ViewportOrg.y;
		//	CString str;
		//	str.Format(_T("%d"), m_Model->u_SubscriptofModel);
		//	m_Model->m_caption = _T("p") + str;
		//	// ��ʾģ�͵�����
		//	dc.TextOutW(m_Model->Position.x, m_Model->Position.y - 17, m_Model->m_caption);

		//	m_pDoc->Vector_Place.push_back(m_Model);
		//}
		//

		bPNisChanged = TRUE;
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);// Ŀ�ķ�����mainframe���ٷ�����dlgmodelchoose1��֪ͨHSTPN�ṹ������
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);
		/*if(m_pParmListDlg == NULL)// ������ϢֵCDlgModelParmList�࣬�������ĸ���
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
	// TODO: �ڴ˴������Ϣ����������
	DeletePlcTrnsArcs();
}

// �ú�������ɾ��Vector_Place/Vector_Trans/arr_Arrow���Ԫ�أ����ͷ��ڴ�
void CHSTPN_SYSView::DeletePlcTrnsArcs()
{
	// �ͷ�����/����б�
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

	// �ͷ�Vector_Place��Ԫ��ָ����ڴ棬���Vector_Place
	for(vector<CPlace*>::iterator iterP=m_pDoc->Vector_Place.begin();iterP!=m_pDoc->Vector_Place.end();iterP++)
	{
		(*iterP)->DestroyWindow();
		delete (*iterP);
	}
	m_pDoc->Vector_Place.clear();
	// �ͷ�Vector_Trans��Ԫ��ָ����ڴ棬���Vector_Trans
	for(vector<CTransition*>::iterator iterT=m_pDoc->Vector_Trans.begin();iterT!=m_pDoc->Vector_Trans.end();iterT++)
	{
		(*iterT)->DestroyWindow();
		delete (*iterT);
	}
	m_pDoc->Vector_Trans.clear();
	// �ͷ�arr_Arrow��Ԫ��ָ����ڴ棬���arr_Arrow
	int n_SizeArcs = m_pDoc->arr_Arrow.GetSize();
	for(int i=0;i<n_SizeArcs;i++)
	{
		delete (CArrow*)m_pDoc->arr_Arrow.GetAt(i);
	}
	m_pDoc->arr_Arrow.RemoveAll();
	// �������ָ���ʼ��
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
		myDocument->Clear();// �ͷ��ӽڵ��ڴ�
	}
	delete myDocument; // �ͷŸ��ڵ㱾��
	bPNisChanged = TRUE;

}

// ����һ��CStringת��Ϊchar*�ĺ��������������ԭ��USES_CONVERSION����ڶ��������ڴ棬���Ҳ����ͷţ���˵��������ɺ������Խ��������
char* CHSTPN_SYSView::CString2char(CString str)
{
	USES_CONVERSION;
	return T2A(str);
}

// ģ��ɾ������
LRESULT CHSTPN_SYSView::OnMDdelete2(WPARAM wParam, LPARAM lParam)
{
	UINT u_sub = wParam;// ģ��ID
	UINT u_which = lParam; // �������Ǳ�Ǩ

	if(u_which == 1)// ��ɾ��ģ��Ϊ����
	{
		vector<CPlace*>::iterator iterp = FindPlace(u_sub);// �ҵ�������vector_Place�е�λ��
		// ����ǰ�ñ�Ǩ�ĺ��ÿ�����
		for(vector<CTransition*>::iterator iter1=(*iterp)->arryPlacePre.begin();iter1!=(*iterp)->arryPlacePre.end();iter1++)//����Ҫɾ��������ǰ�ñ�Ǩ��
		{
			for(vector<CPlace*>::iterator iter2=(*iter1)->arryTransPst.begin();iter2!=(*iter1)->arryTransPst.end();iter2++)// ������Ǩ�ĺ��ÿ������ϣ��ҵ���ɾ���Ŀ�����ɾ����Ԫ��
			{
				if((*iter2)->u_SubscriptofModel==u_sub)
				{
					(*iter1)->arryTransPst.erase(iter2);
					break;
				}
			}
		}
		// ���º��ñ�Ǩ��ǰ�ÿ�����
		for(vector<CTransition*>::iterator iter3 = (*iterp)->arryPlacePst.begin();iter3!=(*iterp)->arryPlacePst.end();iter3++)//����Ҫɾ�������ĺ��ñ�Ǩ��
		{
			for(vector<CPlace*>::iterator iter4=(*iter3)->arryTransPre.begin();iter4!=(*iter3)->arryTransPre.end();iter4++)// ����ÿ�����ñ�Ǩ��ǰ�ÿ������ϣ��ҵ���ɾ��������ɾ����Ԫ��
			{
				if((*iter4)->u_SubscriptofModel == u_sub)
				{
					(*iter3)->arryTransPre.erase(iter4);
					break;
				}
			}
		}
		// �������򻡼�
		int nCountArc = m_pDoc->arr_Arrow.GetSize();
		for(int i= nCountArc-1;i>=0;i--)
		{
			if(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofPlace == u_sub)// ��������������ӵ�ģ������Ҫɾ���Ŀ���ģ�ͣ�˵��������ҲӦ�ñ�ɾ��
			{
				delete ((CArrow*)m_pDoc->arr_Arrow.GetAt(i));
				m_pDoc->arr_Arrow.RemoveAt(i);
			}
		}
		// �ͷ���������б�
		for(vector<IOValue*>::iterator iter3=(*iterp)->arryIOputData.begin();iter3!=(*iterp)->arryIOputData.end();iter3++)
		{
			delete (*iter3);
		}
		(*iterp)->arryIOputData.clear();
		(*iterp)->arryInputofLua.clear();
		(*iterp)->arryOutputofLua.clear();

		// ����������������������б���������п���������ֵ�����Ա�ɾ����������Ӧ�ý���ֵ����������б���ɾ��
		for(vector<CPlace*>::iterator iterp1=m_pDoc->Vector_Place.begin();iterp1!=m_pDoc->Vector_Place.end();iterp1++)
		{
			//--���ڸ���������������ҵ���ɾ��ֵ��λ��
			vector<int> pos1;
			int count1 = 0;
			for(vector<IOValue*>::iterator iter1=(*iterp1)->arryIOputData.begin();iter1!=(*iterp1)->arryIOputData.end();iter1++)
			{
				if((*iter1)->IOType == "input" && (*iter1)->FromModel==(*iterp)->m_caption)// �����ֵΪ�����ͣ�������Դ���ڴ�ɾ�����������֣���ô��¼��ֵ��λ��
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
			//--���������ɾ��ֵ
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

		// ���ٿؼ���ɾ��ģ�ͣ��ͷ��ڴ�
		(*iterp)->DestroyWindow();
		delete (*iterp);
		m_pDoc->Vector_Place.erase(iterp);

	}

	if(u_which == 2)// ��ɾ��ģ��Ϊ��Ǩ
	{
		vector<CTransition*>::iterator itert = FindTrans(u_sub);// �ҵ���Ǩ��Vector_Trans�е�λ��
		for(vector<CPlace*>::iterator iter1=(*itert)->arryTransPre.begin();iter1!=(*itert)->arryTransPre.end();iter1++)// ����Ҫɾ����Ǩ��ǰ�ÿ�����
		{
			for(vector<CTransition*>::iterator iter2=(*iter1)->arryPlacePst.begin();iter2!=(*iter1)->arryPlacePst.end();iter2++)// ����ÿ��ǰ�ÿ����ĺ��ñ�Ǩ�����ҵ���ɾ���ı�Ǩ��ɾ����Ԫ��
			{
				if((*iter2)->u_SubscriptofTrans==u_sub)
				{
					(*iter1)->arryPlacePst.erase(iter2);
					break;
				}
			}
		}
		for(vector<CPlace*>::iterator iter3=(*itert)->arryTransPst.begin();iter3!=(*itert)->arryTransPst.end();iter3++)// ����Ҫɾ����Ǩ�ĺ��ÿ�����
		{
			for(vector<CTransition*>::iterator iter4=(*iter3)->arryPlacePre.begin();iter4!=(*iter3)->arryPlacePre.end();iter4++)//����ÿ�����ÿ�����ǰ�ñ�Ǩ�����ҵ���ɾ���ı�Ǩ��ɾ����Ԫ��
			{
				if((*iter4)->u_SubscriptofTrans==u_sub)
				{
					(*iter3)->arryPlacePre.erase(iter4);
					break;
				}
			}
		}
		// �������򻡼�
		int nCountArc = m_pDoc->arr_Arrow.GetSize();
		for(int i= nCountArc-1;i>=0;i--)
		{
			if(((CArrow*)m_pDoc->arr_Arrow.GetAt(i))->u_SubScriptofTrans == u_sub)// ��������������ӵ�ģ������Ҫɾ���Ŀ���ģ�ͣ�˵��������ҲӦ�ñ�ɾ��
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
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);// Ŀ�ķ�����mainframe���ٷ�����dlgmodelchoose1��֪ͨHSTPN�ṹ������
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

// ���ҿ���
vector<CPlace*>::iterator CHSTPN_SYSView::FindPlace( UINT id)
{
	vector<CPlace*>::iterator iter = m_pDoc->Vector_Place.begin(); // ����һ��������
	int nSize = m_pDoc->Vector_Place.size();                       // ��ȡvector_place�Ĵ�С
	nSize = (int)floor(nSize/2.0);                              // �����־j
	int j = nSize;
	iter = iter + j;                                       // ���������ƽ����м�λ��

	do{
		if(id >= (*iter)->u_SubscriptofModel)                  //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص��ұ�
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
		else                                                   //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص����
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
				// ����������û���ҵ�����ֻʣ��һ���ˣ���Ϊ��һ��û�б�������
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

// ���ұ�Ǩ
vector<CTransition*>::iterator CHSTPN_SYSView::FindTrans(UINT id)
{
	vector<CTransition*>::iterator iter = m_pDoc->Vector_Trans.begin(); // ����һ��������
	int nSize = m_pDoc->Vector_Trans.size();                       // ��ȡvector_Trans�Ĵ�С
	nSize = (int)floor(nSize/2.0);                              // �����־j
	int j = nSize;
	iter = iter + j;                                       // ���������ƽ����м�λ��

	do{
		if(id >= (*iter)->u_SubscriptofTrans)                  //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص��ұ�
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
		else                                                   //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص����
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
				// ����������û���ҵ�����ֻʣ��һ���ˣ���Ϊ��һ��û�б�������
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
	// ���ؼ���ʾΪѡ��״̬
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

	// TODO: �ڴ����ר�ô����/����û���
	SetScrollSizes(MM_TEXT,CSize(5000,6000));

}
// �������ģ��
vector<CCTRLShowMatrix*>::iterator  CHSTPN_SYSView::FindOuts(UINT id)
{
	vector<CCTRLShowMatrix*>::iterator iter = m_pDoc->Vector_OutputModel.begin(); // ����һ��������
	int nSize = m_pDoc->Vector_OutputModel.size();                                // ��ȡVector_OutputModel�Ĵ�С
	nSize = (int)floor(nSize/2.0);                                                // �����־j
	int j = nSize;
	iter = iter + j;                                                              // ���������ƽ����м�λ��

	do{
		if(id >= (*iter)->ID)                  //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص��ұ�
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
		else                                                   //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص����
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
				// ����������û���ҵ�����ֻʣ��һ���ˣ���Ϊ��һ��û�б�������
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

/*// ������������б�ģ�����Ա༭��
void CHSTPN_SYSView::OnShowpramlist()
{
	// TODO: �ڴ���������������
	if(m_pParmListDlg==NULL)
	{
		m_pParmListDlg = new CDlgModelParmList();
		m_pParmListDlg->Create(IDD_DLG_DATALIST,NULL);
	}
	m_pParmListDlg->ShowWindow(SW_SHOW);

}*/
