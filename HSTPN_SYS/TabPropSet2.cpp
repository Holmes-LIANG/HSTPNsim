// TabPropSet2.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "TabPropSet2.h"
//#include "afxdialogex.h"

#include "MainFrm.h"
#include "HSTPN_SYSView.h"

// CTabPropSet2 对话框

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
	// TODO: 在此添加专用代码和/或调用基类

	this->PlaceStyle = placestyle;   //对应库所对象的类型
	this->p_Model    = pModel;       //对应的库所对象的地址指针

	return CDialog::Create(nIDTemplate, pParentWnd);
}

BOOL CTabPropSet2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//-------------------------------------------------------------------------输入/输出表格设置(初始化）-----------Begin--------------------------
	m_List_IOput.ModifyStyle(0,LVS_REPORT);               // 设置为报表模式
	m_List_IOput.SetExtendedStyle(m_List_IOput.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// 间隔线+行选中

	m_List_IOput.InsertColumn(0,L"变量名");               // 插入表头
	//m_List_IOput.InsertColumn(1,L"类型"); 
	m_List_IOput.InsertColumn(1,L"序列");
	m_List_IOput.InsertColumn(2,L"I/O");
	m_List_IOput.InsertColumn(3,L"数据源");
	m_List_IOput.InsertColumn(4,L"数据源变量");
	m_List_IOput.SetBkColor(RGB(255,255,255));

/*	for(int n=0;n<20;n++)
	{
		m_List_IOput.InsertItem(n,L"");
	}
	*/
	CRect rect;  
	m_List_IOput.GetClientRect(rect);                     //获得当前客户区信息   
	m_List_IOput.SetColumnWidth(0, rect.Width() / 5);     //设置列的宽度。   
	m_List_IOput.SetColumnWidth(1, rect.Width() / 5); 
	m_List_IOput.SetColumnWidth(2, rect.Width() / 5);
	m_List_IOput.SetColumnWidth(3, rect.Width() / 5);
	m_List_IOput.SetColumnWidth(4, rect.Width() / 5);
	//-------------------------------------------------------------------------输入/输出表格设置(初始化）-----------End----------------------------

	//------------------------------------------------------设置输入、选择控件的初始状态为不可见，设置上述部分控件的内容----------Begin------------
	m_edit_getIOName.ShowWindow(SW_HIDE);                // 初始状态均为隐藏状态
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
	//------------------------------------------------------设置输入、选择控件的初始状态为不可见，设置上述部分控件的内容----------End--------------

	//------------------------------------------------------当再次点击模型打开对话框时，使保存的输入/输出变量再次显示在表格中-----Begin------------
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
			m_List_IOput.SetItemText(count_row,4,L"无需设置");
		}
		count_row++;
	}
	//------------------------------------------------------当再次点击模型打开对话框时，使保存的输入/输出变量再次显示在表格中-----End--------------
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTabPropSet2::OnDblclk_ListIoput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
    if(pNMListView->iItem==-1)
	{
		return;
	}

    m_row=pNMListView->iItem;                                                             // m_row为被选中行的行序号（int类型成员变量）
    m_column=pNMListView->iSubItem;                                                       // m_column为被选中行的列序号（int类型成员变量）

	UpdateData(TRUE);
	m_List_IOput.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc); // 获取被点击子项的矩形位置和大小
	
	if(m_column == 0)                                                                     // 如果被双击的单元格是第一列，即为IOname，则先获取单元格中的值，
	{                                                                                     // 并设置到edit控件中，目的是，解决“当再次双击第一列时，
		CString str = m_List_IOput.GetItemText(m_row,m_column);                           // 如果不再次输入变量名，则原来的变量名也会消失。”的现象
		m_edit_getIOName.SetWindowText(str);
	}

	InitCtrl_GetIO(m_row, m_column, rc);                                                  // 根据双击的行和列，显示相应控件到单元格位置,其他控件隐藏

	*pResult = 0;
}

