// CDlgMove.cpp: 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgMove.h"

//#include "afxdialogex.h"


// CDlgMove 对话框

IMPLEMENT_DYNAMIC(CDlgMove, CDialogEx)

CDlgMove::CDlgMove(CWnd* pParent /*=nullptr*/)
	: CDialogEx(/*IDD_DIALOG1*/nullptr, pParent)
{
	//Direct3D variables
	d3d = NULL;
	d3ddev = NULL;
	backbuffer = NULL;

	APPTITLE = "Animate Sprite Demo";
	SCREENW = 1024;
	SCREENH = 768;

	imgExplosion = NULL;

	imgMissle = NULL;

	gameover = false;
	frame = 0;
	starttime = 0;

}

CDlgMove::~CDlgMove()
{
}

void CDlgMove::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool CDlgMove::Direct3D_Init(HWND window, int width, int height, bool fullscreen)
{
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) return false;

	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow = window;
	d3dpp.Windowed = (!fullscreen);
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;

	//create Direct3D device
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (!d3ddev) return false;


	//get a pointer to the back buffer surface
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	//create sprite object
	D3DXCreateSprite(d3ddev, &spriteobj);

	return 1;
}

void CDlgMove::Direct3D_Shutdown()
{
	if (spriteobj) spriteobj->Release();

	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}

void CDlgMove::DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source)
{
	//get width/height from source surface
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);

	//create rects for drawing
	RECT source_rect = { 0, 0, (long)desc.Width, (long)desc.Height };
	RECT dest_rect = { (long)x, (long)y, SCREENW/*(long)x + desc.Width*/, SCREENH/*(long)y + desc.Height*/ };

	//draw the source surface onto the dest
	d3ddev->StretchRect(source, &source_rect, dest, &dest_rect, D3DTEXF_NONE);
}

LPDIRECT3DSURFACE9 CDlgMove::LoadSurface(LPCWSTR filename)
{
	LPDIRECT3DSURFACE9 image = NULL;

	//get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK)
		return NULL;

	//create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,         //width of the surface
		info.Height,        //height of the surface
		D3DFMT_X8R8G8B8,    //surface format
		D3DPOOL_DEFAULT,    //memory pool to use
		&image,             //pointer to the surface
		NULL);              //reserved (always NULL)

	if (result != D3D_OK) return NULL;

	//load surface from file into newly created surface
	result = D3DXLoadSurfaceFromFile(
		image,                  //destination surface
		NULL,                   //destination palette
		NULL,                   //destination rectangle
		filename,       //source filename
		NULL,                   //source rectangle
		D3DX_DEFAULT,           //controls how image is filtered
		D3DCOLOR_XRGB(0, 0, 0),   //for transparency (0 for none)
		NULL);                  //source image info (usually NULL)


								//make sure file was loaded okay
	if (result != D3D_OK) return NULL;

	return image;
}


D3DXVECTOR2 CDlgMove::GetBitmapSize(LPCWSTR filename)
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);

	HRESULT result = D3DXGetImageInfoFromFile(filename, &info);

	if (result == D3D_OK)
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);
	else
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);

	return size;
}



LPDIRECT3DTEXTURE9 CDlgMove::LoadTexture(LPCWSTR filename, D3DCOLOR transcolor)
{
	LPDIRECT3DTEXTURE9 texture = NULL;

	//get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK) return NULL;

	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx(
		d3ddev,                //Direct3D device object
		filename,      //bitmap filename
		info.Width,            //bitmap image width
		info.Height,           //bitmap image height
		1,                     //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,       //the type of surface (standard)
		D3DFMT_UNKNOWN,        //surface format (default)
		D3DPOOL_DEFAULT,       //memory class for the texture
		D3DX_DEFAULT,          //image filter
		D3DX_DEFAULT,          //mip filter
		transcolor,            //color key for transparency
		&info,                 //bitmap file info (from loaded file)
		NULL,                  //color palette
		&texture);            //destination texture

							  //make sure the bitmap textre was loaded correctly
	if (result != D3D_OK) return NULL;

	return texture;
}


void CDlgMove::Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framew, int frameh, int framenum, int columns)
{
	D3DXVECTOR3 position((float)destx, (float)desty, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;
	rect.left = (framenum % columns) * framew;
	rect.top = (framenum / columns) * frameh;
	rect.right = rect.left + framew;
	rect.bottom = rect.top + frameh;

	spriteobj->Draw(texture, &rect, NULL, &position, white);
}

void CDlgMove::Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height,
	int frame, int columns, float rotation, float scaling, D3DCOLOR color)
{
	//换算成角度
	rotation = toRadians(rotation);

	//create a scale vector
	D3DXVECTOR2 scale(scaling, scaling);

	//create a translate vector
	D3DXVECTOR2 trans(x, y);

	//set center by dividing width and height by two
	D3DXVECTOR2 center((float)(width * scaling) / 2, (float)(height * scaling) / 2);

	//create 2D transformation matrix
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, rotation, &trans);

	//tell sprite object to use the transform
	spriteobj->SetTransform(&mat);

	//calculate frame location in source image
	int fx = (frame % columns) * width;
	int fy = (frame / columns) * height;
	RECT srcRect = { fx, fy, fx + width, fy + height };

	//draw the sprite frame
	spriteobj->Draw(image, &srcRect, NULL, NULL, color);

	D3DXMatrixIdentity(&mat);
	spriteobj->SetTransform(&mat);
}

void CDlgMove::Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay)
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe) frame = startframe;
		if (frame < startframe) frame = endframe;
	}
}


//bounding  box collision detection
int CDlgMove::Collision(SPRITE sprite1, SPRITE sprite2)
{
	RECT rect1;
	rect1.left = (long)sprite1.x;
	rect1.top = (long)sprite1.y;
	rect1.right = (long)sprite1.x + sprite1.width * sprite1.scaling;
	rect1.bottom = (long)sprite1.y + sprite1.height * sprite1.scaling;

	RECT rect2;
	rect2.left = (long)sprite2.x;
	rect2.top = (long)sprite2.y;
	rect2.right = (long)sprite2.x + sprite2.width * sprite2.scaling;
	rect2.bottom = (long)sprite2.y + sprite2.height * sprite2.scaling;

	RECT dest; //ignored
	return IntersectRect(&dest, &rect1, &rect2);
}

UINT CDlgMove::CreatMove(LPVOID pParam)
{
	//CDlgMove  *pDlg = new CDlgMove;
	//pDlg->Create(IDD_DLG_MOVE);
	//HWND hw = pDlg->GetSafeHwnd();
	//pDlg->ShowWindow(SW_SHOWNORMAL);
	//initialize the game
	CDlgMove  *pDlg = (CDlgMove  *)pParam;
	HWND hw = pDlg->GetSafeHwnd();
	if (!pDlg->Game_Init(hw)) return 0;
	//process game loop 
	while (!pDlg->gameover) {
		pDlg->Game_Run(hw);
		pDlg->ShowWindow(SW_SHOWNORMAL);
	}
	pDlg->EndDialog(0);//关闭非模态对话框
	pDlg->Game_End();//释放资源
	return 1;
}

