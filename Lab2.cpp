#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <tchar.h>
#include "vector"
#include <fstream>
using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define COLUMNS 4

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop App");

static TCHAR szImageName[] = _T("LogoImage");

HINSTANCE hInst;

vector<string> textArr;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawVBorders(HDC hdc, int wndWidth, int wndHeight);
void DrawHBorder(HDC hdc, int wndWidth, int wndHeight);
void DrawTable(HDC hdc, int wndWidth, int wndHeight);
int ReadFile();

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop App"),
			NULL);

		return 1;
	}

	// Store instance handle in our global variable
	hInst = hInstance;

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIDTH, HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop App"),
			NULL);

		return 1;
	}

	if (ReadFile()) {
		MessageBox(NULL,
			_T("Call to ReadFile failed!"),
			_T("Windows Desktop App"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	RECT r;
	if (!GetClientRect(hWnd, &r))
		GetLastError();

	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		// Here your application is laid out.
		// For this introduction, we just print out "Hello, Windows desktop!"
		// in the top left corner.
		hdc = BeginPaint(hWnd, &ps);
		DrawTable(hdc, r.right, r.bottom);
		EndPaint(hWnd, &ps);
		// End application-specific layout section.
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void DrawVBorders(HDC hdc, int cWidth, int wndHeight) {
	for (int i = 1; i < COLUMNS; i++) {
		MoveToEx(hdc, i * cWidth, 0, NULL);
		LineTo(hdc, i * cWidth, wndHeight);
	}
}

void DrawHBorder(HDC hdc, int wndWidth, int rHeight) {
	MoveToEx(hdc, 0, rHeight, NULL);
	LineTo(hdc, wndWidth, rHeight);
}

void DrawTable(HDC hdc, int wndWidth, int wndHeight) {
	/*DrawVBorders(hdc, wndWidth, wndHeight);
	DrawHBorders(hdc, wndWidth, wndHeight);*/
	int cWidth = wndWidth / COLUMNS;
	int rowOffset = 0;
	DrawVBorders(hdc, cWidth, wndHeight);

	for (int strIndex = 0; strIndex < textArr.size(); strIndex++) {
		int maxTextHeight = 0;
		RECT r;
		r.top = rowOffset + 5;
		r.bottom = wndHeight;

		for (int cIndex = 0; cIndex < COLUMNS && strIndex < textArr.size(); cIndex++) {
			r.left = cIndex * cWidth + 5;
			r.right = (cIndex + 1) * cWidth - 5;
			

			const CHAR* str = textArr[strIndex++].c_str();
			int len = strlen(str);

			int currTextHeight = DrawTextA(hdc, (LPCSTR)str, strlen(str), &r, DT_VCENTER | DT_WORDBREAK | DT_NOCLIP | DT_EDITCONTROL);

			if (currTextHeight > maxTextHeight)
				maxTextHeight = currTextHeight;
		}

		rowOffset += maxTextHeight + 5;
		DrawHBorder(hdc, wndWidth, rowOffset);
	}
}

int ReadFile() {
	ifstream file("test.txt", ios::in);

	if (!file) {
		return -1;
	}

	string out;
	while (!file.eof()) {
		getline(file, out);
		if (!out.empty()) {
			textArr.push_back(out);
		}
	}

	file.close();
	return 0;
}