// TabPropSet2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "TabPropSet2.h"
//#include "afxdialogex.h"

#include "MainFrm.h"
#include "HSTPN_SYSView.h"

// CTabPropSet2 �Ի���

IMPLEMENT_DYNAMIC(CTabPropSet2, CDialog)

CTabPropSet2::CTabPropSet2(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPropSet2::IDD, pParent)
	, m_editvalue_IOname(_T(""))
{
	m_row             = -1;
    m_column          = -1;
	str_IOFromcaption = "";
	m_pDoc = (CHSTPN_SYSDoc*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument());
	m_rowforD         = -1;
    m_columnforD      = -1;
}

CTabPropSet2::~CTabPropSet2()
{
}

void CTabPropSet2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IOPUT, m_List_IOput);
	DDX_Control(pDX, IDC_EDIT_GETIONAME, m_edit_getIOName);
	DDX_Control(pDX, IDC_CMB_GETIOTYPE, m_cmb_getIOType);
	DDX_Control(pDX, IDC_CMB_GETIORO, m_cmb_IorO);
	DDX_Control(pDX, IDC_CMB_GETIOPORT, m_cmb_getIOPort);
	DDX_Control(pDX, IDC_CMB_GETIOFROM, m_cmb_getIOFrom);
	DDX_Control(pDX, IDC_CMB_GETIOFROMPORT, m_cmb_getIOFromport);
	DDX_Text(pDX, IDC_EDIT_GETIONAME, m_editvalue_IOname);
}


BEGIN_MESSAGE_MAP(CTabPropSet2, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_IOPUT, &CTabPropSet2::OnDblclk_ListIoput)
	ON_CBN_SELCHANGE(IDC_CMB_GETIOTYPE, &CTabPropSet2::OnCmbCSelchange_Getiotype)
	ON_CBN_SELCHANGE(IDC_CMB_GETIORO, &CTabPropSet2::OnCmbSelchange_Getioro)
	ON_CBN_SELCHANGE(IDC_CMB_GETIOPORT, &CTabPropSet2::OnCmbSelchange_Getioport)
	ON_CBN_SELCHANGE(IDC_CMB_GETIOFROM, &CTabPropSet2::OnCmbSelchange_Getiofrom)
	ON_CBN_SELCHANGE(IDC_CMB_GETIOFROMPORT, &CTabPropSet2::OnCmbSelchng_Getiofromport)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IOPUT, &CTabPropSet2::OnClick_ListIoput)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNADDVALUE, &CTabPropSet2::OnBtnaAddValue)
	ON_BN_CLICKED(IDC_BTNDLTVALUE, &CTabPropSet2::OnBtnDltValue)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IOPUT, &CTabPropSet2::OnLvnItemchangedListIoput)
END_MESSAGE_MAP()


BOOL CTabPropSet2::Create(UINT nIDTemplate, enumPlaceStyle placestyle, CPlace* pModel,  CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	this->PlaceStyle = placestyle;   //��Ӧ�������������
	this->p_Model    = pModel;       //��Ӧ�Ŀ�������ĵ�ַָ��

	return CDialog::Create(nIDTemplate, pParentWnd);
}

