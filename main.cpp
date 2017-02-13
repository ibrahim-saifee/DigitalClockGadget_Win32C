#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>

HBRUSH hBackgroundBrush;

void DrawSingleDigit(HDC hdc,int x,int y, int s,int n)
{
int w=s/5;
x=x+6; y=y+7;
int x1=x+1, y1=y-1;
int x2=x+2+w+w+s, y2=y;
int x3=x2, y3=y+2+w+w+s;
int x4=x1, y4=y+4+(w*4)+s+s;
int x5=x, y5=y3;
int x6=x1, y6=y1+2+w+w+s;

BOOL SevenSegment[10][7]={1,1,1,1,1,1,0,  //0
						  0,0,1,1,0,0,0,  //1
						  0,1,1,0,1,1,1,  //2
						  0,1,1,1,1,0,1,  //3
						  1,0,1,1,0,0,1,  //4
						  1,1,0,1,1,0,1,  //5
						  1,1,0,1,1,1,1,  //6
						  0,1,1,1,0,0,0,  //7
						  1,1,1,1,1,1,1,  //8
						  1,1,1,1,1,0,1,  //9
						 };
POINT PolygonPoints[7][6]={x, y, x+w,y+w ,x+w, y+w+s, x, y+w+w+s, x-w, y+w+s, x-w, y+w,
						   x1,y1, x1+w,y1-w, x1+w+s, y1-w, x1+w+w+s, y1, x1+w+s, y1+w,x1+w, y1+w,
						   x2,y2, x2+w,y2+w ,x2+w, y2+w+s, x2, y2+w+w+s, x2-w, y2+w+s, x2-w, y2+w,
						   x3,y3, x3+w,y3+w ,x3+w, y3+w+s, x3, y3+w+w+s, x3-w, y3+w+s, x3-w, y3+w,
						   x4,y4, x4+w,y4-w, x4+w+s, y4-w, x4+w+w+s, y4, x4+w+s, y4+w,x4+w, y4+w,
						   x5,y5, x5+w,y5+w ,x5+w, y5+w+s, x5, y5+w+w+s, x5-w, y5+w+s, x5-w, y5+w,
						   x6,y6, x6+w,y6-w, x6+w+s, y6-w, x6+w+w+s, y6, x6+w+s, y6+w,x6+w, y6+w,
	                      };

for (int i=0;i<7;i++)
{
	if(SevenSegment[n][i])
	   Polygon(hdc,PolygonPoints[i],6);
}

}


void DrawTime(HDC hdc,int s,BOOL Dots)
{
SYSTEMTIME st;
GetLocalTime(&st);
int w=s/5;
int Gap = 2*(s+w);
int x=5, l=4+6*w+2*s;
int y=l/4+25;
w+=w;

	DrawSingleDigit(hdc,x,20,s,st.wHour/10);
	x+=Gap;
	DrawSingleDigit(hdc,x,20,s,st.wHour%10);
	
	x+=Gap+Gap/2-w/2;
	if(Dots)
	{
		POINT d[4]={x,y, x+w,y-w, x+w+w,y, x+w,y+w};
		Polygon(hdc,d,4);
		
	}
	y+=l/2;
	if(Dots)
	{
		POINT d[4]={x,y, x+w,y-w, x+w+w,y, x+w,y+w};
		Polygon(hdc,d,4);
		
	}
	y-=l/2;
	x+=Gap/2+w/2;
	
	DrawSingleDigit(hdc,x,20,s,st.wMinute/10);
	x+=Gap;
	DrawSingleDigit(hdc,x,20,s,st.wMinute%10);
	
	x+=Gap+Gap/2-w/2;
	if(Dots)
	{
		POINT d[4]={x,y, x+w,y-w, x+w+w,y, x+w,y+w};
		Polygon(hdc,d,4);
	}
	y+=l/2;
	if(Dots)
	{
		POINT d[4]={x,y, x+w,y-w, x+w+w,y, x+w,y+w};
		Polygon(hdc,d,4);
		
	}
	y-=l/2;
	x+=Gap/2+w/2;
	
	DrawSingleDigit(hdc,x,20,s,st.wSecond/10);
	x+=Gap;
	DrawSingleDigit(hdc,x,20,s,st.wSecond%10);
}


