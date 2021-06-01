/************************************************************************
*
*    Copyright         Copyright 2021 NEU Hao Lina.
*    File Name:        DlgMove.h/DlgMove.cpp
*    Description:      实现可视化推演的动画演示
*
*    Version:        V2.0.7
*    Author:         zhao
*    Create Time:    2020-05-04
*
*************************************************************************/

#pragma once

#include "HSTPN_SYSDoc.h"
class CHSTPN_SYSDoc;
#include "HSTPN_SYSSim.h"
#include "MainFrm.h"
#include "HSTPN_SYSSim.h"
#include <unordered_map>
class CMainFrame;
// CDlgMove 对话框
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <Mmsystem.h> 


//libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//定义动画中需要的变量的类型
//typedef struct moveValue {
//	CString Name;
//	double  Value;
//	double  InitValue;
//}moveValue;
//vector<vector<IOValue*>>          arryMoveData;       // 动画变量表
const double PI = 3.1415926535;
const double PI_over_180 = PI / 180.0f;
const int PLANE = 36;
const int BOMB = 36;
const int LAUNCHER = 30;
const int MISSLE = 90;
const int RADAR = 12;
const int SHIP = 1;
float toRadians(double degrees);

//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

//动画演示类
class CDlgMove : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMove)
	
public:
	CDlgMove(CWnd* pParent = nullptr);   // 标准构造函数	
	CDlgMove(const CDlgMove&);
	CDlgMove& operator=(const CDlgMove&);

	//单例模式 未使用
	//static CDlgMove& getInstance() {
	//	static CDlgMove instance;
	//	return instance;
	//}
