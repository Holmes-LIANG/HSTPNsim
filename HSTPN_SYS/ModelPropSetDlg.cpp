// ModelPropSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "ModelPropSetDlg.h"
//#include "afxdialogex.h"


// CPlacePropSetDlg �Ի���

IMPLEMENT_DYNAMIC(CPlacePropSetDlg, CDialogEx)

CPlacePropSetDlg::CPlacePropSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlacePropSetDlg::IDD, pParent)
{

}

CPlacePropSetDlg::CPlacePropSetDlg(enumPlaceStyle placestyle,CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlacePropSetDlg::IDD, pParent)
{
	this->PlaceStyle = placestyle;
	this->p_Parent = (CPlace*)pParent;
}

CPlacePropSetDlg::~CPlacePropSetDlg()
{
}

void CPlacePropSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PROPSET, m_TabPropset);
}


BEGIN_MESSAGE_MAP(CPlacePropSetDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PROPSET, &CPlacePropSetDlg::OnSelchange_TabPropset)
END_MESSAGE_MAP()


// CPlacePropSetDlg ��Ϣ�������


BOOL CPlacePropSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//---------------------------------------------------------------
	// ģ�͵�˵������
	switch(this->PlaceStyle)
	{
	case STYLE_DISCRETE:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"��ɢ������"+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	��ɢ����������ϵͳ�е���ɢ״̬������ɢ��������ʱ����������ʹ�ܣ���ʶ��1");
		break;
	case STYLE_CNTNUS:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"����������"+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	��������������ϵͳ�е�����״̬����Ҫ����Lua�ļ�����д״̬���̣�������������״̬�ﵽ������ֵʱ������ʹ�ܣ���ʶ��1");
		break;
	case STYLE_RAND:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"���������"+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	�������������ϵͳ��ĳ״̬���ֵ�����ԣ�����������Դ˸���ֵ���������Ϊ1ʱ�����������ʹ�ܣ���ʶ��1�����������Ϊ0ʱ�������������ʹ��");
		break;
	case STYLE_TIME:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"ʱ�ӿ�����"+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	ʱ�ӿ���������ϵͳ״̬���ӳ٣���ʱ�ӿ�����ʱ�ﵽʱ����ֵʱ��ʱ�ӿ���ʹ�ܣ���ʶ��1");
		break;
	case STYLE_TCTRL:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"���ƿ�����"+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	���ƿ���������ϵͳ�Ŀ��Ʋ��ԣ����Ʋ��ԵĲ�����������Ϊ�趨������ϵͳ����״̬���н��");
		break;
	case STYLE_LCTRL:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"���ƿ�����"+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	���ƿ���������ϵͳ�Ŀ��Ʋ��ԣ����Ʋ��ԵĲ�����������Ϊ�趨������ϵͳ����״̬���н��");
		break;
	case STYLE_DISIC:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"���߿�����"+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"   ���߿���������ϵͳ�Ŀ��Ʋ��ԣ���Ҫ����Lua�ļ�����д�����㷨�������߹��̽���֮�󣬿���ʹ�ܣ��õ����Զ�Ӧ�ı�ʶ�������ʶ��Ӧ�ı�Ǩ����");
	    break;
	default:
		break;
	}
	
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// ��������Tab�ؼ�����ӱ�ǩҳ
	CRect tabRect;// ��ǩ�ؼ��ͻ�����λ�úʹ�С
	m_TabPropset.InsertItem(0,L"��������");//�����һ����ǩ
	m_TabPropset.InsertItem(1,L"����/�������");//����ڶ�����ǩ                                        // 12.16
	m_tabPropset1.Create(IDD_PROPSET_1,this->PlaceStyle,p_Parent,&m_TabPropset);//������һ����ǩҳ
	m_tabPropset2.Create(IDD_PROPSET_2,this->PlaceStyle,p_Parent,&m_TabPropset);//�����ڶ�����ǩҳ      // 12.16

	m_tabPropset1.m_tabpropset2 = &m_tabPropset2;// Ϊ����ɱ�ǩҳ2����ǩҳ1�Ĵ�ֵ

	m_TabPropset.GetClientRect(&tabRect);//��ȡ��ǩ�ؼ��ͻ�����Rect
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	// ���ݵ����õ�tabRect���á��������á���ǩҳ�ģ�������Ϊ��ʾ
	m_tabPropset1.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
	// ���ݵ����õ�tabRect���á��������á���ǩҳ�ģ�������Ϊ��ʾ
	m_tabPropset2.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
	//----------------------------------------------------------------


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPlacePropSetDlg::OnSelchange_TabPropset(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//-----------------------------------------------------------------
	CRect tabRect;// ��ǩ�ؼ��ͻ�����λ�úʹ�С
	m_TabPropset.GetClientRect(&tabRect);//��ȡ��ǩ�ؼ��ͻ�����Rect
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	switch(m_TabPropset.GetCurSel())
	{
		// ��ѡ���ǩΪ���������á�ʱ����ʹm_tabPropset1��ǩҳ��ʾ��m_tabPropset2��ǩҳ����
	case 0:
		m_tabPropset1.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
		m_tabPropset2.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
		break;
		// ��ѡ���ǩΪ������/������á�ʱ����ʹm_tabPropset1��ǩҳ���أ�m_tabPropset2��ǩҳ��ʾ
	case 1:
		m_tabPropset1.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
		m_tabPropset2.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
	//-----------------------------------------------------------------
}