BOOL CTabPropSet2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//-------------------------------------------------------------------------����/����������(��ʼ����-----------Begin--------------------------
	m_List_IOput.ModifyStyle(0,LVS_REPORT);               // ����Ϊ����ģʽ
	m_List_IOput.SetExtendedStyle(m_List_IOput.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// �����+��ѡ��

	m_List_IOput.InsertColumn(0,L"������");               // �����ͷ
	//m_List_IOput.InsertColumn(1,L"����"); 
	m_List_IOput.InsertColumn(1,L"����");
	m_List_IOput.InsertColumn(2,L"I/O");
	m_List_IOput.InsertColumn(3,L"����Դ");
	m_List_IOput.InsertColumn(4,L"����Դ����");
	m_List_IOput.SetBkColor(RGB(255,255,255));

/*	for(int n=0;n<20;n++)
	{
		m_List_IOput.InsertItem(n,L"");
	}
	*/
	CRect rect;  
	m_List_IOput.GetClientRect(rect);                     //��õ�ǰ�ͻ�����Ϣ   
	m_List_IOput.SetColumnWidth(0, rect.Width() / 5);     //�����еĿ�ȡ�   
	m_List_IOput.SetColumnWidth(1, rect.Width() / 5); 
	m_List_IOput.SetColumnWidth(2, rect.Width() / 5);
	m_List_IOput.SetColumnWidth(3, rect.Width() / 5);
	m_List_IOput.SetColumnWidth(4, rect.Width() / 5);
	//-------------------------------------------------------------------------����/����������(��ʼ����-----------End----------------------------

	//------------------------------------------------------�������롢ѡ��ؼ��ĳ�ʼ״̬Ϊ���ɼ��������������ֿؼ�������----------Begin------------
	m_edit_getIOName.ShowWindow(SW_HIDE);                // ��ʼ״̬��Ϊ����״̬
	m_cmb_getIOType.ShowWindow(SW_HIDE);
	m_cmb_IorO.ShowWindow(SW_HIDE);
	m_cmb_getIOPort.ShowWindow(SW_HIDE);
	m_cmb_getIOFrom.ShowWindow(SW_HIDE);
	m_cmb_getIOFromport.ShowWindow(SW_HIDE);

	/*m_cmb_getIOType.InsertString(0,L"int");              // DataType
	m_cmb_getIOType.InsertString(1,L"float");
	m_cmb_getIOType.InsertString(2,L"double");
	m_cmb_getIOType.InsertString(3,L"bool");
	m_cmb_getIOType.SetCurSel(0);*/
	m_cmb_getIOType.InsertString(0,L"0");
	m_cmb_getIOType.SetCurSel(0);

	m_cmb_IorO.InsertString(0,L"input");                 //input or output
	m_cmb_IorO.InsertString(1,L"output");
	m_cmb_IorO.SetCurSel(0);
	//------------------------------------------------------�������롢ѡ��ؼ��ĳ�ʼ״̬Ϊ���ɼ��������������ֿؼ�������----------End--------------

	//------------------------------------------------------���ٴε��ģ�ʹ򿪶Ի���ʱ��ʹ���������/��������ٴ���ʾ�ڱ����-----Begin------------
	int count_row = 0; 
	for(vector<IOValue*>::iterator iter=arryIOputData.begin();iter!=arryIOputData.end();iter++)
	{
		m_List_IOput.InsertItem(count_row,L"");
		CString str;
		CString str_sequence;
		m_List_IOput.SetItemText(count_row,0,(**iter).Name);
		str_sequence.Format(L"%d",(**iter).Sequence);
		m_List_IOput.SetItemText(count_row,1,str_sequence);//DataType);
		m_List_IOput.SetItemText(count_row,2,(**iter).IOType);
		m_List_IOput.SetItemText(count_row,3,(**iter).FromModel);
		if((**iter).IOType == "input")
		{
			CString str1;
			m_List_IOput.SetItemText(count_row,4,(**iter).FromMDVluName);	
		}
		else
		{
			m_List_IOput.SetItemText(count_row,4,L"��������");
		}
		count_row++;
	}
	//------------------------------------------------------���ٴε��ģ�ʹ򿪶Ի���ʱ��ʹ���������/��������ٴ���ʾ�ڱ����-----End--------------
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTabPropSet2::OnDblclk_ListIoput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect rc;
    if(pNMListView->iItem==-1)
	{
		return;
	}

    m_row=pNMListView->iItem;                                                             // m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
    m_column=pNMListView->iSubItem;                                                       // m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������

	UpdateData(TRUE);
	m_List_IOput.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc); // ��ȡ���������ľ���λ�úʹ�С
	
	if(m_column == 0)                                                                     // �����˫���ĵ�Ԫ���ǵ�һ�У���ΪIOname�����Ȼ�ȡ��Ԫ���е�ֵ��
	{                                                                                     // �����õ�edit�ؼ��У�Ŀ���ǣ���������ٴ�˫����һ��ʱ��
		CString str = m_List_IOput.GetItemText(m_row,m_column);                           // ������ٴ��������������ԭ���ı�����Ҳ����ʧ����������
		m_edit_getIOName.SetWindowText(str);
	}

	InitCtrl_GetIO(m_row, m_column, rc);                                                  // ����˫�����к��У���ʾ��Ӧ�ؼ�����Ԫ��λ��,�����ؼ�����

	*pResult = 0;
}

