#pragma once
#include "afxcmn.h"

#include "TabPropSet1.h"
#include "TabPropSet2.h"

#include "HSTPN_Message.h"

#include "Place.h"
// CPlacePropSetDlg �Ի���

class CPlacePropSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlacePropSetDlg)

public:
	CPlacePropSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CPlacePropSetDlg(enumPlaceStyle placestyle,CWnd* pParent =NULL);   //�Զ���һ�����캯��
	virtual ~CPlacePropSetDlg();

// �Ի�������
	enum { IDD = IDD_PROPSET_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	CTabCtrl m_TabPropset; // ģ���������öԻ����е���������Tab�ؼ�����
	CTabPropSet1 m_tabPropset1;// Tab�ؼ��ĵ�һ����ǩҳ
	CTabPropSet2 m_tabPropset2;// Tab�ؼ��ĵڶ�����ǩҳ
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchange_TabPropset(NMHDR *pNMHDR, LRESULT *pResult);

	enumPlaceStyle PlaceStyle;// ά��һ��ģ�����͵�ö�ٱ����������ڹ���Ի������ʱ����ʼ���������ж����ĸ�ģ�͵Ĳ�����������
	CPlace * p_Parent;        //��ʽ�ı����丸ָ��
};
