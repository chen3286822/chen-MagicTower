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

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
char g_fileName[MAX_PATH];
char g_level[MAX_PATH];

// �˴���ģ���а����ĺ�����ǰ������:
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

	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MAGICTOWER_WINDOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAGICTOWER_WINDOW));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
	bi.lpszTitle=TEXT("ѡ��һ���ļ���");  
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
		
		//��֤�ļ���������
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
	bi.lpszTitle=TEXT("ѡ��һ���ļ���");  
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
		
		//��֤�ļ���������
		Application::sInstance().getMarkup()->Load(path);
		const char* doc = (Application::sInstance().getMarkup()->GetDoc()).c_str();
		Application::sInstance().loadMapFromXml();
		return TRUE;
	}
	return FALSE;
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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


//�����ļ����Ի���
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

//�ؿ����öԻ���
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