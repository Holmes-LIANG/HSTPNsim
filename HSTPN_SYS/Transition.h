#pragma once
/******声明必要的类和头文件******/
#include "afxcoll.h"
//向量容器用到的头文件和std命名空间
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
	//===========成员变量声明================================
	UINT            u_SubscriptofTrans;   // 每个模型都有一个下标，与View类中Vector_Trans向量中的变量的下标对应，相当于ID
	bool            bFire;                // 是否满足发生条件的标识

	vector<CPlace*> arryTransPre;         // 变迁的前置库所构成的数组变量
	vector<CPlace*> arryTransPst;         // 变迁的后置库所构成的数组变量
	vector<CArrow*> arryArrowPre;
	vector<CArrow*> arryArrowPst;         //库所的前后有向弧集合

	CPoint          pt1;                  //变迁模型的两个连接点
	CPoint          pt2;
	CPoint          pt3;
	CPoint          pt4;

	CString         m_caption;           // 变迁的名字

	CPoint                 Position;     // 记录变迁模型在视图类中的位置
//	vector<CPlace> arryTransPreObjct;         // 变迁的前置库所构成的数组变量，保存的对象自身，而不是地址
//	vector<CPlace> arryTransPstObjct;         // 变迁的后置库所构成的数组变量

public:
	//==========成员函数声明=================================
	bool isFire();                       // 描述变迁发生规则的方法

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMdDelete2();
};


