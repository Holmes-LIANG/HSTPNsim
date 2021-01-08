#pragma once
#include <vector>
#include <list>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
//=======枚举，表示库所的类型========
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
//========库所的输入输出变量结构体===========================
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

#define WM_MESSAGE_CREATEMODEL1 WM_USER+501  // 从 ViewModelChoose1 到 MainFrame
#define WM_MESSAGE_CREATEMODEL2 WM_USER+502  // 从 MainFrame 到 HSTPN_SYSView

#define WM_MESSAGE_MOVEMODEL1 WM_USER+503  // 从 CPlace/CTransition 到 MainFrame
#define WM_MESSAGE_MOVEMODEL2 WM_USER+504  // 从 MainFrame 到 HSTPN_SYSView

#define WM_MESSAGE_DRAWLINEEND1 WM_USER+505  // 从 CPlace/CTransition 到 MainFrame
#define WM_MESSAGE_DRAWLINEEND2 WM_USER+506  // 从 MainFrame 到 HSTPN_SYSView

#define WM_MESSAGE_GETTIMESTEP  WM_USER+507  // 从 MainFrame 到 HSTPN_SYSView

//#define WM_MESSAGE_FILEOPEN    WM_USER+508    //从 HSTPN_SYSDoc 到 HSTPN_SYSView
//#define WM_MESSAGE_FILESAVE    WM_USER+509    //从 HSTPN_SYSDoc 到 HSTPN_SYSView
//#define WM_MESSAGE_FILESAVEAS  WM_USER+510    //从 HSTPN_SYSDoc 到 HSTPN_SYSView
//#define WM_MESSAGE_FILENEW     WM_USER+511    //从 HSTPN_SYSDoc 到 HSTPN_SYSView

#define WM_MESSAGE_MDDELETE1     WM_USER+512   //从 CPlace/Transition 到 MainFrame
#define WM_MESSAGE_MDDELETE2     WM_USER+513   //从 MainFrame 到 HSTPN_SYSView

#define WM_MESSAGE_UPDATETREE1     WM_USER+514   //从 HSTPN_SYSView 到 MainFrame
#define WM_MESSAGE_UPDATETREE2     WM_USER+515   //从 MainFrame 到 CDlgModelChoose1

#define WM_MESSAGE_UPDATELIST1     WM_USER+516   //从DlgSMat_OutValue 到 CCTRLShowMatrix


#define TCTRL 0  // 定义两种控制库所显示的形式
#define LCTRL 1

#define WM_MESSAGE_CHANGEPROGRESS  WM_USER+517 //从Sim类到CMainFrame类

#define WM_MESSAGE_UPDATETREEOFMODEL WM_USER+518 // 从View类到CDlgModelParmList类

#define WM_MESSAGE_SCENE WM_USER+519 // 从Scene类到CMainFrame类