// ��ȡIOname
void CTabPropSet2::OnClick_ListIoput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect rc;
    if(pNMListView->iItem==-1)
	{
		return;
	}

    m_rowforD=pNMListView->iItem;                                                             // m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
    m_columnforD=pNMListView->iSubItem;                                                       // m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������



	//-----------------------------------��ȡ������IOname-------------------------------------------------------------
	if(m_column == 0)                                   // m_column==0��ʾ�ϴα�˫�����ǻ�ȡIOname�ı༭����ˣ�ȥ��ȡ�ñ༭���ֵ�����򣬲���ȡ
	{
		UpdateData();                                   //��ɿؼ�����������ݽ������Ӷ���ȡm_editvalue_IOnameֵ��

		SetCtrlStatus(SW_HIDE);                         // ���ÿؼ����ɼ�

		if(arryIOputData.size()>m_row)                  //��������½������Ǹı����б�����������Ӧλ��֮��ı��������
		{	
			m_List_IOput.SetItemText(m_row,m_column,m_editvalue_IOname);

			arryIOputData.at(m_row)->Name = m_editvalue_IOname;
		}
		else
		{
		/*	if(arryIOputData.size()==m_row)            // ����ñ����ڱ����ʱ�µ�һ��,����������һ��������
			{
				m_List_IOput.SetItemText(m_row,m_column,m_editvalue_IOname);

				IOValue* value = new IOValue();
				value->Name = m_editvalue_IOname;
				vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
				arryIOputData.insert(iter,1,value);
			}
			else
			{
				MessageBox(L"���ݱ��п��У�");
			}*/
		}
	}
	else
	{
		SetCtrlStatus(SW_HIDE);                         //���ÿؼ����ɼ�
	}
	//-------------------------------------------------------------------------------------------------
	*pResult = 0;
}


// ��ȡDataType
void CTabPropSet2::OnCmbCSelchange_Getiotype()// change for the function to get Sequence
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//int nSel = m_cmb_getIOType.GetCurSel();  
	//CString str;
	
	/*switch (nSel)                                   // ��ȡ�û�ѡ��
	{
	case 0:
		str = "int";
		break;
	case 1:
		str = "float";
		break;
	case 2:
		str = "double";
		break;
	case 3:
		str = "bool";
		break;
	default:
		break;
	}
	
	SetCtrlStatus(SW_HIDE);                         // �����ֵ��������ÿؼ����ɼ�

	if(arryIOputData.size()>m_row)                  // ����ñ����ڱ����ʱ�µ�һ��,����������һ��������������ǣ�������Ӧλ��֮��ı��������
	{
		m_List_IOput.SetItemText(m_row,m_column,str);

		arryIOputData.at(m_row)->DataType = str;
	}
	else
	{	
		if(arryIOputData.size()==m_row)
		{
			m_List_IOput.SetItemText(m_row,m_column,str);

			IOValue *value = new IOValue();
			value->DataType = str;
			vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
			arryIOputData.insert(iter,1,value);
		}
		else
		{
			MessageBox(L"���ݱ��п��У�");
		}
	}*/
	int nSel = m_cmb_getIOType.GetCurSel(); 
	m_cmb_getIOType.SetCurSel(nSel);
	CString str; 
	m_cmb_getIOType.GetWindowTextW(str);

	SetCtrlStatus(SW_HIDE);

	if(arryIOputData.size()>m_row)                  // ����ñ����ڱ����ʱ�µ�һ��,����������һ��������������ǣ�������Ӧλ��֮��ı��������
	{
		m_List_IOput.SetItemText(m_row,m_column,str);

		arryIOputData.at(m_row)->Sequence = _ttoi(str);
	}
	else
	{	
		if(arryIOputData.size()==m_row)
		{
			m_List_IOput.SetItemText(m_row,m_column,str);

			IOValue *value = new IOValue();
			value->Sequence = _ttoi(str);//���ַ���ת�������Σ������ֻ������ſ�ʼת����һֱ��������Ϊֹ
			vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
			arryIOputData.insert(iter,1,value);
		}
		else
		{
			MessageBox(L"���ݱ��п��У�");
		}
	}
}

// ��ȡIorO   ͬʱ��ÿ��������Valueֵ��ʼ��Ϊ0
//ѡ��"input"or"output"
void CTabPropSet2::OnCmbSelchange_Getioro()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_cmb_IorO.GetCurSel();  
	CString str;
	switch (nSel)                            // ��ȡ�û�ѡ��
	{
	case 0:                                  //��ǩΪ��input��
		str = "input";
		break;
	case 1:                                  //��ǩΪ��output��
		str = "output";
		break;
	default:
		break;
	}
	
	SetCtrlStatus(SW_HIDE);                 // �����ֵ��������ÿؼ����ɼ�

	if(arryIOputData.size()>m_row)          // ����ñ����ڱ����ʱ�µ�һ��,����������һ��������������ǣ�������Ӧλ��֮��ı��������
	{
		m_List_IOput.SetItemText(m_row,m_column,str);

		arryIOputData.at(m_row)->IOType = str;

		arryIOputData.at(m_row)->Value = 0;
	}
	else
	{
		if(arryIOputData.size()==m_row)
		{
			m_List_IOput.SetItemText(m_row,m_column,str);

			IOValue *value = new IOValue();
			value->IOType = str;
			value->Value = 0;
			vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
			arryIOputData.insert(iter,1,value);
		}
		else
		{
			MessageBox(L"���ݱ��п��У�");
		}
	}
}

