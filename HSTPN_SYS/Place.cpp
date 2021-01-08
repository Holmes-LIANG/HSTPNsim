// Model.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <sstream>
#include "HSTPN_SYS.h"
#include "Place.h"

#include "ModelPropSetDlg.h"

#include "MainFrm.h"
// CPlace

IMPLEMENT_DYNAMIC(CPlace, CStatic)

CPlace::CPlace()
{
	bActivation          = FALSE;
	bEnable              = FALSE;
	bisworked            = FALSE;
	//isSocketed           = FALSE;
	//arryPlacePre       =  ;
	//arryPlacePst       =  ;
	PlaceStyle           = STYLE_DISCRETE;
	pt1                  = 0;
	pt2                  = 0;
	pt3                  = 0;
	pt4                  = 0;
	u_SubscriptofModel   = 0;

	dProblty             = 1;
	dTime                = 0;

	m_caption            = "";

	bisUserDef_prob      = TRUE;
	bisUserDef_time      = TRUE;
	bisUserDef_ctrl      = TRUE;
	str_Inputport_prob   = "";
	str_Inputport_time   = "";
	str_Inputport_ctrl   = "";

	bisAssigned          = FALSE;
	Lua_fileName         = "";
	d_TimeStep           = 0.1;
	Position             = 0;
	b_ModelFocus         = FALSE;
	i_RandOutcome        = FALSE;
	d_RandProblty        = 0;

	d_TimeTS             = 0;	

	CtrlPlaceStyle       = TCTRL;

	dTime_downline       = 0;     // ʱ�����������
	dTime_upline         = 1;     // ʱ�����������
	ischoseupdown        = false;
	b_resettime          = false;
	ischoseProgRand      = false;
	isTimeUpDownChange   = false;
	ischoseSocket        = false;
	ipadrs               = "";
	port                 = 0;
	num_input            = 0;
	num_output           = 0;
	msgO                 = "";
	msgI                 = "";

	struct result
	{
		bool bcanEnable;
		int res;
	};

	count = 0;
}

CPlace::~CPlace()
{
}

//���ع��캯��
CPlace::CPlace(const CPlace &C)
{
	this->bActivation        = C.bActivation;
	this->bEnable            = C.bEnable;
	this->arryPlacePre       = C.arryPlacePre;
	this->arryPlacePst       = C.arryPlacePre;
	this->PlaceStyle         = C.PlaceStyle;
	this->pt1                = C.pt1;
	this->pt2                = C.pt2;
	this->pt3                = C.pt3;
	this->pt4                = C.pt4;
	this->u_SubscriptofModel = C.u_SubscriptofModel;
	this->dProblty           = C.dProblty;
	this->dTime              = C.dTime;
	this->d_TimeTS           = C.d_TimeTS;
	this->CtrlPlaceStyle     = C.CtrlPlaceStyle;
	this->dTime_downline     = C.dTime_downline;// ʱ�����������
	this->dTime_upline       = C.dTime_upline;// ʱ�����������
	this->ischoseupdown      = C.ischoseupdown;
	this->b_resettime        = C.b_resettime;
	this->ischoseProgRand    = C.ischoseProgRand;
}
//���ز�����=
CPlace CPlace::operator = (const CPlace &A)
{
	this->bActivation        = A.bActivation;
	this->bEnable            = A.bEnable;
	this->arryPlacePre       = A.arryPlacePre;
	this->arryPlacePst       = A.arryPlacePre;
	this->PlaceStyle         = A.PlaceStyle;
	this->pt1                = A.pt1;
	this->pt2                = A.pt2;
	this->pt3                = A.pt3;
	this->pt4                = A.pt4;
	this->u_SubscriptofModel = A.u_SubscriptofModel;
	this->dProblty           = A.dProblty;
	this->dTime              = A.dTime;
	this->d_TimeTS           = A.d_TimeTS;
	this->CtrlPlaceStyle     = A.CtrlPlaceStyle;
	this->dTime_downline     = A.dTime_downline;// ʱ�����������
	this->dTime_upline       = A.dTime_upline;// ʱ�����������
	this->ischoseupdown      = A.ischoseupdown;
	this->b_resettime        = A.b_resettime;
	this->ischoseProgRand    = A.ischoseProgRand;
	return *this;
}
BEGIN_MESSAGE_MAP(CPlace, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MD_DELETE, &CPlace::OnMdDelete)
	ON_COMMAND(ID_ROTATE_TL, &CPlace::OnRotateTl)
	ON_COMMAND(ID_SHOWPROPDLG, &CPlace::OnShowpropdlg)
END_MESSAGE_MAP()



// CPlace ��Ϣ�������


BOOL CPlace::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,enumPlaceStyle PlaceStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���
	this->PlaceStyle     =  PlaceStyle;	
	this->m_pParentView  =  (CHSTPN_SYSView*)pParentWnd;
	this->bisworked      =  false;
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}


