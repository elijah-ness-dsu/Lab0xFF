// For best results, leave parameters untouched
#define PLOT_SIZE 634	// Add 4 for some reason..
#define NODE_DIAMETER 30

#include <Windows.h>
#include <wingdi.h>
#include <stdio.h>
#include <math.h>

#include "resource.h"

const char g_szClassName[] = "plotWindowClass";

int nodes = 5;
double coordX[5] = {0.296030, 46.122623, 93.621632, 96.502579, 38.901944};
double coordY[5] = {13.983581, 29.273354, 51.704459, 37.726981, 55.699332};
double edges[5][5] = {
	-1.0, 48.309977, 100.660482, 99.093134, 56.838547,
	-1.0, -1.0, 52.529137, 51.084281, 27.394716,
	-1.0, -1.0, -1.0, 14.271290, 54.865319,
	-1.0, -1.0, -1.0, -1.0, 60.339361
	-1.0, -1.0, -1.0, -1.0, -1.0
};

//int nodes = 2;
//double coordX[2] = {0.0, 100.0};
//double coordY[2] = {0.0, 100.0};
//
//double edges[2][2] = {
//	0.0, 141.421356237,
//	141.421356237, 0.0
//};

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//RECT ClientRect;
	//GetClientRect(hWnd, &ClientRect);
	//printf("DIMENSIONS: %d %d %d %d", ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);
	
    switch(msg)
    {
    	HDC hdc;
    	PAINTSTRUCT ps;
    	
        case WM_CLOSE:
            DestroyWindow(hWnd);
        	break;
        case WM_PAINT:
        	
        	hdc = BeginPaint(hWnd, &ps);
			
			// Set transparency to transparent and color to red
			SetBkMode(hdc, 1);
			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255,0,0));
			
			
			for (int j = 0; j < nodes; ++j)
			{
				for (int k = j + 1; k < nodes; ++k)
				{
					if (edges[j][k] > 0.0){
						MoveToEx(hdc, lround(NODE_DIAMETER/2 +(6.0 * coordX[j])), PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY[j])) + 4.0), NULL);
						LineTo(hdc, NODE_DIAMETER/2 + lround((6.0 * coordX[k])), PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY[k])) + 4.0));
						
						char text[7];
						sprintf(text, "%6.2lf", edges[j][k]);
						int x = ((NODE_DIAMETER/2 + lround((6.0 * coordX[k]))) + lround(NODE_DIAMETER/2 +(6.0 * coordX[j]))) / 2;
						int y = PLOT_SIZE - (((NODE_DIAMETER/2 + lround((6.0 * coordY[k])) + 4.0) + (NODE_DIAMETER/2 + lround((6.0 * coordY[j])) + 4.0)))/2;
						RECT textRect;
							textRect.left = x - NODE_DIAMETER;
							textRect.top = y - NODE_DIAMETER;
							textRect.right = x + NODE_DIAMETER;
							textRect.bottom = y + NODE_DIAMETER;
				
						DrawText(hdc, text, -1, &textRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					}
				}
			}
			
			// Reset transparency mode to opaque and color to black
			SetBkMode(hdc, 0);
			SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(0,0,0));
			for (int i = 0; i < nodes; ++i)
			{
				//Left top right bottom
				Ellipse(hdc, 
					NODE_DIAMETER/2 + lround((6.0 * coordX[i] - NODE_DIAMETER/2)),
					PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY[i]) + 4.0 - NODE_DIAMETER/2)),
					NODE_DIAMETER/2 + lround((6.0 * coordX[i] + NODE_DIAMETER/2)),
					PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY[i])) + NODE_DIAMETER/2 + 4.0));
				
				char text[4] = {0, 0, 0, 0};
				sprintf(text, "%d", i);
				
				RECT textRect;
				textRect.left = NODE_DIAMETER/2 + lround((6.0 * coordX[i] - NODE_DIAMETER/2));
				textRect.top = PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY[i]) + 4.0 - NODE_DIAMETER/2));
				textRect.right = NODE_DIAMETER/2 + lround((6.0 * coordX[i] + NODE_DIAMETER/2));
				textRect.bottom = PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY[i])) + NODE_DIAMETER/2 + 4.0);
				
				DrawText(hdc, &text, -1, &textRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
			
   			EndPaint(hWnd, &ps);
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
        	break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int ShowPlot()//HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HINSTANCE hInstance;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    
    wc.hIcon         = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON256));
    wc.hIconSm       = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON256));
	
	
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
	
	RECT Rect;
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = PLOT_SIZE;
	Rect.bottom = PLOT_SIZE;
	AdjustWindowRect(&Rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
	
    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Graph Plot",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
		Rect.right - Rect.left, Rect.bottom - Rect.top,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

int main()
{
	ShowPlot();
}