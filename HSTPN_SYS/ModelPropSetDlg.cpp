// ModelPropSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "ModelPropSetDlg.h"
//#include "afxdialogex.h"


// CPlacePropSetDlg 对话框

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


// CPlacePropSetDlg 消息处理程序


BOOL CPlacePropSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//---------------------------------------------------------------
	// 模型的说明部分
	switch(this->PlaceStyle)
	{
	case STYLE_DISCRETE:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"离散库所："+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	离散库所：描述系统中的离散状态，当离散库所激活时，库所立刻使能，标识置1");
		break;
	case STYLE_CNTNUS:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"连续库所："+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	连续库所：描述系统中的连续状态，需要创建Lua文件，编写状态方程；当连续库所的状态达到库所阈值时，库所使能，标识置1");
		break;
	case STYLE_RAND:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"随机库所："+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	随机库所：描述系统中某状态出现的随机性，当随机库所以此概率值生成随机数为1时，则随机库所使能，标识置1，生成随机数为0时，则随机库所不使能");
		break;
	case STYLE_TIME:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"时延库所："+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	时延库所：描述系统状态的延迟，当时延库所计时达到时间阈值时，时延库所使能，标识置1");
		break;
	case STYLE_TCTRL:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"控制库所："+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	控制库所：描述系统的控制策略，控制策略的产生可以是认为设定或来自系统其他状态运行结果");
		break;
	case STYLE_LCTRL:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"控制库所："+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"	控制库所：描述系统的控制策略，控制策略的产生可以是认为设定或来自系统其他状态运行结果");
		break;
	case STYLE_DISIC:
		SetDlgItemText(IDC_STIC_MDEXPLANATION,L"决策库所："+p_Parent->m_caption);
		SetDlgItemText(IDC_STIC_MDEXPLANATION1,L"   决策库所：描述系统的控制策略，需要创建Lua文件，编写决策算法；当决策过程结束之后，库所使能，得到策略对应的标识，并令标识对应的变迁发生");
	    break;
	default:
		break;
	}
	
	//---------------------------------------------------------------
	//---------------------------------------------------------------
	// 属性设置Tab控件中添加标签页
	CRect tabRect;// 标签控件客户区的位置和大小
	m_TabPropset.InsertItem(0,L"属性设置");//插入第一个标签
	m_TabPropset.InsertItem(1,L"输入/输出设置");//插入第二个标签                                        // 12.16
	m_tabPropset1.Create(IDD_PROPSET_1,this->PlaceStyle,p_Parent,&m_TabPropset);//创建第一个标签页
	m_tabPropset2.Create(IDD_PROPSET_2,this->PlaceStyle,p_Parent,&m_TabPropset);//创建第二个标签页      // 12.16

	m_tabPropset1.m_tabpropset2 = &m_tabPropset2;// 为了完成标签页2到标签页1的传值

	m_TabPropset.GetClientRect(&tabRect);//获取标签控件客户区的Rect
	// 调整tabRect，使其覆盖范围适合放置标签页
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	// 根据调整好的tabRect设置“属性设置”标签页的，并设置为显示
	m_tabPropset1.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
	// 根据调整好的tabRect设置“属性设置”标签页的，并设置为显示
	m_tabPropset2.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
	//----------------------------------------------------------------


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPlacePropSetDlg::OnSelchange_TabPropset(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//-----------------------------------------------------------------
	CRect tabRect;// 标签控件客户区的位置和大小
	m_TabPropset.GetClientRect(&tabRect);//获取标签控件客户区的Rect
	// 调整tabRect，使其覆盖范围适合放置标签页
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	switch(m_TabPropset.GetCurSel())
	{
		// 当选择标签为“属性设置”时，则使m_tabPropset1标签页显示，m_tabPropset2标签页隐藏
	case 0:
		m_tabPropset1.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
		m_tabPropset2.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
		break;
		// 当选择标签为“输入/输出设置”时，则使m_tabPropset1标签页隐藏，m_tabPropset2标签页显示
	case 1:
		m_tabPropset1.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
		m_tabPropset2.SetWindowPos(NULL,tabRect.left,tabRect.top,tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
	//-----------------------------------------------------------------
}
