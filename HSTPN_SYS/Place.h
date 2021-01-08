#pragma once

#include "HSTPN_Message.h"
/******包含必要的类和头文件******/
#include "afxcoll.h"
#include "afxtempl.h"
//向量容器头文件及std命名空间
#include <vector>
using namespace std;
class CTransition;
#include "Transition.h"
class CArrow;
#include "Arrow.h"

#include <random>
#include <time.h>
#include <iomanip>//设置精度
#include "HSTPN_SYSView.h"
//用于socket通信的头文件
#include<stdio.h>
#include<WinSock2.h>
class CHSTPN_SYSView;

//---------------与Lua交互需要连接的lua头文件、连接库--------
extern "C"
{
	#include "lua5.3_Src\src\lua.h"
	#include "lua5.3_Src\src\lauxlib.h"
	#include "lua5.3_Src\src\lualib.h"
}
#pragma comment(lib,"Lua5.3_Interpreter_Translater\\LUA53.lib")
//-----------------------------------------------------------
#pragma comment(lib, "ws2_32.lib")



class CPlace : public CStatic
{
	DECLARE_DYNAMIC(CPlace)

public:
	CPlace();
	virtual ~CPlace();

	CPlace(const CPlace &C);
	CPlace CPlace::operator = (const CPlace &A);

protected:
	DECLARE_MESSAGE_MAP()

public:
	//====================成员变量声明=======================================
	UINT                   u_SubscriptofModel;   // 每个模型都有一个下标，与View类中Vector_Place向量中的变量的下标对应,相当于模型的ID
	bool                   bActivation;          // 描述库所是否满足激活条件的变量，满足为true，不满足为false
	bool                   bEnable;              // 判断库所是否满足使能条件的变量，使能为ture，不满足为false
	enumPlaceStyle         PlaceStyle;           //库所类型
	CString                m_caption;            // 模型的标题 ，如“p1”

	struct                 result
	{
		bool               bcanEnable;
		vector<int>        res;

	};

	vector<int>            disic;				 //多任务决策的数值
	int                    count;                //策略标识的数量,用于计数
	int                    numTag;               //决策库所策略标识的数量
	vector<CTransition*>   arryPlacePre;         //库所的前置变迁
	vector<CTransition*>   arryPlacePst;         //库所的后置变迁
	vector<CArrow*>        arryArrowPre;
	vector<CArrow*>        arryArrowPst;         //库所的前后有向弧集合

	
	CPoint                 pt1;                  //在库所模型中维护4个连接点
	CPoint                 pt2;
	CPoint                 pt3;
	CPoint                 pt4;
	
	double                 dProblty;             // 随机库所的“概率阈值”
	double                 dTime;                // 时延库所的“时间阈值”
	
	CPoint                 Position;             // 记录库所模型在视图类中的位置

	bool                   bisAssigned;          // updateiovalueofmodel 函数中用到的，目的是：当p1输入值收到p2输出的影响时，我们希望p2状态演化完成后，即p2激活后再进行赋值，因此
	                                             // 定义一个bisAssigned，初始化为false，当该库所bActivation值曾被置为true时，bisAssigned赋值为true
	bool                   bisworked;
	vector<IOValue*>       arryIOputData;        //模型的输入/输出值的集合
	vector<InValue>        arryInputData;        //模型的输入变量值集合
	vector<IOValue*>       arryInputofLua;       // 保存Lua文件中状态推演函数的输入
	vector<IOValue*>       arryOutputofLua;

	bool				   bisUserDef_prob;		 // 用于记录该模型随机阈值的获取方式是否为“自定义”，是为true
	bool				   bisUserDef_time;      // 用于记录该模型时间阈值的获取方式是否为“自定义”，是为ture
	bool				   bisUserDef_ctrl;      // 用于记录该模型控制库所激活值的获取方式是否为“自定义”，是为true 

	CString                str_Inputport_prob;   // 用于记录该模型随机库所输入值的端口，当用户选择“来自输入值”时（即当bisUserDef_prob=false时）
	CString                str_Inputport_time;   // 用于记录该模型时间库所输入值的端口，当用户选择“来自输入值”时（即当bisUserDef_time=false时）
	CString                str_Inputport_ctrl;   // 用于记录该模型控制库所输入值的端口，当用户选择“来自输入值”时（即当bisUserDef_ctrl=false时）

	

	CPlace *               m_thisModel;          // 保存库所模型对象的地址

	CString                Lua_fileName;         // 连续库所关联的lua文件的名字或路径

	double                 d_TimeStep;           // 接收用户设置的仿真步长

	bool                   b_ModelFocus;         // 库所处于被选中状态

	//int                    i_RandOutcome;      // 随机库所产生的结果0或1
	bool                    i_RandOutcome;
	int                     rand_tag;
	double                  d_RandProblty;       // 记录产生的随机数

	double                  d_TimeTS;            // 用于保存“时间阈值”，作为一个备份，在库所使能后，赋值为d_Time
	CHSTPN_SYSView       *  m_pParentView;       // 保存View类的指针

	int                     CtrlPlaceStyle;      // 定义控制库所显示为突控型还是拦控型
	bool                    isTimeUpDownChange; // 判断时间的上界或下届是否改变，如果改变则需要重新产生随机分布
	bool                    ischoseSocket;          //表示该库所是否激活通信接口
	string                  ipadrs;              //通信IP地址
	int                     port;                //通信端口
	int                     num_output;
    int                     num_input;
	string                  msgO;                 //输出消息内容
	string                  msgI;                //输入消息内容
public:
	//====================成员函数声明=======================================
	bool                  isEnable(double timestep = 0);           // 描述库所使能规则的方法

	bool                  runLuaFunction(double timestep);     // 执行连续库所中的Lua函数，如果函数顺利被推演，则返回true，否则返回false

	int               runSocketFunction(string ipadrs, int port);
public:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,enumPlaceStyle PlaceStyle,  UINT nID = 0xffff);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMdDelete();
	afx_msg void OnRotateTl();
	afx_msg void OnShowpropdlg();

	double dTime_downline;// 时间区间的下限
	double dTime_upline;// 时间区间的上限
	bool ischoseupdown;
	bool b_resettime;// 控制仅时延库所激活时，可以产生一个时延阈值

	bool ischoseProgRand;//临时加入一个变量，表示是否选择用户自己编程得到随机域值
	bool runS_LuaFunction(double timestep);
	result * runD_LuaFunction(double timestep);
};


