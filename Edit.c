#include"include/Edit.h"
typedef struct DRAWSTRUCT{
    COLORREF borderColor;
    COLORREF textColor;
    COLORREF backColor;
}DRAWSTRUCT;
void OnPaint(HWND hwnd,DRAWSTRUCT *ds){
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
    HFONT hFont,hFontOld;
    GetClientRect(hwnd,&rect);
    Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
    //绘制文本
    RECT textRect={rect.left+5,rect.top+5,rect.right-5,rect.bottom-5};
    text_len=GetWindowTextLength(hwnd);
    if(text_len){
        text=calloc(text_len+1,sizeof(wchar_t));
        GetWindowText(hwnd,text,text_len+1);
        SetBkMode(hdc,TRANSPARENT);
        SetTextColor(hdc,ds->textColor);
        DrawText(hdc,text,text_len,&textRect,0);
        free(text);
        SIZE size={0};
        POINT pos={0};
        hFont=CreateFont();
        hFontOld=SelectObject(hdc,font);
        GetTextExtentPoint32(hdc,text,text_len,&size);
        // GetCaretPos(&pos);
        SetCaretPos(size.cx,pos.y);
    }
    SelectObject(hdc,hPenOld);
    SelectObject(hdc,hBrOld);
    SelectObject(hdc,hFontOld);
    DeleteObject(hPen);
    DeleteObject(hBr);
    DeleteObject(hFont);
    EndPaint(hwnd,&ps);
}
static wchar_t *text=NULL;
static int text_len=0;
void OnChar(HWND hwnd,void *args,WPARAM wParam,LPARAM lParam){
    
    wchar_t ch=wParam;
    switch(ch){
        case VK_BACK:
            if(text_len>=1){
                text[text_len-1]='\0';
                setText(hwnd,text);
                text_len--;
            }
            break;
        default:
            if(NULL==text){
                text=calloc(2,sizeof(wchar_t));
                *text=ch;
                text_len++;
            }else{
                text=realloc(text,sizeof(wchar_t)*(text_len+2));
                text[text_len]=ch;
                text[text_len+1]=0;
                text_len++;
            }
            setText(hwnd,text);
            break;
    }
    
    // OutputDebugString(text);
    // OutputDebugString(TEXT("\n"));
      
}
static void OnLButtonDown(HWND hwnd,void *args,WPARAM wParam,LPARAM lParam){
    CreateCaret(hwnd,NULL,1,10);
    SetCaretPos(5,5);
    ShowCaret(hwnd);
    SetFocus(hwnd);
}
HWND CreateEdit(HINSTANCE hInst,HWND hParent,RECT rect,int nCmdShow,TCHAR *text){
    DRAWSTRUCT *ds=calloc(1,sizeof(DRAWSTRUCT));
    ds->backColor=RGB(0xff,0xff,0xff);
    ds->textColor=RGB(0,0,0);
    ds->borderColor=RGB(0,0,0);
    HWND hwnd=CreateWidget(hInst,hParent,rect,nCmdShow,text,OnPaint,ds,1);
    eventRegister(hwnd,WM_LBUTTONDOWN,OnLButtonDown,NULL);
    eventRegister(hwnd,WM_CHAR,OnChar,NULL);
    return hwnd;
}