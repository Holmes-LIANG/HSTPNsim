// PNTransition.cpp : ʵ���ļ�
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

//���ع��캯��
CTransition::CTransition(const CTransition &C)
{
	this->u_SubscriptofTrans = C.u_SubscriptofTrans;   
	this->bFire              = C.bFire;
	this->arryTransPre       = C.arryTransPre;
	this->arryTransPst       = C.arryTransPst;
	this->pt1                = C.pt1;
	this->pt2                = C.pt2;
}
//���ز�����=
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



// CTransition ��Ϣ�������




void CTransition::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	CString str;
	str.Format(_T("%d"),u_SubscriptofTrans);
	this->m_caption = _T("t")+str; 
	//this->SetWindowTextW(this->m_caption);	

	//------------���������ӵ㸳ֵ-------------------------------
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
	//-------------���Ʊ�Ǩ��������----------------------------
	CRect rectClient;
	GetClientRect(&rectClient);  //��ȡ�ռ�ͻ�����С

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// �����������ģ��ʱ�����Ƚ�ģ�͵��±귢����View�ࣨ�ȷ��͸�MainFrame�࣬�ٷ��͸�View�ࣩ
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_MOVEMODEL1,this->u_SubscriptofTrans,2);//Lparm�е�2��ʾģ��Ϊ��Ǩ

	CStatic::OnLButtonDown(nFlags, point);
}


void CTransition::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//-----------------------------------------
	// ������ڿؼ���̧���ʱ�򣬿������ڻ���״̬����ʱ����˷�����Ϣ��View�࣬ʹ���øÿؼ������ӵ�λ��
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_DRAWLINEEND1,this->u_SubscriptofTrans,2);
	CStatic::OnLButtonUp(nFlags, point);
}


// ������Ǩ��������ķ���
//======================================================
// ���ã��жϱ�Ǩ�Ƿ����㷢�������������㷵��true�����򣬷���false
// ���룺��
// ����������Ƿ����㷢�������ı�ʶ����һ��boolֵ
//
//======================================================
bool CTransition::isFire()
{
	int ipreCount = (this->arryTransPre).size();//ͨ��size��������ȡ��Ǩ��ǰ�ÿ����ĸ���
	int ienableCount = 0;   //��ʼ��һ����������¼��Ǩ��ǰ�ÿ�����ʹ�ܵĸ���
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
						if ((**itera).num == (**iter).disic[i]) {//�ж����໡�ϵ����ֺͲ��Ա�־�Ƿ����
							ienableCount++;//ǰ�õľ��߿����Ըñ�Ǩ��Ч
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CStatic::OnRButtonUp(nFlags, point);
	//----------------������Ҽ������Ǩģ��ʱ,����"ģ�ͱ༭"�����˵�------begin----
	CMenu m_MDeditpopup;
	m_MDeditpopup.LoadMenuW(IDR_MDEDIT_POPUP2);

	ClientToScreen(&point);

	m_MDeditpopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);//GetParent());//����һ�������Ĳ˵�

	//----------------������Ҽ������Ǩģ��ʱ,����"ģ�ͱ༭"�����˵�------end------
}

// ɾ������
void CTransition::OnMdDelete2()
{
	// TODO: �ڴ���������������
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_MDDELETE1,this->u_SubscriptofTrans,2);
}