// 获取IOname
void CTabPropSet2::OnClick_ListIoput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
    if(pNMListView->iItem==-1)
	{
		return;
	}

    m_rowforD=pNMListView->iItem;                                                             // m_row为被选中行的行序号（int类型成员变量）
    m_columnforD=pNMListView->iSubItem;                                                       // m_column为被选中行的列序号（int类型成员变量）



	//-----------------------------------获取并保存IOname-------------------------------------------------------------
	if(m_column == 0)                                   // m_column==0表示上次被双击的是获取IOname的编辑框，因此，去获取该编辑框的值；否则，不获取
	{
		UpdateData();                                   //完成控件与变量的数据交换，从而获取m_editvalue_IOname值。

		SetCtrlStatus(SW_HIDE);                         // 设置控件不可见

		if(arryIOputData.size()>m_row)                  //如果不是新建，而是改变已有变量，则在相应位置之间改变变量属性
		{	
			m_List_IOput.SetItemText(m_row,m_column,m_editvalue_IOname);

			arryIOputData.at(m_row)->Name = m_editvalue_IOname;
		}
		else
		{
		/*	if(arryIOputData.size()==m_row)            // 如果该变量在表格中时新的一行,则在最后插入一个变量；
			{
				m_List_IOput.SetItemText(m_row,m_column,m_editvalue_IOname);

				IOValue* value = new IOValue();
				value->Name = m_editvalue_IOname;
				vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
				arryIOputData.insert(iter,1,value);
			}
			else
			{
				MessageBox(L"数据表有空行！");
			}*/
		}
	}
	else
	{
		SetCtrlStatus(SW_HIDE);                         //设置控件不可见
	}
	//-------------------------------------------------------------------------------------------------
	*pResult = 0;
}


// 获取DataType
void CTabPropSet2::OnCmbCSelchange_Getiotype()// change for the function to get Sequence
{
	// TODO: 在此添加控件通知处理程序代码
	//int nSel = m_cmb_getIOType.GetCurSel();  
	//CString str;
	
	/*switch (nSel)                                   // 获取用户选择
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
	
	SetCtrlStatus(SW_HIDE);                         // 完成数值插入后，设置控件不可见

	if(arryIOputData.size()>m_row)                  // 如果该变量在表格中时新的一行,则在最后插入一个变量；如果不是，则在相应位置之间改变变量属性
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
			MessageBox(L"数据表有空行！");
		}
	}*/
	int nSel = m_cmb_getIOType.GetCurSel(); 
	m_cmb_getIOType.SetCurSel(nSel);
	CString str; 
	m_cmb_getIOType.GetWindowTextW(str);

	SetCtrlStatus(SW_HIDE);

	if(arryIOputData.size()>m_row)                  // 如果该变量在表格中时新的一行,则在最后插入一个变量；如果不是，则在相应位置之间改变变量属性
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
			value->Sequence = _ttoi(str);//将字符串转换成整形，从数字或正负号开始转换，一直到非数字为止
			vector<IOValue*>::iterator iter = arryIOputData.begin()+m_row;
			arryIOputData.insert(iter,1,value);
		}
		else
		{
			MessageBox(L"数据表有空行！");
		}
	}
}

// 获取IorO   同时将每个变量的Value值初始化为0
//选择"input"or"output"
void CTabPropSet2::OnCmbSelchange_Getioro()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_cmb_IorO.GetCurSel();  
	CString str;
	switch (nSel)                            // 获取用户选择
	{
	case 0:                                  //标签为“input”
		str = "input";
		break;
	case 1:                                  //标签为“output”
		str = "output";
		break;
	default:
		break;
	}
	
	SetCtrlStatus(SW_HIDE);                 // 完成数值插入后，设置控件不可见

	if(arryIOputData.size()>m_row)          // 如果该变量在表格中时新的一行,则在最后插入一个变量；如果不是，则在相应位置之间改变变量属性
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
			MessageBox(L"数据表有空行！");
		}
	}
}

