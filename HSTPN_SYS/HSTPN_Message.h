#pragma once
#include <vector>
#include <list>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
//=======ö�٣���ʾ����������========
enum enumPlaceStyle
{
	STYLE_DISCRETE = 1,
	STYLE_CNTNUS = 2,
	STYLE_RAND = 3,
	STYLE_TIME = 4,
	STYLE_TCTRL = 5,
	STYLE_LCTRL = 6,
	STYLE_DISIC = 7
	//STYLE_ = 8
};
//========������������������ṹ��===========================
typedef struct tagIOValue{

	CString Name;
	int     Sequence;
	CString IOType;
	bool    isQuoteG;
	CString GlobalVluName;
	CString FromModel;
	CString FromMDVluName;
	double  Value; 
	double  InitValue;

}IOValue;

typedef struct InputValue{

	CString Name;
	bool    isQuoteG;
	double  Value;

}InValue;

//=====Output Model enum====================================
enum enumOutputStyle
{
	OUT_SHOWMATRIX,
	OUT_TOFILE,
	OUT_ACCUMULATOR
};

typedef vector<double*> VEC_DOUBLE;

#define WM_MESSAGE_CREATEMODEL1 WM_USER+501  // �� ViewModelChoose1 �� MainFrame
#define WM_MESSAGE_CREATEMODEL2 WM_USER+502  // �� MainFrame �� HSTPN_SYSView

#define WM_MESSAGE_MOVEMODEL1 WM_USER+503  // �� CPlace/CTransition �� MainFrame
#define WM_MESSAGE_MOVEMODEL2 WM_USER+504  // �� MainFrame �� HSTPN_SYSView

#define WM_MESSAGE_DRAWLINEEND1 WM_USER+505  // �� CPlace/CTransition �� MainFrame
#define WM_MESSAGE_DRAWLINEEND2 WM_USER+506  // �� MainFrame �� HSTPN_SYSView

#define WM_MESSAGE_GETTIMESTEP  WM_USER+507  // �� MainFrame �� HSTPN_SYSView

//#define WM_MESSAGE_FILEOPEN    WM_USER+508    //�� HSTPN_SYSDoc �� HSTPN_SYSView
//#define WM_MESSAGE_FILESAVE    WM_USER+509    //�� HSTPN_SYSDoc �� HSTPN_SYSView
//#define WM_MESSAGE_FILESAVEAS  WM_USER+510    //�� HSTPN_SYSDoc �� HSTPN_SYSView
//#define WM_MESSAGE_FILENEW     WM_USER+511    //�� HSTPN_SYSDoc �� HSTPN_SYSView

#define WM_MESSAGE_MDDELETE1     WM_USER+512   //�� CPlace/Transition �� MainFrame
#define WM_MESSAGE_MDDELETE2     WM_USER+513   //�� MainFrame �� HSTPN_SYSView

#define WM_MESSAGE_UPDATETREE1     WM_USER+514   //�� HSTPN_SYSView �� MainFrame
#define WM_MESSAGE_UPDATETREE2     WM_USER+515   //�� MainFrame �� CDlgModelChoose1

#define WM_MESSAGE_UPDATELIST1     WM_USER+516   //��DlgSMat_OutValue �� CCTRLShowMatrix


#define TCTRL 0  // �������ֿ��ƿ�����ʾ����ʽ
#define LCTRL 1

#define WM_MESSAGE_CHANGEPROGRESS  WM_USER+517 //��Sim�ൽCMainFrame��

#define WM_MESSAGE_UPDATETREEOFMODEL WM_USER+518 // ��View�ൽCDlgModelParmList��

#define WM_MESSAGE_SCENE WM_USER+519 // ��Scene�ൽCMainFrame��

