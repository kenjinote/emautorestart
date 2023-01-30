#pragma comment(linker, "/opt:nowin98")
#define UNICODE
#pragma comment(lib,"comctl32.lib")
#include <windows.h>
#include <commctrl.h>
#define ID_LISTVIEW 100

HINSTANCE Instance;
WNDPROC OrgListViewProc;

TCHAR szClassName[]=L"window";
HWND	hList;

VOID AddLOG(TCHAR * szLog)
{
	LV_ITEM		item;
	item.mask = LVIF_TEXT;
	
	const int   cch=256;
	TCHAR       szTime[256]; 
	SYSTEMTIME st;
	GetLocalTime(&st);
	wsprintf(szTime,L"%d年%02d月%02d日 %02d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	int i = ListView_GetItemCount(hList);

	item.pszText=szTime;
	item.iItem = i;
	item.iSubItem = 0;
	ListView_InsertItem(hList, &item);
	
	item.pszText=szLog;
	item.iItem = i;
	item.iSubItem = 1;
	ListView_SetItem(hList, &item);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:

		InitCommonControls();
		hList = CreateWindowEx(0,
			WC_LISTVIEW, L"",
			WS_CHILD | WS_VISIBLE | LVS_REPORT,// | LVS_OWNERDRAWFIXED,
			0, 0, 0, 0,
			hWnd,
			(HMENU)ID_LISTVIEW,
			Instance,
			NULL);
		
		if(hList==NULL) return NULL;
		
		LV_COLUMN	lvcol;
		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 256;
		lvcol.pszText = L"TIME";
		lvcol.iSubItem = 0;
		ListView_InsertColumn(hList, 0, &lvcol);
		
		lvcol.fmt = LVCFMT_LEFT;
		lvcol.cx = 512;
		lvcol.pszText = L"LOG";
		lvcol.iSubItem = 1;
		ListView_InsertColumn(hList, 1, &lvcol);

		SetTimer(hWnd,0x1234,1000,NULL);
		return	0;

	case WM_TIMER:
		KillTimer(hWnd,0x1234);
		for(;;){
			HWND hDlg = FindWindow(L"#32770",L"今回の利用状況");
			if(hDlg == NULL) break;
			HWND hBtn = GetDlgItem(hDlg,0x1);
			SendMessage(hBtn, BM_CLICK, 0, 0);
			AddLOG(L"切断を確認。");
			HWND hDlg1 = FindWindow(L"#32770",L"EMOBILE HW ユーティリティ - D02HW");
			if(hDlg1 == NULL) break;
			hDlg1 = GetWindow(hDlg1,GW_CHILD);
			do{
				HWND hDlg2 = GetWindow(hDlg1,GW_CHILD);
				do{
					HWND hDlg3 = GetWindow(hDlg2,GW_CHILD);
					do{
						HWND hBtn = GetDlgItem(hDlg3,0x7d1);
						if(hBtn!=NULL){
							SendMessage(hBtn, BM_CLICK, 0, 0);
							AddLOG(L"再接続しました。");
							goto END;
						}
					}while(((hDlg3 = GetWindow(hDlg3,GW_HWNDNEXT))));
				}while(((hDlg2 = GetWindow(hDlg2,GW_HWNDNEXT))));
			}while(((hDlg1 = GetWindow(hDlg,GW_HWNDNEXT))));
END:
			break;
		}
		SetTimer(hWnd,0x1234,1000,NULL);
		break;

	case WM_DESTROY:
		KillTimer(hWnd,0x1234);
		PostQuitMessage(0);
		return	0;
		
	case WM_SIZE:
		MoveWindow(hList, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return	0;
		
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE hinst,HINSTANCE hPreInst,
				   LPSTR pCmdLine,int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;
	Instance=hinst;
	if(!hPreInst){
		wndclass.style=CS_HREDRAW|CS_VREDRAW;
		wndclass.lpfnWndProc=WndProc;
		wndclass.cbClsExtra=0;
		wndclass.cbWndExtra=0;
		wndclass.hInstance =hinst;
		wndclass.hIcon=NULL;
		wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground=NULL;
		wndclass.lpszMenuName=NULL;
		wndclass.lpszClassName=szClassName;
		if(!RegisterClass(&wndclass))
			return FALSE;
	}
	hWnd=CreateWindow(szClassName,
		L"EMOBILE AUTO RESTART TOOL for D02HW",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hinst,
		NULL);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}









