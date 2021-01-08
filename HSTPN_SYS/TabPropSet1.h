#pragma once

#include "HSTPN_Message.h"
#include "afxwin.h"

#include "TabPropSet2.h"
#include "Place.h"
//#include "ModelPropSetDlg.h"
// CTabPropSet1 对话框
#include "HSTPN_SYSDoc.h"
#include "MainFrm.h"
class CTabPropSet1 : public CDialog
{
	DECLARE_DYNAMIC(CTabPropSet1)

public:
	CTabPropSet1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPropSet1();

// 对话框数据
	enum { IDD = IDD_PROPSET_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CHSTPN_SYSDoc * m_pDoc;
	//=========成员变量===============================
	enumPlaceStyle PlaceStyle;           //控件类型，当标签页被创建时传递进来，用于根据不同库所类型设置对应的编辑框的可用性
	CEdit          m_edit_timevalue;     // 时间阈值设置Edit框对应的控件变量	
	CEdit          m_edt_probvalue;      // 概率阈值设置Edit框对应的控件变量
    CEdit          m_edit_ip;            // IP地址设置Edit框对应的控件变量
	CEdit          m_edit_port;          // 端口号设置Edit框对应的控件变量
	CEdit          m_edit_output;        // 通信时输出变量个数
	CComboBox      m_stts_cmb_activ;     // 初始激活状态设置Edit框对应的控件变量
	CComboBox      m_cmb_cntnus;         // 连续库所对应的连续过程的描述-设置的Edit框对应的控件
	CComboBox      m_stts_cmb_prob;      // 概率阈值的获取方式选择的CmoBo框对应的控件变量
	CComboBox      m_stts_cmb_time;      // 时间阈值的获取方式选择的CmoBo框对应的控件变量
	CComboBox      m_stts_cmb_ctrl;      // 控制库所的激活值获取方式的选择Comobox控件变量
	CComboBox      m_stts_cmb_connect;   // 是否使用socket通信的选择Comobox控件变量

	double         m_Value_Probset;      // 用户设定的概率阈值--与控件关联
	double         m_Value_Timeset;      // 用户设定的时间阈值--与控件关联
	
	CButton        m_btn_cntnus_usr;     // 用户自定义按钮对应的控件变量

	bool           bActivation_set;      // 保存用户设置的初始激活状态

	bool           bisUserDef_prob;      // 表示随机阈值的获取方式是否为“自定义”，是为true
	bool           bisUserDef_time;      // 表示时间阈值的获取方式是否为“自定义”，是为ture
	bool           bisUserDef_ctrl;      // 表示控制库所激活值的获取方式是否为“自定义”，是为true 

	CString        str_Inputport_prob;   // 保存随机库所输入值的端口，当用户选择“来自输入值”时（即当bisUserDef_prob=false时）
	CString        str_Inputport_time;   // 保存时间库所输入值的端口，当用户选择“来自输入值”时（即当bisUserDef_time=false时）
	CString        str_Inputport_ctrl;   // 保存控制库所输入值的端口，当用户选择“来自输入值”时（即当bisUserDef_ctrl=false时）
	CComboBox      m_cmb_portctrl;       // 当选择“来自输入值”时，需要通过该combo控件获取输入端口
	CComboBox      m_cmb_porttime;       // 当选择“来自输入值”时，需要从该cmb控件中获取输入端口号
	CComboBox      m_cmb_portprob;       // 当选择“来自输入值”时，需要从该cmb控件中获取输入端口

	CTabPropSet2  *m_tabpropset2;        // 在TabPropSet1中保存一个CTabPropSet2的指针，当两个标签页2被创建时，将第二个标签页的地址赋给该值，在标签页1中就可以通过该地址访问标签页2的变量了

	CPlace *       p_Model;              // 显式的保存对应Model对象的指针
	CString        Lua_fileName;         // 维护一个与CPlace中相同的Lua文件的文件名对象

public:
	virtual BOOL Create(UINT nIDTemplate, enumPlaceStyle placestyle, CPlace* pModel, CWnd* pParentWnd = NULL);
		
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchange_activstts();
	
	
	afx_msg void OnSelchange_probstts();
	afx_msg void OnTextChng_probvalue();
	afx_msg void OnSelchange_timestts();
	afx_msg void OnTextChng_timevalue();
	
	
	afx_msg void OnSelchange_ctrlstts();
	
	
	afx_msg void OnSelchng_inPortprob();
	afx_msg void OnSelchng_inPorttime();
	afx_msg void OnSelchng_inPortctrl();
	afx_msg void OnBtnClckd_CreateorOpenLua();
	
	double       dTime_downline;        // 时间区间的下限
	double       dTime_upline;          // 时间区间的上限
	CEdit        dTime_edt_down;
	CEdit        dTime_edit_up;
	bool         ischoseupdown;         // 临时加入一个变量，表示是否选择updown区间

	bool         ischoseProgRand;       // 临时加入一个变量，表示是否选择用户自己编程得到随机域值
	bool         ischoseSocket;         // 表示是否选择使用socket接口
	afx_msg void OnEnChangeEdtDown();
	afx_msg void OnEnChangeEdtUp();
	afx_msg void OnBtn_RandUser();
	//afx_msg void OnBtn_DisicUser();
	
	CButton      m_btn_usrProgrand;     // 用户自己编程实现随机阈值的按钮
	
	
	
	afx_msg void OnCbnSelchgComboConnect();
	afx_msg void OnEnChgEditIp();
	afx_msg void OnEnChgEditPort();
	CString str_ipadrs;                 // socket连接的ip地址
	int num_port;                       // socket连接的端口号
	int num_output;                     // socket连接中输出的变量个数
	afx_msg void OnEnChangeEditInput();
	afx_msg void OnEnChangeEditOutput();
	CEdit m_edit_input;                 // socket输入变量个数
	int num_input;                      // 输入变量个数
};
