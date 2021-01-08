// TabPropSet1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "TabPropSet1.h"
//#include "afxdialogex.h"

#include <Windows.h>


// CTabPropSet1 �Ի���

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


// CTabPropSet1 ��Ϣ�������


BOOL CTabPropSet1::Create(UINT nIDTemplate, enumPlaceStyle placestyle,CPlace* pModel, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���
	this->PlaceStyle = placestyle;
	this->p_Model = pModel;

	return CDialog::Create(nIDTemplate, pParentWnd);
}


BOOL CTabPropSet1::OnInitDialog()
{
	CDialog::OnInitDialog();
	Lua_fileName = p_Model->m_caption;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//--------------------------------------------------------------------------------------------------------��һ�εĿؼ�״̬��ʼ��-----Begin---
	// ���ݿ��������ͣ�����������������Ӧ��״̬
	//------------------------------------------------��ʼ����״̬����Combo���ʼ��
	m_stts_cmb_activ.InsertString(0,L"True");      // �����һ��ѡ��
	m_stts_cmb_activ.InsertString(1,L"False");     // ����ڶ���ѡ��
	//------------------------------------------------������ֵ�Ļ�ȡ��ʽѡ���CmoBo��ĳ�ʼ��
	m_stts_cmb_prob.InsertString(0,L"�Զ���");
	m_stts_cmb_prob.InsertString(1,L"��������ֵ");
	m_stts_cmb_prob.InsertString(2,L"��̻�ȡ");
	m_stts_cmb_prob.SetCurSel(0);                  //����Ĭ��ѡ��Ϊ"�Զ���"
	//-----------------------------------------------������ֵ�Ļ�ȡ��ʽѡ���CmoBo��ĳ�ʼ��
	m_stts_cmb_time.InsertString(0,L"�Զ���");
	m_stts_cmb_time.InsertString(1,L"��������ֵ");
	m_stts_cmb_time.InsertString(2,_T("���ʱ��"));
	m_stts_cmb_time.SetCurSel(0);                  //����Ĭ��ѡ��Ϊ"�Զ���"
	//-----------------------------------------------���ƿ����ļ���ֵ�Ļ�ȡ��ʽѡ���CmoBo��ĳ�ʼ��
	m_stts_cmb_ctrl.InsertString(0,L"�Զ���");
	m_stts_cmb_ctrl.InsertString(1,L"��������ֵ");
	m_stts_cmb_ctrl.SetCurSel(0);                  //����Ĭ��ѡ��Ϊ"�Զ���"
	//-----------------------------------------------�������̵Ļ�ȡ��ʽѡ���CmoBo��ĳ�ʼ��
	m_cmb_cntnus.InsertString(0,L"�Զ���");
	m_cmb_cntnus.InsertString(1,L"����...���༭");
	m_cmb_cntnus.SetCurSel(0);
	//-----------------------------------------------ĳЩ�����Ķ���ӿ�ѡ���Cmobo��ĳ�ʼ��
	m_stts_cmb_connect.InsertString(0,L"��");
	m_stts_cmb_connect.InsertString(1,L"��");
	m_stts_cmb_connect.SetCurSel(0);

	// ���ö˿ڻ�ȡ�Ի���ĳ�ʼ״̬------------���ۿؼ�������Σ���˱�ǩ��Ĭ��״̬���ǡ��Զ��塱��������Ӧ�Ŀؼ�Ӧ������ΪHide �� uneanble-��1��
	m_cmb_portprob.ShowWindow(SW_HIDE);
	m_cmb_porttime.ShowWindow(SW_HIDE);
	m_cmb_portctrl.EnableWindow(FALSE);
	// ----------------------------------------���ø����ؼ���Ĭ��״̬�����ݿ������͵Ĳ�ͬ��-------------------------------------------------��2��
	switch(this->PlaceStyle)
	{
	case STYLE_DISCRETE:
		m_stts_cmb_time.EnableWindow(FALSE);// ���ø��ʡ�ʱ�䡢������ؿؼ�������
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
		m_stts_cmb_time.EnableWindow(FALSE);// ����ʱ�䡢������ؿؼ�������
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
		m_stts_cmb_time.EnableWindow(TRUE);// ���������������ؿؼ�������
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
		m_stts_cmb_time.EnableWindow(FALSE);// ���������������ؿؼ�������
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
		m_stts_cmb_time.EnableWindow(FALSE);// ���������������ؿؼ�������
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
	//--------------------------------------------------------------------------------------------------------��һ�εĿؼ�״̬��ʼ��-----End---
	//--------------------------------------------------------------------------------------------------------�ٴδ򿪶Ի��򣬽��ؼ���ʼ��Ϊ����״̬-----Begin---
	//-----------------------------------�ٴδ򿪶Ի���ʱ����ʾ�ѱ��������-------------------------------��1��
	if(this->bActivation_set)
		m_stts_cmb_activ.SetCurSel(0);           //����Ĭ��ѡ��ΪFalse
	else
		m_stts_cmb_activ.SetCurSel(1);           //����Ĭ��ѡ��ΪFalse
	// ----------------------���ر�ģ�͵��������ԶԻ����ٴδ�ʱ�������û�ѡ�񣬽��ؼ���״̬�������û�����2��
	if(bisUserDef_prob)
	{
		m_stts_cmb_prob.SetCurSel(0);           //ѡ���ǩΪ���Զ��塱
		m_cmb_portprob.ShowWindow(SW_HIDE);
		m_edt_probvalue.ShowWindow(SW_SHOW);
		m_btn_usrProgrand.EnableWindow(false);
	}
	else
	{
		m_stts_cmb_prob.SetCurSel(1);           //ѡ���ǩΪ����������ֵ��
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
		m_stts_cmb_time.SetCurSel(1);          //ѡ���ǩΪ����������ֵ��
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
	//--------------------------------------------------------------------------------------------------------�ٴδ򿪶Ի��򣬽��ؼ���ʼ��Ϊ����״̬-----Begin---
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// ��ȡ�û����õĳ�ʼ����״ֵ̬
void CTabPropSet1::OnSelchange_activstts()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��ȡ��Ͽ�ؼ����б����ѡ���������   
    int nSel = m_stts_cmb_activ.GetCurSel(); 
	if(this->PlaceStyle == STYLE_TCTRL || this->PlaceStyle == STYLE_LCTRL)// ��������������ǿ��ƿ���,Ҫ���ݼ���ֵ�Ļ�ȡ��ʽ��ѡ�񼤻�ֵ�Ƿ�Ӹÿؼ��л�ȡ
	{
		if(bisUserDef_ctrl)//�����ʽΪ���Զ��塱����Ӹÿؼ��л�ȡ
		{
			if(nSel == 0)//��ǩΪTrue
			{
				bActivation_set = TRUE;
			}
			if(nSel == 1)//��ǩΪFalse
			{
				bActivation_set = FALSE;
			}
		}
		else
		{
			//do nothing// do nothing��ʾ��ֵӦ�ô�����ֵ��ȡ����ô���ĸ�����ֵ��ȡ�أ�����Ҫ��ֵ��һ����ȡ����ֵ�Ķ˿�
		}
	}
	else                                                                  //����Ƿǿ��ƿ���������ֵ�Ӹÿؼ��л�ȡ
	{
		if(nSel == 0)//��ǩΪTrue
		{
			bActivation_set = TRUE;
		}
		if(nSel == 1)//��ǩΪFalse
		{
			bActivation_set = FALSE;
		}
	}
}

// ��ȡ�û����õĸ�����ֵ��ȡ��ʽ
void CTabPropSet1::OnSelchange_probstts()
{
	// ��ȡ��Ͽ�ؼ����б����ѡ���������   
    int nSel = m_stts_cmb_prob.GetCurSel();   
    if(nSel == 0)                                                 //��ǩΪ���Զ��塱
	{
		bisUserDef_prob = TRUE;
		m_edt_probvalue.EnableWindow(TRUE);                       // ͬʱ���ø�����ֵ��ȡ�༭��Ϊ������״̬��
		m_edt_probvalue.ShowWindow(SW_SHOW);                      // ͬʱ���ñ༭������
		GetDlgItem(IDC_STATIC_PROB)->SetWindowText(L"ֵ��");
		m_cmb_portprob.ShowWindow(SW_HIDE);
		m_cmb_portprob.EnableWindow(true);
		m_btn_usrProgrand.EnableWindow(false);
	}

	if(nSel == 1)                                                 //��ǩΪ����������ֵ��
	{
		bisUserDef_prob = FALSE;
		m_edt_probvalue.EnableWindow(FALSE);                      // ͬʱ���ø�����ֵ��ȡ�༭��Ϊ��������״̬��
		m_edt_probvalue.ShowWindow(SW_HIDE);                      // ͬʱ���ñ༭������
		GetDlgItem(IDC_STATIC_PROB)->SetWindowText(L"������");
		m_cmb_portprob.ShowWindow(SW_SHOW);
		m_cmb_portprob.EnableWindow(TRUE);
		m_btn_usrProgrand.EnableWindow(false);
		// ��ʼ��m_cmb_portprob�ؼ�,֮�������Ϣ��Ӧ��������ȡ�˿�ֵ-----
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
		m_cmb_portprob.SetCurSel(0);//Ĭ��ѡ���±�Ϊ0��ѡ��
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

// ��ȡ������ֵ--���Զ��塱��ʽ��
void CTabPropSet1::OnTextChng_probvalue()
{

	if(bisUserDef_prob)                                           // ���������ֵ�Ļ�ȡ��ʽΪ���Զ��塱
	{
		UpdateData();                                             // ����UpdateData()��ɶԻ����пؼ����ؼ��������������ݽ���
	}
}

// ��ȡʱ����ֵ�Ļ�ȡ��ʽ
void CTabPropSet1::OnSelchange_timestts()
{
	 
    int nSel = m_stts_cmb_time.GetCurSel();                       // ��ȡ��Ͽ�ؼ����б����ѡ���������   
    if(nSel == 0)                                                 // ��ǩΪ���Զ��塱
	{
		bisUserDef_time = TRUE;
		m_edit_timevalue.EnableWindow(TRUE);                      // ͬʱ����ʱ����ֵ��ȡ�༭��Ϊ������״̬��
		m_edit_timevalue.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(L"ֵ��");
		m_cmb_porttime.ShowWindow(SW_HIDE);
		dTime_edt_down.EnableWindow(false);
		dTime_edit_up.EnableWindow(false);
	}
	if(nSel == 1)                                                 // ��ǩΪ����������ֵ��
	{
		bisUserDef_time = FALSE;
		m_edit_timevalue.EnableWindow(FALSE);                     // ͬʱ����ʱ����ֵ��ȡ�༭��Ϊ��������״̬��
		m_edit_timevalue.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(L"������");
		m_cmb_porttime.ShowWindow(SW_SHOW);
		m_cmb_porttime.EnableWindow(true);
		dTime_edt_down.EnableWindow(false);
		dTime_edit_up.EnableWindow(false);
		// ��ʼ��m_cmb_porttime�ؼ�,֮�������Ϣ��Ӧ��������ȡ�˿�ֵ-----
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

// ��ȡʱ����ֵ--���Զ��塱��ʽ��
void CTabPropSet1::OnTextChng_timevalue()
{

	if(bisUserDef_time)                                          // ���ʱ����ֵ�Ļ�ȡ��ʽΪ���Զ��塱
	{
		UpdateData();                                            // ����UpdateData()��ɶԻ����пؼ����ؼ��������������ݽ���
	}

}

// ��ȡ���ƿ�������ֵ�Ļ�ȡ��ʽ
void CTabPropSet1::OnSelchange_ctrlstts()
{
    int nSel = m_stts_cmb_ctrl.GetCurSel();                      // ��ȡ��Ͽ�ؼ����б����ѡ���������
    if(nSel == 0)                                                // ��ǩΪ���Զ��塱
	{
		bisUserDef_ctrl = TRUE;
		m_stts_cmb_activ.EnableWindow(TRUE);                     // ͬʱ���ø�����ֵ��ȡ�༭��Ϊ������״̬��
		m_cmb_portctrl.EnableWindow(FALSE);
	}
	if(nSel == 1)                                                // ��ǩΪ����������ֵ��
	{
		bisUserDef_ctrl = FALSE;
		m_stts_cmb_activ.EnableWindow(FALSE);                    // ͬʱ���ø�����ֵ��ȡ�༭��Ϊ��������״̬��
		m_cmb_portctrl.EnableWindow(TRUE);
		// ��ʼ��m_cmb_portctrl�ؼ�,֮�������Ϣ��Ӧ��������ȡ�˿�ֵ-----
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

// ��ȡdProblty����˿�
void CTabPropSet1::OnSelchng_inPortprob()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_cmb_portprob.GetCurSel();
	CString str;
	m_cmb_portprob.SetCurSel(nSel);
	m_cmb_portprob.GetWindowText(str);
	str_Inputport_prob  = str;
}

// ��ȡdTime����˿�
void CTabPropSet1::OnSelchng_inPorttime()
{
	int nSel = m_cmb_porttime.GetCurSel();
	CString str;
	m_cmb_porttime.SetCurSel(nSel);
	m_cmb_porttime.GetWindowText(str);
	str_Inputport_time = str;
}

// ��ȡCtrl����˿�
void CTabPropSet1::OnSelchng_inPortctrl()
{
	int nSel = m_cmb_portctrl.GetCurSel();
	CString str;
	m_cmb_portctrl.SetCurSel(nSel);
	m_cmb_portctrl.GetWindowText(str);
	str_Inputport_ctrl = str;
}

// ���û�������Զ���Lua�ű���ʱ�������������������Ψһ������Lua�ű�
void CTabPropSet1::OnBtnClckd_CreateorOpenLua()
{
	if (!PathIsDirectory(_T("LuaFile")))//�ж��ļ����Ƿ񴴽�
	{
		::CreateDirectory(_T("LuaFile"), NULL);//����һ���ļ���
	}
	//---------------------------------------�������Lua�༭�������û��༭----------Begin----
	Lua_fileName = p_Model->m_caption;                                 //��ȡ���������֣���νLua�ļ����Լ�������         
	CString Lua_fileName_1 = _T("LuaFile\\") + Lua_fileName + _T(".lua");
	CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	if(file.GetLength()==0)
	{
		USES_CONVERSION;                                                  //��T2A֮ǰ��һ��Ҫ��USES_CONVERSION��ʶ��������ת������
		CString str = L"function  " + Lua_fileName + L"(bcanEnable, step, t, x1, x2, ...)\r\n";
		char * str1 = T2A(str);                                           //���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
		char * str2 = "   \r\n   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���\r\n   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������\r\n   --Note:�����������ڶ����͵����������ֱ�Ϊ���沽��step��ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������\r\n   --Note:���߿������ڶ�������Ϊ���沽��step������������Ϊ��ʶ����n�����ĸ����Ժ�Ĳ�����ʼΪ���Ա�ʶtag1��tag2��... ��tagn��֮���������������x1,x2...\r\n   --��1��lua�ű��в��Ա�ʶ��tag1��tag2������ֵʱ�����������ϵ�����һ�£��������ϲ����ڶ�Ӧ�����֣���Ӧ�ø�ֵΪ0������ֵʱĬ��ֵΪ0����\r\n   --��2�������ϵ����ֱ������0��\r\n   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����\r\n\r\n   --TODO:Write your function here! \r\n\r\n";
		char * str3 = "   \r\n   \r\n   return bcanEnable, step, t, x1, x2, ...\r\n";
		char * str4 = "end\r\n";
		file.Write(str1,strlen(str1));                                   //д��ģ��
		file.Write(str2,strlen(str2));
		file.Write(str3,strlen(str3));
		file.Write(str4,strlen(str4));
	}
	file.Close();
	ShellExecute(0,L"open",L"LuaEditor\\notepad++.exe",Lua_fileName_1,L"",SW_SHOWNORMAL);
	//---------------------------------------�������Lua�༭�������û��༭----------End------
}

// ��ȡʱ�����������down
void CTabPropSet1::OnEnChangeEdtDown()
{
	UpdateData();
	this->p_Model->isTimeUpDownChange = false;
}

// ��ȡʱ�����������up
void CTabPropSet1::OnEnChangeEdtUp()
{
	UpdateData();
	this->p_Model->isTimeUpDownChange = false;
}

// Ϊ�����������Lua�ű�
void CTabPropSet1::OnBtn_RandUser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!PathIsDirectory(_T("LuaFile")))//�ж��ļ����Ƿ񴴽�
	{
		::CreateDirectory(_T("LuaFile"), NULL);//����һ���ļ���
	}
	//---------------------------------------�������Lua�༭�������û��༭----------Begin----
	CString randluafile = p_Model->m_caption;                                 //��ȡ���������֣���νLua�ļ����Լ�������         
	CString randluafile_1 = _T("LuaFile\\S_") +randluafile + _T(".lua");
	CFile file(randluafile_1,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	if(file.GetLength()==0)
	{
		USES_CONVERSION;                                                   //��T2A֮ǰ��һ��Ҫ��USES_CONVERSION��ʶ��
		CString str = L"function  S_"  + randluafile + L"(bcanEnable, random_num)\r\n";
		char * str1 = T2A(str);                                           //���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
		char * str2 = "   \r\n   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���\r\n   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������\r\n   --Note:�ڶ�������Ϊrandom_num���û��������趨�ĸ�����ֵ\r\n   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����\r\n\r\n   --TODO:Write your function here! \r\n\r\n";
		char * str3 = "   \r\n   \r\n   return bcanEnable, random_num\r\n";
		char * str4 = "end\r\n";
		file.Write(str1,strlen(str1));                                   //д��ģ��
		file.Write(str2,strlen(str2));
		file.Write(str3,strlen(str3));
		file.Write(str4,strlen(str4));
	}
	file.Close();
	ShellExecute(0,L"open",L"LuaEditor\\notepad++.exe",randluafile_1,L"",SW_SHOWNORMAL);
	//---------------------------------------�������Lua�༭�������û��༭----------End------
}

/*void CTabPropSet1::OnBtn_DisicUser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!PathIsDirectory(_T("LuaFile")))//�ж��ļ����Ƿ񴴽�
	{
		::CreateDirectory(_T("LuaFile"), NULL);//����һ���ļ���
	}
	//---------------------------------------�������Lua�༭�������û��༭----------Begin----
	CString randluafile = p_Model->m_caption;                                 //��ȡ���������֣���νLua�ļ����Լ�������         
	CString randluafile_1 = _T("LuaFile\\D_") +randluafile + _T(".lua");
	CFile file(randluafile_1,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	if(file.GetLength()==0)
	{
		USES_CONVERSION;                                                   //��T2A֮ǰ��һ��Ҫ��USES_CONVERSION��ʶ��
		CString str = L"function  D_"  + randluafile + L"(bcanEnable, tag, step, t, x1, x2, ...)\r\n";
		char * str1 = T2A(str);                                           //���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
		char * str2 = "   \r\n   --Note:��д������Ҫ����ģ�������ʽ��д������������ĸ�ģ�壬��������������Ĳ���Ԥ���Դ���\r\n   --Note:��������ֵ�ͷ���ֵ�ĵ�һ��Ԫ�ر�����һ��booleanֵ�������жϿ����Ƿ����ʹ������\r\n   --Note:�ڶ����͵����������ֱ�Ϊ���沽����ʱ��t�����沽�������û��ڸ��ļ��г�ʼ������ʱ��t��Ҫ�û����г�ʼ������\r\n   --Note:�����������û��趨������û�Ҫʵ��һ��΢���ֹ��̣���Ҫ��״̬�������������ֵ�зֱ�����\r\n\r\n   --TODO:Write your function here! \r\n\r\n";
		char * str3 = "   \r\n   \r\n   return bcanEnable, tag\r\n";
		char * str4 = "end\r\n";
		file.Write(str1,strlen(str1));                                   //д��ģ��
		file.Write(str2,strlen(str2));
		file.Write(str3,strlen(str3));
		file.Write(str4,strlen(str4));
	}
	file.Close();
	ShellExecute(0,L"open",L"LuaEditor\\notepad++.exe",randluafile_1,L"",SW_SHOWNORMAL);
	//---------------------------------------�������Lua�༭�������û��༭----------End------
}*/




void CTabPropSet1::OnCbnSelchgComboConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
//	this->p_Model->ischoseSocket = true;
}


void CTabPropSet1::OnEnChgEditPort()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
//	this->p_Model->ischoseSocket = true;
}

void CTabPropSet1::OnEnChangeEditInput()
{
	UpdateData();
}



void CTabPropSet1::OnEnChangeEditOutput()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