// 获取IOPort
void CTabPropSet2::OnCmbSelchange_Getioport()
{
	// TODO: 在此添加控件通知处理程序代码
/*	int nSel = m_cmb_getIOPort.GetCurSel();  
	CString str;
	m_cmb_getIOPort.SetCurSel(nSel);              // 先设置选中项为当前项
	m_cmb_getIOPort.GetWindowText(str);           // 然后获取当前项的内容

	SetCtrlStatus(SW_HIDE);                       // 完成数值插入后，设置控件不可见

	int port = _ttoi(str);                        //将CString对象转换成int对象

	if(arryIOputData.size()>m_row)                // 如果该变量在表格中时新的一行,则在最后插入一个变量；如果不是，则在相应位置之间改变变量属性
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
			MessageBox(L"数据表有空行！");
		}
	}
*/
}

// 获取IOFrom
//选择数据源后
void CTabPropSet2::OnCmbSelchange_Getiofrom()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_cmb_getIOFrom.GetCurSel();  
	m_cmb_getIOFrom.SetCurSel(nSel);                     // 先设置选中项为当前项
	m_cmb_getIOFrom.GetWindowText(str_IOFromcaption);    // 然后获取当前项的内容

	SetCtrlStatus(SW_HIDE);                              // 完成数值插入后，设置控件不可见

	if(arryIOputData.size()>m_row)                       // 如果该变量在表格中时新的一行,则在最后插入一个变量；如果不是，则在相应位置之间改变变量属性
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
			MessageBox(L"数据表有空行！");
		}
	}
}

// 获取IOFromPort
void CTabPropSet2::OnCmbSelchng_Getiofromport()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_cmb_getIOFromport.GetCurSel();
	CString str;
	m_cmb_getIOFromport.SetCurSel(nSel);
	m_cmb_getIOFromport.GetWindowText(str);

	SetCtrlStatus(SW_HIDE);                              // 完成数值插入后，设置控件不可见

//	int port = _ttoi(str);                               // 将CString对象转换为int对象

	if(arryIOputData.size()>m_row)                       // 如果该变量在表格中时新的一行,则在最后插入一个变量；如果不是，则在相应位置之间改变变量属性
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
			MessageBox(L"数据表有空行！");
		}
	}
}