// ��ȡIOPort
void CTabPropSet2::OnCmbSelchange_Getioport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
/*	int nSel = m_cmb_getIOPort.GetCurSel();  
	CString str;
	m_cmb_getIOPort.SetCurSel(nSel);              // ������ѡ����Ϊ��ǰ��
	m_cmb_getIOPort.GetWindowText(str);           // Ȼ���ȡ��ǰ�������

	SetCtrlStatus(SW_HIDE);                       // �����ֵ��������ÿؼ����ɼ�

	int port = _ttoi(str);                        //��CString����ת����int����

	if(arryIOputData.size()>m_row)                // ����ñ����ڱ����ʱ�µ�һ��,����������һ��������������ǣ�������Ӧλ��֮��ı��������
	{
		m_List_IOput.SetItemText(m_row,m_column,str);

		arryIOputData.at(m_row)->Port = port;
	}
	else
	{
		
		if(arryIOputData.size()==m_row)
		{
			m_List_IOput.SetItemText(m_row,m_column,str);

			IOValue *value = new IOValue();
			value->Port = port;
			vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
			arryIOputData.insert(iter,1,value);
		}
		else
		{
			MessageBox(L"���ݱ��п��У�");
		}
	}
*/
}

// ��ȡIOFrom
//ѡ������Դ��
void CTabPropSet2::OnCmbSelchange_Getiofrom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_cmb_getIOFrom.GetCurSel();  
	m_cmb_getIOFrom.SetCurSel(nSel);                     // ������ѡ����Ϊ��ǰ��
	m_cmb_getIOFrom.GetWindowText(str_IOFromcaption);    // Ȼ���ȡ��ǰ�������

	SetCtrlStatus(SW_HIDE);                              // �����ֵ��������ÿؼ����ɼ�

	if(arryIOputData.size()>m_row)                       // ����ñ����ڱ����ʱ�µ�һ��,����������һ��������������ǣ�������Ӧλ��֮��ı��������
	{
		m_List_IOput.SetItemText(m_row,m_column,str_IOFromcaption);

		arryIOputData.at(m_row)->FromModel = str_IOFromcaption;
	}
	else
	{
		if(arryIOputData.size()==m_row)
		{
			m_List_IOput.SetItemText(m_row,m_column,str_IOFromcaption);

			IOValue *value = new IOValue();
			value->FromModel = str_IOFromcaption;
			vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
			arryIOputData.insert(iter,1,value);
		}
		else
		{
			MessageBox(L"���ݱ��п��У�");
		}
	}
}

// ��ȡIOFromPort
void CTabPropSet2::OnCmbSelchng_Getiofromport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_cmb_getIOFromport.GetCurSel();
	CString str;
	m_cmb_getIOFromport.SetCurSel(nSel);
	m_cmb_getIOFromport.GetWindowText(str);

	SetCtrlStatus(SW_HIDE);                              // �����ֵ��������ÿؼ����ɼ�

//	int port = _ttoi(str);                               // ��CString����ת��Ϊint����

	if(arryIOputData.size()>m_row)                       // ����ñ����ڱ����ʱ�µ�һ��,����������һ��������������ǣ�������Ӧλ��֮��ı��������
	{
		m_List_IOput.SetItemText(m_row,m_column,str);

		arryIOputData.at(m_row)->FromMDVluName = str;
	}
	else
	{
		if(arryIOputData.size()==m_row)
		{
			m_List_IOput.SetItemText(m_row,m_column,str);

			IOValue *value = new IOValue();
			value->FromMDVluName = str;
			vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
			arryIOputData.insert(iter,1,value);
		}
		else
		{
			MessageBox(L"���ݱ��п��У�");
		}
	}
}