public:
	virtual ~CDlgMove();
	// 对话框数据
	enum { IDD = IDD_DLG_MOVE };

	
	void OnInitDlg();
	//D3D
	//program values
	string APPTITLE;
	int SCREENW;
	int SCREENH;
	bool gameover;

	enum GAMESTATE {
		START = 0,
		RUN = 1,
		END = 2
	};
	GAMESTATE gamestate = START;
	//Direct3D objects
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;
	LPDIRECT3DSURFACE9 backbuffer;
	LPD3DXSPRITE spriteobj;
	//DirectInput objects, devices, and states
	LPDIRECTINPUT8 dinput;
	LPDIRECTINPUTDEVICE8 dimouse;
	LPDIRECTINPUTDEVICE8 dikeyboard;
	DIMOUSESTATE mouse_state;
	XINPUT_GAMEPAD controllers[4];
	char keys[256];
	int frame;
	int starttime;
	double theta = 0;

	LPDIRECT3DTEXTURE9 imgExplosion;    //爆炸图
	LPDIRECT3DTEXTURE9 imgMissle;       //导弹图
	LPDIRECT3DTEXTURE9 imgShip;       //航母图
	LPDIRECT3DTEXTURE9 imgLauncher;
	LPDIRECT3DTEXTURE9 imgRadar;
	LPDIRECT3DTEXTURE9 imgHPlane;
	LPDIRECT3DTEXTURE9 imgHPlane2;
	LPDIRECT3DTEXTURE9 imgGPlane;
	LPDIRECT3DTEXTURE9 imgGPlane2;
	LPDIRECT3DTEXTURE9 imgDPlane;
	LPDIRECT3DTEXTURE9 imgDPlane2;
	LPDIRECT3DTEXTURE9 imgHBomb;
	LPDIRECT3DTEXTURE9 imgGBomb;

	LPDIRECT3DSURFACE9 background;      //背景图片
	LPDIRECT3DSURFACE9 gamestart;       //开始封面图片
	LPDIRECT3DSURFACE9 gamesend1;        //结束封面图片
	LPDIRECT3DSURFACE9 gamesend2;        //结束封面图片

	LPD3DXFONT font18 = NULL;
	LPD3DXFONT font24 = NULL;
	//sprite structure
	struct SPRITE
	{
		float x, y;
		int frame, columns;
		int width, height;
		float scaling, rotation;
		int startframe, endframe;
		int starttime, delay;
		int direction;
		float velx, vely;
		bool alive;
		bool attack;
		bool explosion;
		D3DCOLOR color;

		SPRITE()
		{
			frame = 0;
			columns = 1;
			width = height = 0;
			scaling = 1.0f;
			rotation = 0.0f;
			startframe = endframe = 0;
			direction = 1;
			starttime = delay = 0;
			velx = vely = 0.0f;
			alive = true;
			attack = false;
			explosion = true;
			color = D3DCOLOR_XRGB(255, 255, 255);
		}
	};
	//各类作战资源sprite
	SPRITE explosion;
	SPRITE missle[MISSLE];
	SPRITE ship[SHIP];
	SPRITE radar[RADAR];
	SPRITE launcher[LAUNCHER];
	SPRITE Dplane[PLANE];
	SPRITE Gplane[PLANE];
	SPRITE Hplane[PLANE];
	SPRITE Gbomb[BOMB];
	SPRITE Hbomb[BOMB];
	//战机正在损失的数量
	int hpcountDlbl = 0;
	int hpcountBjx = 0;
	int hpcountYc = 0;
	int gpcountDlbl = 0;
	int gpcountBjx = 0;
	int gpcountYc = 0;
	int dpcountDlbl = 0;
	int dpcountBjx = 0;
	double jcprob = 0.3;
	//战机应该损失的数量
	int lossBjxGjj = 3;
	int lossDlblGjj = 3;
	int lossBjxHzj = 3;
	int lossDlblHzj = 3;
	int lossBjxDzj = 3;
	int lossDlblDzj = 3;

	int hpower = 5;//轰炸机导弹威力
	int gpoewr = 5;//攻击机导弹威力

	 //攻击各个方向的战机
	int planeDzhBjx = 6;
	int planeDzhDlbl = 6;
	int planeDzhYc = 0;

	int planeHzjBjx = 12;
	int planeHzjDlbl = 12;
	int planeHzjYc = 0;

	int planeGjjBjx = 8;
	int planeGjjDlbl = 8;
	int planeGjjYc = 0;
	//导弹的数量
	int missileBjx = 100;
	int missileDlbl = 100;
	int missileYc = 100;
	//雷达生命值
	int healthRadarBjx = 100;
	int healthRadarDlbl = 100;
	int healthRadarYc = 100;
	//战机数量
	int planeHzj = 36;
	int planeGjj = 20;
	int planeDzj = 12;
	//基地生命值
	int healthBjx = 100;
	int healthDlbl = 100;

	//timing variables
	DWORD refresh = 0;
	double refreshcount = 0;
	double refreshcountfordp = 0;//为了使电子战飞机出现后一直绘制
	DWORD screentime = 0;
	double screenfps = 0.0;
	double screencount = 0.0;
	DWORD coretime = 0;
	double corefps = 0.0;
	double corecount = 0.0;
	DWORD currenttime;

	vector<int> moveValue{ 0,0 };       //从HSTPNSim中传进来的值
	vector<unordered_map<string, int> > moveData;
	int timesAttack = 1;


	//Direct3D functions
	bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
	void Direct3D_Shutdown();
	LPDIRECT3DSURFACE9 LoadSurface(LPCWSTR filename);
	void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
	D3DXVECTOR2 GetBitmapSize(LPCWSTR filename);
	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
	void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framew, int frameh, int framenum = 0, int columns = 0);
	void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);
	void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height,
		int frame = 0, int columns = 1, float rotation = 0.0f, float scaling = 1.0f,
		D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	//DirectInput functions
	bool DirectInput_Init(HWND);
	void DirectInput_Update();
	void DirectInput_Shutdown();
	bool Key_Down(int);
	int Mouse_Button(int);
	int Mouse_X();
	int Mouse_Y();

	int Collision(SPRITE sprite1, SPRITE sprite2);

	void Draw_HUD();
	//加载绘制资源
	bool Create_Dplanes();
	void Draw_Dplanes();
	bool Create_Hplanes();
	void Draw_Hplanes();
	bool Create_missles();
	void Draw_missles();
	bool Create_Gbomb();
	bool Create_Hbomb();
	bool Create_radar();
	bool Create_launcher();
	bool Create_ships();
	bool Create_Gplanes();
	bool Create_explosion();
	void Draw_Gbomb();
	void Draw_Hbomb();
	void Draw_launcher();
	void Draw_radar();
	void Draw_ships();
	void Draw_Gplanes();

	void UpdateHplanes();
	void UpdateGplanes();
	void UpdateDplanes();
	//game functions
	bool Game_Init(HWND window);
	void Game_Run(HWND window);
	void Game_End();

	//创建动画循环函数
	static UINT CreatMove(LPVOID pParam);	
	//void PostNcDestroy();//释放内存

    //font functions
	LPD3DXFONT MakeFont(WCHAR* name, int size);
	void FontPrint(LPD3DXFONT font, int x, int y, LPCWSTR text, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	void ValuesInit();
	//从HSTPN中获取更新所需的变量值
	void ValuesUpdate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

//extern CDlgMove  *pDlg = new CDlgMove;