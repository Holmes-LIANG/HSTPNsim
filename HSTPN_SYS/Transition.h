#pragma once
/******������Ҫ�����ͷ�ļ�******/
#include "afxcoll.h"
//���������õ���ͷ�ļ���std�����ռ�
#include <vector>
using namespace std;

class CPlace;
#include "Place.h"
class CArrow;
#include "Arrow.h"
// CTransition

class CTransition : public CStatic
{
	DECLARE_DYNAMIC(CTransition)

public:
	CTransition();
	virtual ~CTransition();

	CTransition(const CTransition &C);
	CTransition CTransition::operator = (const CTransition &A);

protected:
	DECLARE_MESSAGE_MAP()
public:
	//===========��Ա��������================================
	UINT            u_SubscriptofTrans;   // ÿ��ģ�Ͷ���һ���±꣬��View����Vector_Trans�����еı������±��Ӧ���൱��ID
	bool            bFire;                // �Ƿ����㷢�������ı�ʶ

	vector<CPlace*> arryTransPre;         // ��Ǩ��ǰ�ÿ������ɵ��������
	vector<CPlace*> arryTransPst;         // ��Ǩ�ĺ��ÿ������ɵ��������
	vector<CArrow*> arryArrowPre;
	vector<CArrow*> arryArrowPst;         //������ǰ�����򻡼���

	CPoint          pt1;                  //��Ǩģ�͵��������ӵ�
	CPoint          pt2;
	CPoint          pt3;
	CPoint          pt4;

	CString         m_caption;           // ��Ǩ������

	CPoint                 Position;     // ��¼��Ǩģ������ͼ���е�λ��
//	vector<CPlace> arryTransPreObjct;         // ��Ǩ��ǰ�ÿ������ɵ��������������Ķ������������ǵ�ַ
//	vector<CPlace> arryTransPstObjct;         // ��Ǩ�ĺ��ÿ������ɵ��������

public:
	//==========��Ա��������=================================
	bool isFire();                       // ������Ǩ��������ķ���

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMdDelete2();
};


