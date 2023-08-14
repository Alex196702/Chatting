#include"include/Widget.h"
static TCHAR WindowClass[]=TEXT("Widget");
typedef struct EventQueue{
    HWND hwnd;
    int event;
    void(*callback)(HWND hwnd,void * args,WPARAM wParam,LPARAM lParam);
    void *args;
    struct EventQueue *next;
}EventQueue;
typedef struct DRAWSTRUCT{
    COLORREF borderColor;
    COLORREF textColor;
    COLORREF backColor;
}DRAWSTRUCT;
static EventQueue *eq=NULL;
static void OnPaint(HWND hwnd,DRAWSTRUCT *ds){
    PAINTSTRUCT ps;
    HDC hdc;
    HPEN hPen,hPenOld;
    HBRUSH hBr,hBrOld;
    RECT rect;
    int text_len=0;
    wchar_t *text;
    hdc=BeginPaint(hwnd,&ps);
    hPen=CreatePen(PS_SOLID,1,ds->borderColor);
    hBr=CreateSolidBrush(ds->backColor);
    hPenOld=SelectObject(hdc,hPen);
    hBrOld=SelectObject(hdc,hBr);
    GetClientRect(hwnd,&rect);
    Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
    // //绘制文本
    // text_len=GetWindowTextLength(ds->hwnd);
    // if(text_len){
    //     text=calloc(text_len+1,sizeof(wchar_t));
    //     GetWindowText(ds->hwnd,text,text_len+1);
    //     SetBkMode(hdc,TRANSPARENT);
    //     SetTextColor(hdc,ds->textColor);
    //     DrawText(hdc,text,text_len,&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
    //     free(text);
    // }
    SelectObject(hdc,hPenOld);
    SelectObject(hdc,hBrOld);
    DeleteObject(hPen);
    DeleteObject(hBr);
    EndPaint(hwnd,&ps);
}
static int focus=0;
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
    CREATESTRUCT *cs=NULL;
    if(eq){
        EventQueue *tempQ=eq;
        while(tempQ){
            if(tempQ->hwnd==hwnd){
                if(tempQ->event==msg){
                    tempQ->callback(hwnd,tempQ->args,wParam,lParam);
                    return 0;
                }
            }
            tempQ=tempQ->next;
        }
    }
    switch(msg){
        // case WM_CREATE:
        // //给第一个编辑框设置焦点
        //     cs=lParam;
        //     if((cs->lpCreateParams==1)&&(!focus)){
        //         CreateCaret(hwnd,NULL,1,10);
        //         SetCaretPos(5,5);  
        //         ShowCaret(hwnd);
        //         SetFocus(hwnd);
        //         focus=1;
                
        //     }
        //     break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd,msg,wParam,lParam);
            break;
    }
    return 0;
}
int isRegistered=0;
void registerClass(HINSTANCE hInst){
    WNDCLASS wc={0};
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    wc.hInstance=hInst;
    wc.lpszMenuName=NULL;
    wc.style=CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc=WndProc;
    wc.lpszClassName=WindowClass;
    if(!RegisterClass(&wc)){
        MessageBox(NULL,TEXT("注册子窗口类失败！"),TEXT("注册错误"),0);
        return -1;
    }
    isRegistered=1;
}
HWND CreateWidget(HINSTANCE hInst,HWND hParent,RECT rect,int nCmdShow,TCHAR *text,void(*onPaint)(void *args),void* args,void* lpParams){
    if(!isRegistered){
        registerClass(hInst);
    }
    HWND hwnd;
    if(hParent==NULL){
        hwnd=CreateWindow(WindowClass,text,WS_OVERLAPPEDWINDOW,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,hParent,NULL,hInst,lpParams);
        ShowWindow(hwnd,nCmdShow);
        UpdateWindow(hwnd);
    }else{
        hwnd=CreateWindow(WindowClass,text,WS_CHILD|WS_VISIBLE,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,hParent,NULL,hInst,lpParams);
    }   
    if(onPaint){
        eventRegister(hwnd,WM_PAINT,onPaint,args);
    }else{
        DRAWSTRUCT *ds=calloc(1,sizeof(DRAWSTRUCT));
        ds->backColor=RGB(0x73,0x73,0x73);
        ds->textColor=RGB(0xff,0xff,0xff);
        ds->borderColor=RGB(0,0,0);
        eventRegister(hwnd,WM_PAINT,OnPaint,ds);
    }
    InvalidateRect(hwnd,NULL,FALSE);
    return hwnd;
}
void eventRegister(HWND hwnd,int event,void (*callback)(HWND hwnd,void *args),void *args){
    if(eq==NULL){
        eq=calloc(1,sizeof(EventQueue));
        eq->hwnd=hwnd;
        eq->event=event;
        eq->callback=callback;
        eq->args=args;
        eq->next=NULL;
    }else{
        EventQueue *tempQ=eq;
        while(tempQ->next){
            tempQ=tempQ->next;
        }
        tempQ->next=calloc(1,sizeof(EventQueue));
        tempQ=tempQ->next;
        tempQ->hwnd=hwnd;
        tempQ->event=event;
        tempQ->callback=callback;
        tempQ->args=args;
        tempQ->next=NULL;
    }
}
void setBackColor(HWND hwnd,COLORREF color){
    EventQueue *tempQ=eq;
    while(tempQ){
        if((tempQ->hwnd==hwnd)&&(tempQ->event==WM_PAINT)){
            DRAWSTRUCT *ds=tempQ->args;
            ds->backColor=color;
        }
        tempQ=tempQ->next;
    }
    InvalidateRect(hwnd,NULL,FALSE);
}
void setTextColor(HWND hwnd,COLORREF color){
    EventQueue *tempQ=eq;
    while(tempQ){
        if((tempQ->hwnd==hwnd)&&(tempQ->event==WM_PAINT)){
            DRAWSTRUCT *ds=tempQ->args;
            ds->textColor=color;
        }
        tempQ=tempQ->next;
    }
    InvalidateRect(hwnd,NULL,FALSE);
}
void setBorderColor(HWND hwnd,COLORREF color){
    EventQueue *tempQ=eq;
    while(tempQ){
        if((tempQ->hwnd==hwnd)&&(tempQ->event==WM_PAINT)){
            DRAWSTRUCT *ds=tempQ->args;
            ds->borderColor=color;
        }
        tempQ=tempQ->next;
    }
    InvalidateRect(hwnd,NULL,FALSE);
}
void setText(HWND hwnd,LPWSTR lpString){
    SetWindowText(hwnd,lpString);
    InvalidateRect(hwnd,NULL,TRUE);
}
