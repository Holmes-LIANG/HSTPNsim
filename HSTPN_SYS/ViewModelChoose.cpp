// ViewModelChoose.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "ViewModelChoose.h"


// CViewModelChoose

IMPLEMENT_DYNCREATE(CViewModelChoose, CFormView)

CViewModelChoose::CViewModelChoose()
	: CFormView(CViewModelChoose::IDD)
{
	m_dlgmodelchoose1 = (CDlgModelChoose1*)(RUNTIME_CLASS(CDlgModelChoose1)->CreateObject());
	m_dlgmodelchoose  = (CDlgModelChoose*)(RUNTIME_CLASS(CDlgModelChoose)->CreateObject());
}

CViewModelChoose::~CViewModelChoose()
{
}

void CViewModelChoose::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
//	DDX_Control(pDX, IDP_MDS_FNDMNTELEMT, btn_Dlgchos1_Fdmtlelemt);
//	DDX_Control(pDX, IDP_MDS_TYPCLSTRUCT, btn_Dlgchoose2_typclstruc);
//	DDX_Control(pDX, IDP_MDS_PHYSCLMODEL, btn_Dlgchoose3_phyclmodel);
	
}

BEGIN_MESSAGE_MAP(CViewModelChoose, CFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CViewModelChoose ���

#ifdef _DEBUG
void CViewModelChoose::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewModelChoose::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewModelChoose ��Ϣ�������


void CViewModelChoose::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFormView::OnLButtonDown(nFlags, point);
}


int CViewModelChoose::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rectofclent;                              // �����Ի������û�ѡ�����Ԫ�ء����ͽṹ���������̶Ի���ĶԻ���
	GetClientRect(&rectofclent);
	rectofclent.bottom = rectofclent.top + rectofclent.Height()/2-3;//�����ף�ɾ��ҲûӰ��
	m_dlgmodelchoose->Create(NULL,NULL,WS_CHILD|(WS_VISIBLE),rectofclent,this, 100, NULL);


	CRect rect;                                   // �����Ի������û�ѡ�����Ԫ�صĶԻ���
	GetClientRect(&rect);
	rect.top = rect.top + rect.Height()/2+3;
	rect.bottom = rect.bottom;
	m_dlgmodelchoose1->Create(NULL, NULL, WS_CHILD|(WS_VISIBLE), rect,this, 101, NULL);

	return 0;
}


void CViewModelChoose::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (GetSafeHwnd() == NULL)
	{ 
		return;
	} 

	if(m_dlgmodelchoose->GetSafeHwnd()!=NULL)
	{
		CRect rectofclent;
		GetClientRect(&rectofclent);
		m_dlgmodelchoose->SetWindowPos(NULL,rectofclent.left,rectofclent.top,rectofclent.Width(),rectofclent.Height()/2-3,SWP_NOACTIVATE | SWP_NOZORDER);
	}

	if(m_dlgmodelchoose1->GetSafeHwnd()!=NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		m_dlgmodelchoose1->SetWindowPos(NULL, rect.left, rect.top + rect.Height()/2+3, rect.Width(), rect.Height()-rect.Height()/2-3, SWP_NOACTIVATE | SWP_NOZORDER);

	}
}


void CViewModelChoose::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFormView::OnPaint()
	//CRect rect;
	//GetClientRect(&rect); 
 //   //dc.FillSolidRect(rect,RGB(240,240,240));//���ñ�����ɫ
	//CPen pen(PS_SOLID,1,RGB(240,240,240));
	//CPen *oldPen;
	//oldPen = dc.SelectObject(&pen);
	//rect.top = rect.top + rect.Height()/2-3;
	//dc.MoveTo(rect.left,rect.top);
	//dc.LineTo(rect.right,rect.top);

	//dc.MoveTo(rect.left,rect.top+2);
	//dc.LineTo(rect.right,rect.top+2);
	//dc.MoveTo(rect.left,rect.top+3);
	//dc.LineTo(rect.right,rect.top+3);

	//CPen pen1(PS_SOLID,1,RGB(255,255,255));
	//dc.SelectObject(&pen1);
	//dc.MoveTo(rect.left,rect.top+1);
	//dc.LineTo(rect.right,rect.top+1);

	//CPen pen2(PS_SOLID,1,RGB(160,160,160));
	//dc.SelectObject(&pen2);
	//dc.MoveTo(rect.left,rect.top+4);
	//dc.LineTo(rect.right,rect.top+4);

	//CPen pen3(PS_SOLID,1,RGB(100,100,100));
	//dc.SelectObject(&pen3);
	//dc.MoveTo(rect.left,rect.top+5);
	//dc.LineTo(rect.right,rect.top+5);

	//dc.SelectObject(oldPen);

}
