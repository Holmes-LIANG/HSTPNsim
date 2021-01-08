#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "HSTPN_Message.h"
//向量容器头文件及std命名空间
#include <vector>
using namespace std;

// CTabPropSet2 对话框
#include "Place.h"
#include "HSTPN_SYSDoc.h"
class CTabPropSet2 : public CDialog
{
	DECLARE_DYNAMIC(CTabPropSet2)

public:
	CTabPropSet2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPropSet2();

// 对话框数据
	enum { IDD = IDD_PROPSET_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CHSTPN_SYSDoc *m_pDoc;
	//===========成员变量=====================================================
	enumPlaceStyle      PlaceStyle; // 当前库所的类型

	CListCtrl           m_List_IOput;           // 模型的输入输出值列表控件变量
	CEdit               m_edit_getIOName;       // 输入/输出变量的获取编辑框的控件变量
	CComboBox           m_cmb_getIOType;        // 输入/输出变量的类型获取的Combo框的控件变量
	CComboBox           m_cmb_IorO;             // 设置变量是“输入”还是“输出”的类型设置的Combo框的控件变量
	CComboBox           m_cmb_getIOPort;        // 获取输入/输出值的传输端口号
	CComboBox           m_cmb_getIOFrom;        // 获取输入/输出值是从哪个模型传递过来的Comob框的控件变量
	CComboBox           m_cmb_getIOFromport;    // 获取传递过来模型的传递端口的Comob框的控件变量

	int                 m_row;                  // m_row为m_List_IOput的行号
    int                 m_column;               // m_column为m_List_IOput的列号

	int                 m_rowforD;              // m_row为m_List_IOput的行号,删除操作时用的
    int                 m_columnforD;           // m_column为m_List_IOput的列号,删除操作时用的

	vector<IOValue*>    arryIOputData;          // 模型输入/输出值的集合

	CString             str_IOFromcaption;      // 模型输入变量来源模型的caption
	CString             m_editvalue_IOname;     // 输入输出变量的name--控件变量

	CPlace *            p_Model;                // 显式的保存对应Model对象的指针

public:
	//============成员函数==========================================================
	virtual BOOL Create(UINT nIDTemplate, enumPlaceStyle placestyle, CPlace* pModel,  CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();

	afx_msg void OnDblclk_ListIoput(NMHDR *pNMHDR, LRESULT *pResult); // Double Clicked -- to get the row and column
	afx_msg void OnClick_ListIoput(NMHDR *pNMHDR, LRESULT *pResult);  // Single Clicked -- to set the name of value
	afx_msg void OnCmbCSelchange_Getiotype();                         // to get the type of value;
	afx_msg void OnCmbSelchange_Getioro();                            // to get the In or Out of value
	afx_msg void OnCmbSelchange_Getioport();
	afx_msg void OnCmbSelchange_Getiofrom();
	afx_msg void OnCmbSelchng_Getiofromport();

	
	afx_msg void OnPaint();

	void         InitCtrl_GetIO(int rowofchart, int columnofchart, CRect rc); //相关控件的初始化

	void         SetCtrlStatus(UINT status);
	afx_msg void OnBtnaAddValue();
	afx_msg void OnBtnDltValue();
	afx_msg void OnLvnItemchangedListIoput(NMHDR *pNMHDR, LRESULT *pResult);
};
