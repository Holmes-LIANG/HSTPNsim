// Model.cpp : 实现文件
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

	dTime_downline       = 0;     // 时间区间的下限
	dTime_upline         = 1;     // 时间区间的上限
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

//重载构造函数
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
	this->dTime_downline     = C.dTime_downline;// 时间区间的下限
	this->dTime_upline       = C.dTime_upline;// 时间区间的上限
	this->ischoseupdown      = C.ischoseupdown;
	this->b_resettime        = C.b_resettime;
	this->ischoseProgRand    = C.ischoseProgRand;
}
//重载操作符=
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
	this->dTime_downline     = A.dTime_downline;// 时间区间的下限
	this->dTime_upline       = A.dTime_upline;// 时间区间的上限
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



// CPlace 消息处理程序


BOOL CPlace::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,enumPlaceStyle PlaceStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
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

	//-------获取仿真步长---
	this->d_TimeStep   = ((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_TimeStep;
	
	//----------给维护的四个连接点赋值---------------------
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

	//-----------绘制库所模型的外观-----------------------
	CRect                rectClient;
	GetClientRect(&rectClient);
	CRect                rect(&rectClient);
	CRect                rect_token;
	rect_token.left    = rect.left + 12;
	rect_token.right   = rect.right - 12;
	rect_token.top     = rect.top + 12;
	rect_token.bottom  = rect.bottom - 12;
	
	CPen           *     oldpen_old;
	CPen                 pen(PS_SOLID,2,RGB(0,0,0));             // 创建画笔，颜色为黑色
	oldpen_old         = dc.SelectObject(&pen);

	if(this->PlaceStyle   == STYLE_DISCRETE)                       // 画出离散库所的外型
	{	
		if(!this->bEnable)                                       // 未使能状态下为白色
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			if(this->bActivation)                                // 如果是激活状态，画空心标识
			{
				CPen         pen(PS_SOLID,2,RGB(108,226,108));   // 创建画笔
				oldpen_old = dc.SelectObject(&pen);
				CBrush       newbrush(RGB(255,255,255));
				CBrush  *    oldbrush;
				oldbrush   = dc.SelectObject(&newbrush);
				dc.Ellipse(&rect_token);
				dc.SelectObject(oldbrush);
				dc.SelectObject(oldpen_old);
			}
		}
		else                                                     // 使能状态下为绿色
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			// 画token
			CPen             pen(PS_SOLID,2,RGB(108,226,108));   // 创建画笔
			oldpen_old     = dc.SelectObject(&pen);
			CBrush           newbrush(RGB(108,226,108));
			CBrush  *        oldbrush;
			oldbrush       = dc.SelectObject(&newbrush);
			dc.Ellipse(&rect_token);
			dc.SelectObject(oldbrush);
			dc.SelectObject(oldpen_old);
		}
	}
	if(this->PlaceStyle   == STYLE_CNTNUS)                        // 画出连续库所的外形
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
			if(this->bActivation)// 如果是激活状态，画空心标识
			{
				CPen         pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔
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
			// 画Token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔，颜色为黑色
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
	if(this->PlaceStyle == STYLE_RAND)// 画出随机库所的外形
	{
		CPen newpen(PS_SOLID,4,RGB(0,0,0));
		//CPen *oldpen;
		oldpen_old = dc.SelectObject(&newpen);
		dc.SelectObject(&oldpen_old);
		if(!this->bEnable)
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			if(this->bActivation)// 如果是激活状态，画空心标识
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔
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
			//画token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔，颜色为黑色
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
	if(this->PlaceStyle == STYLE_TIME)// 画出时间库所的外形
	{
		CPen newpen(PS_DOT,1,RGB(0,0,0));
		//CPen *oldpen;
		oldpen_old = dc.SelectObject(&newpen);
		dc.SelectObject(&oldpen_old);
		if(!this->bEnable)
		{
			dc.Ellipse(&rect);
			dc.SelectObject(oldpen_old);
			if(this->bActivation)// 如果是激活状态，画空心标识
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔
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
			//画token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔，颜色为黑色
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

	if(this->PlaceStyle == STYLE_TCTRL)// 画出突控库所的外形
	{
		if(this->CtrlPlaceStyle == TCTRL)
		{
		dc.Ellipse(&rect);//画外圆
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
			if(this->bActivation)// 如果是激活状态，画空心标识
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔
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
			//画token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔，颜色为黑色
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
			if(this->bActivation)// 如果是激活状态，画空心标识
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔
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
			//画token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔，颜色为黑色
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
	if(this->PlaceStyle == STYLE_LCTRL)// 画出突控库所的外形
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
			if(this->bActivation)// 如果是激活状态，画空心标识
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔
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
			//画token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔，颜色为黑色
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
			
			if(this->bActivation)// 如果是激活状态，画空心标识
			{
				CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔
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
			//画token
			CPen pen(PS_SOLID,2,RGB(108,226,108)); // 创建画笔，颜色为黑色
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	// 当鼠标左键点击模型时，首先将模型的下标发生给View类（先发送给MainFrame类，再发送给View类）
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_MOVEMODEL1,this->u_SubscriptofModel,1);//Lparm中的1表示模型为库所

	CStatic::OnLButtonDown(nFlags, point);
}


void CPlace::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//-----------------------------------------
	// 当鼠标在控件内抬起的时候，可能是在画线状态结束时，因此发生消息到View类，使其获得该控件的连接点位置
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_DRAWLINEEND1,this->u_SubscriptofModel,1);

	CStatic::OnLButtonUp(nFlags, point);
}

// 描述库所使能规则的方法
//=============================================
// 作用：判断库所是否满足使能条件;
//       该函数的主要目的，当库所满足使能条件时，调用该函数，给库所的使能标识置true
// 输入：库所中的状态是否演化完成，即库所中所包含的计算过程是否完成的“标识”，为一个bool变量
// 返回值：返回“库所满足使能条件的标识”；
// 调用：
//       PNPlace p1;
//       bool bflatenb = p1.isEnable();
//============================================
bool CPlace::isEnable(double timestep/* = 0*/)
{
	switch(this->PlaceStyle)
	{

	case STYLE_DISCRETE:                                    // 离散库所
		{
			if(this->bActivation)
			{
				this->bEnable      = true;
				this->bActivation  = false;
				this->bisAssigned  = true;
			}
		}break;	

	case STYLE_CNTNUS://连续库所
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

	case STYLE_RAND://随机库所
		{
			/*srand((unsigned int)time(0));                 // 获取当前系统时间作为rand()函数的seed，这样可以保证rand()每次产生的随机数是不同的
			int a = rand()%10;
			int b = rand()%10;
			int c = rand()%10;
			int d = rand()%10;                              // abcd分别是0-9之间的随机数
			double dprob = (a*1000+b*100+c*10+d)/10000.0;   // 这样做是为了消除两次系统时间太近对随机数产生的影响*/
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

	case STYLE_TIME:                                         // 时间库所
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

	case STYLE_TCTRL:                                        // 控制库所
		{
			if(this->bActivation)
			{
				this->bEnable      = true;
				this->bActivation  = false;
				this->bisAssigned  = true;
			}
		}
		break;

	case STYLE_DISIC:                                        //决策库所
		{
			result *resofD;
			resofD                = runD_LuaFunction(timestep);
			this->bEnable         = resofD  ->bcanEnable;
			this->disic           = resofD->res;

			////lua脚本中用于决策任务的有效策略标识
			//for (int i = 0; i < resofD->res.size(); i++) {
			//	if (resofD->res.at(i)>0) this->count++;
			//}
			this->count = this->numTag-1;//不自减一运行时会使库所多发生一次

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


// 双击模型，弹出“模型属性设置”对话框，完成模型包括“bActivation”等值得初始化 
void CPlace::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
	// 当在模型上双击时，显示属性设置对话框
	CPlacePropSetDlg m_dlgModelpropset(this->PlaceStyle,m_thisModel);                // 声明弹出的属性对话框对象
	
	// 将当前模型的属性值给属性设置页,目的：当再次双击打开对话框时，这些值能显示在对话框上
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

	// 调用domodal方法弹出对话框，如果用户点击OK按钮，则获取对话框上的设置，并完成库所属性值得初始化
	if(IDOK == m_dlgModelpropset.DoModal())                              
	{
		switch(this->PlaceStyle)
		{
			case STYLE_DISCRETE:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				break;

			case STYLE_CNTNUS:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;				
				this->Lua_fileName                      = m_dlgModelpropset.m_tabPropset1.Lua_fileName;              // Lua文件名
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
				this->Lua_fileName                      = m_dlgModelpropset.m_tabPropset1.Lua_fileName;              // Lua文件名
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
		this->Invalidate();               // 目的：当用户设置设置完成后，如bActivation设为true后，立刻更新库所模型的显示
		this->UpdateWindow();
	}


	CStatic::OnLButtonDblClk(nFlags, point);
}

// to run the file including the function of Lua
bool CPlace::runLuaFunction(double timestep)
{
	//---------------------------步骤1;赋值-------------------------------------
	this->d_TimeStep = timestep;
	arryOutputofLua.at(1)->Value = this->d_TimeStep;                              // 为step步长赋值
	bool bcanEnable = FALSE;                                                      // 定义一个Bool值，作为函数的第一个输入值，和第一个返回值，并最终作为最终连续库所使能与否的判断依据

	                                                                              // 先获取Lua文件的输入变量组，并利用输出变量数组对其进行赋值
	int sizeofLuaIN  = arryInputofLua.size();                                     // 获取输入变量的个数
	int sizeofLuaOUT = arryOutputofLua.size();                                    // 获取输入变量的个数
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
		MessageBox(L"输入到Lua脚本中的参数的个数应≥输出参数的个数");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
	}
	if(this->ischoseSocket)
	{
		for(int i = 2;i<(this->num_output+2);i++)
		{
			int b = (int)arryInputofLua.at(i)->Value;//强制转换，可能丢失精度
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
	
	//--------------------------步骤2：获取Lua文件名及函数名--------------------
	CString Lua_fileName_1 = _T("LuaFile\\") + this->m_caption + _T(".lua");     // 获取相应的文件名和路径
	char * str_luafilename;
	char * str_luafucname;
	USES_CONVERSION;
    str_luafilename = T2A(Lua_fileName_1);                                       // 调用函数，T2A和W2A均支持ATL和MFC中的字符转换
	str_luafucname = T2A(this->m_caption);

	//--------------------------步骤3：创建一个Lua状态机-------------------------
	lua_State *L = luaL_newstate();//1.
	if(L==NULL)
	{
		MessageBox(L"创建Lua堆栈失败");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);//线程处理，设置事件为有信号状态，也就是在CHSTPN_SYSSim类的HSTPN_Simulation()函数中，设置函数退出，即进程结束
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	lua_checkstack(L, 1000);//扩大栈的可用尺寸，栈的默认尺寸是20，此函数会确保堆栈上至少有 sz 个空位
	//--------------------------步骤4：打开状态机L中的所有的Lua标准库------------
	luaL_openlibs(L); 
	//--------------------------步骤5：加载Lua文件；即把一个文件加载为Lua代码块--
	int bRet = luaL_loadfile(L,str_luafilename);
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);//取出错误信息
		CString Error(errorstr);
		MessageBox(L"加载Lua脚本文件失败! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------步骤6：运行一次Lua文件---------------------------
	bRet = lua_pcall(L,0,0,0);                                                  // 4.运行一次Lua文件，因为只有运行一次该文件，文件中的内容才能被访问；
	                                                                            // L表示执行环境，即Lua虚拟机；第二个参数是输入值的个数，第三个参数是
	                                                                            // 返回值的个数，最后一个参数用于发生错误处理时的代码返回
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);                              // 取出错误信息
		CString Error(errorstr);
		MessageBox(L"编译Lua脚本文件失败! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------步骤7：读取Lua文件中的Lua函数--------------------
	lua_getglobal(L,str_luafucname);                                           // 5.读取函数，压入栈中；当然，如果文件中含有一些全局变量也可以通过lua_getglobal()函数进行读取并压入栈中
	//--------------------------步骤8：Lua函数的输入参数压栈---------------------
	lua_pushboolean(L,(int)arryInputofLua.at(0)->Value);                       // 将第一个boolean变量压栈  
	for(int i=1;i<sizeofLuaIN;i++)                                             // 将其他输入参数压入栈中
	{
		lua_pushnumber(L,arryInputofLua.at(i)->Value);
	}
	//--------------------------步骤9：调用Lua函数--------------------------------
	int iRet = lua_pcall(L,sizeofLuaIN,sizeofLuaOUT,0);                        // int iRet = lua_pcall(L,2,1,0); // 6.调用函数，调用完成后，会将返回结果压入栈中
	                                                                           // 2表示参数的个数，1表示返回结果的个数
	                                                                           // 如果调用出错，会将错误信息压入栈中
	if(iRet)
	{
		const char* errorstr = lua_tostring(L,-1);                             // 取出错误信息
		CString Error(errorstr);
		MessageBox(L"函数调用错误! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		lua_close(L);
		return FALSE;
	}
	//--------------------------步骤10：Lua函数输出值压栈--------------------------
	if(lua_isboolean(L,-sizeofLuaOUT))                                         // 取出最后一个参数，布尔标识
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
		MessageBox(L"Lua函数的第一个输出值必须为bool型");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	for(int i=0;i<sizeofLuaOUT-1;i++)                                          // 取出其他参数
	{
		if(lua_isnumber(L,-(i+1)))
		{
			arryOutputofLua.at(sizeofLuaOUT-i-1)->Value = lua_tonumber(L,-(i+1));
		}
		else
		{
			MessageBox(L"Lua函数的其他输出值必须为number型");
			SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
			m_pParentView->GetDocument()->m_simThread = NULL;
			return FALSE;
		}
	}
	//--------------------------步骤111：关闭Lua虚拟机----------------------------
	lua_close(L);    

	return bcanEnable;
}

// tell the View class the drawing of Arcs is finish
void CPlace::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CStatic::OnRButtonUp(nFlags, point);
	//----------------当鼠标右键点击库所模型时,弹出"模型编辑"弹出菜单------begin----
	CMenu m_MDeditpopup;
	m_MDeditpopup.LoadMenuW(IDR_MDEDIT_POPUP1);

	ClientToScreen(&point);

	m_MDeditpopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);//GetParent());//创建一个浮动的菜单

	//----------------当鼠标右键点击库所模型时,弹出"模型编辑"弹出菜单------end------
}

// delete this place
void CPlace::OnMdDelete()
{
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_MDDELETE1,this->u_SubscriptofModel,1);
}

// 旋转库所
void CPlace::OnRotateTl()
{
	// TODO: 在此添加命令处理程序代码
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

// 弹出属性设置对话框
void CPlace::OnShowpropdlg()
{
	// 当点击模型右键菜单中“属性”，显示属性设置对话框
	CPlacePropSetDlg m_dlgModelpropset(this->PlaceStyle,m_thisModel);                // 声明弹出的属性对话框对象
	
	// 将当前模型的属性值给属性设置页,目的：当再次双击打开对话框时，这些值能显示在对话框上
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

	// 调用domodal方法弹出对话框，如果用户点击OK按钮，则获取对话框上的设置，并完成库所属性值得初始化
	if(IDOK == m_dlgModelpropset.DoModal())                              
	{
		switch(this->PlaceStyle)
		{
			case STYLE_DISCRETE:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				break;

			case STYLE_CNTNUS:
				this->bActivation                       = m_dlgModelpropset.m_tabPropset1.bActivation_set;
				this->Lua_fileName                      = m_dlgModelpropset.m_tabPropset1.Lua_fileName;              // Lua文件名
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
		this->Invalidate();               // 目的：当用户设置设置完成后，如bActivation设为true后，立刻更新库所模型的显示
		this->UpdateWindow();
	}
}


// 编写随机库所的Lua脚本调用函数
bool CPlace::runS_LuaFunction(double timestep)
{
	//---------------------------步骤1;赋值-------------------------------------
	this->d_TimeStep = timestep;
	arryOutputofLua.at(1)->Value = this->dProblty;                                         // 为随机阈值赋值
	bool bcanEnable = FALSE;                                                               // 定义一个Bool值，作为函数的第一个输入值，和第一个返回值，并最终作为最终连续库所使能与否的判断依据

	                                                                                       // 先获取Lua文件的输入变量组，并利用输出变量数组对其进行赋值
	int sizeofLuaIN  = arryInputofLua.size();                                              // 获取输入变量的个数
	int sizeofLuaOUT = arryOutputofLua.size();                                             // 获取输入变量的个数
	if(sizeofLuaIN >= sizeofLuaOUT)
	{
		for(int i = 0;i<sizeofLuaOUT;i++)
		{
			arryInputofLua.at(i)->Value = arryOutputofLua.at(i)->Value;
		}
	}
	else
	{
		MessageBox(L"输入到Lua脚本中的参数的个数应≥输出参数的个数");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
	}
	
	//--------------------------步骤2：获取Lua文件名及函数名--------------------
	CString Lua_fileName_1 = _T("LuaFile\\S_") + this->m_caption + _T(".lua");            // 获取相应的文件名和路径
	char * str_luafilename;
	char * str_luafucname;
	USES_CONVERSION;
    str_luafilename = T2A(Lua_fileName_1);                                                // 调用函数，T2A和W2A均支持ATL和MFC中的字符转换
	str_luafucname = T2A(_T("S_")+this->m_caption);

	//--------------------------步骤3：创建一个Lua状态机-------------------------
	lua_State *L = luaL_newstate();//1.
	if(L==NULL)
	{
		MessageBox(L"创建Lua堆栈失败");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	lua_checkstack(L, 1000);
	//--------------------------步骤4：打开状态机L中的所有的Lua标准库------------
	luaL_openlibs(L); 
	//--------------------------步骤5：加载Lua文件；即把一个文件加载为Lua代码块--
	int bRet = luaL_loadfile(L,str_luafilename);
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);                                         // 取出错误信息
		CString Error(errorstr);
		MessageBox(L"加载Lua脚本文件失败! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------步骤6：运行一次Lua文件---------------------------
	bRet = lua_pcall(L,0,0,0);                                                            // 4.运行一次Lua文件，因为只有运行一次该文件，文件中的内容才能被访问；
	                                                                                      // L表示执行环境，即Lua虚拟机；第二个参数是输入值的个数，第三个参数是
	                                                                                      // 返回值的个数，最后一个参数用于发生错误处理时的代码返回
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);//取出错误信息
		CString Error(errorstr);
		MessageBox(L"编译Lua脚本文件失败! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------步骤7：读取Lua文件中的Lua函数--------------------
	lua_getglobal(L,str_luafucname);                                                     // 5.读取函数，压入栈中；当然，如果文件中含有一些全局变量也可以通过lua_getglobal()函数进行读取并压入栈中
	//--------------------------步骤8：Lua函数的输入参数压栈---------------------
	lua_pushboolean(L,(int)arryInputofLua.at(0)->Value);                                 // 将第一个boolean变量压栈  
	for(int i=1;i<sizeofLuaIN;i++)                                                       // 将其他输入参数压入栈中
	{
		lua_pushnumber(L,arryInputofLua.at(i)->Value);
	}
	//--------------------------步骤9：调用Lua函数--------------------------------
	int iRet = lua_pcall(L,sizeofLuaIN,sizeofLuaOUT,0);                                  // int iRet = lua_pcall(L,2,1,0); // 6.调用函数，调用完成后，会将返回结果压入栈中
	                                                                                     // 2表示参数的个数，1表示返回结果的个数
	                                                                                     // 如果调用出错，会将错误信息压入栈中
	if(iRet)
	{
		const char* errorstr = lua_tostring(L,-1);                                       // 取出错误信息
		CString Error(errorstr);
		MessageBox(L"函数调用错误! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		lua_close(L);
		return FALSE;
	}
	//--------------------------步骤10：Lua函数输出值压栈--------------------------
	if(lua_isboolean(L,-sizeofLuaOUT))                                                   // 取出最后一个参数，布尔标识
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
		MessageBox(L"Lua函数的第一个输出值必须为bool型");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	for(int i=0;i<sizeofLuaOUT-1;i++)                                                   // 取出其他参数
	{
		if(lua_isnumber(L,-(i+1)))
		{
			arryOutputofLua.at(sizeofLuaOUT-i-1)->Value = lua_tonumber(L,-(i+1));
		}
		else
		{
			MessageBox(L"Lua函数的其他输出值必须为number型");
			SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
			m_pParentView->GetDocument()->m_simThread = NULL;
			return FALSE;
		}
	}
	//--------------------------步骤111：关闭Lua虚拟机----------------------------
	lua_close(L);    

	return bcanEnable;
}


// 编写决策库所的Lua脚本调用函数
CPlace::result * CPlace::runD_LuaFunction(double timestep)
{
	//---------------------------步骤1;赋值-------------------------------------
	result *resofD = new result;
	this->d_TimeStep = timestep; 
	arryOutputofLua.at(1)->Value = this->d_TimeStep;                              // 为step步长赋值
	bool bcanEnable = FALSE;                                                      // 定义一个Bool值，作为函数的第一个输入值，和第一个返回值，并最终作为最终连续库所使能与否的判断依据
	
	vector<int> resultofD;                                                        // 先获取Lua文件的输入变量组，并利用输出变量数组对其进行赋值
	
	
	
	int sizeofLuaIN  = arryInputofLua.size();                                     // 获取输入变量的个数
	int sizeofLuaOUT = arryOutputofLua.size();                                    // 获取输入变量的个数
	
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
		MessageBox(L"输入到Lua脚本中的参数的个数应≥输出参数的个数");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
	}
	if(this->ischoseSocket)
	{
		msgO = "";
		msgI = "";
		for(int i = 3;i<(this->num_output+3);i++)
		{
			int b = (int)arryInputofLua.at(i)->Value;//强制转换，可能丢失精度
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
	
	//--------------------------步骤2：获取Lua文件名及函数名--------------------
	CString Lua_fileName_1 = _T("LuaFile\\") + this->m_caption + _T(".lua");     // 获取相应的文件名和路径
	char * str_luafilename;
	char * str_luafucname;
	USES_CONVERSION;
    str_luafilename = T2A(Lua_fileName_1);                                       // 调用函数，T2A和W2A均支持ATL和MFC中的字符转换
	str_luafucname = T2A(this->m_caption);

	//--------------------------步骤3：创建一个Lua状态机-------------------------
	lua_State *L = luaL_newstate();//1.
	if(L==NULL)
	{
		MessageBox(L"创建Lua堆栈失败");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	lua_checkstack(L, 1000);
	//--------------------------步骤4：打开状态机L中的所有的Lua标准库------------
	luaL_openlibs(L); 
	//--------------------------步骤5：加载Lua文件；即把一个文件加载为Lua代码块--
	int bRet = luaL_loadfile(L,str_luafilename);
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);//取出错误信息
		CString Error(errorstr);
		MessageBox(L"加载Lua脚本文件失败! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------步骤6：运行一次Lua文件---------------------------
	bRet = lua_pcall(L,0,0,0);                                                  // 4.运行一次Lua文件，因为只有运行一次该文件，文件中的内容才能被访问；
	                                                                            // L表示执行环境，即Lua虚拟机；第二个参数是输入值的个数，第三个参数是
	                                                                            // 返回值的个数，最后一个参数用于发生错误处理时的代码返回
	if(bRet)
	{
		const char* errorstr = lua_tostring(L,-1);                              // 取出错误信息
		CString Error(errorstr);
		MessageBox(L"编译Lua脚本文件失败! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}
	//--------------------------步骤7：读取Lua文件中的Lua函数--------------------
	lua_getglobal(L,str_luafucname);                                           // 5.读取函数，压入栈中；当然，如果文件中含有一些全局变量也可以通过lua_getglobal()函数进行读取并压入栈中
	//--------------------------步骤8：Lua函数的输入参数压栈---------------------
	lua_pushboolean(L,(int)arryInputofLua.at(0)->Value);                       // 将第一个boolean变量压栈  
	for(int i=1;i<sizeofLuaIN;i++)                                             // 将其他输入参数压入栈中
	{
		lua_pushnumber(L,arryInputofLua.at(i)->Value);
	}
	//--------------------------步骤9：调用Lua函数--------------------------------
	int iRet = lua_pcall(L,sizeofLuaIN,sizeofLuaOUT,0);                        // int iRet = lua_pcall(L,2,1,0); // 6.调用函数，调用完成后，会将返回结果压入栈中
	                                                                           // 2表示参数的个数，1表示返回结果的个数
	                                                                           // 如果调用出错，会将错误信息压入栈中
	if(iRet)
	{
		const char* errorstr = lua_tostring(L,-1);                             // 取出错误信息
		CString Error(errorstr);
		MessageBox(L"函数调用错误! \r\n 错误信息："+Error);
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		lua_close(L);
		return FALSE;
	}
	//--------------------------步骤10：Lua函数输出值压栈--------------------------
	if(lua_isboolean(L,-sizeofLuaOUT))                                         // 取出最后一个参数，布尔标识
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
		MessageBox(L"Lua函数的第一个输出值必须为bool型");
		SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
		m_pParentView->GetDocument()->m_simThread = NULL;
		return FALSE;
	}

	numTag = lua_tonumber(L, -sizeofLuaOUT + 2);                             //获取策略标识的数量
	for(int i=0;i<sizeofLuaOUT-1;i++)                                          // 取出其他参数
	{
		if(lua_isnumber(L,-(i+1)))
		{
			arryOutputofLua.at(sizeofLuaOUT-i-1)->Value = lua_tonumber(L,-(i+1));
			if(bcanEnable&&i<numTag)//获去lua脚本中tag类型的参数
				resultofD.push_back(lua_tonumber(L,-sizeofLuaOUT +i+3));			
		}
		else
		{
			MessageBox(L"Lua函数的其他输出值必须为number型");
			SetEvent(m_pParentView->GetDocument()->m_StopSimEvent->m_hObject);
			m_pParentView->GetDocument()->m_simThread = NULL;
			return FALSE;
		}
	}
	//--------------------------步骤111：关闭Lua虚拟机----------------------------
	lua_close(L);    
	  
	resofD->bcanEnable = bcanEnable;
	resofD->res = resultofD;

	return resofD;
}


int CPlace::runSocketFunction(string ipadrs, int port)
{
	const int buf_size = 64;
	WSADATA wsd; //WSADATA变量  
	SOCKET sHost; //服务器套接字  
	SOCKADDR_IN servAddr; //服务器地址  
	char buf[buf_size] = "\0"; //接收数据缓冲区  
	char bufRecv[buf_size];
	bool flag = true;
//	char * ip;
//	itoa(this->ipadrs, ip, 10);
	int retVal; //返回值
	//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)  
	{  
		//cout << "WSAStartup failed!" << endl;
		MessageBox(L"WSAStartup failed");
		return -1;
	}  
	//创建套接字  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(INVALID_SOCKET == sHost)  
	{  
		//cout << "socket failed!" << endl;
		MessageBox(L"socket failed");
		WSACleanup();//释放套接字资源  
		return -1;  
	}  
	//设置服务器地址  
	servAddr.sin_family =AF_INET;  
	servAddr.sin_addr.s_addr = inet_addr(this->ipadrs.c_str());  
	servAddr.sin_port = htons((short)this->port);  
	int nServAddlen  = sizeof(servAddr);
	//连接服务器  
	retVal=connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr));  
	if(SOCKET_ERROR == retVal)  
	{  
		//cout << "connect failed!" << endl;
		MessageBox(L"connect failed");
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return -1;  
	}
	while(flag)  
	{  
		//向服务器发送数据  
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
			closesocket(sHost); //关闭套接字  
			WSACleanup(); //释放套接字资源  
			return -1;  
		}  
		//RecvLine(sHost, bufRecv);  
		ZeroMemory(bufRecv, buf_size);  
		recv(sHost, bufRecv,buf_size , 0); // 接收服务器端的数据， 只接收5个字符
		this->msgI = bufRecv;
		flag = false;
	}
	//退出  
	closesocket(sHost); //关闭套接字  
	WSACleanup(); //释放套接字资源  
	return 0;
}
