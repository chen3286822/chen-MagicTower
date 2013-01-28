#include "Application.h"

// 
// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
// {
// 	Application::sCreate();
// 
// 	if(Application::sInstance().systemInit(NULL))
// 	{
// 		Application::sInstance().loadResource();
// 		Application::sInstance().run();
// 		Application::sInstance().freeResource();
// 	}
// 	Application::sInstance().cleanUp();
// 
// 	Application::sDestroy();
// 	return 0;
// }


#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
char g_fileName[MAX_PATH];
char g_level[MAX_PATH];

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	InputFileNameProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ConfigMapProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Application::sCreate();

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MAGICTOWER_WINDOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAGICTOWER_WINDOW));

	// 主消息循环:
// 	while (GetMessage(&msg, NULL, 0, 0))
// 	{
// 		if (Application::sInstance().getHGE()->System_GetState(HGE_HWND))
// 			Application::sInstance().getHGE()->System_Start();
// 
// 		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
// 		{
// 			TranslateMessage(&msg);
// 			DispatchMessage(&msg);
// 		}
// 	}
	for(;;)
	{
		if(Application::sInstance().getHGE()->System_GetState(HGE_HWND))
			Application::sInstance().getHGE()->System_Start();

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			if(msg.message == WM_QUIT) break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				DispatchMessage(&msg);
			}
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAGICTOWER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MAGICTOWER_WINDOW);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MAGICTOWER));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

BOOL	popFileSaveDlg(HWND hwnd)
{
	BROWSEINFO bi;  
	char buffer[MAX_PATH];  
	ZeroMemory(buffer,MAX_PATH);  
	bi.hwndOwner=hwnd;
	bi.pidlRoot=NULL;  
	bi.pszDisplayName=buffer;  
	bi.lpszTitle=TEXT("选择一个文件夹");  
	bi.ulFlags=BIF_EDITBOX | BIF_RETURNFSANCESTORS;  
	bi.lpfn=NULL;  
	bi.lParam=0;  
	bi.iImage=0;
	LPITEMIDLIST pList=NULL;  
	if((pList=SHBrowseForFolder(&bi))!=NULL)  
	{  
		char path[MAX_PATH];
		ZeroMemory(path,MAX_PATH);
		SHGetPathFromIDList(pList,path);
		memset(g_fileName,'\0',MAX_PATH);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_INPUTFILENAME), Application::sInstance().getHGE()->System_GetState(HGE_HWNDPARENT), InputFileNameProc);
		
		//验证文件名可行性
		strcat(path,"\\");
		strcat(path,g_fileName);
		Application::sInstance().getMarkup()->Save(path);

		return TRUE;
	}
	return FALSE;
}

BOOL	popFileOpenDlg(HWND hwnd)
{
	BROWSEINFO bi;  
	char buffer[MAX_PATH];  
	ZeroMemory(buffer,MAX_PATH);  
	bi.hwndOwner=hwnd;
	bi.pidlRoot=NULL;  
	bi.pszDisplayName=buffer;  
	bi.lpszTitle=TEXT("选择一个文件夹");  
	bi.ulFlags=BIF_EDITBOX | BIF_BROWSEINCLUDEFILES | BIF_RETURNFSANCESTORS;  
	bi.lpfn=NULL;  
	bi.lParam=0;  
	bi.iImage=0;
	LPITEMIDLIST pList=NULL;  
	if((pList=SHBrowseForFolder(&bi))!=NULL)  
	{  
		char path[MAX_PATH];
		ZeroMemory(path,MAX_PATH);
		SHGetPathFromIDList(pList,path);
		
		//验证文件名可行性
		Application::sInstance().getMarkup()->Load(path);
		const char* doc = (Application::sInstance().getMarkup()->GetDoc()).c_str();
		Application::sInstance().loadMapFromXml();
		return TRUE;
	}
	return FALSE;
}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	CMarkup* newXml;
//	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		if(Application::sInstance().systemInit(hWnd))
		{
			Application::sInstance().loadResource();
			
			Application::sInstance().run();
		}
		MoveWindow(Application::sInstance().getHGE()->System_GetState(HGE_HWND),
			CHILDWND_OFFX, CHILDWND_OFFY, APP_WIDTH, APP_HEIGHT, true);
		return 0;
	case WM_SIZE:
		MoveWindow(Application::sInstance().getHGE()->System_GetState(HGE_HWND),
			CHILDWND_OFFX, CHILDWND_OFFY, APP_WIDTH, APP_HEIGHT, true);
		break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			Application::sInstance().freeResource();
			Application::sInstance().cleanUp();
			Application::sDestroy();
			PostQuitMessage(0);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_NEWFILE:		
			if(Application::sInstance().getMarkup()!=NULL)
			{
				CMarkup* temp = Application::sInstance().getMarkup();
				gSafeDelete(temp);
				Application::sInstance().setMarkup(NULL);
			}
			newXml = new CMarkup;
			Application::sInstance().setMarkup(newXml);
			break;
		case IDM_SAVEFILE:
			if(Application::sInstance().getConfig().level==-1)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CONFIGMAP), hWnd, ConfigMapProc);
				return 0;
			}
			if(Application::sInstance().saveMapToXml())
					popFileSaveDlg(hWnd);
				
			return 0;
		case IDM_OPENFILE:
			if(Application::sInstance().getMarkup()==NULL)
			{
				newXml = new CMarkup;
				Application::sInstance().setMarkup(newXml);
			}
			popFileOpenDlg(hWnd);
			return 0;
		case IDM_CONFIG:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CONFIGMAP), hWnd, ConfigMapProc);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		Application::sInstance().freeResource();
		Application::sInstance().cleanUp();
		Application::sDestroy();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//输入文件名对话框
INT_PTR CALLBACK InputFileNameProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			GetDlgItemText(hDlg,IDC_EDIT1,g_fileName,MAX_PATH);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)FALSE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//关卡设置对话框
INT_PTR CALLBACK ConfigMapProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			Config& config = Application::sInstance().getConfig();
			if(config.level != -1)
			{
				char strLevel[50] = {0};
				sprintf(strLevel,"%d",config.level);
				SetDlgItemText(hDlg,IDC_EDIT2,strLevel);
			}
			return (INT_PTR)TRUE;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			Config& config = Application::sInstance().getConfig();
			config.clear();
			GetDlgItemText(hDlg,IDC_EDIT2,g_level,MAX_PATH);
			config.level = atoi(g_level);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)FALSE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}