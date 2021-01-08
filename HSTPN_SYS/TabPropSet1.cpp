// TabPropSet1.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "TabPropSet1.h"
//#include "afxdialogex.h"

#include <Windows.h>


// CTabPropSet1 对话框

IMPLEMENT_DYNAMIC(CTabPropSet1, CDialog)

CTabPropSet1::CTabPropSet1(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPropSet1::IDD, pParent)
	, m_Value_Probset(1)
	, m_Value_Timeset(0)
	, dTime_downline(0)
	, dTime_upline(1)
	, str_ipadrs(_T(""))
	, num_port(0)
	, num_output(0)
	, num_input(0)
{
	bActivation_set = FALSE;
	bisUserDef_prob = TRUE;
	bisUserDef_time = TRUE;
	bisUserDef_ctrl = TRUE;

//	m_fromportctrl = "";
//	m_fromportprob = "";
//	m_formporttime = "";

	str_Inputport_prob = "";
	str_Inputport_time = "";
	str_Inputport_ctrl = "";

	Lua_fileName     = "";

	m_pDoc = (CHSTPN_SYSDoc*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument());
	ischoseupdown = false;
	ischoseProgRand = false;
	ischoseSocket = false;
}

CTabPropSet1::~CTabPropSet1()
{
}

void CTabPropSet1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_TIMEVALUE, m_edit_timevalue);
	DDX_Control(pDX, IDC_EDIT_PROBVALUE, m_edt_probvalue);
	DDX_Control(pDX, IDC_COMBO_BACTV, m_stts_cmb_activ);
	DDX_Control(pDX, IDC_COMBO_CNTNUS, m_cmb_cntnus);
	DDX_Control(pDX, IDC_COMBO_PROB, m_stts_cmb_prob);
	DDX_Control(pDX, IDC_COMBO_TIME, m_stts_cmb_time);
	DDX_Text(pDX, IDC_EDIT_PROBVALUE, m_Value_Probset);
	DDX_Text(pDX, IDC_EDIT_TIMEVALUE, m_Value_Timeset);
	DDX_Control(pDX, IDC_BTN_CNTNUS_USER, m_btn_cntnus_usr);
	DDX_Control(pDX, IDC_COMBO_CTRL, m_stts_cmb_ctrl);
	DDX_Control(pDX, IDC_CMB_PORTCTRL, m_cmb_portctrl);
	DDX_Control(pDX, IDC_CMB_PORTTIME, m_cmb_porttime);
	DDX_Control(pDX, IDC_CMB_PORTPROB, m_cmb_portprob);
	DDX_Text(pDX, IDC_EDT_DOWN, dTime_downline);
	DDX_Text(pDX, IDC_EDT_UP, dTime_upline);
	DDX_Control(pDX, IDC_EDT_DOWN, dTime_edt_down);
	DDX_Control(pDX, IDC_EDT_UP, dTime_edit_up);
	DDX_Control(pDX, IDC_BTN_RAND_USER, m_btn_usrProgrand);
	DDX_Control(pDX, IDC_EDIT_IP, m_edit_ip);
	DDX_Control(pDX, IDC_EDIT_PORT, m_edit_port);
	DDX_Control(pDX, IDC_COMBO_CONNECT, m_stts_cmb_connect);
	DDX_Text(pDX, IDC_EDIT_IP, str_ipadrs);
	DDX_Text(pDX, IDC_EDIT_PORT, num_port);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_edit_output);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, num_output);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_edit_input);
	DDX_Text(pDX, IDC_EDIT_INPUT, num_input);
}


