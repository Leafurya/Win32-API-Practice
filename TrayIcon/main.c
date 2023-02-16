#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

#define WM_TRAY_ALERT 		WM_USER+1

LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);

HWND mainWnd;
RECT mainRect;
HINSTANCE g_hInst;
LPSTR mainWndClass="mainWndClass";

NOTIFYICONDATA trayicon;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)MainWndProc;
	WndClass.lpszClassName=mainWndClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	AllocConsole();
	freopen("COIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	hWnd=CreateWindow(mainWndClass,mainWndClass,WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,CW_USEDEFAULT,500,500,
		  NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while(GetMessage(&Message,0,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	FreeConsole();

	return Message.wParam;
}
	void ShowNotification(){ //알림 보이기 
		NOTIFYICONDATA ballData;
		memset(&ballData,0,sizeof(ballData));
		ballData.cbSize=sizeof(NOTIFYICONDATA);
		ballData.hWnd=mainWnd;
		ballData.uID=100;
		ballData.uFlags=NIF_INFO;
		ballData.uCallbackMessage=WM_TRAY_ALERT;
		ballData.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		ballData.uTimeout=1500;
		ballData.dwInfoFlags=NIIF_INFO;
		strcpy(ballData.szInfoTitle,"트레이 아이콘");
		strcpy(ballData.szInfo,"마우스 우클릭");
		Shell_NotifyIcon(NIM_ADD,&ballData);
	}
LRESULT CALLBACK MainWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch(iMessage) {
		case WM_CREATE:
			//트레이 아이콘 만들기 
			memset(&trayicon,0,sizeof(trayicon));
			trayicon.cbSize=sizeof(NOTIFYICONDATA);
			trayicon.hWnd=hWnd;
			trayicon.uID=100;
			trayicon.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
			trayicon.uCallbackMessage=WM_TRAY_ALERT;
			trayicon.hIcon=LoadIcon(NULL,IDI_APPLICATION);
			strcpy(trayicon.szTip,"트레이 아이콘");
			Shell_NotifyIcon(NIM_ADD,&trayicon);
			
			SendMessage(hWnd,WM_SIZE,0,0);
			return 0;
		case WM_SIZE:
			if(wParam!=SIZE_MINIMIZED){
				GetClientRect(hWnd,&mainRect);
			}
			return 0;
		case WM_TRAY_ALERT:
			switch(LOWORD(lParam)){
				case WM_LBUTTONDOWN:
					MessageBox(hWnd,"아이콘 클릭","알림",MB_OK);
					break;
				case WM_RBUTTONDOWN:
					printf("rbuttondown\n");
					ShowNotification();
					break;
			}
			return 0;
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			TextOut(hdc,0,0,"hello",5);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_DESTROY:
			Shell_NotifyIcon(NIM_DELETE,&trayicon); //고스트 트레이아이콘 방지용 
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