// ĳЩ�ؼ��ĳ�ʼ��
void CTabPropSet2::InitCtrl_GetIO(int rowofchart, int columnofchart, CRect rc)
{

	if(columnofchart==0)                                 // ���˫�����ǵ�һ�� name
	{
		rc.left+=15;
		rc.right+=18;
		rc.top+=35;
		rc.bottom+=35;
		m_edit_getIOName.ShowWindow(SW_SHOW);
		m_edit_getIOName.MoveWindow(&rc);
		m_edit_getIOName.SetFocus();

		m_cmb_getIOType.ShowWindow(SW_HIDE);
		m_cmb_IorO.ShowWindow(SW_HIDE);
		m_cmb_getIOPort.ShowWindow(SW_HIDE);
		m_cmb_getIOFrom.ShowWindow(SW_HIDE);
		m_cmb_getIOFromport.ShowWindow(SW_HIDE);
	}
	if(columnofchart==1)                                 // ���˫�����ǵڶ��� IOType
	{
		rc.left+=21;
		rc.right+=17;
		rc.top+=35;
		rc.bottom+=30;
		m_cmb_getIOType.ShowWindow(SW_SHOW);
		m_cmb_getIOType.MoveWindow(&rc);
		m_cmb_getIOType.SetFocus();

		m_edit_getIOName.ShowWindow(SW_HIDE);
		m_cmb_IorO.ShowWindow(SW_HIDE);
		m_cmb_getIOPort.ShowWindow(SW_HIDE);
		m_cmb_getIOFrom.ShowWindow(SW_HIDE);
		m_cmb_getIOFromport.ShowWindow(SW_HIDE);

		int count = arryIOputData.size()+2;
		m_cmb_getIOType.ResetContent();
		CString str_count;
		for(int i=0;i<count;i++)
		{
			str_count.Format(L"%d",i);
			m_cmb_getIOType.InsertString(i,str_count);
		}
	}
	if(columnofchart==2)                                 // ���˫�����ǵ����� IorO
	{
		rc.left+=20;
		rc.right+=17;
		rc.top+=35;
		rc.bottom+=35;
		m_cmb_IorO.ShowWindow(SW_SHOW);
		m_cmb_IorO.MoveWindow(&rc);
		m_cmb_IorO.SetFocus();

		m_edit_getIOName.ShowWindow(SW_HIDE);
		m_cmb_getIOType.ShowWindow(SW_HIDE);
		m_cmb_getIOPort.ShowWindow(SW_HIDE);
		m_cmb_getIOFrom.ShowWindow(SW_HIDE);
		m_cmb_getIOFromport.ShowWindow(SW_HIDE);
	}
	if(columnofchart==3)                                 // ���˫�����ǵ����� IOFrom
	{
		rc.left+=21;
		rc.right+=17;
		rc.top+=35;
		rc.bottom+=100;
		m_cmb_getIOFrom.ShowWindow(SW_SHOW);
		m_cmb_getIOFrom.MoveWindow(&rc);
		m_cmb_getIOFrom.SetFocus();

		m_edit_getIOName.ShowWindow(SW_HIDE);
		m_cmb_getIOType.ShowWindow(SW_HIDE);
		m_cmb_IorO.ShowWindow(SW_HIDE);
		m_cmb_getIOPort.ShowWindow(SW_HIDE);
		m_cmb_getIOFromport.ShowWindow(SW_HIDE);
		//----------------��ʼ��m_cmb_getIOFrom�ؼ�--------------------------------------------------------------------------------------------------
		if(m_List_IOput.GetItemText(rowofchart,0)!="" && arryIOputData.size()>rowofchart)
		{
			if(arryIOputData.at(rowofchart)->IOType == "input")   // ���Ϊ��input����������ǩ�����Ѵ����Ŀ���ģ���趨
			{
				int i = 0;
				m_cmb_getIOFrom.ResetContent();                   //���������
				// ��ȡView���б����Vector_Place����
				
				for(vector<CPlace*>::iterator iter = m_pDoc->Vector_Place.begin();iter!=m_pDoc->Vector_Place.end();iter++)//�Ȼ�ȡFrameʵ�����ٻ�ȡViewʵ��	 
				{
					CString str = (**iter).m_caption;             //��ȡ�Ѵ����Ŀ����ı���
					m_cmb_getIOFrom.InsertString(i,str);
					i++;
				}
				m_cmb_getIOFrom.SetCurSel(0);
			}
			if(arryIOputData.at(rowofchart)->IOType == "output")  // ���Ϊ��output����������ǩ���ݿ���ģ����ά���ı����趨
			{
				m_cmb_getIOFrom.ResetContent();//���������
				CString strofvalue[56] = {L"������ʶ",L"���沽��"/*,L"ʱ��t"*/,L"����1",L"����2",L"����3",L"����4",L"����5",L"����6",L"����7",L"����8",L"����9",L"����10",
					                                                           L"����11",L"����12",L"����13",L"����14",L"����15",L"����16",L"����17",L"����18",L"����19",L"����20",
																			   L"����21",L"����22",L"����23",L"����24",L"����25",L"����26",L"����27",L"����28",L"����29",L"����30",
																			   L"����31",L"����32",L"����33",L"����34",L"����35",L"����36",L"����37",L"����38",L"����39",L"����40",
																			   L"����41",L"����42",L"����43",L"����44",L"����45",L"����46",L"����47",L"����48",L"����49",L"����50",
																			   L"�����ʶ",L"�����ʶ��ȡ����",L"ʹ�ܱ�ʶ",L"ʹ�ܱ�ʶ��ȡ����"};
				CString strofvalueforRand[56] = {L"������ʶ",L"������ֵ"/*,L"ʱ��t"*/,L"����1",L"����2",L"����3",L"����4",L"����5",L"����6",L"����7",L"����8",L"����9",L"����10",
					                                                           L"����11",L"����12",L"����13",L"����14",L"����15",L"����16",L"����17",L"����18",L"����19",L"����20",
																			   L"����21",L"����22",L"����23",L"����24",L"����25",L"����26",L"����27",L"����28",L"����29",L"����30",
																			   L"����31",L"����32",L"����33",L"����34",L"����35",L"����36",L"����37",L"����38",L"����39",L"����40",
																			   L"����41",L"����42",L"����43",L"����44",L"����45",L"����46",L"����47",L"����48",L"����49",L"����50",
																			   L"�����ʶ",L"�����ʶ��ȡ����",L"ʹ�ܱ�ʶ",L"ʹ�ܱ�ʶ��ȡ����"};
				int iofvalue = 0;
				int iofvalueforRand = 0;
				switch(this->PlaceStyle)
				{
				case STYLE_DISCRETE:
					m_cmb_getIOFrom.InsertString(0,L"�����ʶ");
					m_cmb_getIOFrom.InsertString(1,L"�����ʶ��ȡ����");
					m_cmb_getIOFrom.InsertString(2,L"ʹ�ܱ�ʶ");
					m_cmb_getIOFrom.InsertString(3,L"ʹ�ܱ�ʶ��ȡ����");
					break;
				case STYLE_CNTNUS:
					for(vector<IOValue*>::iterator iter = this->arryIOputData.begin();iter!=this->arryIOputData.end();iter++)
					{
						if((**iter).IOType == "output" && ((**iter).FromModel=="������ʶ" || (**iter).FromModel=="���沽��"  
							|| (**iter).FromModel=="����1" || (**iter).FromModel=="����2" || (**iter).FromModel=="����3" || (**iter).FromModel=="����4" 
							|| (**iter).FromModel=="����5" || (**iter).FromModel=="����6" || (**iter).FromModel=="����7" || (**iter).FromModel=="����8" 
							|| (**iter).FromModel=="����9" || (**iter).FromModel=="����10"|| (**iter).FromModel=="����11" || (**iter).FromModel=="����12" 
							|| (**iter).FromModel=="����13" || (**iter).FromModel=="����14" || (**iter).FromModel=="����15" || (**iter).FromModel=="����16" 
							|| (**iter).FromModel=="����17" || (**iter).FromModel=="����18"|| (**iter).FromModel=="����19" || (**iter).FromModel=="����20"
							|| (**iter).FromModel=="����21" || (**iter).FromModel=="����22"|| (**iter).FromModel=="����23" || (**iter).FromModel=="����24"
							|| (**iter).FromModel=="����25" || (**iter).FromModel=="����26"|| (**iter).FromModel=="����27" || (**iter).FromModel=="����28"
							|| (**iter).FromModel=="����29" || (**iter).FromModel=="����30"|| (**iter).FromModel=="����31" || (**iter).FromModel=="����32"
							|| (**iter).FromModel=="����33" || (**iter).FromModel=="����34"|| (**iter).FromModel=="����35" || (**iter).FromModel=="����36"
							|| (**iter).FromModel=="����37" || (**iter).FromModel=="����38"|| (**iter).FromModel=="����39" || (**iter).FromModel=="����40"
							|| (**iter).FromModel=="����41" || (**iter).FromModel=="����42"|| (**iter).FromModel=="����43" || (**iter).FromModel=="����44"
							|| (**iter).FromModel=="����45" || (**iter).FromModel=="����46"|| (**iter).FromModel=="����47" || (**iter).FromModel=="����48"
							|| (**iter).FromModel=="����49" || (**iter).FromModel=="����50"
							))
						{
							m_cmb_getIOFrom.InsertString(iofvalue,strofvalue[iofvalue]);
							iofvalue++;
						}
					}
					break;
				case STYLE_RAND:
					for(vector<IOValue*>::iterator iter = this->arryIOputData.begin();iter!=this->arryIOputData.end();iter++)
					{
						if((**iter).IOType == "output" && ((**iter).FromModel=="������ʶ" || (**iter).FromModel=="������ֵ" /*���沽��*/ 
							|| (**iter).FromModel=="����1" || (**iter).FromModel=="����2" || (**iter).FromModel=="����3" || (**iter).FromModel=="����4" 
							|| (**iter).FromModel=="����5" || (**iter).FromModel=="����6" || (**iter).FromModel=="����7" || (**iter).FromModel=="����8" 
							|| (**iter).FromModel=="����9" || (**iter).FromModel=="����10"|| (**iter).FromModel=="����11" || (**iter).FromModel=="����12" 
							|| (**iter).FromModel=="����13" || (**iter).FromModel=="����14" || (**iter).FromModel=="����15" || (**iter).FromModel=="����16" 
							|| (**iter).FromModel=="����17" || (**iter).FromModel=="����18"|| (**iter).FromModel=="����19" || (**iter).FromModel=="����20"
							|| (**iter).FromModel=="����21" || (**iter).FromModel=="����22"|| (**iter).FromModel=="����23" || (**iter).FromModel=="����24"
							|| (**iter).FromModel=="����25" || (**iter).FromModel=="����26"|| (**iter).FromModel=="����27" || (**iter).FromModel=="����28"
							|| (**iter).FromModel=="����29" || (**iter).FromModel=="����30"|| (**iter).FromModel=="����31" || (**iter).FromModel=="����32"
							|| (**iter).FromModel=="����33" || (**iter).FromModel=="����34"|| (**iter).FromModel=="����35" || (**iter).FromModel=="����36"
							|| (**iter).FromModel=="����37" || (**iter).FromModel=="����38"|| (**iter).FromModel=="����39" || (**iter).FromModel=="����40"
							|| (**iter).FromModel=="����41" || (**iter).FromModel=="����42"|| (**iter).FromModel=="����43" || (**iter).FromModel=="����44"
							|| (**iter).FromModel=="����45" || (**iter).FromModel=="����46"|| (**iter).FromModel=="����47" || (**iter).FromModel=="����48"
							|| (**iter).FromModel=="����49" || (**iter).FromModel=="����50"
							))
						{
							m_cmb_getIOFrom.InsertString(iofvalueforRand,strofvalueforRand[iofvalueforRand]);
							iofvalueforRand++;
						}
					}
					//m_cmb_getIOFrom.InsertString(iofvalueforRand,L"������ֵ");
					m_cmb_getIOFrom.InsertString(iofvalueforRand,L"�����ʶ");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+1,L"�����ʶ��ȡ����");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+2,L"ʹ�ܱ�ʶ");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+3,L"ʹ�ܱ�ʶ��ȡ����");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+4,L"������");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+5,L"��������ȡ����");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+6,L"�������ֵ");
					break;
				case STYLE_TIME:
					m_cmb_getIOFrom.InsertString(0,L"ʱ����ֵ");
					m_cmb_getIOFrom.InsertString(1,L"�����ʶ");
					m_cmb_getIOFrom.InsertString(2,L"�����ʶ��ȡ����");
					m_cmb_getIOFrom.InsertString(3,L"ʹ�ܱ�ʶ");
					m_cmb_getIOFrom.InsertString(4,L"ʹ�ܱ�ʶ��ȡ����");
					break;
				case STYLE_TCTRL:
					m_cmb_getIOFrom.InsertString(0,L"�����ʶ");
					m_cmb_getIOFrom.InsertString(1,L"�����ʶ��ȡ����");
					m_cmb_getIOFrom.InsertString(2,L"ʹ�ܱ�ʶ");
					m_cmb_getIOFrom.InsertString(3,L"ʹ�ܱ�ʶ��ȡ����");
					break;
				case STYLE_LCTRL:
					m_cmb_getIOFrom.InsertString(0,L"�����ʶ");
					m_cmb_getIOFrom.InsertString(1,L"�����ʶ��ȡ����");
					m_cmb_getIOFrom.InsertString(2,L"ʹ�ܱ�ʶ");
					m_cmb_getIOFrom.InsertString(3,L"ʹ�ܱ�ʶ��ȡ����");
					break;
				default:
					break;
				}
				m_cmb_getIOFrom.SetCurSel(0);
			}
		}
		else
		{
			MessageBox(L"�������������");
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------
	}
	if(columnofchart==4)
	{
		rc.left+=21;
		rc.right+=17;
		rc.top+=35;
		rc.bottom+=35;
		m_cmb_getIOFromport.ShowWindow(SW_SHOW);
		m_cmb_getIOFromport.MoveWindow(&rc);
		m_cmb_getIOFromport.SetFocus();

		m_edit_getIOName.ShowWindow(SW_HIDE);
		m_cmb_getIOType.ShowWindow(SW_HIDE);
		m_cmb_IorO.ShowWindow(SW_HIDE);
		m_cmb_getIOPort.ShowWindow(SW_HIDE);
		m_cmb_getIOFrom.ShowWindow(SW_HIDE);
		//------------------��ʼ��m_cmb_getIOFromport�ؼ�--------------------------------------------------------------------------------------------
		if(m_List_IOput.GetItemText(rowofchart,0)!="" && arryIOputData.size()>rowofchart)
		{
			m_cmb_getIOFromport.ResetContent();                                   // ���������
			if(arryIOputData.at(rowofchart)->IOType == "input")                   // ���Ϊ��input����������ǩ�����Ѵ����Ŀ���ģ���趨
			{
				if(arryIOputData.at(rowofchart)->FromModel == p_Model->m_caption) //���ģ�͵�������Դ��ģ������
				{
					int n=0;
					for(vector<IOValue*>::iterator iter = this->arryIOputData.begin();iter!= this->arryIOputData.end();iter++)
					{
						if((**iter).IOType == "output")
						{
							//CString str2;
							//str2.Format(L"%d",n+1);
							//m_cmb_getIOFromport.InsertString(n,str2);
							m_cmb_getIOFromport.InsertString(n,(**iter).Name);
							n++;
						}
					}
				}
				else                                                             // ���ģ�͵�������Դ����ģ������������������
				{
					for(vector<CPlace*>::iterator iter=m_pDoc->Vector_Place.begin();iter!=m_pDoc->Vector_Place.end();iter++)
					{
						int n = 0;
						if((**iter).m_caption == /*str_IOFromcaption*/arryIOputData.at(rowofchart)->FromModel)   //���caption��ѡ�е�Դģ�͵�����һ�£��������ά����arryOutputData
						{
							for(vector<IOValue*>::iterator iter1 = (**iter).arryIOputData.begin();iter1 != (**iter).arryIOputData.end();iter1++)
							{
								if((**iter1).IOType == "output")               //Ѱ������Ϊ��output���ı�������¼�����
								{
									//CString str2;
									//str2.Format(L"%d",n+1);
									//m_cmb_getIOFromport.InsertString(n,str2);
									
									m_cmb_getIOFromport.InsertString(n,(**iter1).Name);
									n++;
								}
							}
						}
					}
				}
				m_cmb_getIOFromport.SetCurSel(0);
			}
			
			if(arryIOputData.at(rowofchart)->IOType == "output") // ���Ϊ��output����������ǩ���ݿ���ģ����ά���ı����趨
			{
				m_cmb_getIOFromport.ShowWindow(SW_HIDE);
				m_List_IOput.SetItemText(m_row,m_column,L"��������");
				
			}
		}
		else
		{
			MessageBox(L"�������������");
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------
	}
}