BEGIN_MESSAGE_MAP(CTabPropSet1, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_BACTV, &CTabPropSet1::OnSelchange_activstts)
	ON_CBN_SELCHANGE(IDC_COMBO_PROB, &CTabPropSet1::OnSelchange_probstts)
	ON_EN_CHANGE(IDC_EDIT_PROBVALUE, &CTabPropSet1::OnTextChng_probvalue)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME, &CTabPropSet1::OnSelchange_timestts)
	ON_EN_CHANGE(IDC_EDIT_TIMEVALUE, &CTabPropSet1::OnTextChng_timevalue)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRL, &CTabPropSet1::OnSelchange_ctrlstts)
	ON_CBN_SELCHANGE(IDC_CMB_PORTPROB, &CTabPropSet1::OnSelchng_inPortprob)
	ON_CBN_SELCHANGE(IDC_CMB_PORTTIME, &CTabPropSet1::OnSelchng_inPorttime)
	ON_CBN_SELCHANGE(IDC_CMB_PORTCTRL, &CTabPropSet1::OnSelchng_inPortctrl)
	ON_BN_CLICKED(IDC_BTN_CNTNUS_USER, &CTabPropSet1::OnBtnClckd_CreateorOpenLua)
	ON_EN_CHANGE(IDC_EDT_DOWN, &CTabPropSet1::OnEnChangeEdtDown)
	ON_EN_CHANGE(IDC_EDT_UP, &CTabPropSet1::OnEnChangeEdtUp)
	ON_BN_CLICKED(IDC_BTN_RAND_USER, &CTabPropSet1::OnBtn_RandUser)
	ON_CBN_SELCHANGE(IDC_COMBO_CONNECT, &CTabPropSet1::OnCbnSelchgComboConnect)
	ON_EN_CHANGE(IDC_EDIT_IP, &CTabPropSet1::OnEnChgEditIp)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CTabPropSet1::OnEnChgEditPort)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CTabPropSet1::OnEnChangeEditInput)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT, &CTabPropSet1::OnEnChangeEditOutput)
END_MESSAGE_MAP()


// CTabPropSet1 消息处理程序


BOOL CTabPropSet1::Create(UINT nIDTemplate, enumPlaceStyle placestyle,CPlace* pModel, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类
	this->PlaceStyle = placestyle;
	this->p_Model = pModel;

	return CDialog::Create(nIDTemplate, pParentWnd);
}