bool CDlgMove::Game_Init(HWND window)
{
	//initialize Direct3D
	if (!Direct3D_Init(window, SCREENW, SCREENH, false))
	{
		MessageBox(_T("Error initializing Direct3D"), _T("ERROR"));
		return false;
	}
	//初始化输入设备
	DirectInput_Init(window);
	//create some fonts
	font18 = MakeFont(_T("Arial Bold"), 18);
	font24 = MakeFont(_T("Arial Bold"), 24);


	//初始化作战资源
	//if (!Create_Dplanes()) return false;
	//if (!Create_Hplanes()) return false;
	//if (!Create_Gplanes())   return false;
	if (!Create_missles()) return false;
	if (!Create_Gbomb())   return false;
	if (!Create_Hbomb())   return false;
	if (!Create_radar())   return false;
	if (!Create_launcher()) return false;
	if (!Create_ships())    return false;
	if (!Create_explosion()) return false;

	//加载背景
	background = LoadSurface(_T("bg.png"));
	if (!background) return false;
	gamestart = LoadSurface(_T("start.png"));
	if (!gamestart) return false;
	gamesend1 = LoadSurface(_T("end1.png"));
	if (!gamesend1) return false;
	gamesend2 = LoadSurface(_T("end2.png"));
	if (!gamesend2) return false;
	//ValuesInit();//初始化作战资源

	return true;
}

void CDlgMove::Game_Run(HWND window)
{
	//make sure the Direct3D device is valid
	if (!d3ddev) return;

	LPDIRECT3DTEXTURE9 texture = NULL;

	//clear the scene
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(25, 25, 255), 1.0f, 0);

	//update input devices
	DirectInput_Update();

	//按下鼠标和空格键仿真动画开始
	if (gamestate == START)
		if (Mouse_Button(0) || Key_Down(DIK_SPACE))
			gamestate = RUN;
	
	ValuesInit();

	switch (gamestate)
	{
	case GAMESTATE::RUN:
		//get current ticks
		currenttime = timeGetTime();
		//calculate core frame rate
		corecount += 1.0;
		if (currenttime > coretime + 1000)
		{
			corefps = corecount;
			corecount = 0.0;
			coretime = currenttime;
		}

		if (currenttime > refresh + 16)
		{
			refresh = currenttime;
			refreshcount++;
			refreshcountfordp++;
			if (refreshcount == 700)
				Create_Hplanes();
			if (refreshcount > 700)
				UpdateHplanes();
			if (refreshcount == 400)
				Create_Gplanes();
			if (refreshcount > 400)
				UpdateGplanes();
			if (refreshcount == 200)
				Create_Dplanes();
			if (refreshcount > 200)
				UpdateDplanes();
		}
	default:
		break;
	}

	//start rendering
	if (d3ddev->BeginScene());
	//start drawing
	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
	//run update at ~60 hz
	switch (gamestate)
	{
	case GAMESTATE::START:
		//绘制开始画面
		DrawSurface(backbuffer, 0, 0, gamestart);
		break;
	case GAMESTATE::RUN:
		//绘制背景图
		DrawSurface(backbuffer, 0, 0, background);

		//显示文字
		Draw_HUD();
		//绘制作战资源
		Draw_ships();
		Draw_Hbomb();
		Draw_Gbomb();
		Draw_launcher();
		Draw_missles();
		Draw_radar();

		if (refreshcount > 700)
			Draw_Hplanes();
		if (refreshcount > 400)
			Draw_Gplanes();
		if (refreshcountfordp > 200)
			Draw_Dplanes();
		break;
	case GAMESTATE::END:
		if (healthDlbl == 0 && healthBjx == 0)
			DrawSurface(backbuffer, 0, 0, gamesend1);//美军胜利
		else
			DrawSurface(backbuffer, 0, 0, gamesend2);//利军胜利
		break;
	}


	//stop drawing
	spriteobj->End();

	//stop rendering
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);

	//Escape key ends program
	//exit when escape key is pressed
	if (KEY_DOWN(VK_ESCAPE)) gameover = true;

}

void CDlgMove::Game_End()
{
	//free memory and shut down
	//if (imgHPlane) imgHPlane->Release();
	if (imgMissle) imgMissle->Release();

	if (font18) font18->Release();
	//DirectInput_Shutdown();
	//Direct3D_Shutdown();
}

BEGIN_MESSAGE_MAP(CDlgMove, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()

END_MESSAGE_MAP()


void CDlgMove::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	gameover = true;
	CDialogEx::OnClose();
}
//void CDlgMove::PostNcDestroy()
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	delete this;
//}

float toRadians(double degrees)
{
	return degrees * PI_over_180;
}

LPD3DXFONT CDlgMove::MakeFont(WCHAR* name, int size)
{
	LPD3DXFONT font = NULL;

	D3DXFONT_DESC desc = {
		size,                   //height
		0,                      //width
		0,                      //weight
		0,                      //miplevels
		false,                  //italic
		DEFAULT_CHARSET,        //charset
		OUT_TT_PRECIS,          //output precision
		CLIP_DEFAULT_PRECIS,    //quality
		DEFAULT_PITCH,          //pitch and family
		_T("")                  //font name
	};

	//strcpy(desc.FaceName, name.c_str());

	D3DXCreateFontIndirect(d3ddev, &desc, &font);

	return font;
}

void CDlgMove::FontPrint(LPD3DXFONT font, int x, int y, LPCWSTR text, D3DCOLOR color)
{
	//figure out the text boundary
	RECT rect = { x, y, 0, 0 };
	font->DrawText(NULL, text, _tcslen(text), &rect, DT_CALCRECT, color);

	//print the text
	font->DrawText(spriteobj, text, _tcslen(text), &rect, DT_LEFT, color);
}

void CDlgMove::Draw_HUD()
{

	wchar_t cDhealth[10];
	wchar_t cBhealth[10];
	wchar_t cDmissles[10];
	wchar_t cBmissles[10];
	wchar_t cYmissles[10];

	wchar_t cDradarHealth[10];
	wchar_t cBradarHealth[10];
	wchar_t cYradarHealth[10];

	wchar_t cDplanes[10];
	wchar_t cGplanes[10];
	wchar_t cHplanes[10];

	_itow_s(healthDlbl, cDhealth, 10, 10);
	_itow_s(healthBjx, cBhealth, 10, 10);
	_itow_s(missileDlbl, cDmissles, 10, 10);
	_itow_s(missileBjx, cBmissles, 10, 10);
	_itow_s(missileYc, cYmissles, 10, 10);

	_itow_s(healthRadarDlbl, cDradarHealth, 10, 10);
	_itow_s(healthRadarBjx, cBradarHealth, 10, 10);
	_itow_s(healthRadarYc, cYradarHealth, 10, 10);

	_itow_s(planeDzj, cDplanes, 10, 10);
	_itow_s(planeGjj, cGplanes, 10, 10);
	_itow_s(planeHzj, cHplanes, 10, 10);
	int y = SCREENH - 12;
	D3DCOLOR color = D3DCOLOR_ARGB(200, 255, 255, 255);
	//demonstrate font output
	FontPrint(font24, 800, 0, _T("利比亚"), color);
	FontPrint(font18, 800, 24, _T("的黎波里生命值："), color);
	FontPrint(font18, 940, 24, cDhealth, color);
	FontPrint(font18, 800, 42, _T("班加西生命值："), color);
	FontPrint(font18, 940, 42, cBhealth, color);
	FontPrint(font18, 800, 60, _T("的黎波里近程导弹数量："), color);
	FontPrint(font18, 990, 60, cDmissles, color);
	FontPrint(font18, 800, 78, _T("班加西近程导弹数量："), color);
	FontPrint(font18, 990, 78, cBmissles, color);
	FontPrint(font18, 800, 96, _T("远程导弹数量："), color);
	FontPrint(font18, 940, 96, cYmissles, color);

	FontPrint(font18, 800, 114, _T("的黎波里雷达生命值："), color);
	FontPrint(font18, 990, 114, cDradarHealth, color);
	FontPrint(font18, 800, 132, _T("班加西雷达生命值："), color);
	FontPrint(font18, 990, 132, cBradarHealth, color);
	FontPrint(font18, 800, 150, _T("远程雷达生命值："), color);
	FontPrint(font18, 940, 150, cYradarHealth, color);

	FontPrint(font24, 620, 0, _T("美国"), color);
	FontPrint(font18, 620, 24, _T("电子战飞机数量："), color);
	FontPrint(font18, 760, 24, cDplanes, color);
	FontPrint(font18, 620, 42, _T("攻击机数量："), color);
	FontPrint(font18, 760, 42, cGplanes, color);
	FontPrint(font18, 620, 60, _T("轰炸机数量："), color);
	FontPrint(font18, 760, 60, cHplanes, color);
}

