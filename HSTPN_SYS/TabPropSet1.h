#pragma once

#include "HSTPN_Message.h"
#include "afxwin.h"

#include "TabPropSet2.h"
#include "Place.h"
//#include "ModelPropSetDlg.h"
// CTabPropSet1 �Ի���
#include "HSTPN_SYSDoc.h"
#include "MainFrm.h"
class CTabPropSet1 : public CDialog
{
	DECLARE_DYNAMIC(CTabPropSet1)

public:
	CTabPropSet1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPropSet1();

// �Ի�������
	enum { IDD = IDD_PROPSET_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHSTPN_SYSDoc * m_pDoc;
	//=========��Ա����===============================
	enumPlaceStyle PlaceStyle;           //�ؼ����ͣ�����ǩҳ������ʱ���ݽ��������ڸ��ݲ�ͬ�����������ö�Ӧ�ı༭��Ŀ�����
	CEdit          m_edit_timevalue;     // ʱ����ֵ����Edit���Ӧ�Ŀؼ�����	
	CEdit          m_edt_probvalue;      // ������ֵ����Edit���Ӧ�Ŀؼ�����
    CEdit          m_edit_ip;            // IP��ַ����Edit���Ӧ�Ŀؼ�����
	CEdit          m_edit_port;          // �˿ں�����Edit���Ӧ�Ŀؼ�����
	CEdit          m_edit_output;        // ͨ��ʱ�����������
	CComboBox      m_stts_cmb_activ;     // ��ʼ����״̬����Edit���Ӧ�Ŀؼ�����
	CComboBox      m_cmb_cntnus;         // ����������Ӧ���������̵�����-���õ�Edit���Ӧ�Ŀؼ�
	CComboBox      m_stts_cmb_prob;      // ������ֵ�Ļ�ȡ��ʽѡ���CmoBo���Ӧ�Ŀؼ�����
	CComboBox      m_stts_cmb_time;      // ʱ����ֵ�Ļ�ȡ��ʽѡ���CmoBo���Ӧ�Ŀؼ�����
	CComboBox      m_stts_cmb_ctrl;      // ���ƿ����ļ���ֵ��ȡ��ʽ��ѡ��Comobox�ؼ�����
	CComboBox      m_stts_cmb_connect;   // �Ƿ�ʹ��socketͨ�ŵ�ѡ��Comobox�ؼ�����

	double         m_Value_Probset;      // �û��趨�ĸ�����ֵ--��ؼ�����
	double         m_Value_Timeset;      // �û��趨��ʱ����ֵ--��ؼ�����
	
	CButton        m_btn_cntnus_usr;     // �û��Զ��尴ť��Ӧ�Ŀؼ�����

	bool           bActivation_set;      // �����û����õĳ�ʼ����״̬

	bool           bisUserDef_prob;      // ��ʾ�����ֵ�Ļ�ȡ��ʽ�Ƿ�Ϊ���Զ��塱����Ϊtrue
	bool           bisUserDef_time;      // ��ʾʱ����ֵ�Ļ�ȡ��ʽ�Ƿ�Ϊ���Զ��塱����Ϊture
	bool           bisUserDef_ctrl;      // ��ʾ���ƿ�������ֵ�Ļ�ȡ��ʽ�Ƿ�Ϊ���Զ��塱����Ϊtrue 

	CString        str_Inputport_prob;   // ���������������ֵ�Ķ˿ڣ����û�ѡ����������ֵ��ʱ������bisUserDef_prob=falseʱ��
	CString        str_Inputport_time;   // ����ʱ���������ֵ�Ķ˿ڣ����û�ѡ����������ֵ��ʱ������bisUserDef_time=falseʱ��
	CString        str_Inputport_ctrl;   // ������ƿ�������ֵ�Ķ˿ڣ����û�ѡ����������ֵ��ʱ������bisUserDef_ctrl=falseʱ��
	CComboBox      m_cmb_portctrl;       // ��ѡ����������ֵ��ʱ����Ҫͨ����combo�ؼ���ȡ����˿�
	CComboBox      m_cmb_porttime;       // ��ѡ����������ֵ��ʱ����Ҫ�Ӹ�cmb�ؼ��л�ȡ����˿ں�
	CComboBox      m_cmb_portprob;       // ��ѡ����������ֵ��ʱ����Ҫ�Ӹ�cmb�ؼ��л�ȡ����˿�

	CTabPropSet2  *m_tabpropset2;        // ��TabPropSet1�б���һ��CTabPropSet2��ָ�룬��������ǩҳ2������ʱ�����ڶ�����ǩҳ�ĵ�ַ������ֵ���ڱ�ǩҳ1�оͿ���ͨ���õ�ַ���ʱ�ǩҳ2�ı�����

	CPlace *       p_Model;              // ��ʽ�ı����ӦModel�����ָ��
	CString        Lua_fileName;         // ά��һ����CPlace����ͬ��Lua�ļ����ļ�������

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
	
	double       dTime_downline;        // ʱ�����������
	double       dTime_upline;          // ʱ�����������
	CEdit        dTime_edt_down;
	CEdit        dTime_edit_up;
	bool         ischoseupdown;         // ��ʱ����һ����������ʾ�Ƿ�ѡ��updown����

	bool         ischoseProgRand;       // ��ʱ����һ����������ʾ�Ƿ�ѡ���û��Լ���̵õ������ֵ
	bool         ischoseSocket;         // ��ʾ�Ƿ�ѡ��ʹ��socket�ӿ�
	afx_msg void OnEnChangeEdtDown();
	afx_msg void OnEnChangeEdtUp();
	afx_msg void OnBtn_RandUser();
	//afx_msg void OnBtn_DisicUser();
	
	CButton      m_btn_usrProgrand;     // �û��Լ����ʵ�������ֵ�İ�ť
	
	
	
	afx_msg void OnCbnSelchgComboConnect();
	afx_msg void OnEnChgEditIp();
	afx_msg void OnEnChgEditPort();
	CString str_ipadrs;                 // socket���ӵ�ip��ַ
	int num_port;                       // socket���ӵĶ˿ں�
	int num_output;                     // socket����������ı�������
	afx_msg void OnEnChangeEditInput();
	afx_msg void OnEnChangeEditOutput();
	CEdit m_edit_input;                 // socket�����������
	int num_input;                      // �����������
};
