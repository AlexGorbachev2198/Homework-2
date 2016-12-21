#include "stdafx.h"
#include "ForDZ2.h"
#define MAX_LOADSTRING 100
HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MyDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
    MSG msg;
	HACCEL hAccelTable;
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FORDZ2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FORDZ2));
    while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FORDZ2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_3DDKSHADOW);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FORDZ2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; 
   hWnd = CreateWindow(szWindowClass, "CRYPTATOR", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
unsigned Shift_Right(unsigned a, int n) 
{
	unsigned Mask = 0; 
	for (int i = 0; i<2; i++)
	{
		Mask <<= 1; 
		Mask += 1; 
	}
	unsigned b = a & Mask; 
	b <<= 6; 
	a >>= 2;
	return a | b; 
}
unsigned Shift_Left(unsigned a, int n) 
{
	unsigned Mask = 0; 
	for (int i = 0; i<2; i++)
	{
		Mask <<= 1; 
		Mask += 1; 
	}
	Mask <<= 6; 
    unsigned b = a & Mask; 
	b >>= 6; 
	a <<= 2; 
	return a | b; 
}
void Crypting(char *input, char *output, int n, int Key)
{
	srand(Key);
	unsigned char a, b, Gamm;
	for (int i = 0; i < n; i++)
	{
		memcpy(&a, input + i, 1);
		Gamm = rand();
		b = Gamm ^ a;
		b = Shift_Left(b, 8);
		memcpy(output + i, &b, 1);
	}
}
void Encrypting(char *input, char *output, int n, int Key)
{
	srand(Key);
	unsigned char a, b, Gamm;
	for (int i = 0; i < n; i++)
	{
		memcpy(&a, input + i, 1);
		a = Shift_Right(a, 8);
		Gamm = rand();
		b = Gamm ^ a;
		memcpy(output + i, &b, 1);
	}
}
char Text[256];
int Lenght = 0;
int Key;
OPENFILENAME ofn; 
TCHAR NameFile[256] = "Sample.shf"; 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
		unsigned int KY;
	case WM_KEYDOWN:
		KY = wParam;
		if ((KY == VK_BACK) && (Lenght > 0))
		{
			Text[Lenght - 1] = 0;
			Lenght--;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (KY == VK_DELETE)
		{
			for (unsigned int i = 0; i < Lenght; i++)
			{
				Text[i] = 0;
			}
			Lenght = 0;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_CHAR:
		if (wParam != VK_BACK && wParam != VK_DELETE) {
			Text[Lenght] = wParam;
			Lenght++;
			Text[Lenght] = 0;
			hdc = GetDC(hWnd);
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 40, 40, Text, Lenght);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_32773:
			MessageBox(0, "Backspace - удаление последнего элемента.\nDelete - удаление всего введенного текста.", "Клавиши", MB_OK);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_32771:
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = NameFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = 255;
			ofn.lpstrFilter = "Шифрованные файлы (*.shf)\0*.shf\0Все файлы\0*.*\0";
			ofn.nFilterIndex = 0;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = "shf";
			if (GetOpenFileName(&ofn))
			{
				if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MyDialog) == IDOK)
				{
					FILE *pF1;
					char Shifr[512];
					pF1 = fopen(NameFile, "rb");
					if (pF1 == 0)
					{
						MessageBox(hWnd, "Ошибка открытия файла.", "Error", MB_OK | MB_ICONHAND);
						return TRUE;
					}
					fread(&Lenght, sizeof(int), 1, pF1);
					fread(Shifr, sizeof(char), Lenght, pF1);
					fclose(pF1);
					Encrypting(Shifr, Text, Lenght, Key);
					Text[Lenght] = 0;
					InvalidateRect(hWnd, 0, TRUE);
				}
				else MessageBox(hWnd, "Ключ не введен.", "Сообщение", MB_OK | MB_ICONHAND);
			}
			break;
		case ID_32772:
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = "Шифрованные файлы (*.shf)\0*.shf\0Все файлы\0*.*\0";
			ofn.lpstrFile = NameFile;
			ofn.nMaxFile = 255;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
			ofn.lpstrTitle = NULL;
			ofn.lpstrDefExt = "shf";
			if (GetSaveFileName(&ofn))
			{
				if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MyDialog) == IDOK)
				{
					char Shifr[512];
					Crypting(Text, Shifr, Lenght, Key);
					FILE *pF1;
					pF1 = fopen(NameFile, "wb");
					if (pF1 == 0)
					{
						MessageBox(hWnd, "Не удалось открыть файл.", "Сообщение об ошибке", MB_OK | MB_ICONHAND);
						return TRUE;
					}
					fwrite(&Lenght, sizeof(int), 1, pF1);
					fwrite(Shifr, sizeof(char), Lenght, pF1);
					fclose(pF1);
					MessageBox(hWnd, "Данные сохранены.", "Сообщение", MB_OK | MB_ICONINFORMATION);
				}
				else MessageBox(hWnd, "Ключ не введен.", "Сообщение", MB_OK | MB_ICONHAND);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, 40, 40, Text, Lenght);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
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
INT_PTR CALLBACK MyDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK)
			{
				char Str[32];
				SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXT, 31, (LPARAM)Str);
				if (sscanf(Str, "%d", &Key) < 1)
				{
					MessageBox(hDlg, "Неверный формат ключа.", "Key error", MB_OK | MB_ICONHAND);
					return (INT_PTR)TRUE;
				}
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
