#pragma once
#include "afxcmn.h"

#include "TabPropSet1.h"
#include "TabPropSet2.h"

#include "HSTPN_Message.h"

#include "Place.h"
// CPlacePropSetDlg 对话框

class CPlacePropSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlacePropSetDlg)

public:
	CPlacePropSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	CPlacePropSetDlg(enumPlaceStyle placestyle,CWnd* pParent =NULL);   //自定义一个构造函数
	virtual ~CPlacePropSetDlg();

// 对话框数据
	enum { IDD = IDD_PROPSET_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	CTabCtrl m_TabPropset; // 模型属性设置对话框中的属性设置Tab控件变量
	CTabPropSet1 m_tabPropset1;// Tab控件的第一个标签页
	CTabPropSet2 m_tabPropset2;// Tab控件的第二个标签页
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchange_TabPropset(NMHDR *pNMHDR, LRESULT *pResult);

	enumPlaceStyle PlaceStyle;// 维护一个模型类型的枚举变量，用于在构造对话框对象时被初始化，用于判断是哪个模型的参数将被设置
	CPlace * p_Parent;        //显式的保存其父指针
};