void GetDayName(char * ptr, int WeekDay)
{
 switch(WeekDay)
 {
    case 0:
         strcpy(ptr," Sunday  ");
         break;    
    case 1:
         strcpy(ptr," Monday  ");
         break;    
    case 2:
         strcpy(ptr," Tuesday ");
         break;
    case 3:
         strcpy(ptr,"Wednesday");
         break;
    case 4:
         strcpy(ptr,"Thursday ");
         break;    
    case 5:
         strcpy(ptr,"  Friday ");
         break;    
    case 6:
         strcpy(ptr,"Saturday ");
         break;    
 }
}


/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
HDC hdc,hdcBuffer;
HBITMAP hBufferBitmap;
PAINTSTRUCT ps;
RECT rc;
static BOOL Dots;
static HBRUSH hRedBrush;
static int xDiff,yDiff,xScroll=1;
POINT pt;
static BOOL hold,xScrollPositive;
char date[10];
SYSTEMTIME st;
		
		/* Upon destruction, tell the main thread to stop */
		case WM_CREATE:
			hRedBrush=CreateSolidBrush(RGB(255,50,50));
			Dots=FALSE;
			SetTimer(hwnd,1,500,NULL);
			SetTimer(hwnd,2,1,NULL);
			break;
			
		case WM_LBUTTONDOWN:
			SetCapture(hwnd);
			xDiff=LOWORD(lParam);
			yDiff=HIWORD(lParam);
			hold=1;
			break;
			
		case WM_MOUSEMOVE:
			if(hold)
			{
				pt.x=LOWORD(lParam);
				pt.y=HIWORD(lParam);
				ClientToScreen(hwnd,&pt);
				pt.x-=xDiff; pt.y-=yDiff;
				MoveWindow(hwnd,pt.x,pt.y,400,100,FALSE);
			}
			break;
		
		case WM_LBUTTONUP:
			hold=0;
			ReleaseCapture();
			break;
			
		case WM_TIMER:
			switch(wParam)
			{
				case 1:
					Dots=~Dots;
					break;
				case 2:
					if(xScrollPositive)
						{
						 xScroll+=1;
						 if(xScroll>150) xScrollPositive=0;
						}
					else
						{
						 xScroll-=1;
						 if(xScroll<1) xScrollPositive=1;
						}
					break;
			}
			InvalidateRect(hwnd,NULL,FALSE);
			break;
		
		case WM_ERASEBKGND:
			break;
		case WM_PAINT:
			GetClientRect(hwnd,&rc);
			hdc=BeginPaint(hwnd,&ps);
			hdcBuffer=CreateCompatibleDC(hdc);
			hBufferBitmap=CreateCompatibleBitmap(hdc,rc.right,rc.bottom);
			SelectObject(hdcBuffer,hBufferBitmap);
			FillRect(hdcBuffer,&rc,hBackgroundBrush);
			SelectObject(hdcBuffer,hRedBrush);
			DrawTime(hdcBuffer,12,Dots);
			SetBkMode(hdcBuffer,TRANSPARENT);
			SetTextColor(hdcBuffer,RGB(255,50,50));
			GetLocalTime(&st);
			sprintf(date,"%d%d-%d%d-%d",st.wDay/10,st.wDay%10,st.wMonth/10,st.wMonth%10,st.wYear);
			TextOut(hdcBuffer,xScroll,5,date,10);
            GetDayName(&date[0],st.wDayOfWeek);
			TextOut(hdcBuffer,150-xScroll,75,date,9);
			BitBlt(hdc,0,0,rc.right,rc.bottom,hdcBuffer,0,0,SRCCOPY);
			DeleteDC(hdcBuffer);
			DeleteObject(hBufferBitmap);
			EndPaint(hwnd,&ps);
			break;
		
		case WM_DESTROY: {
			DeleteObject(hRedBrush);
			KillTimer(hwnd,1);
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}


/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(hInstance,MAKEINTRESOURCE(100));
	
	hBackgroundBrush=CreateSolidBrush(RGB(1,1,255));
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = hBackgroundBrush;
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_LAYERED|WS_EX_TOPMOST|WS_EX_TOOLWINDOW,"WindowClass","Caption",WS_POPUP|WS_VISIBLE,
		600, /* x */
		10, /* y */
		400, /* width */
		100, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	SetLayeredWindowAttributes(hwnd,RGB(1,1,255),100,1);
	
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate key codes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}
	return Msg.wParam;
}