bool CDlgMove::Create_Dplanes()
{
	imgDPlane = LoadTexture(_T("Dplane.png"));
	imgDPlane2 = LoadTexture(_T("Dplane2.png"));
	if (!imgDPlane) return false;
	int dis = 20;
	if (planeDzhDlbl == 6) {
		for (int i = 0; i < 6; i++)
		{
			Dplane[i].velx = -0.4;
			Dplane[i].vely = 0;
			Dplane[i].width = 100;
			Dplane[i].height = 100;
			Dplane[i].rotation = 30;
			Dplane[i].scaling = 0.3;
		}
		int x = 550, y = 150;
		Dplane[0].x = x;
		Dplane[1].x = x + dis;
		Dplane[2].x = x + dis;
		Dplane[3].x = x + 2 * dis;
		Dplane[4].x = x + 2 * dis;
		Dplane[5].x = x + 2 * dis;
		Dplane[0].y = y;
		Dplane[1].y = y - dis;
		Dplane[2].y = y + dis;
		Dplane[3].y = y - 2 * dis;
		Dplane[4].y = y;
		Dplane[5].y = y + 2 * dis;
	}
	if (planeDzhBjx == 6) {
		for (int i = 6; i < 12; i++)
		{
			Dplane[i].velx = 0.4;
			Dplane[i].vely = 0;
			Dplane[i].width = 100;
			Dplane[i].height = 100;
			Dplane[i].rotation = -30;
			Dplane[i].scaling = 0.3;
		}
		int x = 650, y = 150;
		Dplane[6].x = x;
		Dplane[7].x = x - dis;
		Dplane[8].x = x - dis;
		Dplane[9].x = x - 2 * dis;
		Dplane[10].x = x - 2 * dis;
		Dplane[11].x = x - 2 * dis;
		Dplane[6].y = y;
		Dplane[7].y = y - dis;
		Dplane[8].y = y + dis;
		Dplane[9].y = y - 2 * dis;
		Dplane[10].y = y;
		Dplane[11].y = y + 2 * dis;
	}
	if (planeDzhYc == 6) {
		for (int i = 12; i < 18; i++)
		{
			Dplane[i].velx = -0;
			Dplane[i].vely = 0;
			Dplane[i].width = 100;
			Dplane[i].height = 100;
			Dplane[i].rotation = 30;
			Dplane[i].scaling = 0.3;
		}
		int x = 600, y = 150;
		Dplane[12].x = x;
		Dplane[13].x = x + dis;
		Dplane[14].x = x + dis;
		Dplane[15].x = x + 2 * dis;
		Dplane[16].x = x + 2 * dis;
		Dplane[17].x = x + 2 * dis;
		Dplane[12].y = y;
		Dplane[13].y = y - dis;
		Dplane[14].y = y + dis;
		Dplane[15].y = y - 2 * dis;
		Dplane[16].y = y;
		Dplane[17].y = y + 2 * dis;
	}
	return true;
}

