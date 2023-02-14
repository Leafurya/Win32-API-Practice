#include <windows.h>
#include <doublebuffer.h> 
#include <stdio.h>

LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK ChildWndProc(HWND,UINT,WPARAM,LPARAM);

HWND mainWnd;
RECT mainRect;
HINSTANCE g_hInst;
LPSTR mainWndClass="mainWndClass";

HWND childWnd;
RECT childRect;
LPSTR childWndClass="childWndClass";

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
	
	WndClass.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
	WndClass.lpfnWndProc=(WNDPROC)ChildWndProc;
	WndClass.lpszClassName=childWndClass;
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
	void DrawMainWnd(HDC hdc,RECT rt){
		FillRect(hdc,&rt,(HBRUSH)(COLOR_WINDOW));
		SetBkMode(hdc,TRANSPARENT);
	}
LRESULT CALLBACK MainWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch(iMessage) {
		case WM_CREATE:
			SendMessage(hWnd,WM_SIZE,0,0);
			childWnd=CreateWindow(childWndClass,NULL,WS_CHILD|WS_VISIBLE,0,100,500,500,hWnd,NULL,g_hInst,NULL);
			return 0;
		case WM_SIZE:
			if(wParam!=SIZE_MINIMIZED){
				GetClientRect(hWnd,&mainRect);
			}
			return 0;
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
				DoubleBuffer(hdc,mainRect,DrawMainWnd);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_LBUTTONDOWN:
			printf("parent click\n");
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
	void DrawChildWnd(HDC hdc,RECT rt){
		FillRect(hdc,&rt,(HBRUSH)(COLOR_WINDOW));
		SetBkMode(hdc,TRANSPARENT);
	}
LRESULT CALLBACK ChildWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch(iMessage) {
		case WM_CREATE:
			SendMessage(hWnd,WM_SIZE,0,0);
			return 0;
		case WM_SIZE:
			if(wParam!=SIZE_MINIMIZED){
				GetClientRect(hWnd,&childRect);
			}
			return 0;
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
				DoubleBuffer(hdc,childRect,DrawChildWnd);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_LBUTTONDOWN:
			printf("child click\n");
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