BOOL CTabPropSet1::OnInitDialog()
{
	CDialog::OnInitDialog();
	Lua_fileName = p_Model->m_caption;
	// TODO:  在此添加额外的初始化
	//--------------------------------------------------------------------------------------------------------第一次的控件状态初始化-----Begin---
	// 根据库所的类型，设置属性设置项相应的状态
	//------------------------------------------------初始激活状态设置Combo框初始化
	m_stts_cmb_activ.InsertString(0,L"True");      // 插入第一个选项
	m_stts_cmb_activ.InsertString(1,L"False");     // 插入第二个选项
	//------------------------------------------------概率阈值的获取方式选择的CmoBo框的初始化
	m_stts_cmb_prob.InsertString(0,L"自定义");
	m_stts_cmb_prob.InsertString(1,L"来自输入值");
	m_stts_cmb_prob.InsertString(2,L"编程获取");
	m_stts_cmb_prob.SetCurSel(0);                  //设置默认选项为"自定义"
	//-----------------------------------------------概率阈值的获取方式选择的CmoBo框的初始化
	m_stts_cmb_time.InsertString(0,L"自定义");
	m_stts_cmb_time.InsertString(1,L"来自输入值");
	m_stts_cmb_time.InsertString(2,_T("随机时间"));
	m_stts_cmb_time.SetCurSel(0);                  //设置默认选项为"自定义"
	//-----------------------------------------------控制库所的激活值的获取方式选择的CmoBo框的初始化
	m_stts_cmb_ctrl.InsertString(0,L"自定义");
	m_stts_cmb_ctrl.InsertString(1,L"来自输入值");
	m_stts_cmb_ctrl.SetCurSel(0);                  //设置默认选项为"自定义"
	//-----------------------------------------------连续过程的获取方式选择的CmoBo框的初始化
	m_cmb_cntnus.InsertString(0,L"自定义");
	m_cmb_cntnus.InsertString(1,L"其他...待编辑");
	m_cmb_cntnus.SetCurSel(0);
	//-----------------------------------------------某些库所的对外接口选择的Cmobo框的初始化
	m_stts_cmb_connect.InsertString(0,L"否");
	m_stts_cmb_connect.InsertString(1,L"是");
	m_stts_cmb_connect.SetCurSel(0);

	// 设置端口获取对话框的初始状态------------无论控件类型如何，因此标签的默认状态都是“自定义”，所以相应的控件应该设置为Hide 或 uneanble-（1）
	m_cmb_portprob.ShowWindow(SW_HIDE);
	m_cmb_porttime.ShowWindow(SW_HIDE);
	m_cmb_portctrl.EnableWindow(FALSE);
	// ----------------------------------------设置各个控件的默认状态（根据库所类型的不同）-------------------------------------------------（2）
	switch(this->PlaceStyle)
	{
	case STYLE_DISCRETE:
		m_stts_cmb_time.EnableWindow(FALSE);// 设置概率、时间、连续相关控件不可用
		m_edit_timevalue.EnableWindow(FALSE);
		m_stts_cmb_prob.EnableWindow(FALSE);
		m_edt_probvalue.EnableWindow(FALSE);
		m_stts_cmb_ctrl.EnableWindow(FALSE);
		m_cmb_cntnus.EnableWindow(FALSE);
		m_btn_cntnus_usr.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(FALSE);
		dTime_edt_down.EnableWindow(FALSE);
		dTime_edit_up.EnableWindow(FALSE);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_stts_cmb_connect.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		break;
	case STYLE_CNTNUS:
		m_stts_cmb_time.EnableWindow(FALSE);
		m_edit_timevalue.EnableWindow(FALSE);
		m_stts_cmb_prob.EnableWindow(FALSE);
		m_edt_probvalue.EnableWindow(FALSE);
		m_stts_cmb_ctrl.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(FALSE);
		dTime_edt_down.EnableWindow(FALSE);
		dTime_edit_up.EnableWindow(FALSE);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		break;
	case STYLE_RAND:
		m_stts_cmb_time.EnableWindow(FALSE);// 设置时间、连续相关控件不可用
		m_edit_timevalue.EnableWindow(FALSE);
		m_stts_cmb_prob.EnableWindow(TRUE);
		m_edt_probvalue.EnableWindow(TRUE);
		m_stts_cmb_ctrl.EnableWindow(FALSE);
		m_cmb_cntnus.EnableWindow(FALSE);
		m_btn_cntnus_usr.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(FALSE);
		dTime_edt_down.EnableWindow(FALSE);
		dTime_edit_up.EnableWindow(FALSE);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_stts_cmb_connect.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		break;
	case STYLE_TIME:
		m_stts_cmb_time.EnableWindow(TRUE);// 设置随机、连续相关控件不可用
		m_edit_timevalue.EnableWindow(TRUE);
		m_stts_cmb_prob.EnableWindow(FALSE);
		m_edt_probvalue.EnableWindow(FALSE);
		m_stts_cmb_ctrl.EnableWindow(FALSE);
		m_cmb_cntnus.EnableWindow(FALSE);
		m_btn_cntnus_usr.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(FALSE);
		dTime_edt_down.EnableWindow(FALSE);
		dTime_edit_up.EnableWindow(FALSE);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_stts_cmb_connect.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		break;
	case STYLE_TCTRL:
		m_stts_cmb_time.EnableWindow(FALSE);// 设置随机、连续相关控件不可用
		m_edit_timevalue.EnableWindow(FALSE);
		m_stts_cmb_prob.EnableWindow(FALSE);
		m_edt_probvalue.EnableWindow(FALSE);
		m_stts_cmb_ctrl.EnableWindow(TRUE);
		m_cmb_cntnus.EnableWindow(FALSE);
		m_btn_cntnus_usr.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(FALSE);
		dTime_edt_down.EnableWindow(FALSE);
		dTime_edit_up.EnableWindow(FALSE);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_stts_cmb_connect.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		break;
	case STYLE_LCTRL:
		m_stts_cmb_time.EnableWindow(FALSE);// 设置随机、连续相关控件不可用
		m_edit_timevalue.EnableWindow(FALSE);
		m_stts_cmb_prob.EnableWindow(FALSE);
		m_edt_probvalue.EnableWindow(FALSE);
		m_stts_cmb_ctrl.EnableWindow(TRUE);
		m_cmb_cntnus.EnableWindow(FALSE);
		m_btn_cntnus_usr.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(FALSE);
		dTime_edt_down.EnableWindow(FALSE);
		dTime_edit_up.EnableWindow(FALSE);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_stts_cmb_connect.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		break;
	case STYLE_DISIC:
		m_stts_cmb_time.EnableWindow(FALSE);
		m_edit_timevalue.EnableWindow(FALSE);
		m_stts_cmb_prob.EnableWindow(FALSE);
		m_edt_probvalue.EnableWindow(FALSE);
		m_stts_cmb_ctrl.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(FALSE);
		dTime_edt_down.EnableWindow(FALSE);
		dTime_edit_up.EnableWindow(FALSE);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		break;
	default:
		break;
	}
	//--------------------------------------------------------------------------------------------------------第一次的控件状态初始化-----End---
	//--------------------------------------------------------------------------------------------------------再次打开对话框，将控件初始化为保存状态-----Begin---
	//-----------------------------------再次打开对话框时，显示已保存的设置-------------------------------（1）
	if(this->bActivation_set)
		m_stts_cmb_activ.SetCurSel(0);           //设置默认选项为False
	else
		m_stts_cmb_activ.SetCurSel(1);           //设置默认选项为False
	// ----------------------当关闭模型的设置属性对话框，再次打开时，根据用户选择，将控件的状态重新设置回来（2）
	if(bisUserDef_prob)
	{
		m_stts_cmb_prob.SetCurSel(0);           //选择标签为“自定义”
		m_cmb_portprob.ShowWindow(SW_HIDE);
		m_edt_probvalue.ShowWindow(SW_SHOW);
		m_btn_usrProgrand.EnableWindow(false);
	}
	else
	{
		m_stts_cmb_prob.SetCurSel(1);           //选择标签为“来自输入值”
		m_cmb_portprob.ShowWindow(SW_SHOW);
		m_edt_probvalue.ShowWindow(SW_HIDE);
		m_btn_usrProgrand.EnableWindow(false);
		CString str;
		str = this->str_Inputport_prob;
		m_cmb_portprob.SetWindowTextW(str);
	}
	if(this->ischoseProgRand)
	{
		m_stts_cmb_prob.SetCurSel(0);
		m_cmb_portprob.EnableWindow(false);
		m_edt_probvalue.EnableWindow(false);
		m_btn_usrProgrand.EnableWindow(true);
	}
	if(bisUserDef_time)
	{
		m_stts_cmb_time.SetCurSel(0);
		m_cmb_porttime.ShowWindow(SW_HIDE);
		m_edit_timevalue.ShowWindow(SW_SHOW);
	}
	else
	{
		m_stts_cmb_time.SetCurSel(1);          //选择标签为“来自输入值”
		m_cmb_porttime.ShowWindow(SW_SHOW);
		m_edit_timevalue.ShowWindow(SW_HIDE);
		CString str;
		str = this->str_Inputport_time;
		m_cmb_porttime.SetWindowTextW(str);
	}
	if(this->ischoseupdown)
	{
		m_stts_cmb_time.SetCurSel(2);
		m_cmb_porttime.ShowWindow(SW_HIDE);
		m_edit_timevalue.ShowWindow(SW_HIDE);
	}

	if(bisUserDef_ctrl)
	{
		m_stts_cmb_ctrl.SetCurSel(0);
		m_cmb_portctrl.EnableWindow(FALSE);
	}
	else
	{
		m_stts_cmb_ctrl.SetCurSel(1);
		m_stts_cmb_activ.EnableWindow(FALSE);
		m_cmb_portctrl.EnableWindow(TRUE);
		CString str;
		str = this->str_Inputport_ctrl;
		m_cmb_portctrl.SetWindowTextW(str);
	}
	if(ischoseSocket)
	{
		m_stts_cmb_connect.SetCurSel(1);
		m_edit_ip.EnableWindow(TRUE);
		m_edit_port.EnableWindow(TRUE);
		m_edit_input.EnableWindow(TRUE);
		m_edit_output.EnableWindow(TRUE);
	}
	else
	{
		m_stts_cmb_connect.SetCurSel(0);
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
	}
	//--------------------------------------------------------------------------------------------------------再次打开对话框，将控件初始化为保存状态-----Begin---
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 获取用户设置的初始激活状态值
void CTabPropSet1::OnSelchange_activstts()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取组合框控件的列表框中选中项的索引   
    int nSel = m_stts_cmb_activ.GetCurSel(); 
	if(this->PlaceStyle == STYLE_TCTRL || this->PlaceStyle == STYLE_LCTRL)// 如果库所的类型是控制库所,要根据激活值的获取方式来选择激活值是否从该控件中获取
	{
		if(bisUserDef_ctrl)//如果方式为“自定义”，则从该控件中获取
		{
			if(nSel == 0)//标签为True
			{
				bActivation_set = TRUE;
			}
			if(nSel == 1)//标签为False
			{
				bActivation_set = FALSE;
			}
		}
		else
		{
			//do nothing// do nothing表示该值应该从输入值获取，那么从哪个输入值获取呢，则需要在值的一栏获取输入值的端口
		}
	}
	else                                                                  //如果是非控制库所，激活值从该控件中获取
	{
		if(nSel == 0)//标签为True
		{
			bActivation_set = TRUE;
		}
		if(nSel == 1)//标签为False
		{
			bActivation_set = FALSE;
		}
	}
}