void CPlace::OnPaint()
{
	CPaintDC             dc(this); // device context for painting

	CString              str;
	str.Format(_T("%d"),u_SubscriptofModel);
	this->m_caption    = _T("p")+str; 	

	//-------��ȡ���沽��---
	this->d_TimeStep   = ((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_TimeStep;
	
	//----------��ά�����ĸ����ӵ㸳ֵ---------------------
	CRect                rectofCtrl;
	this->GetWindowRect(&rectofCtrl);
	this->pt1.x        = rectofCtrl.left;
	this->pt1.y        = rectofCtrl.top+rectofCtrl.Height()/2;
	this->pt2.x        = rectofCtrl.left+rectofCtrl.Width()/2;
	this->pt2.y        = rectofCtrl.bottom;
	this->pt3.x        = rectofCtrl.right;
	this->pt3.y        = rectofCtrl.top+rectofCtrl.Height()/2;
	this->pt4.x        = rectofCtrl.left+rectofCtrl.Width()/2;
	this->pt4.y        = rectofCtrl.top+20;

	//-----------���ƿ���ģ�͵����-----------------------
	CRect                rectClient;
	GetClientRect(&rectClient);
	CRect                rect(&rectClient);
	CRect                rect_token;
	rect_token.left    = rect.left + 12;
	rect_token.right   = rect.right - 12;
	rect_token.top     = rect.top + 12;
	rect_token.bottom  = rect.bottom - 12;
	
	CPen           *     oldpen_old;
	CPen                 pen(PS_SOLID,2,RGB(0,0,0));             // �������ʣ���ɫΪ��ɫ
	oldpen_old         = dc.SelectObject(&pen);

	if(this->PlaceStyle   == STYLE_DISCRETE)                       // ������ɢ����������
	{	
		if(!this->bEnable)                                       // δʹ��״̬��Ϊ��ɫ
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			if(this->bActivation)                                // ����Ǽ���״̬�������ı�ʶ
			{
				CPen         pen(PS_SOLID,2,RGB(108,226,108));   // ��������
				oldpen_old = dc.SelectObject(&pen);
				CBrush       newbrush(RGB(255,255,255));
				CBrush  *    oldbrush;
				oldbrush   = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else                                                     // ʹ��״̬��Ϊ��ɫ
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			// ��token
			CPen             pen(PS_SOLID,2,RGB(108,226,108));   // ��������
			oldpen_old     = dc.SelectObject(&pen);
			CBrush           newbrush(RGB(108,226,108));
			CBrush  *        oldbrush;
			oldbrush       = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
	}
	if(this->PlaceStyle   == STYLE_CNTNUS)                        // ������������������
	{	
		dc.Ellipse(&rect);
		dc.SelectObject(oldpen_old);
		CRect                rect1(&rect);
		rect1.left         = rect1.left   +  rect1.Width()/7.0;
		rect1.top          = rect1.top    + rect1.Height()/7.0;
		rect1.right        = rect1.right  -  rect1.Width()/7.0;
		rect1.bottom       = rect1.bottom - rect1.Height()/7.0;
		if(!this->bEnable)
		{
			dc.Ellipse(&rect1);
			dc.SelectObject(oldpen_old);
			if(this->bActivation)// ����Ǽ���״̬�������ı�ʶ
			{
				CPen         pen(PS_SOLID,2,RGB(108,226,108)); // ��������
				oldpen_old = dc.SelectObject(&pen);
				CBrush       newbrush(RGB(255,255,255));
				CBrush  *    oldbrush;
				oldbrush   = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else
		{
			dc.Ellipse(&rect1);
			dc.SelectObject(oldpen_old);
			// ��Token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // �������ʣ���ɫΪ��ɫ
			//CPen *oldpen;
			oldpen_old = dc.SelectObject(&pen);
			CBrush newbrush(RGB(108,226,108));
			CBrush *oldbrush;
			oldbrush = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
	}
	if(this->PlaceStyle == STYLE_RAND)// �����������������
	{
		CPen newpen(PS_SOLID,4,RGB(0,0,0));
		//CPen *oldpen;
		oldpen_old = dc.SelectObject(&newpen);
		dc.SelectObject(&oldpen_old);
		if(!this->bEnable)
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			if(this->bActivation)// ����Ǽ���״̬�������ı�ʶ
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // ��������
				//CPen *oldpen;
				oldpen_old = dc.SelectObject(&pen);
				CBrush newbrush(RGB(255,255,255));
				CBrush *oldbrush;
				oldbrush = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			//��token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // �������ʣ���ɫΪ��ɫ
			//CPen *oldpen;
			oldpen_old = dc.SelectObject(&pen);
			CBrush newbrush(RGB(108,226,108));
			CBrush *oldbrush;
			oldbrush = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
	}
	if(this->PlaceStyle == STYLE_TIME)// ����ʱ�����������
	{
		CPen newpen(PS_DOT,1,RGB(0,0,0));
		//CPen *oldpen;
		oldpen_old = dc.SelectObject(&newpen);
		dc.SelectObject(&oldpen_old);
		if(!this->bEnable)
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			if(this->bActivation)// ����Ǽ���״̬�������ı�ʶ
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // ��������
				//CPen *oldpen;
				oldpen_old = dc.SelectObject(&pen);
				CBrush newbrush(RGB(255,255,255));
				CBrush *oldbrush;
				oldbrush = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			//��token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // �������ʣ���ɫΪ��ɫ
			//CPen *oldpen;
			oldpen_old = dc.SelectObject(&pen);
			CBrush newbrush(RGB(108,226,108));
			CBrush *oldbrush;
			oldbrush = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
	}

	if(this->PlaceStyle == STYLE_TCTRL)// ����ͻ�ؿ���������
	{
		if(this->CtrlPlaceStyle == TCTRL)
		{
		dc.Ellipse(&rect);//����Բ
		CBrush brush(RGB(0,0,0));
		CBrush *oldbrush1;
		oldbrush1 = dc.SelectObject(&brush);
		CRect rect1;
		rect1.left = rect.left + (rect.Width()/2) - 3;
		rect1.top = rect.top + 2;
		rect1.right = rect.right - (rect.Width()/2) + 3;
		rect1.bottom = rect.bottom - 2;
		dc.FillRect(&rect1,&brush);
		dc.SelectObject(oldbrush1);

		if(!this->bEnable)
		{
			if(this->bActivation)// ����Ǽ���״̬�������ı�ʶ
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // ��������
				//CPen *oldpen;
				oldpen_old = dc.SelectObject(&pen);
				CBrush newbrush(RGB(255,255,255));
				CBrush *oldbrush;
				oldbrush = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else
		{
			//��token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // �������ʣ���ɫΪ��ɫ
			//CPen *oldpen;
			oldpen_old = dc.SelectObject(&pen);
			CBrush newbrush(RGB(108,226,108));
			CBrush *oldbrush;
			oldbrush = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
		}
		if(this->CtrlPlaceStyle == LCTRL)
		{
		dc.Ellipse(&rect);
		CBrush brush(RGB(0,0,0));
		CBrush *oldbrush1;
		oldbrush1 = dc.SelectObject(&brush);
		CRect rect1;
		rect1.left = rect.left + 2;
		rect1.top = rect.top + (rect.Height()/2) - 3;
		rect1.right = rect.right - 2;
		rect1.bottom = rect.bottom - (rect.Height()/2) + 3;
		dc.FillRect(&rect1,&brush);
		dc.SelectObject(oldbrush1);	
		if(!this->bEnable)
		{
			if(this->bActivation)// ����Ǽ���״̬�������ı�ʶ
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // ��������
				//CPen *oldpen;
				oldpen_old = dc.SelectObject(&pen);
				CBrush newbrush(RGB(255,255,255));
				CBrush *oldbrush;
				oldbrush = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else
		{
			//��token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // �������ʣ���ɫΪ��ɫ
			//CPen *oldpen;
			oldpen_old = dc.SelectObject(&pen);
			CBrush newbrush(RGB(108,226,108));
			CBrush *oldbrush;
			oldbrush = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
		}
	}
	if(this->PlaceStyle == STYLE_LCTRL)// ����ͻ�ؿ���������
	{
		dc.Ellipse(&rect);
		CBrush brush(RGB(0,0,0));
		CBrush *oldbrush1;
		oldbrush1 = dc.SelectObject(&brush);
		CRect rect1;
		rect1.left = rect.left + 2;
		rect1.top = rect.top + (rect.Height()/2) - 3;
		rect1.right = rect.right - 2;
		rect1.bottom = rect.bottom - (rect.Height()/2) + 3;
		dc.FillRect(&rect1,&brush);
		dc.SelectObject(oldbrush1);	
		if(!this->bEnable)
		{
			if(this->bActivation)// ����Ǽ���״̬�������ı�ʶ
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // ��������
				//CPen *oldpen;
				oldpen_old = dc.SelectObject(&pen);
				CBrush newbrush(RGB(255,255,255));
				CBrush *oldbrush;
				oldbrush = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else
		{
			//��token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // �������ʣ���ɫΪ��ɫ
			//CPen *oldpen;
			oldpen_old = dc.SelectObject(&pen);
			CBrush newbrush(RGB(108,226,108));
			CBrush *oldbrush;
			oldbrush = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
		
	}
	if(this->PlaceStyle == STYLE_DISIC)
	{
		CPen newpen(PS_DOT,1,RGB(0,0,0));
		oldpen_old=dc.SelectObject(&newpen);
		dc.Ellipse(&rect);
//		dc.SelectObject(&oldpen_old);
		
		CRect rect1(&rect);
		rect1.left   = rect1.left   +  rect1.Width()/7.0;
		rect1.top    = rect1.top    + rect1.Height()/7.0;
		rect1.right  = rect1.right  -  rect1.Width()/7.0;
		rect1.bottom = rect1.bottom - rect1.Height()/7.0;

		dc.Ellipse(&rect1);
		dc.SelectObject(oldpen_old);
		if(!this->bEnable)
		{
			
			if(this->bActivation)// ����Ǽ���״̬�������ı�ʶ
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // ��������
			//CPen *oldpen;
				oldpen_old = dc.SelectObject(&pen);
				CBrush newbrush(RGB(255,255,255));
				CBrush *oldbrush;
				oldbrush = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else
		{
		//	dc.Ellipse(&rect1);
		//	dc.SelectObject(oldpen_old);
			//��token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // �������ʣ���ɫΪ��ɫ
			//CPen *oldpen;
			oldpen_old = dc.SelectObject(&pen);
			CBrush newbrush(RGB(108,226,108));
			CBrush *oldbrush;
			oldbrush = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
	}
	ReleaseDC(&dc);
}

void CPlace::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	// �����������ģ��ʱ�����Ƚ�ģ�͵��±귢����View�ࣨ�ȷ��͸�MainFrame�࣬�ٷ��͸�View�ࣩ
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_MOVEMODEL1,this->u_SubscriptofModel,1);//Lparm�е�1��ʾģ��Ϊ����

	CStatic::OnLButtonDown(nFlags, point);
}


void CPlace::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//-----------------------------------------
	// ������ڿؼ���̧���ʱ�򣬿������ڻ���״̬����ʱ����˷�����Ϣ��View�࣬ʹ���øÿؼ������ӵ�λ��
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_DRAWLINEEND1,this->u_SubscriptofModel,1);

	CStatic::OnLButtonUp(nFlags, point);
}

// ��������ʹ�ܹ���ķ���
//=============================================
// ���ã��жϿ����Ƿ�����ʹ������;
//       �ú�������ҪĿ�ģ�����������ʹ������ʱ�����øú�������������ʹ�ܱ�ʶ��true
// ���룺�����е�״̬�Ƿ��ݻ���ɣ����������������ļ�������Ƿ���ɵġ���ʶ����Ϊһ��bool����
// ����ֵ�����ء���������ʹ�������ı�ʶ����
// ���ã�
//       PNPlace p1;
//       bool bflatenb = p1.isEnable();
//============================================
bool CPlace::isEnable(double timestep/* = 0*/)
{
	switch(this->PlaceStyle)
	{

	case STYLE_DISCRETE:                                    // ��ɢ����
		{
			if(this->bActivation)
			{
				this->bEnable      = true;
				this->bActivation  = false;
				this->bisAssigned  = true;
			}
		}break;	

	case STYLE_CNTNUS://��������
		{
			this->bEnable          = runLuaFunction(timestep);
			this->bisworked        = true;
			if(this->bEnable)
			{
				this->bActivation  = false;
				this->bisAssigned  = true;
				//this->bisworked    = true;
			}
		}break;

	case STYLE_RAND://�������
		{
			/*srand((unsigned int)time(0));                 // ��ȡ��ǰϵͳʱ����Ϊrand()������seed���������Ա�֤rand()ÿ�β�����������ǲ�ͬ��
			int a = rand()%10;
			int b = rand()%10;
			int c = rand()%10;
			int d = rand()%10;                              // abcd�ֱ���0-9֮��������
			double dprob = (a*1000+b*100+c*10+d)/10000.0;   // ��������Ϊ����������ϵͳʱ��̫���������������Ӱ��*/
			if(this->ischoseProgRand)
			{
				this->bEnable = this->runS_LuaFunction(timestep);
				this->dProblty = this->arryOutputofLua.at(1)->Value;
			}

			static std::default_random_engine random(time(NULL));
			static std::uniform_real_distribution<double> dis(0.0, 1.0);
			d_RandProblty = dis(random);

			if(d_RandProblty<=(this->dProblty))
			{
				this->i_RandOutcome = true;//1;
				this->rand_tag = 1;
			}
			else
			{
				this->i_RandOutcome = false;//0;
				this->rand_tag = 0;
			}
			this->bEnable      = true;
			this->bActivation  = false;
			this->bisAssigned  = true;
		}break;

	case STYLE_TIME:                                         // ʱ�����
		{
			if(this->ischoseupdown && !(this->b_resettime))
			{
				//this->dTime_downline = m_dlgModelpropset.m_tabPropset1.dTime_downline;
				//this->dTime_upline = m_dlgModelpropset.m_tabPropset1.dTime_upline;
				double aver = (this->dTime_upline+this->dTime_downline)/2.0;
				double sigma = (this->dTime_upline-this->dTime_downline)/3.0;
				if(aver==0 &&  sigma==0)
				{
					this->dTime = 0;
				}
				else
				{
					static std::default_random_engine gen1;  
					static std::normal_distribution<double> distribution1(aver,sigma);
					if(!this->isTimeUpDownChange)
					{
						std::default_random_engine gen2;  
						std::normal_distribution<double> distribution2(aver,sigma); 
						distribution1 = distribution2;
						this->isTimeUpDownChange = true;
					}
					
					this->dTime = distribution1(gen1);
				}
				this->d_TimeTS      = this->dTime;
				this->b_resettime   = true;
			}

			this->dTime            = this->dTime - timestep;
			if(this->dTime<=0)
			{
				this->bEnable      = true;
				this->bActivation  = false;
				this->bisAssigned  = true;
				this->dTime        = this->d_TimeTS;
				this->b_resettime  = false;
			}
		}break;

	case STYLE_TCTRL:                                        // ���ƿ���
		{
			if(this->bActivation)
			{
				this->bEnable      = true;
				this->bActivation  = false;
				this->bisAssigned  = true;
			}
		}
		break;

	case STYLE_DISIC:                                        //���߿���
		{
			result *resofD;
			resofD                = runD_LuaFunction(timestep);
			this->bEnable         = resofD  ->bcanEnable;
			this->disic           = resofD->res;

			////lua�ű������ھ����������Ч���Ա�ʶ
			//for (int i = 0; i < resofD->res.size(); i++) {
			//	if (resofD->res.at(i)>0) this->count++;
			//}
			this->count = this->numTag-1;//���Լ�һ����ʱ��ʹ�����෢��һ��

			this->bisworked       = true;
			if(this->bEnable)
			{
				this->bActivation = false;
				this->bisAssigned = true;
			}
		}
		break;
	default:
		break;
	}
	return this->bEnable;
}


// ˫��ģ�ͣ�������ģ���������á��Ի������ģ�Ͱ�����bActivation����ֵ�ó�ʼ�� 
void CPlace::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
	// ����ģ����˫��ʱ����ʾ�������öԻ���
	CPlacePropSetDlg m_dlgModelpropset(this->PlaceStyle,m_thisModel);                // �������������ԶԻ������
	
	// ����ǰģ�͵�����ֵ����������ҳ,Ŀ�ģ����ٴ�˫���򿪶Ի���ʱ����Щֵ����ʾ�ڶԻ�����
	m_dlgModelpropset.m_tabPropset1.bActivation_set     = this->bActivation;             
	m_dlgModelpropset.m_tabPropset1.m_Value_Probset     = this->dProblty;
	m_dlgModelpropset.m_tabPropset1.m_Value_Timeset     = this->dTime;
	m_dlgModelpropset.m_tabPropset1.bisUserDef_prob     = this->bisUserDef_prob;
	m_dlgModelpropset.m_tabPropset1.bisUserDef_time     = this->bisUserDef_time;
	m_dlgModelpropset.m_tabPropset1.bisUserDef_ctrl     = this->bisUserDef_ctrl;
	m_dlgModelpropset.m_tabPropset1.str_Inputport_prob  = this->str_Inputport_prob;
	m_dlgModelpropset.m_tabPropset1.str_Inputport_time  = this->str_Inputport_time;
	m_dlgModelpropset.m_tabPropset1.str_Inputport_ctrl  = this->str_Inputport_ctrl;
	m_dlgModelpropset.m_tabPropset1.dTime_downline      = this->dTime_downline;
	m_dlgModelpropset.m_tabPropset1.dTime_upline        = this->dTime_upline;
	m_dlgModelpropset.m_tabPropset1.ischoseupdown       = this->ischoseupdown;
	m_dlgModelpropset.m_tabPropset1.ischoseProgRand     = this->ischoseProgRand;
	m_dlgModelpropset.m_tabPropset1.ischoseSocket       = this->ischoseSocket;
	m_dlgModelpropset.m_tabPropset1.str_ipadrs          = this->ipadrs.c_str();
	m_dlgModelpropset.m_tabPropset1.num_port            = this->port;
	m_dlgModelpropset.m_tabPropset1.num_output          = this->num_output;
	m_dlgModelpropset.m_tabPropset1.num_input           = this->num_input;

	// ����domodal���������Ի�������û����OK��ť�����ȡ�Ի����ϵ����ã�����ɿ�������ֵ�ó�ʼ��
	if(IDOK == m_dlgModelpropset.DoModal())                              
	{
		switch(this->PlaceStyle)
		{
			case STYLE_DISCRETE:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				break;

			case STYLE_CNTNUS:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;				
				this->Lua_fileName                      = m_dlgModelpropset.m_tabPropset1.Lua_fileName;              // Lua�ļ���
				this->ischoseSocket                     = m_dlgModelpropset.m_tabPropset1.ischoseSocket;
				if(this->ischoseSocket)
				{
					USES_CONVERSION;
					this->ipadrs                        = W2A(m_dlgModelpropset.m_tabPropset1.str_ipadrs);
					this->port                          = m_dlgModelpropset.m_tabPropset1.num_port;
					this->num_output                    = m_dlgModelpropset.m_tabPropset1.num_output;
					this->num_input                     = m_dlgModelpropset.m_tabPropset1.num_input;
				}
				break;

			case STYLE_RAND:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->dProblty                          = m_dlgModelpropset.m_tabPropset1.m_Value_Probset;
				this->bisUserDef_prob                   = m_dlgModelpropset.m_tabPropset1.bisUserDef_prob;
				this->str_Inputport_prob                = m_dlgModelpropset.m_tabPropset1.str_Inputport_prob;
				this->ischoseProgRand                   = m_dlgModelpropset.m_tabPropset1.ischoseProgRand;
				break;

			case STYLE_TIME:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->dTime                             = m_dlgModelpropset.m_tabPropset1.m_Value_Timeset;
				this->ischoseupdown                     = m_dlgModelpropset.m_tabPropset1.ischoseupdown;
				if(this->ischoseupdown)
				{
					this->dTime_downline                = m_dlgModelpropset.m_tabPropset1.dTime_downline;
					this->dTime_upline                  = m_dlgModelpropset.m_tabPropset1.dTime_upline;
					double aver                         = (dTime_upline-dTime_downline)/2.0;
					double sigma                        = (dTime_upline-dTime_downline)/3.0;
					if(aver==0 &&  sigma==0)
					{
						this->dTime = 0;
					}
					else
					{
						static std::default_random_engine        gen;  
						static std::normal_distribution<double>  distribution(aver,sigma);
						this->dTime                     = distribution(gen);
					}
				}

				this->bisUserDef_time                   = m_dlgModelpropset.m_tabPropset1.bisUserDef_time;
				this->str_Inputport_time                = m_dlgModelpropset.m_tabPropset1.str_Inputport_time;
				this->d_TimeTS                          = this->dTime;
				break;

			case STYLE_TCTRL:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->bisUserDef_ctrl                   = m_dlgModelpropset.m_tabPropset1.bisUserDef_ctrl;
				this->str_Inputport_ctrl                = m_dlgModelpropset.m_tabPropset1.str_Inputport_ctrl;
				break;

			case STYLE_DISIC:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->Lua_fileName                      = m_dlgModelpropset.m_tabPropset1.Lua_fileName;              // Lua�ļ���
				this->ischoseSocket                     = m_dlgModelpropset.m_tabPropset1.ischoseSocket;
				if(this->ischoseSocket)
				{
					USES_CONVERSION;
					this->ipadrs                        = W2A(m_dlgModelpropset.m_tabPropset1.str_ipadrs);
					this->port                          = m_dlgModelpropset.m_tabPropset1.num_port;
					this->num_output                    = m_dlgModelpropset.m_tabPropset1.num_output;
					this->num_input                     = m_dlgModelpropset.m_tabPropset1.num_input;
				}
				break;
			default:
				break;
		}
		this->Invalidate();               // Ŀ�ģ����û�����������ɺ���bActivation��Ϊtrue�����̸��¿���ģ�͵���ʾ
		this->UpdateWindow();
	}


	CStatic::OnLButtonDblClk(nFlags, point);
}

// to run the file including the function of Lua
bool CPlace::runLuaFunction(double timestep)
{
	//---------------------------����1;��ֵ-------------------------------------
	this->d_TimeStep = timestep;
	arryOutputofLua.at(1)->Value = this->d_TimeStep;                              // Ϊstep������ֵ
	bool bcanEnable = FALSE;                                                      // ����һ��Boolֵ����Ϊ�����ĵ�һ������ֵ���͵�һ������ֵ����������Ϊ������������ʹ�������ж�����

	                                                                              // �Ȼ�ȡLua�ļ�����������飬����������������������и�ֵ
	int sizeofLuaIN  = arryInputofLua.size();                                     // ��ȡ��������ĸ���
	int sizeofLuaOUT = arryOutputofLua.size();                                    // ��ȡ��������ĸ���
	for(int i = 0;i<sizeofLuaIN;i++)
	{
		InValue a;
		a.Name     = arryInputofLua.at(i)->Name;
		a.isQuoteG = arryInputofLua.at(i)->isQuoteG;
		a.Value    = arryInputofLua.at(i)->Value;
		if(this->bisworked)
		{
			this->arryInputData.at(i) = a;
		}
		else
		{
			this->arryInputData.push_back(a);
		}
	}
	if(sizeofLuaIN >= sizeofLuaOUT)
	{
		for(int i = 0;i<sizeofLuaOUT;i++)
		{
			arryInputofLua.at(i)->Value = arryOutputofLua.at(i)->Value;
		}
	}
	else
	{
		MessageBox(L"���뵽Lua�ű��еĲ����ĸ���Ӧ����������ĸ���");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
	}
	if(this->ischoseSocket)
	{
		for(int i = 2;i<(this->num_output+2);i++)
		{
			int b = (int)arryInputofLua.at(i)->Value;//ǿ��ת�������ܶ�ʧ����
			stringstream stream;
			stream << b;
			string c = stream.str();
			this->msgO = this->msgO + c;
		}
		int isconnected = runSocketFunction(this->ipadrs, this->port);
		if(isconnected == 0)
		{
			for(int i = 0;i < this->msgI.length();i++)
			{
				double a;
				stringstream stream;
				stream<<msgI.at(i);
				stream>>a;
				arryInputofLua.at(num_output + 2 + i)->Value = a;
			}
		}
	}
	
	//--------------------------����2����ȡLua�ļ�����������--------------------
	CString Lua_fileName_1 = _T("LuaFile\\") + this->m_caption + _T(".lua");     // ��ȡ��Ӧ���ļ�����·��
	char * str_luafilename;
	char * str_luafucname;
	USES_CONVERSION;
    str_luafilename = T2A(Lua_fileName_1);                                       // ���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
	str_luafucname = T2A(this->m_caption);

	//--------------------------����3������һ��Lua״̬��-------------------------
	lua_State *L = luaL_newstate();//1.
	if(L==NULL)
	{
		MessageBox(L"����Lua��ջʧ��");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);//�̴߳��������¼�Ϊ���ź�״̬��Ҳ������CHSTPN_SYSSim���HSTPN_Simulation()�����У����ú����˳��������̽���
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	lua_checkstack(L, 1000);//����ջ�Ŀ��óߴ磬ջ��Ĭ�ϳߴ���20���˺�����ȷ����ջ�������� sz ����λ
	//--------------------------����4����״̬��L�е����е�Lua��׼��------------
	luaL_openlibs(L); 
	//--------------------------����5������Lua�ļ�������һ���ļ�����ΪLua�����--
	int bRet = luaL_loadfile(L,str_luafilename);
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);//ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"����Lua�ű��ļ�ʧ��! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------����6������һ��Lua�ļ�---------------------------
	bRet = lua_pcall(L,0,0,0);                                                  // 4.����һ��Lua�ļ�����Ϊֻ������һ�θ��ļ����ļ��е����ݲ��ܱ����ʣ�
	                                                                            // L��ʾִ�л�������Lua��������ڶ�������������ֵ�ĸ�����������������
	                                                                            // ����ֵ�ĸ��������һ���������ڷ���������ʱ�Ĵ��뷵��
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);                              // ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"����Lua�ű��ļ�ʧ��! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------����7����ȡLua�ļ��е�Lua����--------------------
	lua_getglobal(L,str_luafucname);                                           // 5.��ȡ������ѹ��ջ�У���Ȼ������ļ��к���һЩȫ�ֱ���Ҳ����ͨ��lua_getglobal()�������ж�ȡ��ѹ��ջ��
	//--------------------------����8��Lua�������������ѹջ---------------------
	lua_pushboolean(L,(int)arryInputofLua.at(0)->Value);                       // ����һ��boolean����ѹջ  
	for(int i=1;i<sizeofLuaIN;i++)                                             // �������������ѹ��ջ��
	{
		lua_pushnumber(L,arryInputofLua.at(i)->Value);
	}
	//--------------------------����9������Lua����--------------------------------
	int iRet = lua_pcall(L,sizeofLuaIN,sizeofLuaOUT,0);                        // int iRet = lua_pcall(L,2,1,0); // 6.���ú�����������ɺ󣬻Ὣ���ؽ��ѹ��ջ��
	                                                                           // 2��ʾ�����ĸ�����1��ʾ���ؽ���ĸ���
	                                                                           // ������ó����Ὣ������Ϣѹ��ջ��
	if(iRet)
	{
		const char* errorstr = lua_tostring(L,-1);                             // ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"�������ô���! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		lua_close(L);
		return FALSE;
	}
	//--------------------------����10��Lua�������ֵѹջ--------------------------
	if(lua_isboolean(L,-sizeofLuaOUT))                                         // ȡ�����һ��������������ʶ
	{
		arryOutputofLua.at(0)->Value = lua_toboolean(L,-sizeofLuaOUT);
		if(lua_toboolean(L,-sizeofLuaOUT)==0)
		{
			bcanEnable = false;
		}
		else
		{
			bcanEnable = true;
		}
	}
	else
	{
		MessageBox(L"Lua�����ĵ�һ�����ֵ����Ϊbool��");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	for(int i=0;i<sizeofLuaOUT-1;i++)                                          // ȡ����������
	{
		if(lua_isnumber(L,-(i+1)))
		{
			arryOutputofLua.at(sizeofLuaOUT-i-1)->Value = lua_tonumber(L,-(i+1));
		}
		else
		{
			MessageBox(L"Lua�������������ֵ����Ϊnumber��");
			SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
			m_pParentView->GetDocument()->m_simThread = NULL;
			return FALSE;
		}
	}
	//--------------------------����111���ر�Lua�����----------------------------
	lua_close(L);    

	return bcanEnable;
}

// tell the View class the drawing of Arcs is finish
void CPlace::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CStatic::OnRButtonUp(nFlags, point);
	//----------------������Ҽ��������ģ��ʱ,����"ģ�ͱ༭"�����˵�------begin----
	CMenu m_MDeditpopup;
	m_MDeditpopup.LoadMenuW(IDR_MDEDIT_POPUP1);

	ClientToScreen(&point);

	m_MDeditpopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);//GetParent());//����һ�������Ĳ˵�

	//----------------������Ҽ��������ģ��ʱ,����"ģ�ͱ༭"�����˵�------end------
}

// delete this place
void CPlace::OnMdDelete()
{
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_MDDELETE1,this->u_SubscriptofModel,1);
}

// ��ת����
void CPlace::OnRotateTl()
{
	// TODO: �ڴ���������������
	if(this->PlaceStyle == STYLE_TCTRL)
	{
		if(this->CtrlPlaceStyle == TCTRL)
		{
			this->CtrlPlaceStyle = LCTRL;
		}
		else
		{
			this->CtrlPlaceStyle = TCTRL;
		}
		this->Invalidate();
		this->UpdateWindow();
	}
}

// �����������öԻ���
void CPlace::OnShowpropdlg()
{
	// �����ģ���Ҽ��˵��С����ԡ�����ʾ�������öԻ���
	CPlacePropSetDlg m_dlgModelpropset(this->PlaceStyle,m_thisModel);                // �������������ԶԻ������
	
	// ����ǰģ�͵�����ֵ����������ҳ,Ŀ�ģ����ٴ�˫���򿪶Ի���ʱ����Щֵ����ʾ�ڶԻ�����
	m_dlgModelpropset.m_tabPropset1.bActivation_set     = this->bActivation;             
	m_dlgModelpropset.m_tabPropset1.m_Value_Probset     = this->dProblty;
	m_dlgModelpropset.m_tabPropset1.m_Value_Timeset     = this->dTime;
	m_dlgModelpropset.m_tabPropset1.bisUserDef_prob     = this->bisUserDef_prob;
	m_dlgModelpropset.m_tabPropset1.bisUserDef_time     = this->bisUserDef_time;
	m_dlgModelpropset.m_tabPropset1.bisUserDef_ctrl     = this->bisUserDef_ctrl;
	m_dlgModelpropset.m_tabPropset1.str_Inputport_prob  = this->str_Inputport_prob;
	m_dlgModelpropset.m_tabPropset1.str_Inputport_time  = this->str_Inputport_time;
	m_dlgModelpropset.m_tabPropset1.str_Inputport_ctrl  = this->str_Inputport_ctrl;
	m_dlgModelpropset.m_tabPropset1.dTime_downline      = this->dTime_downline;
	m_dlgModelpropset.m_tabPropset1.dTime_upline        = this->dTime_upline;
	m_dlgModelpropset.m_tabPropset1.ischoseupdown       = this->ischoseupdown;
	m_dlgModelpropset.m_tabPropset1.ischoseProgRand     = this->ischoseProgRand;

	// ����domodal���������Ի�������û����OK��ť�����ȡ�Ի����ϵ����ã�����ɿ�������ֵ�ó�ʼ��
	if(IDOK == m_dlgModelpropset.DoModal())                              
	{
		switch(this->PlaceStyle)
		{
			case STYLE_DISCRETE:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				break;

			case STYLE_CNTNUS:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->Lua_fileName                      = m_dlgModelpropset.m_tabPropset1.Lua_fileName;              // Lua�ļ���
				break;

			case STYLE_RAND:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->dProblty                          = m_dlgModelpropset.m_tabPropset1.m_Value_Probset;
				this->bisUserDef_prob                   = m_dlgModelpropset.m_tabPropset1.bisUserDef_prob;
				this->str_Inputport_prob                = m_dlgModelpropset.m_tabPropset1.str_Inputport_prob;
				this->ischoseProgRand                   = m_dlgModelpropset.m_tabPropset1.ischoseProgRand;
				break;

			case STYLE_TIME:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->dTime                             = m_dlgModelpropset.m_tabPropset1.m_Value_Timeset;
				this->ischoseupdown                     = m_dlgModelpropset.m_tabPropset1.ischoseupdown;
				if(this->ischoseupdown)
				{
					this->dTime_downline                = m_dlgModelpropset.m_tabPropset1.dTime_downline;
					this->dTime_upline                  = m_dlgModelpropset.m_tabPropset1.dTime_upline;
					double aver                         = (dTime_upline-dTime_downline)/2.0;
					double sigma                        = (dTime_upline-dTime_downline)/3.0;
					if(aver==0 &&  sigma==0)
					{
						this->dTime = 0;
					}
					else
					{
						static std::default_random_engine        gen;  
						static std::normal_distribution<double>  distribution(aver,sigma);
						this->dTime                     = distribution(gen);
					}
				}

				this->bisUserDef_time                   = m_dlgModelpropset.m_tabPropset1.bisUserDef_time;
				this->str_Inputport_time                = m_dlgModelpropset.m_tabPropset1.str_Inputport_time;
				this->d_TimeTS                          = this->dTime;
				break;

			case STYLE_TCTRL:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->bisUserDef_ctrl                   = m_dlgModelpropset.m_tabPropset1.bisUserDef_ctrl;
				this->str_Inputport_ctrl                = m_dlgModelpropset.m_tabPropset1.str_Inputport_ctrl;
				break;

			default:
				break;
		}
		this->Invalidate();               // Ŀ�ģ����û�����������ɺ���bActivation��Ϊtrue�����̸��¿���ģ�͵���ʾ
		this->UpdateWindow();
	}
}


// ��д���������Lua�ű����ú���
bool CPlace::runS_LuaFunction(double timestep)
{
	//---------------------------����1;��ֵ-------------------------------------
	this->d_TimeStep = timestep;
	arryOutputofLua.at(1)->Value = this->dProblty;                                         // Ϊ�����ֵ��ֵ
	bool bcanEnable = FALSE;                                                               // ����һ��Boolֵ����Ϊ�����ĵ�һ������ֵ���͵�һ������ֵ����������Ϊ������������ʹ�������ж�����

	                                                                                       // �Ȼ�ȡLua�ļ�����������飬����������������������и�ֵ
	int sizeofLuaIN  = arryInputofLua.size();                                              // ��ȡ��������ĸ���
	int sizeofLuaOUT = arryOutputofLua.size();                                             // ��ȡ��������ĸ���
	if(sizeofLuaIN >= sizeofLuaOUT)
	{
		for(int i = 0;i<sizeofLuaOUT;i++)
		{
			arryInputofLua.at(i)->Value = arryOutputofLua.at(i)->Value;
		}
	}
	else
	{
		MessageBox(L"���뵽Lua�ű��еĲ����ĸ���Ӧ����������ĸ���");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
	}
	
	//--------------------------����2����ȡLua�ļ�����������--------------------
	CString Lua_fileName_1 = _T("LuaFile\\S_") + this->m_caption + _T(".lua");            // ��ȡ��Ӧ���ļ�����·��
	char * str_luafilename;
	char * str_luafucname;
	USES_CONVERSION;
    str_luafilename = T2A(Lua_fileName_1);                                                // ���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
	str_luafucname = T2A(_T("S_")+this->m_caption);

	//--------------------------����3������һ��Lua״̬��-------------------------
	lua_State *L = luaL_newstate();//1.
	if(L==NULL)
	{
		MessageBox(L"����Lua��ջʧ��");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	lua_checkstack(L, 1000);
	//--------------------------����4����״̬��L�е����е�Lua��׼��------------
	luaL_openlibs(L); 
	//--------------------------����5������Lua�ļ�������һ���ļ�����ΪLua�����--
	int bRet = luaL_loadfile(L,str_luafilename);
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);                                         // ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"����Lua�ű��ļ�ʧ��! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------����6������һ��Lua�ļ�---------------------------
	bRet = lua_pcall(L,0,0,0);                                                            // 4.����һ��Lua�ļ�����Ϊֻ������һ�θ��ļ����ļ��е����ݲ��ܱ����ʣ�
	                                                                                      // L��ʾִ�л�������Lua��������ڶ�������������ֵ�ĸ�����������������
	                                                                                      // ����ֵ�ĸ��������һ���������ڷ���������ʱ�Ĵ��뷵��
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);//ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"����Lua�ű��ļ�ʧ��! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------����7����ȡLua�ļ��е�Lua����--------------------
	lua_getglobal(L,str_luafucname);                                                     // 5.��ȡ������ѹ��ջ�У���Ȼ������ļ��к���һЩȫ�ֱ���Ҳ����ͨ��lua_getglobal()�������ж�ȡ��ѹ��ջ��
	//--------------------------����8��Lua�������������ѹջ---------------------
	lua_pushboolean(L,(int)arryInputofLua.at(0)->Value);                                 // ����һ��boolean����ѹջ  
	for(int i=1;i<sizeofLuaIN;i++)                                                       // �������������ѹ��ջ��
	{
		lua_pushnumber(L,arryInputofLua.at(i)->Value);
	}
	//--------------------------����9������Lua����--------------------------------
	int iRet = lua_pcall(L,sizeofLuaIN,sizeofLuaOUT,0);                                  // int iRet = lua_pcall(L,2,1,0); // 6.���ú�����������ɺ󣬻Ὣ���ؽ��ѹ��ջ��
	                                                                                     // 2��ʾ�����ĸ�����1��ʾ���ؽ���ĸ���
	                                                                                     // ������ó����Ὣ������Ϣѹ��ջ��
	if(iRet)
	{
		const char* errorstr = lua_tostring(L,-1);                                       // ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"�������ô���! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		lua_close(L);
		return FALSE;
	}
	//--------------------------����10��Lua�������ֵѹջ--------------------------
	if(lua_isboolean(L,-sizeofLuaOUT))                                                   // ȡ�����һ��������������ʶ
	{
		arryOutputofLua.at(0)->Value = lua_toboolean(L,-sizeofLuaOUT);
		if(lua_toboolean(L,-sizeofLuaOUT)==0)
		{
			bcanEnable = false;
		}
		else
		{
			bcanEnable = true;
		}
	}
	else
	{
		MessageBox(L"Lua�����ĵ�һ�����ֵ����Ϊbool��");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	for(int i=0;i<sizeofLuaOUT-1;i++)                                                   // ȡ����������
	{
		if(lua_isnumber(L,-(i+1)))
		{
			arryOutputofLua.at(sizeofLuaOUT-i-1)->Value = lua_tonumber(L,-(i+1));
		}
		else
		{
			MessageBox(L"Lua�������������ֵ����Ϊnumber��");
			SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
			m_pParentView->GetDocument()->m_simThread = NULL;
			return FALSE;
		}
	}
	//--------------------------����111���ر�Lua�����----------------------------
	lua_close(L);    

	return bcanEnable;
}


// ��д���߿�����Lua�ű����ú���
CPlace::result * CPlace::runD_LuaFunction(double timestep)
{
	//---------------------------����1;��ֵ-------------------------------------
	result *resofD = new result;
	this->d_TimeStep = timestep; 
	arryOutputofLua.at(1)->Value = this->d_TimeStep;                              // Ϊstep������ֵ
	bool bcanEnable = FALSE;                                                      // ����һ��Boolֵ����Ϊ�����ĵ�һ������ֵ���͵�һ������ֵ����������Ϊ������������ʹ�������ж�����
	
	vector<int> resultofD;                                                        // �Ȼ�ȡLua�ļ�����������飬����������������������и�ֵ
	
	
	
	int sizeofLuaIN  = arryInputofLua.size();                                     // ��ȡ��������ĸ���
	int sizeofLuaOUT = arryOutputofLua.size();                                    // ��ȡ��������ĸ���
	
	for(int i = 0;i<sizeofLuaIN;i++)
	{
		InValue a;
		a.Name     = arryInputofLua.at(i)->Name;
		a.isQuoteG = arryInputofLua.at(i)->isQuoteG;
		a.Value    = arryInputofLua.at(i)->Value;
		if(this->bisworked)
		{
			this->arryInputData.at(i) = a;
		}
		else
		{
			this->arryInputData.push_back(a);
		}
	}
	if(sizeofLuaIN >= sizeofLuaOUT)
	{
		for(int i = 0;i<sizeofLuaOUT;i++)
		{
			arryInputofLua.at(i)->Value = arryOutputofLua.at(i)->Value;
		}
	}
	else
	{
		MessageBox(L"���뵽Lua�ű��еĲ����ĸ���Ӧ����������ĸ���");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
	}
	if(this->ischoseSocket)
	{
		msgO = "";
		msgI = "";
		for(int i = 3;i<(this->num_output+3);i++)
		{
			int b = (int)arryInputofLua.at(i)->Value;//ǿ��ת�������ܶ�ʧ����
			stringstream stream;
			stream << b;
			string c = stream.str();
			this->msgO = this->msgO + c;
		}
		int isconnected = runSocketFunction(this->ipadrs, this->port);
		if(isconnected == 0)
		{
			for(int i = 0;i < this->msgI.length();i++)
			{
				double a;
				stringstream stream;
				stream<<msgI.at(i);
				stream>>a;
				arryInputofLua.at(num_output + 3 + i)->Value = a;
			}
		}
		msgO = "";
		msgI = "";
	}
	
	//--------------------------����2����ȡLua�ļ�����������--------------------
	CString Lua_fileName_1 = _T("LuaFile\\") + this->m_caption + _T(".lua");     // ��ȡ��Ӧ���ļ�����·��
	char * str_luafilename;
	char * str_luafucname;
	USES_CONVERSION;
    str_luafilename = T2A(Lua_fileName_1);                                       // ���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
	str_luafucname = T2A(this->m_caption);

	//--------------------------����3������һ��Lua״̬��-------------------------
	lua_State *L = luaL_newstate();//1.
	if(L==NULL)
	{
		MessageBox(L"����Lua��ջʧ��");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	lua_checkstack(L, 1000);
	//--------------------------����4����״̬��L�е����е�Lua��׼��------------
	luaL_openlibs(L); 
	//--------------------------����5������Lua�ļ�������һ���ļ�����ΪLua�����--
	int bRet = luaL_loadfile(L,str_luafilename);
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);//ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"����Lua�ű��ļ�ʧ��! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------����6������һ��Lua�ļ�---------------------------
	bRet = lua_pcall(L,0,0,0);                                                  // 4.����һ��Lua�ļ�����Ϊֻ������һ�θ��ļ����ļ��е����ݲ��ܱ����ʣ�
	                                                                            // L��ʾִ�л�������Lua��������ڶ�������������ֵ�ĸ�����������������
	                                                                            // ����ֵ�ĸ��������һ���������ڷ���������ʱ�Ĵ��뷵��
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);                              // ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"����Lua�ű��ļ�ʧ��! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------����7����ȡLua�ļ��е�Lua����--------------------
	lua_getglobal(L,str_luafucname);                                           // 5.��ȡ������ѹ��ջ�У���Ȼ������ļ��к���һЩȫ�ֱ���Ҳ����ͨ��lua_getglobal()�������ж�ȡ��ѹ��ջ��
	//--------------------------����8��Lua�������������ѹջ---------------------
	lua_pushboolean(L,(int)arryInputofLua.at(0)->Value);                       // ����һ��boolean����ѹջ  
	for(int i=1;i<sizeofLuaIN;i++)                                             // �������������ѹ��ջ��
	{
		lua_pushnumber(L,arryInputofLua.at(i)->Value);
	}
	//--------------------------����9������Lua����--------------------------------
	int iRet = lua_pcall(L,sizeofLuaIN,sizeofLuaOUT,0);                        // int iRet = lua_pcall(L,2,1,0); // 6.���ú�����������ɺ󣬻Ὣ���ؽ��ѹ��ջ��
	                                                                           // 2��ʾ�����ĸ�����1��ʾ���ؽ���ĸ���
	                                                                           // ������ó����Ὣ������Ϣѹ��ջ��
	if(iRet)
	{
		const char* errorstr = lua_tostring(L,-1);                             // ȡ��������Ϣ
		CString Error(errorstr);
		MessageBox(L"�������ô���! \r\n ������Ϣ��"+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		lua_close(L);
		return FALSE;
	}
	//--------------------------����10��Lua�������ֵѹջ--------------------------
	if(lua_isboolean(L,-sizeofLuaOUT))                                         // ȡ�����һ��������������ʶ
	{
		arryOutputofLua.at(0)->Value = lua_toboolean(L,-sizeofLuaOUT);
		if(lua_toboolean(L,-sizeofLuaOUT)==0)
		{
			bcanEnable = false;
		}
		else
		{
			bcanEnable = true;
		}
	}
	else
	{
		MessageBox(L"Lua�����ĵ�һ�����ֵ����Ϊbool��");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}

	numTag = lua_tonumber(L, -sizeofLuaOUT + 2);                             //��ȡ���Ա�ʶ������
	for(int i=0;i<sizeofLuaOUT-1;i++)                                          // ȡ����������
	{
		if(lua_isnumber(L,-(i+1)))
		{
			arryOutputofLua.at(sizeofLuaOUT-i-1)->Value = lua_tonumber(L,-(i+1));
			if(bcanEnable&&i<numTag)//��ȥlua�ű���tag���͵Ĳ���
				resultofD.push_back(lua_tonumber(L,-sizeofLuaOUT +i+3));			
		}
		else
		{
			MessageBox(L"Lua�������������ֵ����Ϊnumber��");
			SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
			m_pParentView->GetDocument()->m_simThread = NULL;
			return FALSE;
		}
	}
	//--------------------------����111���ر�Lua�����----------------------------
	lua_close(L);    
	  
	resofD->bcanEnable = bcanEnable;
	resofD->res = resultofD;

	return resofD;
}


int CPlace::runSocketFunction(string ipadrs, int port)
{
	const int buf_size = 64;
	WSADATA wsd; //WSADATA����  
	SOCKET sHost; //�������׽���  
	SOCKADDR_IN servAddr; //��������ַ  
	char buf[buf_size] = "\0"; //�������ݻ�����  
	char bufRecv[buf_size];
	bool flag = true;
//	char * ip;
//	itoa(this->ipadrs, ip, 10);
	int retVal; //����ֵ
	//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)  
	{  
		//cout << "WSAStartup failed!" << endl;
		MessageBox(L"WSAStartup failed");
		return -1;
	}  
	//�����׽���  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(INVALID_SOCKET == sHost)  
	{  
		//cout << "socket failed!" << endl;
		MessageBox(L"socket failed");
		WSACleanup();//�ͷ��׽�����Դ  
		return -1;  
	}  
	//���÷�������ַ  
	servAddr.sin_family =AF_INET;  
	servAddr.sin_addr.s_addr = inet_addr(this->ipadrs.c_str());  
	servAddr.sin_port = htons((short)this->port);  
	int nServAddlen  = sizeof(servAddr);
	//���ӷ�����  
	retVal=connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr));  
	if(SOCKET_ERROR == retVal)  
	{  
		//cout << "connect failed!" << endl;
		MessageBox(L"connect failed");
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return -1;  
	}
	while(flag)  
	{  
		//���������������  
		ZeroMemory(buf, buf_size); 
		for(int i = 0; i < this->msgO.length(); i++)
		{
			buf[i] = this->msgO[i];
		}
		retVal = send(sHost, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retVal)  
		{  
			//cout << "send failed!" << endl;
			MessageBox(L"send failed");
			closesocket(sHost); //�ر��׽���  
			WSACleanup(); //�ͷ��׽�����Դ  
			return -1;  
		}  
		//RecvLine(sHost, bufRecv);  
		ZeroMemory(bufRecv, buf_size);  
		recv(sHost, bufRecv,buf_size , 0); // ���շ������˵����ݣ� ֻ����5���ַ�
		this->msgI = bufRecv;
		flag = false;
	}
	//�˳�  
	closesocket(sHost); //�ر��׽���  
	WSACleanup(); //�ͷ��׽�����Դ  
	return 0;
}
