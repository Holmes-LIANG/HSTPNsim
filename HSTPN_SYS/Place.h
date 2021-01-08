#pragma once

#include "HSTPN_Message.h"
/******������Ҫ�����ͷ�ļ�******/
#include "afxcoll.h"
#include "afxtempl.h"
//��������ͷ�ļ���std�����ռ�
#include <vector>
using namespace std;
class CTransition;
#include "Transition.h"
class CArrow;
#include "Arrow.h"

#include <random>
#include <time.h>
#include <iomanip>//���þ���
#include "HSTPN_SYSView.h"
//����socketͨ�ŵ�ͷ�ļ�
#include<stdio.h>
#include<WinSock2.h>
class CHSTPN_SYSView;

//---------------��Lua������Ҫ���ӵ�luaͷ�ļ������ӿ�--------
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
	//====================��Ա��������=======================================
	UINT                   u_SubscriptofModel;   // ÿ��ģ�Ͷ���һ���±꣬��View����Vector_Place�����еı������±��Ӧ,�൱��ģ�͵�ID
	bool                   bActivation;          // ���������Ƿ����㼤�������ı���������Ϊtrue��������Ϊfalse
	bool                   bEnable;              // �жϿ����Ƿ�����ʹ�������ı�����ʹ��Ϊture��������Ϊfalse
	enumPlaceStyle         PlaceStyle;           //��������
	CString                m_caption;            // ģ�͵ı��� ���硰p1��

	struct                 result
	{
		bool               bcanEnable;
		vector<int>        res;

	};

	vector<int>            disic;				 //��������ߵ���ֵ
	int                    count;                //���Ա�ʶ������,���ڼ���
	int                    numTag;               //���߿������Ա�ʶ������
	vector<CTransition*>   arryPlacePre;         //������ǰ�ñ�Ǩ
	vector<CTransition*>   arryPlacePst;         //�����ĺ��ñ�Ǩ
	vector<CArrow*>        arryArrowPre;
	vector<CArrow*>        arryArrowPst;         //������ǰ�����򻡼���

	
	CPoint                 pt1;                  //�ڿ���ģ����ά��4�����ӵ�
	CPoint                 pt2;
	CPoint                 pt3;
	CPoint                 pt4;
	
	double                 dProblty;             // ��������ġ�������ֵ��
	double                 dTime;                // ʱ�ӿ����ġ�ʱ����ֵ��
	
	CPoint                 Position;             // ��¼����ģ������ͼ���е�λ��

	bool                   bisAssigned;          // updateiovalueofmodel �������õ��ģ�Ŀ���ǣ���p1����ֵ�յ�p2�����Ӱ��ʱ������ϣ��p2״̬�ݻ���ɺ󣬼�p2������ٽ��и�ֵ�����
	                                             // ����һ��bisAssigned����ʼ��Ϊfalse�����ÿ���bActivationֵ������Ϊtrueʱ��bisAssigned��ֵΪtrue
	bool                   bisworked;
	vector<IOValue*>       arryIOputData;        //ģ�͵�����/���ֵ�ļ���
	vector<InValue>        arryInputData;        //ģ�͵��������ֵ����
	vector<IOValue*>       arryInputofLua;       // ����Lua�ļ���״̬���ݺ���������
	vector<IOValue*>       arryOutputofLua;

	bool				   bisUserDef_prob;		 // ���ڼ�¼��ģ�������ֵ�Ļ�ȡ��ʽ�Ƿ�Ϊ���Զ��塱����Ϊtrue
	bool				   bisUserDef_time;      // ���ڼ�¼��ģ��ʱ����ֵ�Ļ�ȡ��ʽ�Ƿ�Ϊ���Զ��塱����Ϊture
	bool				   bisUserDef_ctrl;      // ���ڼ�¼��ģ�Ϳ��ƿ�������ֵ�Ļ�ȡ��ʽ�Ƿ�Ϊ���Զ��塱����Ϊtrue 

	CString                str_Inputport_prob;   // ���ڼ�¼��ģ�������������ֵ�Ķ˿ڣ����û�ѡ����������ֵ��ʱ������bisUserDef_prob=falseʱ��
	CString                str_Inputport_time;   // ���ڼ�¼��ģ��ʱ���������ֵ�Ķ˿ڣ����û�ѡ����������ֵ��ʱ������bisUserDef_time=falseʱ��
	CString                str_Inputport_ctrl;   // ���ڼ�¼��ģ�Ϳ��ƿ�������ֵ�Ķ˿ڣ����û�ѡ����������ֵ��ʱ������bisUserDef_ctrl=falseʱ��

	

	CPlace *               m_thisModel;          // �������ģ�Ͷ���ĵ�ַ

	CString                Lua_fileName;         // ��������������lua�ļ������ֻ�·��

	double                 d_TimeStep;           // �����û����õķ��沽��

	bool                   b_ModelFocus;         // �������ڱ�ѡ��״̬

	//int                    i_RandOutcome;      // ������������Ľ��0��1
	bool                    i_RandOutcome;
	int                     rand_tag;
	double                  d_RandProblty;       // ��¼�����������

	double                  d_TimeTS;            // ���ڱ��桰ʱ����ֵ������Ϊһ�����ݣ��ڿ���ʹ�ܺ󣬸�ֵΪd_Time
	CHSTPN_SYSView       *  m_pParentView;       // ����View���ָ��

	int                     CtrlPlaceStyle;      // ������ƿ�����ʾΪͻ���ͻ���������
	bool                    isTimeUpDownChange; // �ж�ʱ����Ͻ���½��Ƿ�ı䣬����ı�����Ҫ���²�������ֲ�
	bool                    ischoseSocket;          //��ʾ�ÿ����Ƿ񼤻�ͨ�Žӿ�
	string                  ipadrs;              //ͨ��IP��ַ
	int                     port;                //ͨ�Ŷ˿�
	int                     num_output;
    int                     num_input;
	string                  msgO;                 //�����Ϣ����
	string                  msgI;                //������Ϣ����
public:
	//====================��Ա��������=======================================
	bool                  isEnable(double timestep = 0);           // ��������ʹ�ܹ���ķ���

	bool                  runLuaFunction(double timestep);     // ִ�����������е�Lua�������������˳�������ݣ��򷵻�true�����򷵻�false

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

	double dTime_downline;// ʱ�����������
	double dTime_upline;// ʱ�����������
	bool ischoseupdown;
	bool b_resettime;// ���ƽ�ʱ�ӿ�������ʱ�����Բ���һ��ʱ����ֵ

	bool ischoseProgRand;//��ʱ����һ����������ʾ�Ƿ�ѡ���û��Լ���̵õ������ֵ
	bool runS_LuaFunction(double timestep);
	result * runD_LuaFunction(double timestep);
};