// 获取用户设置的概率阈值获取方式
void CTabPropSet1::OnSelchange_probstts()
{
	// 获取组合框控件的列表框中选中项的索引   
    int nSel = m_stts_cmb_prob.GetCurSel();   
    if(nSel == 0)                                                 //标签为“自定义”
	{
		bisUserDef_prob = TRUE;
		m_edt_probvalue.EnableWindow(TRUE);                       // 同时设置概率阈值获取编辑框为“可用状态”
		m_edt_probvalue.ShowWindow(SW_SHOW);                      // 同时设置编辑框隐藏
		GetDlgItem(IDC_STATIC_PROB)->SetWindowText(L"值：");
		m_cmb_portprob.ShowWindow(SW_HIDE);
		m_cmb_portprob.EnableWindow(true);
		m_btn_usrProgrand.EnableWindow(false);
	}

	if(nSel == 1)                                                 //标签为“来自输入值”
	{
		bisUserDef_prob = FALSE;
		m_edt_probvalue.EnableWindow(FALSE);                      // 同时设置概率阈值获取编辑框为“不可用状态”
		m_edt_probvalue.ShowWindow(SW_HIDE);                      // 同时设置编辑框隐藏
		GetDlgItem(IDC_STATIC_PROB)->SetWindowText(L"变量：");
		m_cmb_portprob.ShowWindow(SW_SHOW);
		m_cmb_portprob.EnableWindow(TRUE);
		m_btn_usrProgrand.EnableWindow(false);
		// 初始化m_cmb_portprob控件,之后添加消息响应函数，获取端口值-----
		int i = 0;
		CString str_port;
		m_cmb_portprob.ResetContent();
		for(vector<IOValue*>::iterator iter=p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
		{
			if((**iter).IOType == "input")
			{
				str_port = (**iter).Name;
				m_cmb_portprob.InsertString(i,str_port);
				i++;
			}
		}
		m_cmb_portprob.SetCurSel(0);//默认选中下标为0的选项
	  //--------------------------------------------------------------------
	}

	if(nSel==2)
	{
		this->ischoseProgRand = true;
		m_edt_probvalue.EnableWindow(false);
		m_cmb_portprob.EnableWindow(FALSE);
		m_btn_usrProgrand.EnableWindow(true);
	}
	else
		this->ischoseProgRand = false;
}       

// 获取概率阈值--“自定义”方式下
void CTabPropSet1::OnTextChng_probvalue()
{

	if(bisUserDef_prob)                                           // 如果概率阈值的获取方式为“自定义”
	{
		UpdateData();                                             // 调用UpdateData()完成对话框中控件到控件关联变量的数据交换
	}
}

// 获取时间阈值的获取方式
void CTabPropSet1::OnSelchange_timestts()
{
	 
    int nSel = m_stts_cmb_time.GetCurSel();                       // 获取组合框控件的列表框中选中项的索引   
    if(nSel == 0)                                                 // 标签为“自定义”
	{
		bisUserDef_time = TRUE;
		m_edit_timevalue.EnableWindow(TRUE);                      // 同时设置时间阈值获取编辑框为“可用状态”
		m_edit_timevalue.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(L"值：");
		m_cmb_porttime.ShowWindow(SW_HIDE);
		dTime_edt_down.EnableWindow(false);
		dTime_edit_up.EnableWindow(false);
	}
	if(nSel == 1)                                                 // 标签为“来自输入值”
	{
		bisUserDef_time = FALSE;
		m_edit_timevalue.EnableWindow(FALSE);                     // 同时设置时间阈值获取编辑框为“不可用状态”
		m_edit_timevalue.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(L"变量：");
		m_cmb_porttime.ShowWindow(SW_SHOW);
		m_cmb_porttime.EnableWindow(true);
		dTime_edt_down.EnableWindow(false);
		dTime_edit_up.EnableWindow(false);
		// 初始化m_cmb_porttime控件,之后添加消息响应函数，获取端口值-----
		int i = 0;
		CString str_port;
		m_cmb_porttime.ResetContent();
		for(vector<IOValue*>::iterator iter=p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
		{
			if((**iter).IOType == "input")
			{
				str_port = (**iter).Name;
				m_cmb_porttime.InsertString(i,str_port);
				i++;
			}
		}
		m_cmb_porttime.SetCurSel(0);
	  //--------------------------------------------------------------------
	}
	if(nSel == 2)
	{
		this->ischoseupdown = true;
		m_edit_timevalue.EnableWindow(false);
		m_cmb_porttime.EnableWindow(false);
		dTime_edt_down.EnableWindow(true);
		dTime_edit_up.EnableWindow(true);
	}
	else
		this->ischoseupdown = false;
}

// 获取时间阈值--“自定义”方式下
void CTabPropSet1::OnTextChng_timevalue()
{

	if(bisUserDef_time)                                          // 如果时间阈值的获取方式为“自定义”
	{
		UpdateData();                                            // 调用UpdateData()完成对话框中控件到控件关联变量的数据交换
	}

}

// 获取控制库所激活值的获取方式
void CTabPropSet1::OnSelchange_ctrlstts()
{
    int nSel = m_stts_cmb_ctrl.GetCurSel();                      // 获取组合框控件的列表框中选中项的索引
    if(nSel == 0)                                                // 标签为“自定义”
	{
		bisUserDef_ctrl = TRUE;
		m_stts_cmb_activ.EnableWindow(TRUE);                     // 同时设置概率阈值获取编辑框为“可用状态”
		m_cmb_portctrl.EnableWindow(FALSE);
	}
	if(nSel == 1)                                                // 标签为“来自输入值”
	{
		bisUserDef_ctrl = FALSE;
		m_stts_cmb_activ.EnableWindow(FALSE);                    // 同时设置概率阈值获取编辑框为“不可用状态”
		m_cmb_portctrl.EnableWindow(TRUE);
		// 初始化m_cmb_portctrl控件,之后添加消息响应函数，获取端口值-----
		int i = 0;
		CString str_port;
		m_cmb_portctrl.ResetContent();
		for(vector<IOValue*>::iterator iter=p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
		{
			if((**iter).IOType == "input" | (**iter).IOType == "in/output")
			{
				str_port = (**iter).Name;
				m_cmb_portctrl.InsertString(i,str_port);
				i++;
			}
		}
		m_cmb_portctrl.SetCurSel(0);
	  //--------------------------------------------------------------------
	}
}

// 获取dProblty输入端口
void CTabPropSet1::OnSelchng_inPortprob()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_cmb_portprob.GetCurSel();
	CString str;
	m_cmb_portprob.SetCurSel(nSel);
	m_cmb_portprob.GetWindowText(str);
	str_Inputport_prob  = str;
}

// 获取dTime输入端口
void CTabPropSet1::OnSelchng_inPorttime()
{
	int nSel = m_cmb_porttime.GetCurSel();
	CString str;
	m_cmb_porttime.SetCurSel(nSel);
	m_cmb_porttime.GetWindowText(str);
	str_Inputport_time = str;
}

// 获取Ctrl输入端口
void CTabPropSet1::OnSelchng_inPortctrl()
{
	int nSel = m_cmb_portctrl.GetCurSel();
	CString str;
	m_cmb_portctrl.SetCurSel(nSel);
	m_cmb_portctrl.GetWindowText(str);
	str_Inputport_ctrl = str;
}

// 当用户点击“自定义Lua脚本”时，创建或打开与连续库所唯一关联的Lua脚本
void CTabPropSet1::OnBtnClckd_CreateorOpenLua()
{
	if (!PathIsDirectory(_T("LuaFile")))//判断文件夹是否创建
	{
		::CreateDirectory(_T("LuaFile"), NULL);//创建一个文件夹
	}
	//---------------------------------------创建或打开Lua编辑环境供用户编辑----------Begin----
	Lua_fileName = p_Model->m_caption;                                 //获取库所的名字，所谓Lua文件名以及函数名         
	CString Lua_fileName_1 = _T("LuaFile\\") + Lua_fileName + _T(".lua");
	CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	if(file.GetLength()==0)
	{
		USES_CONVERSION;                                                  //用T2A之前，一定要加USES_CONVERSION标识符，用来转换类型
		CString str = L"function  " + Lua_fileName + L"(bcanEnable, step, t, x1, x2, ...)\r\n";
		char * str1 = T2A(str);                                           //调用函数，T2A和W2A均支持ATL和MFC中的字符转换
		char * str2 = "   \r\n   --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误\r\n   --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件\r\n   --Note:连续库所：第二个和第三个参数分别为仿真步长step和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作\r\n   --Note:决策库所：第二个参数为仿真步长step，第三个参数为标识数量n，第四个及以后的参数开始为策略标识tag1、tag2、... 、tagn，之后可以是其他变量x1,x2...\r\n   --（1）lua脚本中策略标识（tag1、tag2…）赋值时必须与有向弧上的数字一致，若有向弧上不存在对应的数字，则应该赋值为0（不赋值时默认值为0）；\r\n   --（2）有向弧上的数字必须大于0；\r\n   --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明\r\n\r\n   --TODO:Write your function here! \r\n\r\n";
		char * str3 = "   \r\n   \r\n   return bcanEnable, step, t, x1, x2, ...\r\n";
		char * str4 = "end\r\n";
		file.Write(str1,strlen(str1));                                   //写入模板
		file.Write(str2,strlen(str2));
		file.Write(str3,strlen(str3));
		file.Write(str4,strlen(str4));
	}
	file.Close();
	ShellExecute(0,L"open",L"LuaEditor\\notepad++.exe",Lua_fileName_1,L"",SW_SHOWNORMAL);
	//---------------------------------------创建或打开Lua编辑环境供用户编辑----------End------
}

// 获取时间区间的下限down
void CTabPropSet1::OnEnChangeEdtDown()
{
	UpdateData();
	this->p_Model->isTimeUpDownChange = false;
}

// 获取时间区间的上限up
void CTabPropSet1::OnEnChangeEdtUp()
{
	UpdateData();
	this->p_Model->isTimeUpDownChange = false;
}

// 为随机库所创建Lua脚本
void CTabPropSet1::OnBtn_RandUser()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!PathIsDirectory(_T("LuaFile")))//判断文件夹是否创建
	{
		::CreateDirectory(_T("LuaFile"), NULL);//创建一个文件夹
	}
	//---------------------------------------创建或打开Lua编辑环境供用户编辑----------Begin----
	CString randluafile = p_Model->m_caption;                                 //获取库所的名字，所谓Lua文件名以及函数名         
	CString randluafile_1 = _T("LuaFile\\S_") +randluafile + _T(".lua");
	CFile file(randluafile_1,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	if(file.GetLength()==0)
	{
		USES_CONVERSION;                                                   //用T2A之前，一定要加USES_CONVERSION标识符
		CString str = L"function  S_"  + randluafile + L"(bcanEnable, random_num)\r\n";
		char * str1 = T2A(str);                                           //调用函数，T2A和W2A均支持ATL和MFC中的字符转换
		char * str2 = "   \r\n   --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误\r\n   --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件\r\n   --Note:第二个参数为random_num，用户可自行设定的概率阈值\r\n   --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明\r\n\r\n   --TODO:Write your function here! \r\n\r\n";
		char * str3 = "   \r\n   \r\n   return bcanEnable, random_num\r\n";
		char * str4 = "end\r\n";
		file.Write(str1,strlen(str1));                                   //写入模板
		file.Write(str2,strlen(str2));
		file.Write(str3,strlen(str3));
		file.Write(str4,strlen(str4));
	}
	file.Close();
	ShellExecute(0,L"open",L"LuaEditor\\notepad++.exe",randluafile_1,L"",SW_SHOWNORMAL);
	//---------------------------------------创建或打开Lua编辑环境供用户编辑----------End------
}

/*void CTabPropSet1::OnBtn_DisicUser()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!PathIsDirectory(_T("LuaFile")))//判断文件夹是否创建
	{
		::CreateDirectory(_T("LuaFile"), NULL);//创建一个文件夹
	}
	//---------------------------------------创建或打开Lua编辑环境供用户编辑----------Begin----
	CString randluafile = p_Model->m_caption;                                 //获取库所的名字，所谓Lua文件名以及函数名         
	CString randluafile_1 = _T("LuaFile\\D_") +randluafile + _T(".lua");
	CFile file(randluafile_1,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	if(file.GetLength()==0)
	{
		USES_CONVERSION;                                                   //用T2A之前，一定要加USES_CONVERSION标识符
		CString str = L"function  D_"  + randluafile + L"(bcanEnable, tag, step, t, x1, x2, ...)\r\n";
		char * str1 = T2A(str);                                           //调用函数，T2A和W2A均支持ATL和MFC中的字符转换
		char * str2 = "   \r\n   --Note:编写代码需要按照模板给定格式编写，不能随意更改该模板，否则可能引起程序的不可预见性错误\r\n   --Note:函数输入值和返回值的第一个元素必须是一个boolean值，用于判断库所是否具有使能条件\r\n   --Note:第二个和第三个参数分别为仿真步长和时钟t，仿真步长无需用户在该文件中初始化，但时钟t需要用户进行初始化工作\r\n   --Note:其他参数由用户设定，如果用户要实现一个微积分过程，需要将状态变量在输入输出值中分别声明\r\n\r\n   --TODO:Write your function here! \r\n\r\n";
		char * str3 = "   \r\n   \r\n   return bcanEnable, tag\r\n";
		char * str4 = "end\r\n";
		file.Write(str1,strlen(str1));                                   //写入模板
		file.Write(str2,strlen(str2));
		file.Write(str3,strlen(str3));
		file.Write(str4,strlen(str4));
	}
	file.Close();
	ShellExecute(0,L"open",L"LuaEditor\\notepad++.exe",randluafile_1,L"",SW_SHOWNORMAL);
	//---------------------------------------创建或打开Lua编辑环境供用户编辑----------End------
}*/




void CTabPropSet1::OnCbnSelchgComboConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_stts_cmb_connect.GetCurSel();
	if(nSel == 0)
	{
		m_edit_ip.EnableWindow(FALSE);
		m_edit_port.EnableWindow(FALSE);
		m_edit_output.EnableWindow(FALSE);
		m_edit_input.EnableWindow(FALSE);
		this->ischoseSocket = false;
	}
	if(nSel == 1)
	{
		m_edit_ip.EnableWindow(TRUE);
		m_edit_port.EnableWindow(TRUE);
		m_edit_output.EnableWindow(TRUE);
		m_edit_input.EnableWindow(TRUE);
		this->ischoseSocket = true;
	}
	else
		this->ischoseSocket = false;
}


void CTabPropSet1::OnEnChgEditIp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
//	this->p_Model->ischoseSocket = true;
}


void CTabPropSet1::OnEnChgEditPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
//	this->p_Model->ischoseSocket = true;
}

void CTabPropSet1::OnEnChangeEditInput()
{
	UpdateData();
}



void CTabPropSet1::OnEnChangeEditOutput()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData();
	// TODO:  在此添加控件通知处理程序代码
}
