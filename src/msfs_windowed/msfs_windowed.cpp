//------------------------------------------------------------------------------
//
//  PMDG NG3 CDU connection sample 
// 
//------------------------------------------------------------------------------

#include <windows.h>
#include <wchar.h>
#include <gdiplus.h>

#include "..\..\inc\SimConnect.h"
#include "..\..\inc\PMDG_NG3_SDK.h"

#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib, "..\\..\\lib\\MSFS\\SimConnect.lib")

using namespace Gdiplus;

#define TIMER_ID	1

HANDLE  hSimConnect = NULL;

static enum DATA_REQUEST_ID {
	CDU_DATA_REQUEST
};

// This structure receives and keeps the contents of the CDU screen
PMDG_NG3_CDU_Screen screen;

// CDU screen cell size in pixels
#define CELL_WIDTH	30
#define CELL_HEIGHT	45

VOID OnPaint(HDC hdc)
{
	Graphics     graphics(hdc);

	SolidBrush   blackBrush(Color::Black);
	SolidBrush   whiteBrush(Color::White);
	SolidBrush   greenBrush(Color::LightGreen);
	SolidBrush   cyanBrush(Color::Cyan);
	SolidBrush   magentaBrush(Color::Magenta);
	SolidBrush   amberBrush(Color::Orange);
	SolidBrush   redBrush(Color::Red);
	SolidBrush   grayBrush(Color::Gray);

	FontFamily   fontFamily(L"Microsoft Sans Serif");
	Font	     largeFont(&fontFamily, 42, FontStyleBold, UnitPixel);
	Font	     smallFont(&fontFamily, 32, FontStyleBold, UnitPixel);
	StringFormat format;

	WCHAR        wstr[2];
	Brush* brush;
	Font* font;

	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentFar);

	// Draw each screen cell
	for (int x = 0; x < CDU_COLUMNS; ++x)
	{
		for (int y = 0; y < CDU_ROWS; ++y)
		{
			PMDG_NG3_CDU_Cell* cell = &(screen.Cells[x][y]);

			// Cell background: normally black; gray if Reverse video flag is set
			if (cell->Flags & PMDG_NG3_CDU_FLAG_REVERSE)
				brush = &grayBrush;
			else
				brush = &blackBrush;
			graphics.FillRectangle(brush, x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT);

			// Font - large or small
			if (cell->Flags & PMDG_NG3_CDU_FLAG_SMALL_FONT)
				font = &smallFont;
			else
				font = &largeFont;

			// Symbol color
			switch (cell->Color)
			{
			case PMDG_NG3_CDU_COLOR_WHITE:
				brush = &whiteBrush;
				break;
			case PMDG_NG3_CDU_COLOR_GREEN:
				brush = &greenBrush;
				break;
			case PMDG_NG3_CDU_COLOR_CYAN:
				brush = &cyanBrush;
				break;
			case PMDG_NG3_CDU_COLOR_MAGENTA:
				brush = &magentaBrush;
				break;
			case PMDG_NG3_CDU_COLOR_AMBER:
				brush = &amberBrush;
				break;
			case PMDG_NG3_CDU_COLOR_RED:
				brush = &redBrush;
			}

			// Unused flag: draw symbol in gray
			if (cell->Flags & PMDG_NG3_CDU_FLAG_UNUSED)
				brush = &grayBrush;

			// Draw the symbol
			swprintf_s(wstr, 2, L"%lc", cell->Symbol);
			graphics.DrawString(wstr, 1, font, PointF((x + 0.5f) * CELL_WIDTH, (y + 1.0f) * CELL_HEIGHT), &format, brush);
		}
	}
}

VOID initSimConnect()
{
	HRESULT hr;

	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "PMDG NG3 CDU Test", NULL, 0, 0, 0)))
	{
		// Associate an ID with the PMDG data area name
		hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_CDU_0_NAME, PMDG_NG3_CDU_0_ID);

		// Define the data area structure - this is a required step
		hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_CDU_0_DEFINITION, 0, sizeof(PMDG_NG3_CDU_Screen), 0, 0);

		// Sign up for notification of data change.  
		// SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED flag asks for the data to be sent only when some of the data is changed.
		hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CDU_0_ID, CDU_DATA_REQUEST, PMDG_NG3_CDU_0_DEFINITION,
			SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
	}
}

bool checkSimConnect()
{
	SIMCONNECT_RECV* pData;
	DWORD cbData;

	HRESULT hr = SimConnect_GetNextDispatch(hSimConnect, &pData, &cbData);

	if (SUCCEEDED(hr))
	{
		switch (pData->dwID)
		{
		case SIMCONNECT_RECV_ID_CLIENT_DATA: // Receive and save the NG3 CDU screen data block
		{
			SIMCONNECT_RECV_CLIENT_DATA* pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;

			switch (pObjData->dwRequestID)
			{
			case CDU_DATA_REQUEST:
			{
				PMDG_NG3_CDU_Screen* pS = (PMDG_NG3_CDU_Screen*)&pObjData->dwData;
				memcpy(&screen, pS, sizeof(PMDG_NG3_CDU_Screen));
				return true;
			}
			}
			break;
		}
		}
	}
	return false;
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("PMDG NG3 CDU Connection Test");
	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("PMDG NG3 CDU Connection Test"), // window class name
		TEXT("PMDG NG3 CDU Connection Test"), // window caption
		WS_OVERLAPPEDWINDOW, // window style
		CW_USEDEFAULT, // initial x position
		CW_USEDEFAULT, // initial y position
		CW_USEDEFAULT, // initial x size
		CW_USEDEFAULT, // initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		hInstance, // program instance handle
		NULL); // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
		initSimConnect();							// Sign up to PMDG data connection
		SetTimer(hWnd, TIMER_ID, 100, NULL);		// Set up a timer to check for new data 10 times a second
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, TIMER_ID);
		PostQuitMessage(0);
		return 0;

	case WM_TIMER:
		if (checkSimConnect())						// Check for new data
			InvalidateRect(hWnd, NULL, NULL);		// Redraw the window if the CDU contents have changed
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}