void CTabPropSet2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

// ���ƿؼ�ȫ����ʾ������
void CTabPropSet2::SetCtrlStatus(UINT status)
{
	m_edit_getIOName.ShowWindow(status);// �����ֵ��������ÿؼ����ɼ�
	m_cmb_getIOType.ShowWindow(status);
	m_cmb_IorO.ShowWindow(status);
	m_cmb_getIOPort.ShowWindow(status);
	m_cmb_getIOFrom.ShowWindow(status);
	m_cmb_getIOFromport.ShowWindow(status);
}

// ��ӱ���
void CTabPropSet2::OnBtnaAddValue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n = arryIOputData.size(); // ��ȡ���б����ĸ���
	m_List_IOput.InsertItem(n,_T(""));
	m_List_IOput.SetItemText(n,0,_T("name"));
	m_List_IOput.SetItemText(n,1,_T("0"));
	m_List_IOput.SetItemText(n,2,_T("output"));
	m_List_IOput.SetItemText(n,3,_T("������ʶ"));
	m_List_IOput.SetItemText(n,4,_T("��������"));

	IOValue *value = new IOValue();

	value->Name = _T("name");
	value->Sequence = 0;
	value->IOType = _T("output");
	value->FromModel = _T("������ʶ");
	arryIOputData.push_back(value);
}

// ɾ������
void CTabPropSet2::OnBtnDltValue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_rowforD/m_columnforD
	if(m_rowforD != -1)
	{
		vector<IOValue*>::iterator iter = arryIOputData.begin()+m_rowforD;
		delete (*iter);//arryIOputData.at(m_rowforD);
		arryIOputData.erase(iter);

		

		// �����б����ʾ
		m_List_IOput.DeleteItem(m_rowforD);
		/*int nsize = arryIOputData.size();
		for(int i = m_rowforD;i<nsize;i++)
		{
			m_List_IOput.DeleteColumn(i);
		}
		for(int i = m_rowforD;i<nsize-1;i++)
		{}*/
		
		m_rowforD = -1;
	}


}


void CTabPropSet2::OnLvnItemchangedListIoput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