void CDlgMove::Draw_Dplanes()
{
	//获取变量在动画中显示
	for (int i = 0; i < 12; i++)
	{
		if (Dplane[i].alive) {
			if (i < 12 && i >= 6) {
				//if (Dplane[6].x < 800) {
				//	Dplane[i].x += Dplane[i].velx;
				//	Dplane[i].y += Dplane[i].vely;
				//}
				Sprite_Transform_Draw(imgDPlane2, Dplane[i].x, Dplane[i].y, Dplane[i].width, Dplane[i].height, 0, 1, Dplane[i].rotation, Dplane[i].scaling);
			}
			else {
				//if (Dplane[0].x > 300) {
				//	Dplane[i].x += Dplane[i].velx;
				//	Dplane[i].y += Dplane[i].vely;
				//}
				Sprite_Transform_Draw(imgDPlane, Dplane[i].x, Dplane[i].y, Dplane[i].width, Dplane[i].height, 0, 1, Dplane[i].rotation, Dplane[i].scaling);
			}
		}

		if (!Dplane[i].alive&&Dplane[i].explosion) {
			Sprite_Animate(frame, 0, 29, 1, starttime, 90);
			Sprite_Transform_Draw(imgExplosion, Dplane[i].x, Dplane[i].y, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
			if (frame == 29)
				Dplane[i].explosion = false;
		}
	}
}

bool CDlgMove::Create_Hplanes()
{
	imgHPlane = LoadTexture(_T("Hplane.png"));
	if (!imgHPlane) return false;
	imgHPlane2 = LoadTexture(_T("Hplane2.png"));
	if (!imgHPlane2) return false;
	int dis = 25;
	if (planeHzjDlbl == 12) {
		for (int i = 0; i < 12; i++)
		{
			Hplane[i].velx = -0.4;
			Hplane[i].vely = 0.4;
			Hplane[i].width = 100;
			Hplane[i].height = 100;
			Hplane[i].rotation = 60;
			Hplane[i].scaling = 0.3;
			Hplane[i].alive = true;
			Hplane[i].attack = false;
			Hplane[i].explosion = true;
		}
		int x = 530, y = 150;
		Hplane[0].x = x;
		Hplane[1].x = Hplane[0].x + dis;
		Hplane[2].x = Hplane[1].x + dis;

		Hplane[3].x = x + dis / 2;
		Hplane[4].x = Hplane[3].x + dis;
		Hplane[5].x = Hplane[4].x + dis;

		Hplane[6].x = x + dis;
		Hplane[7].x = Hplane[6].x + dis;
		Hplane[8].x = Hplane[7].x + dis;

		Hplane[9].x = x + dis * 3 / 2;
		Hplane[10].x = Hplane[9].x + dis;
		Hplane[11].x = Hplane[10].x + dis;

		Hplane[0].y = y;
		Hplane[1].y = y;
		Hplane[2].y = y;

		Hplane[3].y = y - dis;
		Hplane[4].y = y - dis;
		Hplane[5].y = y - dis;

		Hplane[6].y = y - 2 * dis;
		Hplane[7].y = y - 2 * dis;
		Hplane[8].y = y - 2 * dis;

		Hplane[9].y = y - 3 * dis;
		Hplane[10].y = y - 3 * dis;
		Hplane[11].y = y - 3 * dis;
	}
	if (planeHzjBjx == 12) {
		for (int i = 12; i < 24; i++)
		{
			Hplane[i].velx = 0.3;
			Hplane[i].vely = 0.4;
			Hplane[i].width = 100;
			Hplane[i].height = 100;
			Hplane[i].rotation = 0;
			Hplane[i].scaling = 0.3;
			Hplane[i].alive = true;
			Hplane[i].attack = false;
			Hplane[i].explosion = true;
		}
		int x = 780, y = 150;
		Hplane[12].x = x;
		Hplane[13].x = Hplane[12].x - dis;
		Hplane[14].x = Hplane[13].x - dis;

		Hplane[15].x = x - dis / 2;
		Hplane[16].x = Hplane[15].x - dis;
		Hplane[17].x = Hplane[16].x - dis;

		Hplane[18].x = x - dis;
		Hplane[19].x = Hplane[18].x - dis;
		Hplane[20].x = Hplane[19].x - dis;

		Hplane[21].x = x - dis * 3 / 2;
		Hplane[22].x = Hplane[21].x - dis;
		Hplane[23].x = Hplane[22].x - dis;

		Hplane[12].y = y;
		Hplane[13].y = y;
		Hplane[14].y = y;

		Hplane[15].y = y - dis;
		Hplane[16].y = y - dis;
		Hplane[17].y = y - dis;

		Hplane[18].y = y - 2 * dis;
		Hplane[19].y = y - 2 * dis;
		Hplane[20].y = y - 2 * dis;

		Hplane[21].y = y - 3 * dis;
		Hplane[22].y = y - 3 * dis;
		Hplane[23].y = y - 3 * dis;
	}
	if (planeHzjYc == 12) {
		for (int i = 24; i < 36; i++)
		{
			Hplane[i].velx = -0.1;
			Hplane[i].vely = 0.4;
			Hplane[i].width = 100;
			Hplane[i].height = 100;
			Hplane[i].rotation = 30;
			Hplane[i].scaling = 0.3;
			Hplane[i].alive = true;
			Hplane[i].attack = false;
			Hplane[i].explosion = true;
		}
		int x = 600, y = 150;
		Hplane[24].x = x;
		Hplane[25].x = Hplane[24].x + dis;
		Hplane[26].x = Hplane[25].x + dis;

		Hplane[27].x = x + dis / 2;
		Hplane[28].x = Hplane[27].x + dis;
		Hplane[29].x = Hplane[28].x + dis;

		Hplane[30].x = x + dis;
		Hplane[31].x = Hplane[30].x + dis;
		Hplane[32].x = Hplane[31].x + dis;

		Hplane[33].x = x + dis * 3 / 2;
		Hplane[34].x = Hplane[33].x + dis;
		Hplane[35].x = Hplane[34].x + dis;

		Hplane[24].y = y;
		Hplane[25].y = y;
		Hplane[26].y = y;

		Hplane[27].y = y - dis;
		Hplane[28].y = y - dis;
		Hplane[29].y = y - dis;

		Hplane[30].y = y - 2 * dis;
		Hplane[31].y = y - 2 * dis;
		Hplane[32].y = y - 2 * dis;

		Hplane[33].y = y - 3 * dis;
		Hplane[34].y = y - 3 * dis;
		Hplane[35].y = y - 3 * dis;
	}
	return true;
}

void CDlgMove::UpdateDplanes() {
	for (int i = 0; i < 12; i++)
	{
		if (Dplane[i].alive) {
			if (i >= 6) {
				if (Dplane[9].x < 800) {
					Dplane[i].x += Dplane[i].velx;
					Dplane[i].y += Dplane[i].vely;
				}
			}
			else {
				if (Dplane[3].x > 300) {
					Dplane[i].x += Dplane[i].velx;
					Dplane[i].y += Dplane[i].vely;
				}
			}
		}
	}
}
void CDlgMove::UpdateHplanes() {
	for (int i = 0; i < PLANE; i++)
	{
		if (Hplane[i].alive) {
			//计算战机位置
			Hplane[i].x += Hplane[i].velx;
			Hplane[i].y += Hplane[i].vely;
		}
	}
}
void CDlgMove::UpdateGplanes() {
	for (int i = 0; i < PLANE; i++)
	{
		if (Gplane[i].alive) {
			//计算战机位置
			Gplane[i].x += Gplane[i].velx;
			Gplane[i].y += Gplane[i].vely;
		}
	}
}

void CDlgMove::Draw_Hplanes()
{
	//获取变量在动画中显示
	for (int i = 0; i < PLANE; i++)
	{
		if (Hplane[i].alive) {
			if (i<24 && i >= 12)
				Sprite_Transform_Draw(imgHPlane2, Hplane[i].x, Hplane[i].y, Hplane[i].width, Hplane[i].height, 0, 1, Hplane[i].rotation, Hplane[i].scaling);
			else
				Sprite_Transform_Draw(imgHPlane, Hplane[i].x, Hplane[i].y, Hplane[i].width, Hplane[i].height, 0, 1, Hplane[i].rotation, Hplane[i].scaling);

			//Sprite_Transform_Draw(imgHBomb, Hplane.x, Hplane.y+60, Hbomb.width, Hbomb.height, 0, 1, Hbomb.rotation, Hbomb.scaling);
		}

		if (!Hplane[i].alive&&Hplane[i].explosion) {
			Sprite_Animate(frame, 0, 29, 1, starttime, 90);
			Sprite_Transform_Draw(imgExplosion, Hplane[i].x, Hplane[i].y, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
			if (frame == 29)
				Hplane[i].explosion = false;
		}
	}
	ValuesUpdate();
}

bool CDlgMove::Create_missles()
{
	//加载导弹
	imgMissle = LoadTexture(_T("missle3.png"));
	if (!imgMissle) return false;

	hpcountDlbl = 0;
	hpcountBjx = 0;
	hpcountYc = 0;
	gpcountDlbl = 0;
	gpcountBjx = 0;
	gpcountYc = 0;
	//dpcountDlbl = 0;
	//dpcountBjx = 0;

	for (int i = 0; i < 60; i++) {
		missle[i].width = 100;
		missle[i].height = 100;
		missle[i].columns = 1;
		missle[i].startframe = 0;
		missle[i].endframe = 0;
		missle[i].velx = 0.02;
		missle[i].vely = 0.2;
		missle[i].rotation = 60;
		missle[i].scaling = 0.1;
		missle[i].alive = true;
		missle[i].attack = false;
	}
	//远程导弹只攻击电子战飞机
	if (planeDzhDlbl >0 || planeDzhDlbl > 0) {
		for (int i = 60; i < 90; i++) {
			missle[i].width = 100;
			missle[i].height = 100;
			missle[i].columns = 1;
			missle[i].startframe = 0;
			missle[i].endframe = 0;
			missle[i].velx = 0.04;
			missle[i].vely = 0.2;
			missle[i].rotation = 60;
			missle[i].scaling = 0.1;
			missle[i].alive = true;
			missle[i].attack = false;
		}

	}
	return true;
}
void CDlgMove::Draw_missles()
{

	for (int i = 0; i < 60; i++) {
		//计算导弹初始位置
		if (!missle[i].attack&&missle[i].alive&&i < 30 && Gplane[i % (planeGjjBjx + planeGjjDlbl)].y > 300) {
			missle[i].x = launcher[i % 20].x;
			missle[i].y = launcher[i % 20].y;
			if (i < 15)missileDlbl--;
			else  missileBjx--;
			missle[i].attack = true;
		}
		if (!missle[i].attack&&missle[i].alive&&i >= 30 && i<60 && Hplane[i % (planeHzjBjx + planeHzjDlbl)].y>300) {
			missle[i].x = launcher[i % 20].x;
			missle[i].y = launcher[i % 20].y;
			if (i >= 45)missileDlbl--;
			else  missileBjx--;
			missle[i].attack = true;
		}
		if (missle[i].alive&&missle[i].attack) {
			missle[i].x += missle[i].velx;
			missle[i].y -= missle[i].vely;
			Sprite_Transform_Draw(imgMissle, missle[i].x, missle[i].y, missle[i].width, missle[i].height, 0, 1, missle[i].rotation += 0.01, missle[i].scaling);
			for (int j = 0; j < PLANE; j++) {
				//轰炸机三个方向战机损失数量
				if (Collision(missle[i], Hplane[j]) && Hplane[j].alive) {
					if (j < 12) {
						hpcountDlbl++;
						if (hpcountDlbl <= lossDlblHzj) {
							Hplane[j].alive = false;
							planeHzj--;
						}
					}
					if (j >= 12 && j < 24) {
						hpcountBjx++;
						if (hpcountBjx <= lossBjxHzj) {
							Hplane[j].alive = false;
							planeHzj--;
						}
					}
					if (j >= 24 && j < 36) {
						hpcountYc++;
						if (hpcountYc <= lossDlblHzj) {
							Hplane[j].alive = false;
							planeHzj--;
						}
					}
					missle[i].alive = false;
				}

				//攻击机三个方向战机损失数量
				if (Collision(missle[i], Gplane[j]) && Gplane[j].alive) {
					if (j < 8) {
						gpcountDlbl++;
						if (gpcountDlbl <= lossDlblGjj) {
							Gplane[j].alive = false;
							planeGjj--;
						}
					}
					if (j >= 8 && j < 16) {
						gpcountBjx++;
						if (gpcountBjx <= lossBjxGjj) {
							Gplane[j].alive = false;
							planeGjj--;
						}
					}
					if (j >= 16 && j < 24) {
						gpcountYc++;
						if (gpcountYc <= lossDlblGjj) {
							Gplane[j].alive = false;
							planeGjj--;
						}
					}
					missle[i].alive = false;
				}
			}
		}
	}
	for (int i = 60; i < 90; i++) {
		if (!missle[i].attack&&missle[i].alive && (Dplane[i % 6].x >0 && Dplane[i % 6].x < 350 || Dplane[i % 6 + 6].x > 750)) {
			missle[i].x = launcher[i % 10 + 20].x;
			missle[i].y = launcher[i % 10 + 20].y;
			missileYc--;
			missle[i].attack = true;
		}
		if (missle[i].alive&&missle[i].attack) {
			if (i >= 60 && i < 75) {
				missle[i].x -= 2 * missle[i].velx;
				missle[i].rotation = 40;
			}
			if (i >= 75 && i < 90) {
				missle[i].x += 2 * missle[i].velx;
			}
			missle[i].y -= missle[i].vely;
			Sprite_Transform_Draw(imgMissle, missle[i].x, missle[i].y, missle[i].width, missle[i].height, 0, 1, missle[i].rotation += 0.01, missle[i].scaling);
		}
		for (int j = 0; j < PLANE; j++) {
			//电子战飞机三个方向战机损失数量
			if (Collision(missle[i], Dplane[j]) && Dplane[j].alive) {
				if (j < 6) {
					dpcountDlbl++;
					if (dpcountDlbl <= lossBjxDzj) {
						Dplane[j].alive = false;
						planeDzj--;
					}
				}
				else if (j >= 6 && j < 12) {
					dpcountBjx++;
					if (dpcountBjx <= lossDlblDzj) {
						Dplane[j].alive = false;
						planeDzj--;
					}
				}
				missle[i].alive = false;
			}
		}
	}
}

bool CDlgMove::Create_explosion()
{
	//load explosion sprite
	imgExplosion = LoadTexture(_T("explosion_30_128.tga"));
	if (!imgExplosion) return false;
	explosion.x = 300;
	explosion.y = 100;
	explosion.width = explosion.height = 128;
	explosion.columns = 6;
	explosion.rotation = 0;
	explosion.scaling = 0.5;
}
bool CDlgMove::Create_Gplanes()
{
	imgGPlane = LoadTexture(_T("Gplane.png"));
	if (!imgGPlane) return false;
	imgGPlane2 = LoadTexture(_T("Gplane2.png"));
	if (!imgGPlane2) return false;

	int dis = 20;
	if (planeGjjDlbl == 8) {
		for (int i = 0; i < 8; i++) {

			Gplane[i].width = 100;
			Gplane[i].height = 100;
			Gplane[i].rotation = 60;
			Gplane[i].scaling = 0.3;
			Gplane[i].velx = -0.4;
			Gplane[i].vely = 0.4;
			Gplane[i].alive = true;
			Gplane[i].attack = false;
			Gplane[i].explosion = true;
		}
		int x1 = 500, y1 = 150;
		Gplane[0].x = x1;
		Gplane[1].x = x1 + dis;
		Gplane[2].x = x1 + dis;
		Gplane[3].x = x1 + 2 * dis;
		Gplane[4].x = x1 + 3 * dis;
		Gplane[5].x = x1 + 3 * dis;
		Gplane[6].x = x1 + 4 * dis;
		Gplane[7].x = x1 + 4 * dis;

		Gplane[0].y = y1;
		Gplane[1].y = y1 - dis;
		Gplane[2].y = y1 + dis;
		Gplane[3].y = y1;
		Gplane[4].y = y1 + dis;
		Gplane[5].y = y1 - dis;
		Gplane[6].y = y1 + 2 * dis;
		Gplane[7].y = y1 - 2 * dis;
	}
	if (planeGjjBjx == 8) {
		for (int i = 8; i < 16; i++) {
			Gplane[i].width = 100;
			Gplane[i].height = 100;
			Gplane[i].rotation = -30;
			Gplane[i].scaling = 0.3;
			Gplane[i].velx = 0.4;
			Gplane[i].vely = 0.4;
			Gplane[i].alive = true;
			Gplane[i].attack = false;
			Gplane[i].explosion = true;
		}
		int x2 = 750, y2 = 150;
		Gplane[8].x = x2;
		Gplane[9].x = x2 - dis;
		Gplane[10].x = x2 - dis;
		Gplane[11].x = x2 - 2 * dis;
		Gplane[12].x = x2 - 3 * dis;
		Gplane[13].x = x2 - 3 * dis;
		Gplane[14].x = x2 - 4 * dis;
		Gplane[15].x = x2 - 4 * dis;

		Gplane[8].y = y2;
		Gplane[9].y = y2 - dis;
		Gplane[10].y = y2 + dis;
		Gplane[11].y = y2;
		Gplane[12].y = y2 + dis;
		Gplane[13].y = y2 - dis;
		Gplane[14].y = y2 + 2 * dis;
		Gplane[15].y = y2 - 2 * dis;
	}
	if (planeGjjYc == 8) {
		for (int i = 16; i < 24; i++) {

			Gplane[i].width = 100;
			Gplane[i].height = 100;
			Gplane[i].rotation = 30;
			Gplane[i].scaling = 0.3;
			Gplane[i].velx = -0.4;
			Gplane[i].vely = 0.4;
			Gplane[i].alive = true;
			Gplane[i].attack = false;
			Gplane[i].explosion = true;
		}
		int x3 = 650, y3 = 150;
		Gplane[16].x = x3;
		Gplane[17].x = x3 + dis;
		Gplane[18].x = x3 + dis;
		Gplane[19].x = x3 + 2 * dis;
		Gplane[20].x = x3 + 3 * dis;
		Gplane[21].x = x3 + 3 * dis;
		Gplane[22].x = x3 + 4 * dis;
		Gplane[23].x = x3 + 4 * dis;

		Gplane[16].y = y3;
		Gplane[17].y = y3 - dis;
		Gplane[18].y = y3 + dis;
		Gplane[19].y = y3;
		Gplane[20].y = y3 + dis;
		Gplane[21].y = y3 - dis;
		Gplane[22].y = y3 + 2 * dis;
		Gplane[23].y = y3 - 2 * dis;
	}
}


bool CDlgMove::Create_ships()
{
	//加载航空母舰
	imgShip = LoadTexture(_T("carrier3.png"));
	if (!imgShip) return false;
	for (int i = 0; i < SHIP; i++) {

		ship[i].width = 100;
		ship[i].height = 98;
		ship[i].columns = 1;
		ship[i].startframe = 0;
		ship[i].endframe = 0;
		ship[i].velx = -0.13f;
		ship[i].vely = 0.1f;
		ship[i].rotation = 0;
		ship[i].scaling = 1;
	}
	ship[0].x = 700;
	ship[0].y = 0;
	//ship[1].x = 200;
	//ship[1].y = 200;
}

void CDlgMove::Draw_ships() {
	for (int i = 0; i < SHIP; i++) {
		if (ship[i].y < 100) {
			ship[i].x += ship[i].velx;
			ship[i].y += ship[i].vely;
		}
		Sprite_Transform_Draw(imgShip, ship[i].x, ship[i].y, ship[i].width, ship[i].height, ship[i].frame, ship[i].columns, ship[i].rotation, ship[i].scaling);
	}
}
bool CDlgMove::Create_launcher()
{
	//加载导弹发射架
	imgLauncher = LoadTexture(_T("car.png"));
	if (!imgLauncher) return false;
	int x1 = 250, y1 = 560;//的黎波里方向
	int x2 = 880, y2 = 480;//班加西方向
	int x3 = 520, y3 = 720;//远程导弹方向
	int dis = 25;
	launcher[0].x = x1;
	launcher[0].y = y1;
	launcher[5].x = x1;
	launcher[5].y = y1 + dis;
	launcher[10].x = x2;
	launcher[10].y = y2;
	launcher[15].x = x2;
	launcher[15].y = y2 + dis;
	launcher[20].x = x3;
	launcher[20].y = y3;
	launcher[25].x = x3;
	launcher[25].y = y3 + dis;
	for (int i = 0; i < LAUNCHER; i++) {
		launcher[i].width = 100;
		launcher[i].height = 79;//158
		launcher[i].columns = 1;
		launcher[i].startframe = 0;
		launcher[i].endframe = 0;
		launcher[i].velx = -1.0f;
		launcher[i].rotation = 0;
		launcher[i].scaling = 0.2;
		launcher[i].alive = true;
		if (i>0 && i < 5) {
			launcher[i].x = launcher[i - 1].x + dis;
			launcher[i].y = y1;
		}
		if (i > 5 && i < 10) {
			launcher[i].x = launcher[i - 1].x + dis;
			launcher[i].y = y1 + dis;
		}
		if (i >10 && i < 15) {
			launcher[i].x = launcher[i - 1].x + dis;
			launcher[i].y = y2;
		}
		if (i > 15 && i < 20) {
			launcher[i].x = launcher[i - 1].x + dis;
			launcher[i].y = y2 + dis;
		}
		if (i > 20 && i < 25) {
			launcher[i].x = launcher[i - 1].x + dis;
			launcher[i].y = y3;
		}
		if (i > 25 && i < 30) {
			launcher[i].x = launcher[i - 1].x + dis;
			launcher[i].y = y3 + dis;
		}
	}
}
void CDlgMove::Draw_launcher() {

	for (int i = 0; i < 30; i++) {
		if (launcher[i].alive) {
			for (int j = 0; j < BOMB; j++)
				//导弹与发射架接触使其爆炸
				//if (Collision(launcher[i], Hbomb[j])&& Hbomb[j].alive) {
				//	launcher[i].attack = true;
				//	Hbomb[j].alive    = false;
				//	//if(j<12) healthBjx-=5;
				//	//if (j >= 12 && j < 24) healthDlbl -= 5;
				//}
				Sprite_Transform_Draw(imgLauncher, launcher[i].x, launcher[i].y, launcher[i].width, launcher[i].height, 0, 1, launcher[i].rotation, launcher[i].scaling);
		}
		if (launcher[i].attack&&launcher[i].explosion) {
			Sprite_Animate(frame, 0, 29, 1, starttime, 90);
			Sprite_Transform_Draw(imgExplosion, launcher[i].x - launcher[i].scaling*launcher[i].width, launcher[i].y - launcher[i].scaling*launcher[i].height, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
			if (frame == 29)
				launcher[i].explosion = false;
		}
	}
}
bool CDlgMove::Create_radar()
{
	//加载雷达
	imgRadar = LoadTexture(_T("radar2.png"));
	if (!imgRadar) return false;
	int x1 = 280, y1 = 530;
	int x2 = 900, y2 = 440;
	int x3 = 540, y3 = 690;
	int dis = 30;
	for (int i = 0; i < RADAR; i++) {
		radar[i].width = 100;
		radar[i].height = 108;
		radar[i].columns = 1;
		radar[i].startframe = 0;
		radar[i].endframe = 0;
		radar[i].velx = -1.0f;
		radar[i].rotation = 0;
		radar[i].scaling = 0.2;
	}
	radar[0].x = x1;
	radar[0].y = y1;
	radar[1].x = x1 + dis;
	radar[1].y = y1;
	radar[2].x = x1 + dis * 2;
	radar[2].y = y1 - dis / 2;
	radar[3].x = x1 - dis;
	radar[3].y = y1 - dis / 2;

	radar[4].x = x2;
	radar[4].y = y2;
	radar[5].x = x2 + dis;
	radar[5].y = y2;
	radar[6].x = x2 + dis * 2;
	radar[6].y = y2 - dis / 2;
	radar[7].x = x2 - dis;
	radar[7].y = y2 - dis / 2;

	radar[8].x = x3;
	radar[8].y = y3;
	radar[9].x = x3 + dis;
	radar[9].y = y3;
	radar[10].x = x3 + dis * 2;
	radar[10].y = y3 - dis / 2;
	radar[11].x = x3 - dis;
	radar[11].y = y3 - dis / 2;
}
void CDlgMove::Draw_radar()
{
	for (int i = 0; i < RADAR; i++) {
		Sprite_Transform_Draw(imgRadar, radar[i].x, radar[i].y, radar[i].width, radar[i].height, 0, 1, radar[i].rotation, radar[i].scaling);
	}
}

bool CDlgMove::Create_Hbomb()
{
	//加载轰炸弹
	imgHBomb = LoadTexture(_T("Hbomb.png"));
	if (!imgHBomb) return false;
	for (int i = 0; i < BOMB; i++)
	{
		Hbomb[i].x = -10;
		Hbomb[i].y = -10;
		Hbomb[i].width = 100;
		Hbomb[i].height = 28;
		Hbomb[i].columns = 1;
		Hbomb[i].startframe = 0;
		Hbomb[i].endframe = 0;
		Hbomb[i].velx = 0.03;
		Hbomb[i].vely = 0.1;
		Hbomb[i].scaling = 0.1;
		Hbomb[i].alive = true;
		Hbomb[i].explosion = true;
		if (i >= 12 && i<24)
			Hbomb[i].rotation = 240;
		else
			Hbomb[i].rotation = 300;
	}

}
void CDlgMove::Draw_Hbomb() {
	//计算导弹位置
	for (int i = 0; i< BOMB; i++) {
		if (!Hplane[i].attack&&Hbomb[i].alive&&Hplane[i].y > 400 && (i < 12)) {
			Hbomb[i].x = Hplane[i].x + Hplane[i].width*Hplane[i].scaling / 2;
			Hbomb[i].y = Hplane[i].y + Hplane[i].height*Hplane[i].scaling / 2;
			Hplane[i].attack = true;
		}
		if (!Hplane[i].attack&&Hbomb[i].alive&&Hplane[i].y>400 && (i >= 12 && i<24)) {
			Hbomb[i].x = Hplane[i].x + Hplane[i].width*Hplane[i].scaling / 2;
			Hbomb[i].y = Hplane[i].y + Hplane[i].height*Hplane[i].scaling / 2;
			Hplane[i].attack = true;
		}
		if (!Hplane[i].attack&&Hbomb[i].alive&&Hplane[i].y >400 && (i >= 24 && i<36)) {
			Hbomb[i].x = Hplane[i].x + Hplane[i].width*Hplane[i].scaling / 2;
			Hbomb[i].y = Hplane[i].y + Hplane[i].height*Hplane[i].scaling / 2;
			Hplane[i].attack = true;
		}
		//if (Hbomb[i].y > 600) Hbomb[i].alive = false;
		if (Hbomb[i].alive) {
			if (Hplane[i].attack&& i < 12) {
				Hbomb[i].x -= Hbomb[i].velx;
				Hbomb[i].y += Hbomb[i].vely;
			}
			else if (Hplane[i].attack&& i >= 12 && i<24) {
				Hbomb[i].x += Hbomb[i].velx;
				Hbomb[i].y += Hbomb[i].vely;
			}
			else if (Hplane[i].attack&& i >= 24 && i < 36) {
				Hbomb[i].y += Hbomb[i].vely;
			}

			Sprite_Transform_Draw(imgHBomb, Hbomb[i].x, Hbomb[i].y, Hbomb[i].width, Hbomb[i].height, 0, 1, Hbomb[i].rotation += 0.0015, Hbomb[i].scaling);
		}
		//实现触地后爆炸，不同方向距离不一样
		if (i < 36 && i >= 24) {
			if (Hbomb[i].y > 700 && Hbomb[i].explosion) {
				Sprite_Animate(frame, 0, 29, 1, starttime, 90);
				Sprite_Transform_Draw(imgExplosion, Hbomb[i].x - Hbomb[i].scaling*Hbomb[i].width, Hbomb[i].y + Hbomb[i].scaling*Hbomb[i].height, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
				Hbomb[i].alive = false;
				if (frame == 29)
					Hbomb[i].explosion = false;
			}
		}
		else if (i < 24 && i >= 12) {
			if (Hbomb[i].y > 480 && Hbomb[i].explosion) {
				Sprite_Animate(frame, 0, 29, 1, starttime, 90);
				Sprite_Transform_Draw(imgExplosion, Hbomb[i].x - Hbomb[i].scaling*Hbomb[i].width, Hbomb[i].y + Hbomb[i].scaling*Hbomb[i].height, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
				Hbomb[i].alive = false;
				if (frame == 29) {
					healthBjx -= hpower;
					Hbomb[i].explosion = false;
				}
			}
		}
		else {
			if (Hbomb[i].y > 550 && Hbomb[i].explosion) {
				Sprite_Animate(frame, 0, 29, 1, starttime, 90);
				Sprite_Transform_Draw(imgExplosion, Hbomb[i].x - Hbomb[i].scaling*Hbomb[i].width, Hbomb[i].y + Hbomb[i].scaling*Hbomb[i].height, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
				Hbomb[i].alive = false;
				if (frame == 29) {
					healthDlbl -= hpower;
					Hbomb[i].explosion = false;
				}
			}
		}

	}

}
bool CDlgMove::Create_Gbomb()
{
	imgGBomb = LoadTexture(_T("Gbomb.png"));
	if (!imgGBomb) return false;
	for (int i = 0; i < BOMB; i++) {
		Gbomb[i].width = 100;
		Gbomb[i].height = 36;
		Gbomb[i].columns = 1;
		Gbomb[i].startframe = 0;
		Gbomb[i].endframe = 0;
		Gbomb[i].velx = 0.01;
		Gbomb[i].vely = 0.1;
		Gbomb[i].rotation = 330;
		Gbomb[i].scaling = 0.1;
		Gbomb[i].alive = true;
		Gbomb[i].explosion = true;
	}
}
void CDlgMove::Draw_Gbomb() {

	//计算导弹位置
	for (int i = 0; i< BOMB; i++) {
		if (!Gplane[i].attack&&Gbomb[i].alive&&Gplane[i].y > 400 && (i < 8)) {
			Gbomb[i].x = Gplane[i].x + Gplane[i].width*Gplane[i].scaling / 2;
			Gbomb[i].y = Gplane[i].y + Gplane[i].height*Gplane[i].scaling / 2;
			Gplane[i].attack = true;
		}
		if (!Gplane[i].attack&&Gbomb[i].alive&&Gplane[i].y > 350 && (i >= 8 && i < 16)) {
			Gbomb[i].x = Gplane[i].x + Gplane[i].width*Gplane[i].scaling / 2;
			Gbomb[i].y = Gplane[i].y + Gplane[i].height*Gplane[i].scaling / 2;
			Gplane[i].attack = true;
		}
		if (!Gplane[i].attack&&Gbomb[i].alive&&Gplane[i].y > 400 && (i >= 16 && i < 24)) {
			Gbomb[i].x = Gplane[i].x + Gplane[i].width*Gplane[i].scaling / 2;
			Gbomb[i].y = Gplane[i].y + Gplane[i].height*Gplane[i].scaling / 2;
			Gplane[i].attack = true;
		}

		if (Gbomb[i].alive) {
			Gbomb[i].x -= Gbomb[i].velx;
			Gbomb[i].y += Gbomb[i].vely;
			Sprite_Transform_Draw(imgGBomb, Gbomb[i].x, Gbomb[i].y, Gbomb[i].width, Gbomb[i].height, 0, 1, Gbomb[i].rotation += 0.0015, Gbomb[i].scaling);
		}
		//攻击机爆炸动画
		if (i<8 && Gbomb[i].y > 500 && Gbomb[i].explosion) {
			Sprite_Animate(frame, 0, 29, 1, starttime, 90);
			Sprite_Transform_Draw(imgExplosion, Gbomb[i].x - 0.3*Gbomb[i].width, Gbomb[i].y - 0.6*Gbomb[i].height, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
			Gbomb[i].alive = false;
			if (frame == 29) {
				healthRadarDlbl -= gpoewr;
				Gbomb[i].explosion = false;
			}
		}
		if (i < 16 && i >= 8 && Gbomb[i].y > 450 && Gbomb[i].explosion) {
			Sprite_Animate(frame, 0, 29, 1, starttime, 90);
			Sprite_Transform_Draw(imgExplosion, Gbomb[i].x - 0.3*Gbomb[i].width, Gbomb[i].y - 0.6*Gbomb[i].height, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
			Gbomb[i].alive = false;
			if (frame == 29) {
				healthRadarBjx -= gpoewr;
				Gbomb[i].explosion = false;
			}
		}
		if (i < 24 && i >= 16 && Gbomb[i].y > 650 && Gbomb[i].explosion) {
			Sprite_Animate(frame, 0, 29, 1, starttime, 90);
			Sprite_Transform_Draw(imgExplosion, Gbomb[i].x - 0.3*Gbomb[i].width, Gbomb[i].y - 0.6*Gbomb[i].height, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
			Gbomb[i].alive = false;
			if (frame == 29) {
				healthRadarYc -= gpoewr;
				Gbomb[i].explosion = false;
			}
		}
	}
}

void CDlgMove::Draw_Gplanes() {

	//战机发射导弹
	for (int i = 0; i < 24; i++) {
		if (Gplane[i].alive) {
			//Gplane[i].x += Gplane[i].velx;
			//Gplane[i].y += Gplane[i].vely;
			if (i<16 && i >= 8)
				Sprite_Transform_Draw(imgGPlane2, Gplane[i].x, Gplane[i].y, Gplane[i].width, Gplane[i].height, 0, 1, Gplane[i].rotation, Gplane[i].scaling);
			else
				Sprite_Transform_Draw(imgGPlane, Gplane[i].x, Gplane[i].y, Gplane[i].width, Gplane[i].height, 0, 1, Gplane[i].rotation, Gplane[i].scaling);

		}

		if (!Gplane[i].alive&&Gplane[i].explosion) {
			Sprite_Animate(frame, 0, 29, 1, starttime, 90);
			Sprite_Transform_Draw(imgExplosion, Gplane[i].x, Gplane[i].y, explosion.width, explosion.height, frame, explosion.columns, explosion.rotation, explosion.scaling);
			if (frame == 29)
				Gplane[i].explosion = false;
		}
	}

}
void CDlgMove::ValuesInit() {
	//对应变量赋值
	moveData=CHSTPN_SYSSim::getMoveData();
	planeHzjBjx = moveData[0]["plane_hzj_bjx"];
	planeHzjDlbl = moveData[0]["plane_hzj_dlbl"];
	planeHzjYc = moveData[0]["plane_hzj_yc"];

	planeDzhBjx = moveData[0]["plane_dzj_bjx"];
	planeDzhDlbl = moveData[0]["plane_dzj_dlbl"];
	planeDzhYc = moveData[0]["plane_dzj_yc"];

	planeGjjBjx = moveData[0]["plane_gjj_bjx"];
	planeGjjDlbl = moveData[0]["plane_gjj_dlbl"];
	planeGjjYc = moveData[0]["plane_gjj_yc"];

	missileBjx = moveData[0]["missile_bjx"];
	missileDlbl = moveData[0]["missile_dlbl"];
	missileYc = moveData[0]["missile_yc"];
	//雷达生命值
	healthRadarBjx = moveData[0]["health_radar_bjx"];
	healthRadarDlbl = moveData[0]["health_radar_dlbl"];
	healthRadarYc = moveData[0]["health_radar_yc"];
	//战机数量
	planeHzj = moveData[0]["plane_hzj"];
	planeGjj = moveData[0]["plane_gjj"];
	planeDzj = moveData[0]["plane_dzj"];
	//基地生命值
	healthBjx = moveData[0]["health_bjx"];
	healthDlbl = moveData[0]["health_dlbl"];
}
void CDlgMove::ValuesUpdate() {
	//传递所需变量
	moveData = CHSTPN_SYSSim::getMoveData();
	if ((Hplane[11].y >= 800 || Hplane[23].y >= 800) && moveData.size() > timesAttack) {
		//将战机全部失活，并复位位置
		for (int i = 0; i < 36; i++) {

			Hplane[i].alive = false;
			Hplane[i].x = -1000;
			Hplane[i].y = -1000;
			Hbomb[i].alive = false;
			Hbomb[i].x = -1000;
			Hbomb[i].y = -1000;
		}
		for (int i = 0; i < 24; i++) {
			Gplane[i].alive = false;
			Gplane[i].x = -1000;
			Gplane[i].y = -1000;
			Gbomb[i].alive = false;
			Gbomb[i].x = -1000;
			Gbomb[i].y = -1000;
		}

		//对应变量赋值
		planeHzjBjx = moveData[timesAttack]["plane_hzj_bjx"];
		planeHzjDlbl = moveData[timesAttack]["plane_hzj_dlbl"];
		planeHzjYc = moveData[timesAttack]["plane_hzj_yc"];

		planeDzhBjx = moveData[timesAttack]["plane_dzj_bjx"];
		planeDzhDlbl = moveData[timesAttack]["plane_dzj_dlbl"];
		planeDzhYc = moveData[timesAttack]["plane_dzj_yc"];

		planeGjjBjx = moveData[timesAttack]["plane_gjj_bjx"];
		planeGjjDlbl = moveData[timesAttack]["plane_gjj_dlbl"];
		planeGjjYc = moveData[timesAttack]["plane_gjj_yc"];

		missileBjx = moveData[timesAttack]["missile_bjx"];
		missileDlbl = moveData[timesAttack]["missile_dlbl"];
		missileYc = moveData[timesAttack]["missile_yc"];
		//雷达生命值
		healthRadarBjx = moveData[timesAttack]["health_radar_bjx"];
		healthRadarDlbl = moveData[timesAttack]["health_radar_dlbl"];
		healthRadarYc = moveData[timesAttack]["health_radar_yc"];
		//战机数量
		planeHzj = moveData[timesAttack]["plane_hzj"];
		planeGjj = moveData[timesAttack]["plane_gjj"];
		planeDzj = moveData[timesAttack]["plane_dzj"];
		//基地生命值
		healthBjx = moveData[timesAttack]["health_bjx"];
		healthDlbl = moveData[timesAttack]["health_dlbl"];

		missileBjx == moveData[timesAttack]["missile_bjx"];
		missileDlbl == moveData[timesAttack]["missile_dlbl"];
		missileYc == moveData[timesAttack]["missile_yc"];
		if (moveData.size() - 1 > timesAttack) {
			//启动延时
			refreshcount = 0;

			//重新初始化战机
			Create_Hplanes();
			Create_Hbomb();
			Create_Gplanes();
			Create_Gbomb();
			//Create_Dplanes();
			Create_missles();
		}
		timesAttack++;
	}
	//仿真结束，设置状态位
	if (moveData.size() == timesAttack)
		gamestate = END;
}

bool CDlgMove::DirectInput_Init(HWND hwnd)
{
	//initialize DirectInput object
	DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	//initialize the keyboard
	dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();

	//initialize the mouse
	dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	dimouse->SetDataFormat(&c_dfDIMouse);
	dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dimouse->Acquire();
	d3ddev->ShowCursor(false);

	return true;
}

void CDlgMove::DirectInput_Update()
{
	//update mouse
	dimouse->Poll();
	if (!SUCCEEDED(dimouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse_state)))
	{
		//mouse device lose, try to re-acquire
		dimouse->Acquire();
	}

	//update keyboard
	dikeyboard->Poll();
	if (!SUCCEEDED(dikeyboard->GetDeviceState(256, (LPVOID)&keys)))
	{
		//keyboard device lost, try to re-acquire
		dikeyboard->Acquire();
	}

	//update controllers
	for (int i = 0; i < 4; i++)
	{
		ZeroMemory(&controllers[i], sizeof(XINPUT_STATE));

		//get the state of the controller
		XINPUT_STATE state;
		DWORD result = XInputGetState(i, &state);

		//store state in global controllers array
		if (result == 0) controllers[i] = state.Gamepad;
	}
}


int CDlgMove::Mouse_X()
{
	return mouse_state.lX;
}

int CDlgMove::Mouse_Y()
{
	return mouse_state.lY;
}

int CDlgMove::Mouse_Button(int button)
{
	return mouse_state.rgbButtons[button] & 0x80;
}

bool CDlgMove::Key_Down(int key)
{
	return (bool)(keys[key] & 0x80);
}


void CDlgMove::DirectInput_Shutdown()
{
	if (dikeyboard)
	{
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
	if (dimouse)
	{
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
}


void CDlgMove::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

}

