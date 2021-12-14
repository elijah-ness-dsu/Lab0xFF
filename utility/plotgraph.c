// PARAMETERS
// FOR BEST RESULTS, LEAVE UNTOUCHED SO DRAWING DISPLAYS CLEARLY
#define PLANE_SIZE 600
#define NODE_DIAMETER 30
#define PLOT_SIZE (PLANE_SIZE+NODE_DIAMETER+4)	// Add 4 for some reason..
// END PARAMETERS




// Include Win32 and GDI
#include <Windows.h>
#include <wingdi.h>

// Include standard libraries
#include <stdio.h>
#include <math.h>

// Include local dependencies
#include "resource.h"

// Avoid declaring prototypes twice
#include "plotgraph.h"

// Declare globals
int nodes_plot;
double *coordX_plot;
double *coordY_plot;
double *edges_plot;
int algorithm_plot;		// Used to color lines. 0-2 Initial (not solution), Exact, Hueristic
int *printOrder_plot;

int type_plot;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    	// Create structures for GDI drawing
    	HDC hdc;
    	PAINTSTRUCT ps;
    	
    	// Interpret messages
        case WM_CLOSE:
            DestroyWindow(hWnd);
        	break;
        case WM_PAINT:
        	
        	// Initialize painting
        	hdc = BeginPaint(hWnd, &ps);
			
			// Set transparency to transparent
			SetBkMode(hdc, 1);
			SelectObject(hdc, GetStockObject(DC_PEN));
			
			int offset;
			// If the graph is standard
			if (type_plot == 0)
			{
				offset = 0;
			}
			else if (type_plot == 1)
			{
				offset = (PLANE_SIZE)/2;
			}
			else
			{
				printf("Error: Parameter plotType must be in range 0-1.\n");
				exit(1);
			}
			
			if (algorithm_plot > 0)
			{
				if (algorithm_plot == 2)
					SetDCPenColor(hdc, RGB(0,0,255));
				else
					SetDCPenColor(hdc, RGB(0,255,0));

				// Account for going back to final 0 in the solution table
				for (int j = 1; j < nodes_plot; ++j)
				{
					MoveToEx(hdc, offset + NODE_DIAMETER/2 + lround((6.0 * coordX_plot[printOrder_plot[j-1]])), PLOT_SIZE - (offset + NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[j-1]])) + 4.0), NULL);
					LineTo(hdc, offset + NODE_DIAMETER/2 + lround((6.0 * coordX_plot[printOrder_plot[j]])), PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[j]])) + 4.0));
					
					char text[7];
					sprintf(text, "%6.2lf", edges_plot[printOrder_plot[j-1]*nodes_plot + (printOrder_plot[j])]);
					int x = offset + ((NODE_DIAMETER/2 + lround((6.0 * coordX_plot[printOrder_plot[j-1]]))) + lround(NODE_DIAMETER/2 +(6.0 * coordX_plot[printOrder_plot[j]]))) / 2;
					int y = PLOT_SIZE - (offset + (((NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[j-1]])) + 4.0) + (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[j]])) + 4.0)))/2);
					RECT textRect;
						textRect.left = x - NODE_DIAMETER;
						textRect.top = y - NODE_DIAMETER;
						textRect.right = x + NODE_DIAMETER;
						textRect.bottom = y + NODE_DIAMETER;
					DrawText(hdc, text, -1, &textRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					
					// Paint the last line back to 0
					if (j == nodes_plot - 1)
					{
						MoveToEx(hdc, lround(NODE_DIAMETER/2 +(6.0 * coordX_plot[printOrder_plot[0]])), PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[0]])) + 4.0), NULL);
						LineTo(hdc, NODE_DIAMETER/2 + lround((6.0 * coordX_plot[printOrder_plot[j]])), PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[j]])) + 4.0));
					
						sprintf(text, "%6.2lf", edges_plot[printOrder_plot[0]*nodes_plot + (printOrder_plot[j])]);
						int x = ((NODE_DIAMETER/2 + lround((6.0 * coordX_plot[printOrder_plot[j]]))) + lround(NODE_DIAMETER/2 +(6.0 * coordX_plot[printOrder_plot[0]]))) / 2;
						int y = PLOT_SIZE - (((NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[j]])) + 4.0) + (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[printOrder_plot[0]])) + 4.0)))/2;
						RECT textRect;
							textRect.left = x - NODE_DIAMETER;
							textRect.top = y - NODE_DIAMETER;
							textRect.right = x + NODE_DIAMETER;
							textRect.bottom = y + NODE_DIAMETER;
						
						DrawText(hdc, text, -1, &textRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					}
				}
				
			}
			else
			{
				SetDCPenColor(hdc, RGB(255,0,0));
				for (int j = 0; j < nodes_plot; ++j)
				{
					for (int k = j + 1; k < nodes_plot; ++k)
					{
						if (edges_plot[j*nodes_plot + k] > 0.0){
							MoveToEx(hdc, lround(NODE_DIAMETER/2 +(6.0 * coordX_plot[j])), PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[j])) + 4.0), NULL);
							LineTo(hdc, NODE_DIAMETER/2 + lround((6.0 * coordX_plot[k])), PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[k])) + 4.0));
							
							char text[7];
							sprintf(text, "%6.2lf", edges_plot[j*nodes_plot + k]);
							int x = ((NODE_DIAMETER/2 + lround((6.0 * coordX_plot[k]))) + lround(NODE_DIAMETER/2 +(6.0 * coordX_plot[j]))) / 2;
							int y = PLOT_SIZE - (((NODE_DIAMETER/2 + lround((6.0 * coordY_plot[k])) + 4.0) + (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[j])) + 4.0)))/2;
							RECT textRect;
								textRect.left = x - NODE_DIAMETER;
								textRect.top = y - NODE_DIAMETER;
								textRect.right = x + NODE_DIAMETER;
								textRect.bottom = y + NODE_DIAMETER;
					
							DrawText(hdc, text, -1, &textRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
						}
					}
				}
			}
			
			// Reset transparency mode to opaque
			SetBkMode(hdc, 0);
			SelectObject(hdc, GetStockObject(DC_PEN));

			for (int i = 0; i < nodes_plot; ++i)
			{
				// Set color to black
				SetDCPenColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(0,0,0));
				
				if (i == 0)
				{
					SetDCBrushColor(hdc, RGB(0,0,0));
					SelectObject(hdc, GetStockObject(DC_BRUSH));
					//Left top right bottom
					Ellipse(hdc, 
						NODE_DIAMETER/2 + lround((6.0 * coordX_plot[i] - NODE_DIAMETER/2)),
						PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[i]) + 4.0 - NODE_DIAMETER/2)),
						NODE_DIAMETER/2 + lround((6.0 * coordX_plot[i] + NODE_DIAMETER/2)),
						PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[i])) + NODE_DIAMETER/2 + 4.0));
						
					SetDCBrushColor(hdc, RGB(255,255,255));
					SetTextColor(hdc, RGB(255,255,255));
				}
				else
				{
					Ellipse(hdc, 
						NODE_DIAMETER/2 + lround((6.0 * coordX_plot[i] - NODE_DIAMETER/2)),
						PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[i]) + 4.0 - NODE_DIAMETER/2)),
						NODE_DIAMETER/2 + lround((6.0 * coordX_plot[i] + NODE_DIAMETER/2)),
						PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[i])) + NODE_DIAMETER/2 + 4.0));
						
					SetTextColor(hdc, RGB(0,0,0));
				}
				
				SetDCPenColor(hdc, RGB(255,255,255));
				SelectObject(hdc, GetStockObject(DC_PEN));
				
				char text[4] = {0, 0, 0, 0};
				sprintf(text, "%d", i);
				
				RECT textRect;
				textRect.left = NODE_DIAMETER/2 + lround((6.0 * coordX_plot[i] - NODE_DIAMETER/2));
				textRect.top = PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[i]) + 4.0 - NODE_DIAMETER/2));
				textRect.right = NODE_DIAMETER/2 + lround((6.0 * coordX_plot[i] + NODE_DIAMETER/2));
				textRect.bottom = PLOT_SIZE - (NODE_DIAMETER/2 + lround((6.0 * coordY_plot[i])) + NODE_DIAMETER/2 + 4.0);
				
				DrawText(hdc, text, -1, &textRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
			
			// Finish painting environment
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

int ShowWin()
{
	HINSTANCE hInstance;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WndProc;
    wc.style         = 0;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    
    if (algorithm_plot == 0)
    {
    	wc.lpszClassName = "IntitialGraphClass";
	}
	else
	{
		wc.lpszClassName = "ResultGraphClass";
	}
    
    wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON256));
    wc.hIconSm = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON256));
	
	
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
        wc.lpszClassName,
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

int ShowPathPlot(int N, int **bestOrder, int algType)
{
	// Pass the parameters into the globals for updated information 
	nodes_plot = N;
	printOrder_plot = (*bestOrder);
	algorithm_plot = algType;
	
	ShowWin();
}

int MakeGraphPlot(int N, double **matrix, double *arrayX, double *arrayY, int plotType, int algType)
{
	// Pass the parameters into the global variables
	nodes_plot = N;
	edges_plot = (*matrix);
	coordX_plot = arrayX;
	coordY_plot = arrayY;
	type_plot = plotType;
	algorithm_plot = algType;
	
	ShowWin();
}