// 某些控件的初始化
void CTabPropSet2::InitCtrl_GetIO(int rowofchart, int columnofchart, CRect rc)
{

	if(columnofchart==0)                                 // 如果双击的是第一列 name
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
	if(columnofchart==1)                                 // 如果双击的是第二列 IOType
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
	if(columnofchart==2)                                 // 如果双击的是第三列 IorO
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
	if(columnofchart==3)                                 // 如果双击的是第四列 IOFrom
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
		//----------------初始化m_cmb_getIOFrom控件--------------------------------------------------------------------------------------------------
		if(m_List_IOput.GetItemText(rowofchart,0)!="" && arryIOputData.size()>rowofchart)
		{
			if(arryIOputData.at(rowofchart)->IOType == "input")   // 如果为“input”，下拉标签根据已创建的库所模型设定
			{
				int i = 0;
				m_cmb_getIOFrom.ResetContent();                   //清空所有项
				// 获取View类中保存的Vector_Place集合
				
				for(vector<CPlace*>::iterator iter = m_pDoc->Vector_Place.begin();iter!=m_pDoc->Vector_Place.end();iter++)//先获取Frame实例，再获取View实例	 
				{
					CString str = (**iter).m_caption;             //获取已创建的库所的标题
					m_cmb_getIOFrom.InsertString(i,str);
					i++;
				}
				m_cmb_getIOFrom.SetCurSel(0);
			}
			if(arryIOputData.at(rowofchart)->IOType == "output")  // 如果为“output”，下拉标签根据库所模型中维护的变量设定
			{
				m_cmb_getIOFrom.ResetContent();//清空所有项
				CString strofvalue[56] = {L"布尔标识",L"仿真步长"/*,L"时钟t"*/,L"参数1",L"参数2",L"参数3",L"参数4",L"参数5",L"参数6",L"参数7",L"参数8",L"参数9",L"参数10",
					                                                           L"参数11",L"参数12",L"参数13",L"参数14",L"参数15",L"参数16",L"参数17",L"参数18",L"参数19",L"参数20",
																			   L"参数21",L"参数22",L"参数23",L"参数24",L"参数25",L"参数26",L"参数27",L"参数28",L"参数29",L"参数30",
																			   L"参数31",L"参数32",L"参数33",L"参数34",L"参数35",L"参数36",L"参数37",L"参数38",L"参数39",L"参数40",
																			   L"参数41",L"参数42",L"参数43",L"参数44",L"参数45",L"参数46",L"参数47",L"参数48",L"参数49",L"参数50",
																			   L"激活标识",L"激活标识（取反）",L"使能标识",L"使能标识（取反）"};
				CString strofvalueforRand[56] = {L"布尔标识",L"概率阈值"/*,L"时钟t"*/,L"参数1",L"参数2",L"参数3",L"参数4",L"参数5",L"参数6",L"参数7",L"参数8",L"参数9",L"参数10",
					                                                           L"参数11",L"参数12",L"参数13",L"参数14",L"参数15",L"参数16",L"参数17",L"参数18",L"参数19",L"参数20",
																			   L"参数21",L"参数22",L"参数23",L"参数24",L"参数25",L"参数26",L"参数27",L"参数28",L"参数29",L"参数30",
																			   L"参数31",L"参数32",L"参数33",L"参数34",L"参数35",L"参数36",L"参数37",L"参数38",L"参数39",L"参数40",
																			   L"参数41",L"参数42",L"参数43",L"参数44",L"参数45",L"参数46",L"参数47",L"参数48",L"参数49",L"参数50",
																			   L"激活标识",L"激活标识（取反）",L"使能标识",L"使能标识（取反）"};
				int iofvalue = 0;
				int iofvalueforRand = 0;
				switch(this->PlaceStyle)
				{
				case STYLE_DISCRETE:
					m_cmb_getIOFrom.InsertString(0,L"激活标识");
					m_cmb_getIOFrom.InsertString(1,L"激活标识（取反）");
					m_cmb_getIOFrom.InsertString(2,L"使能标识");
					m_cmb_getIOFrom.InsertString(3,L"使能标识（取反）");
					break;
				case STYLE_CNTNUS:
					for(vector<IOValue*>::iterator iter = this->arryIOputData.begin();iter!=this->arryIOputData.end();iter++)
					{
						if((**iter).IOType == "output" && ((**iter).FromModel=="布尔标识" || (**iter).FromModel=="仿真步长"  
							|| (**iter).FromModel=="参数1" || (**iter).FromModel=="参数2" || (**iter).FromModel=="参数3" || (**iter).FromModel=="参数4" 
							|| (**iter).FromModel=="参数5" || (**iter).FromModel=="参数6" || (**iter).FromModel=="参数7" || (**iter).FromModel=="参数8" 
							|| (**iter).FromModel=="参数9" || (**iter).FromModel=="参数10"|| (**iter).FromModel=="参数11" || (**iter).FromModel=="参数12" 
							|| (**iter).FromModel=="参数13" || (**iter).FromModel=="参数14" || (**iter).FromModel=="参数15" || (**iter).FromModel=="参数16" 
							|| (**iter).FromModel=="参数17" || (**iter).FromModel=="参数18"|| (**iter).FromModel=="参数19" || (**iter).FromModel=="参数20"
							|| (**iter).FromModel=="参数21" || (**iter).FromModel=="参数22"|| (**iter).FromModel=="参数23" || (**iter).FromModel=="参数24"
							|| (**iter).FromModel=="参数25" || (**iter).FromModel=="参数26"|| (**iter).FromModel=="参数27" || (**iter).FromModel=="参数28"
							|| (**iter).FromModel=="参数29" || (**iter).FromModel=="参数30"|| (**iter).FromModel=="参数31" || (**iter).FromModel=="参数32"
							|| (**iter).FromModel=="参数33" || (**iter).FromModel=="参数34"|| (**iter).FromModel=="参数35" || (**iter).FromModel=="参数36"
							|| (**iter).FromModel=="参数37" || (**iter).FromModel=="参数38"|| (**iter).FromModel=="参数39" || (**iter).FromModel=="参数40"
							|| (**iter).FromModel=="参数41" || (**iter).FromModel=="参数42"|| (**iter).FromModel=="参数43" || (**iter).FromModel=="参数44"
							|| (**iter).FromModel=="参数45" || (**iter).FromModel=="参数46"|| (**iter).FromModel=="参数47" || (**iter).FromModel=="参数48"
							|| (**iter).FromModel=="参数49" || (**iter).FromModel=="参数50"
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
						if((**iter).IOType == "output" && ((**iter).FromModel=="布尔标识" || (**iter).FromModel=="概率阈值" /*仿真步长*/ 
							|| (**iter).FromModel=="参数1" || (**iter).FromModel=="参数2" || (**iter).FromModel=="参数3" || (**iter).FromModel=="参数4" 
							|| (**iter).FromModel=="参数5" || (**iter).FromModel=="参数6" || (**iter).FromModel=="参数7" || (**iter).FromModel=="参数8" 
							|| (**iter).FromModel=="参数9" || (**iter).FromModel=="参数10"|| (**iter).FromModel=="参数11" || (**iter).FromModel=="参数12" 
							|| (**iter).FromModel=="参数13" || (**iter).FromModel=="参数14" || (**iter).FromModel=="参数15" || (**iter).FromModel=="参数16" 
							|| (**iter).FromModel=="参数17" || (**iter).FromModel=="参数18"|| (**iter).FromModel=="参数19" || (**iter).FromModel=="参数20"
							|| (**iter).FromModel=="参数21" || (**iter).FromModel=="参数22"|| (**iter).FromModel=="参数23" || (**iter).FromModel=="参数24"
							|| (**iter).FromModel=="参数25" || (**iter).FromModel=="参数26"|| (**iter).FromModel=="参数27" || (**iter).FromModel=="参数28"
							|| (**iter).FromModel=="参数29" || (**iter).FromModel=="参数30"|| (**iter).FromModel=="参数31" || (**iter).FromModel=="参数32"
							|| (**iter).FromModel=="参数33" || (**iter).FromModel=="参数34"|| (**iter).FromModel=="参数35" || (**iter).FromModel=="参数36"
							|| (**iter).FromModel=="参数37" || (**iter).FromModel=="参数38"|| (**iter).FromModel=="参数39" || (**iter).FromModel=="参数40"
							|| (**iter).FromModel=="参数41" || (**iter).FromModel=="参数42"|| (**iter).FromModel=="参数43" || (**iter).FromModel=="参数44"
							|| (**iter).FromModel=="参数45" || (**iter).FromModel=="参数46"|| (**iter).FromModel=="参数47" || (**iter).FromModel=="参数48"
							|| (**iter).FromModel=="参数49" || (**iter).FromModel=="参数50"
							))
						{
							m_cmb_getIOFrom.InsertString(iofvalueforRand,strofvalueforRand[iofvalueforRand]);
							iofvalueforRand++;
						}
					}
					//m_cmb_getIOFrom.InsertString(iofvalueforRand,L"概率阈值");
					m_cmb_getIOFrom.InsertString(iofvalueforRand,L"激活标识");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+1,L"激活标识（取反）");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+2,L"使能标识");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+3,L"使能标识（取反）");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+4,L"随机结果");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+5,L"随机结果（取反）");
					m_cmb_getIOFrom.InsertString(iofvalueforRand+6,L"随机概率值");
					break;
				case STYLE_TIME:
					m_cmb_getIOFrom.InsertString(0,L"时间阈值");
					m_cmb_getIOFrom.InsertString(1,L"激活标识");
					m_cmb_getIOFrom.InsertString(2,L"激活标识（取反）");
					m_cmb_getIOFrom.InsertString(3,L"使能标识");
					m_cmb_getIOFrom.InsertString(4,L"使能标识（取反）");
					break;
				case STYLE_TCTRL:
					m_cmb_getIOFrom.InsertString(0,L"激活标识");
					m_cmb_getIOFrom.InsertString(1,L"激活标识（取反）");
					m_cmb_getIOFrom.InsertString(2,L"使能标识");
					m_cmb_getIOFrom.InsertString(3,L"使能标识（取反）");
					break;
				case STYLE_LCTRL:
					m_cmb_getIOFrom.InsertString(0,L"激活标识");
					m_cmb_getIOFrom.InsertString(1,L"激活标识（取反）");
					m_cmb_getIOFrom.InsertString(2,L"使能标识");
					m_cmb_getIOFrom.InsertString(3,L"使能标识（取反）");
					break;
				default:
					break;
				}
				m_cmb_getIOFrom.SetCurSel(0);
			}
		}
		else
		{
			MessageBox(L"请输入变量名！");
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
		//------------------初始化m_cmb_getIOFromport控件--------------------------------------------------------------------------------------------
		if(m_List_IOput.GetItemText(rowofchart,0)!="" && arryIOputData.size()>rowofchart)
		{
			m_cmb_getIOFromport.ResetContent();                                   // 清空所有项
			if(arryIOputData.at(rowofchart)->IOType == "input")                   // 如果为“input”，下拉标签根据已创建的库所模型设定
			{
				if(arryIOputData.at(rowofchart)->FromModel == p_Model->m_caption) //如果模型的数据来源是模型自身
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
				else                                                             // 如果模型的数据来源不是模型自身，而是其他库所
				{
					for(vector<CPlace*>::iterator iter=m_pDoc->Vector_Place.begin();iter!=m_pDoc->Vector_Place.end();iter++)
					{
						int n = 0;
						if((**iter).m_caption == /*str_IOFromcaption*/arryIOputData.at(rowofchart)->FromModel)   //如果caption与选中的源模型的名称一致，则遍历其维护的arryOutputData
						{
							for(vector<IOValue*>::iterator iter1 = (**iter).arryIOputData.begin();iter1 != (**iter).arryIOputData.end();iter1++)
							{
								if((**iter1).IOType == "output")               //寻找类型为“output”的变量，记录其个数
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
			
			if(arryIOputData.at(rowofchart)->IOType == "output") // 如果为“output”，下拉标签根据库所模型中维护的变量设定
			{
				m_cmb_getIOFromport.ShowWindow(SW_HIDE);
				m_List_IOput.SetItemText(m_row,m_column,L"无需设置");
				
			}
		}
		else
		{
			MessageBox(L"请输入变量名！");
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------
	}
}


void CTabPropSet2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

// 控制控件全部显示或隐藏
void CTabPropSet2::SetCtrlStatus(UINT status)
{
	m_edit_getIOName.ShowWindow(status);// 完成数值插入后，设置控件不可见
	m_cmb_getIOType.ShowWindow(status);
	m_cmb_IorO.ShowWindow(status);
	m_cmb_getIOPort.ShowWindow(status);
	m_cmb_getIOFrom.ShowWindow(status);
	m_cmb_getIOFromport.ShowWindow(status);
}

// 添加变量
void CTabPropSet2::OnBtnaAddValue()
{
	// TODO: 在此添加控件通知处理程序代码
	int n = arryIOputData.size(); // 获取已有变量的个数
	m_List_IOput.InsertItem(n,_T(""));
	m_List_IOput.SetItemText(n,0,_T("name"));
	m_List_IOput.SetItemText(n,1,_T("0"));
	m_List_IOput.SetItemText(n,2,_T("output"));
	m_List_IOput.SetItemText(n,3,_T("布尔标识"));
	m_List_IOput.SetItemText(n,4,_T("无需设置"));

	IOValue *value = new IOValue();

	value->Name = _T("name");
	value->Sequence = 0;
	value->IOType = _T("output");
	value->FromModel = _T("布尔标识");
	arryIOputData.push_back(value);
}

// 删除变量
void CTabPropSet2::OnBtnDltValue()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_rowforD/m_columnforD
	if(m_rowforD != -1)
	{
		vector<IOValue*>::iterator iter = arryIOputData.begin()+m_rowforD;
		delete (*iter);//arryIOputData.at(m_rowforD);
		arryIOputData.erase(iter);

		

		// 更新列表的显示
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
