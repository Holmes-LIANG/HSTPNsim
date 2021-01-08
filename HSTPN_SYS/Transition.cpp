// PNTransition.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "Transition.h"
#include "HSTPN_Message.h"
#include "MainFrm.h"
// CTransition

IMPLEMENT_DYNAMIC(CTransition, CStatic)

CTransition::CTransition()
{
	u_SubscriptofTrans = 0;   
	bFire              = FALSE;
	//arryTransPre     = ;
	//arryTransPst     = ;
	pt1                = 0;
	pt2                = 0;
	pt3                = 0;
	pt4                = 0;
	Position           = 0;
}

CTransition::~CTransition()
{
}

//重载构造函数
CTransition::CTransition(const CTransition &C)
{
	this->u_SubscriptofTrans = C.u_SubscriptofTrans;   
	this->bFire              = C.bFire;
	this->arryTransPre       = C.arryTransPre;
	this->arryTransPst       = C.arryTransPst;
	this->pt1                = C.pt1;
	this->pt2                = C.pt2;
}
//重载操作符=
CTransition CTransition::operator = (const CTransition &A)
{
	this->u_SubscriptofTrans = A.u_SubscriptofTrans;   
	this->bFire              = A.bFire;
	this->arryTransPre       = A.arryTransPre;
	this->arryTransPst       = A.arryTransPst;
	this->pt1                = A.pt1;
	this->pt2                = A.pt2;
	return *this;
}
BEGIN_MESSAGE_MAP(CTransition, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MD_DELETE2, &CTransition::OnMdDelete2)
END_MESSAGE_MAP()



// CTransition 消息处理程序




void CTransition::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	CString str;
	str.Format(_T("%d"),u_SubscriptofTrans);
	this->m_caption = _T("t")+str; 
	//this->SetWindowTextW(this->m_caption);	

	//------------给两个连接点赋值-------------------------------
	CRect rectofCtrl;
	this->GetWindowRect(&rectofCtrl);
	this->pt1.x = rectofCtrl.left;
	this->pt1.y = rectofCtrl.top+rectofCtrl.Height()/2.0;
	this->pt2.x = rectofCtrl.right;
	this->pt2.y  = rectofCtrl.top+rectofCtrl.Height()/2.0;
	this->pt3.x = rectofCtrl.left + rectofCtrl.Width()/2.0;
	this->pt3.y = rectofCtrl.top + rectofCtrl.Height();
	this->pt4.x = rectofCtrl.left + rectofCtrl.Width()/2.0;
	this->pt4.y = rectofCtrl.top;
	//-----------------------------------------------------------
	//-------------绘制变迁对象的外观----------------------------
	CRect rectClient;
	GetClientRect(&rectClient);  //获取空间客户区大小

	CPen pen(PS_SOLID,2,RGB(0,0,0));
	CPen *oldpen = dc.SelectObject(&pen);
	if(!this->bFire)
	{
		dc.Rectangle(&rectClient);
	}
	else
	{
		CBrush newbrush(RGB(0,200,0));
		CBrush *oldbrush;
		oldbrush = dc.SelectObject(&newbrush);
		dc.Rectangle(&rectClient);
		dc.SelectObject(oldbrush);
	}
	//-----------------------------------------------------------
	
}


void CTransition::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 当鼠标左键点击模型时，首先将模型的下标发生给View类（先发送给MainFrame类，再发送给View类）
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_MOVEMODEL1,this->u_SubscriptofTrans,2);//Lparm中的2表示模型为变迁

	CStatic::OnLButtonDown(nFlags, point);
}


void CTransition::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//-----------------------------------------
	// 当鼠标在控件内抬起的时候，可能是在画线状态结束时，因此发生消息到View类，使其获得该控件的连接点位置
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_DRAWLINEEND1,this->u_SubscriptofTrans,2);
	CStatic::OnLButtonUp(nFlags, point);
}


// 描述变迁发生规则的方法
//======================================================
// 作用：判断变迁是否满足发生条件，若满足返回true；否则，返回false
// 输入：无
// 输出：返回是否满足发生条件的标识，是一个bool值
//
//======================================================
bool CTransition::isFire()
{
	int ipreCount = (this->arryTransPre).size();//通过size方法，获取变迁的前置库所的个数
	int ienableCount = 0;   //初始化一个变量，记录变迁的前置库所中使能的个数
	for (vector<CPlace*>::iterator iter = (this->arryTransPre).begin(); iter != (this->arryTransPre).end(); iter++)
	{
		if ((**iter).bEnable && (**iter).PlaceStyle == STYLE_DISIC)
		{
			for (vector<CArrow*>::iterator itera = (this->arryArrowPre).begin(); 
				itera != (this->arryArrowPre).end(); itera++)
			{
				if ((**itera).m_pEdit != NULL )
				{
					for (int i = 0; i < (**iter).disic.size(); i++)
						if ((**itera).num == (**iter).disic[i]) {//判断有相弧上的数字和策略标志是否相等
							ienableCount++;//前置的决策库所对该变迁有效
							break;
						}		
				}
		    }
		}
		else if ((**iter).bEnable && (**iter).PlaceStyle == STYLE_RAND)
		{
			for (vector<CArrow*>::iterator itera = (this->arryArrowPre).begin(); itera != (this->arryArrowPre).end(); itera++)
			{
				if ((**itera).m_pEdit != NULL && (**itera).num == (**iter).rand_tag)
				{
					ienableCount++;
				}
			}
		}
		else if ((**iter).bEnable && ((**iter).PlaceStyle != STYLE_DISIC) && ((**iter).PlaceStyle != STYLE_RAND))
		{
			ienableCount++;
		}
	}
	return (ienableCount == ipreCount) ? true : false;
}


void CTransition::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CStatic::OnRButtonUp(nFlags, point);
	//----------------当鼠标右键点击变迁模型时,弹出"模型编辑"弹出菜单------begin----
	CMenu m_MDeditpopup;
	m_MDeditpopup.LoadMenuW(IDR_MDEDIT_POPUP2);

	ClientToScreen(&point);

	m_MDeditpopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);//GetParent());//创建一个浮动的菜单

	//----------------当鼠标右键点击变迁模型时,弹出"模型编辑"弹出菜单------end------
}

// 删除操作
void CTransition::OnMdDelete2()
{
	// TODO: 在此添加命令处理程序代码
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_MDDELETE1,this->u_SubscriptofTrans,2);
}
